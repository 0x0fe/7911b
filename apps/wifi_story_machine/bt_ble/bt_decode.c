#include "server/audio_server.h"
#include "server/server_core.h"
#include "btstack/avctp_user.h"
#include "app_config.h"
#include "app_music.h"
#include "asm/sbc.h"
#include "asm/eq.h"
#include "asm/clock.h"
#include "btstack/a2dp_media_codec.h"

#ifdef CONFIG_BT_ENABLE

#define LOG_TAG_CONST        BT
#define LOG_TAG             "[BT]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#define LOG_DUMP_ENABLE
#define LOG_CLI_ENABLE
#include "debug.h"

#define BT_MUSIC_DECODE
#define BT_PHONE_DECODE
#if TCFG_USER_EMITTER_ENABLE
#define BT_EMITTER_ENABLE
#endif

extern void bt_music_set_mute_status(u8 mute);
extern int check_a2dp_media_if_mute(void);
extern int sbc_energy_check(u8 *packet, u16 size);
extern void earphone_a2dp_audio_codec_close(void);
extern void phone_call_end(void);
extern int a2dp_media_get_total_buffer_size(void);
extern void switch_rf_coexistence_config_table(u8 index);


#ifdef BT_MUSIC_DECODE

#define A2DP_CODEC_SBC			0x00
#define A2DP_CODEC_MPEG12		0x01
#define A2DP_CODEC_MPEG24		0x02

#define BT_AUDIO_FORMAT_SBC     1
#define BT_AUDIO_FORMAT_AAC     2
#define BT_AUDIO_FORMAT_APTX    3
#define BT_AUDIO_FORMAT_LADC    4
#define BT_AUDIO_FORMAT_CVSD    5
#define BT_AUDIO_FORMAT_MSBC    6

#define BT_DEC_RATE_MAX_STEP       100
#define BT_DEC_RATE_INC_STEP       2
#define BT_DEC_RATE_DEC_STEP       2


struct bt_media_vfs_handle {
    u8    format;
    u8    not_mute_cnt;
    u8    a2dp_open;
    u8    ready;
    u16   seqn;
    s16   adjust_step;
    void *p_addr;
    int   p_len;
    int   p_pos;
    void *audio_dec;
    u32   begin_size;
    u32   top_size;
    u32   bottom_size;
};

static struct bt_media_vfs_handle bt_media_vfs_handle;
#define mfs      (&bt_media_vfs_handle)

static void sbc_dec_event_handler(void *priv, int argc, int *argv)
{
    union audio_req req = {0};
    switch (argv[0]) {
    case AUDIO_SERVER_EVENT_END:
        earphone_a2dp_audio_codec_close();
        break;
    default:
        break;
    }
}

#define RB16(b)    (u16)(((u8 *)b)[0] << 8 | (((u8 *)b))[1])

static int get_rtp_header_len(u8 *buf, int len)
{
    int ext, csrc;
    int byte_len;
    int header_len = 0;
    u8 *data = buf;

    if (RB16(buf + 2) != (u16)(mfs->seqn + 1) && mfs->seqn) {
        putchar('K');
    }
    mfs->seqn = RB16(buf + 2);

    csrc = buf[0] & 0x0f;
    ext  = buf[0] & 0x10;

    byte_len = 12 + 4 * csrc;
    buf += byte_len;
    if (ext) {
        ext = (RB16(buf + 2) + 1) << 2;
    }

    header_len = byte_len + ext + 1;
    if (mfs->format == BT_AUDIO_FORMAT_SBC) {
        /*sbc???????????????*/
        while (data[header_len] != 0x9c) {
            if (++header_len > len) {
                log_w("sbc warn\n");
                return len;
            }
        }
    }

    if (header_len != byte_len + ext + 1) {
        /* log_w("sbc warn!!!\n"); */
        /* put_buf(data, len); */
    }

    return header_len;
}

static void media_stream_free(void)
{
    if (mfs->p_addr) {
        a2dp_media_free_packet(mfs->p_addr);
        mfs->p_addr = NULL;
        mfs->p_pos = 0;
        mfs->p_len = 0;
    }
}

static int bt_sbc_file_read(void *priv, void *buf, u32 len)
{
    u32 r_len = 0;
    u32 offset = 0;

    do {
        if (!mfs->p_addr) {
            mfs->p_len = a2dp_media_get_packet((u8 **)&mfs->p_addr);
            if (!mfs->p_addr) {
                if (!mfs->a2dp_open) {
                    return 0;
                }
                if (mfs->p_len == -EINVAL) {
                    return -1;
                }
                continue;
            }
            mfs->p_pos = get_rtp_header_len(mfs->p_addr, mfs->p_len);
            if (mfs->p_pos >= len) {
                return 0;
            }
            if (check_a2dp_media_if_mute()) {
                if (a2dp_get_status() == BT_MUSIC_STATUS_STARTING && sbc_energy_check(mfs->p_addr, mfs->p_len) > 1000) {
                    if (++mfs->not_mute_cnt > 50) {
                        bt_music_set_mute_status(0);
                        mfs->not_mute_cnt = 0;
                    }
                }
            } else {
                mfs->not_mute_cnt = 0;
            }
        }

        if (len == (u32) - 1) {	//?????????
            if (0x800 > mfs->p_len) {
                memcpy(buf, (u8 *)mfs->p_addr, mfs->p_len);
                offset = mfs->p_len;
                media_stream_free();
                break;
            } else {
                memcpy(buf, (u8 *)mfs->p_addr, 0x20);
                offset = 0x20;
                media_stream_free();
                break;
            }
        }

        if (!mfs->ready) {
            //????????????100ms???????????????
            while (a2dp_media_get_remain_play_time(0) < 100) {
                if (!mfs->a2dp_open) {
                    return 0;
                }
                os_time_dly(1);
            }
            mfs->ready = 1;
        }

        r_len = mfs->p_len - mfs->p_pos;
        if (r_len > (len - offset)) {
            r_len = len - offset;
        }

        memcpy((u8 *)buf + offset, (u8 *)mfs->p_addr + mfs->p_pos, r_len);
        mfs->p_pos += r_len;
        offset += r_len;

        if (mfs->p_pos >= mfs->p_len) {
            media_stream_free();
            if (mfs->format == BT_AUDIO_FORMAT_SBC) {
                break;
            }
        }
    } while (offset < len);

    return offset;
}

static int bt_sbc_file_seek(void *priv, u32 offset, int orig)
{
    return 0;
}

static const struct audio_vfs_ops bt_sbc_audio_dec_vfs_ops = {
    .fread = bt_sbc_file_read,
    .fseek = bt_sbc_file_seek,
};

static int bt_sync(void *priv, u32 data_size, u16 *in_rate, u16 *out_rate)
{
    data_size = a2dp_media_get_total_data_len();

    if (data_size < mfs->bottom_size) {
        /* putchar('<'); */
        mfs->adjust_step += BT_DEC_RATE_INC_STEP;
        if (mfs->adjust_step < 0) {
            mfs->adjust_step += BT_DEC_RATE_INC_STEP * 2;
        }
    } else if (data_size > mfs->top_size) {
        /* putchar('>'); */
        mfs->adjust_step -= BT_DEC_RATE_DEC_STEP;
        if (mfs->adjust_step > 0) {
            mfs->adjust_step -= BT_DEC_RATE_DEC_STEP * 2;
        }
    } else {
        /* putchar('='); */
        if (mfs->adjust_step > 0) {
            mfs->adjust_step -= (mfs->adjust_step * BT_DEC_RATE_INC_STEP) / BT_DEC_RATE_MAX_STEP;
            if (mfs->adjust_step > 0) {
                mfs->adjust_step--;
            }
        } else if (mfs->adjust_step < 0) {
            mfs->adjust_step += ((0 - mfs->adjust_step) * BT_DEC_RATE_DEC_STEP) / BT_DEC_RATE_MAX_STEP;
            if (mfs->adjust_step < 0) {
                mfs->adjust_step++;
            }
        }
    }

    if (mfs->adjust_step < -BT_DEC_RATE_MAX_STEP) {
        mfs->adjust_step = -BT_DEC_RATE_MAX_STEP;
        /* putchar('{'); */
    } else if (mfs->adjust_step > BT_DEC_RATE_MAX_STEP) {
        mfs->adjust_step = BT_DEC_RATE_MAX_STEP;
        /* putchar('}'); */
    }

    *out_rate += mfs->adjust_step;

    return 0;
}

void *earphone_a2dp_audio_codec_open(int media_type, u8 volume)
{
    union audio_req req = {0};
    int err = 0;

    mfs->audio_dec = server_open("audio_server", "dec");
    if (!mfs->audio_dec) {
        return NULL;
    }

    /*??????????????????(????????????????????????,??????????????????)*/
    server_register_event_handler(mfs->audio_dec, mfs, sbc_dec_event_handler);

    switch (media_type) {
    case A2DP_CODEC_SBC:
        req.dec.dec_type = "sbc";
        mfs->format = BT_AUDIO_FORMAT_SBC;
        break;
    case A2DP_CODEC_MPEG24:
        req.dec.dec_type = "aac";
        mfs->format = BT_AUDIO_FORMAT_AAC;
        break;
    }

    req.dec.cmd             = AUDIO_DEC_OPEN;
    req.dec.volume          = volume ? volume : get_app_music_volume();
    req.dec.output_buf      = NULL;
    req.dec.output_buf_len  = 2 * 1024;
    req.dec.channel         = 0;
    req.dec.sample_rate     = 0;
    req.dec.vfs_ops         = &bt_sbc_audio_dec_vfs_ops;
#if defined CONFIG_AUDIO_DEC_PLAY_SOURCE
    req.dec.sample_source   = CONFIG_AUDIO_DEC_PLAY_SOURCE;
#else
    req.dec.sample_source   = "dac";
#endif
#if defined CONFIG_AUDIO_MIX_ENABLE
    req.dec.attr            = AUDIO_ATTR_REAL_TIME;
    req.dec.dec_sync        = bt_sync;
#endif
#if TCFG_EQ_ENABLE
#if defined EQ_CORE_V1
    req.dec.attr            |= AUDIO_ATTR_EQ_EN;
#if TCFG_LIMITER_ENABLE
    req.dec.attr           |= AUDIO_ATTR_EQ32BIT_EN;
#endif
#if TCFG_DRC_ENABLE
    req.dec.attr           |= AUDIO_ATTR_DRC_EN;
#endif
#else
    struct eq_s_attr eq_attr = {0};
    extern void set_eq_req_attr_parm(struct eq_s_attr * eq_attr);
    extern void *get_eq_hdl(void);
    set_eq_req_attr_parm(&eq_attr);
    req.dec.eq_attr = &eq_attr;
    req.dec.eq_hdl = get_eq_hdl();
#endif
#endif

    mfs->a2dp_open = 1;

    err = server_request(mfs->audio_dec, AUDIO_REQ_DEC, &req);
    if (err) {
        mfs->a2dp_open = 0;
        server_close(mfs->audio_dec);
        mfs->audio_dec = NULL;
        return NULL;
    }

    mfs->seqn = 0;
    mfs->adjust_step = 0;

    int a2dp_total_size = a2dp_media_get_total_buffer_size();
    mfs->begin_size = a2dp_total_size * 6 / 10;
    mfs->top_size = a2dp_total_size / 2;
    mfs->bottom_size = a2dp_total_size * 3 / 10;

    req.dec.cmd = AUDIO_DEC_START;
    server_request(mfs->audio_dec, AUDIO_REQ_DEC, &req);

    return mfs->audio_dec;
}

void earphone_a2dp_audio_codec_close(void)
{
    union audio_req req = {0};

    mfs->a2dp_open = 0;

    if (!mfs->audio_dec) {
        return;
    }

    req.dec.cmd = AUDIO_DEC_STOP;
    server_request(mfs->audio_dec, AUDIO_REQ_DEC, &req);

    server_close(mfs->audio_dec);
    media_stream_free();

    mfs->audio_dec = NULL;
    mfs->ready = 0;
}

#endif


#ifdef BT_PHONE_DECODE

extern void lmp_private_free_esco_packet(void *packet);
extern void *lmp_private_get_esco_packet(int *len, u32 *hash);
extern int lmp_private_send_esco_packet(void *priv, u8 *packet, int len);
extern u32 lmp_private_get_tx_remain_buffer(void);

struct bt_phone_vfs_handle {
    u8    state;
    u8    format;
    u8    mute;
    u8    lmp_open;
    void *p_addr;
    int   p_len;
    int   p_pos;
    int   frame_offset;
    int   err_cnt;
    int   frame_len;
    void *audio_dec;
    void *audio_enc;
};

static struct bt_phone_vfs_handle bt_phone_vfs_handle;
#define pfs      (&bt_phone_vfs_handle)


static u8 headcheck(u8 *buf)
{
    int sync_word = buf[0] | ((buf[1] & 0x0f) << 8);
    return (sync_word == 0x801) && (buf[2] == 0xAD);
}

static const u8 msbc_mute_data[] = {
    0xAD, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x00, 0x00, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76,
    0xDB, 0x6D, 0xDD, 0xB6, 0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76, 0xDB, 0x6D, 0xDD,
    0xB6, 0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76, 0xDB, 0x6D, 0xDD, 0xB6, 0xDB, 0x77,
    0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76, 0xDB, 0x6C, 0x00,
};

static void phone_stream_free(void)
{
    if (pfs->p_addr) {
        lmp_private_free_esco_packet(pfs->p_addr);
        pfs->p_addr = NULL;
        pfs->p_pos = 0;
        pfs->p_len = 0;
    }
}

static int bt_phone_file_read(void *priv, void *buf, u32 len)
{
    u32 r_len = 0;
    u32 offset = 0;
    u32 hash = 0;

    do {
        if (!pfs->p_addr) {
            if (pfs->mute) {
                pfs->p_addr = (void *)msbc_mute_data;
                pfs->p_len = sizeof(msbc_mute_data);
            } else {
                pfs->p_addr = (u8 *)lmp_private_get_esco_packet(&pfs->p_len, &hash);
            }
            if (!pfs->p_addr) {
                pfs->p_len = 0;
                if (!pfs->lmp_open) {
                    log_info("lmp file close");
                    return 0;
                } else {
#if 0
                    os_time_dly(1);
                    continue;
#else
                    return -1;
#endif
                }
            }
            pfs->p_pos = 0;
        }

        if (pfs->frame_offset == 0) {
            if (pfs->format == BT_AUDIO_FORMAT_MSBC) {
                u32 head_offset = 0;
                u8 *ptr = (u8 *)pfs->p_addr + pfs->p_pos;
                int check_len = pfs->p_len - pfs->p_pos;
                while (head_offset < check_len) {
                    if (ptr[head_offset] == 0xAD) {
                        if (head_offset >= 2) {
                            if (!headcheck(ptr + head_offset - 2)) {
                                /* put_buf(pfs->p_addr, pfs->p_len); */
                                pfs->err_cnt++;
                                head_offset++;
                                continue;
                            }
                        }
                        break;
                    }
                    pfs->err_cnt++;
                    head_offset++;
                }

                if (pfs->err_cnt >= pfs->frame_len + 2) {
                    phone_stream_free();
                    pfs->mute = 1;
                    pfs->err_cnt = 0;
                    continue;
                }

                if (pfs->p_pos >= pfs->p_len) {
                    phone_stream_free();
                    continue;
                }
                pfs->p_pos += head_offset;
                pfs->err_cnt = 0;
            } else if (pfs->format == BT_AUDIO_FORMAT_CVSD) {
                pfs->frame_len = pfs->p_len;
            }
        }

        r_len = pfs->frame_len - pfs->frame_offset;

        if (r_len > (len - offset)) {
            r_len = len - offset;
        }

        if (r_len > (pfs->p_len - pfs->p_pos)) {
            r_len = pfs->p_len - pfs->p_pos;
        }

        memcpy((u8 *)buf + offset, (u8 *)pfs->p_addr + pfs->p_pos, r_len);

        offset += r_len;
        pfs->p_pos += r_len;

        pfs->frame_offset += r_len;
        if (pfs->frame_offset >= pfs->frame_len) {
            pfs->frame_offset = 0;
        }

        if (pfs->p_pos >= pfs->p_len) {
            if (pfs->mute) {
                pfs->p_addr = NULL;
                pfs->p_len = 0;
                pfs->p_pos = 0;
                pfs->mute = 0;
            } else {
                phone_stream_free();
                break;
            }
        }
    } while (offset < len);

    if (offset < len) {
        putchar('K');
        return -1;	//????????????????????????????????????????????????????????????
    }

    return offset;
}

static int bt_phone_file_write(void *priv, void *buf, u32 len)
{
    if (!pfs->lmp_open) {
        return 0;
    }
    if (lmp_private_get_tx_remain_buffer() >= len + 28) {
        lmp_private_send_esco_packet(NULL, buf, len);
    }
    return len;
}

static int bt_phone_file_seek(void *priv, u32 offset, int orig)
{
    return 0;
}

static int bt_phone_file_len(void *priv)
{
    return -1;
}

static const struct audio_vfs_ops bt_phone_audio_dec_vfs_ops = {
    .fread = bt_phone_file_read,
    .fwrite = bt_phone_file_write,
    .fseek = bt_phone_file_seek,
    .flen  = bt_phone_file_len,
};


static void phone_call_dec_event_handler(void *priv, int argc, int *argv)
{
    switch (argv[0]) {
    /* case AUDIO_SERVER_EVENT_ERR: */
    case AUDIO_SERVER_EVENT_END:
        phone_call_end();
        break;
    default:
        break;
    }
}

static void *phone_decode_start(int format, u8 volume)
{
    union audio_req req = {0};

    pfs->audio_dec = server_open("audio_server", "dec");
    if (!pfs->audio_dec) {
        return NULL;
    }

    server_register_event_handler(pfs->audio_dec, NULL, phone_call_dec_event_handler);

    pfs->format = format;
    pfs->frame_offset = 0;
    pfs->err_cnt = 0;

    if (format == BT_AUDIO_FORMAT_MSBC) {
        req.dec.dec_type = "msbc";
        req.dec.sample_rate = 16000;
        pfs->frame_len = 58;
        pfs->mute = 0;
    } else if (format == BT_AUDIO_FORMAT_CVSD) {
        req.dec.dec_type = "cvsd";
        req.dec.sample_rate = 8000;
    }

    req.dec.cmd             = AUDIO_DEC_OPEN;
    req.dec.volume          = volume ? volume : get_app_music_volume();
    req.dec.output_buf      = NULL;
    req.dec.output_buf_len  = 1024 * 4;
    req.dec.channel         = 2;
    req.dec.priority        = 1;
    req.dec.vfs_ops         = &bt_phone_audio_dec_vfs_ops;
#if defined CONFIG_AUDIO_DEC_PLAY_SOURCE
    req.dec.sample_source   = CONFIG_AUDIO_DEC_PLAY_SOURCE;
#else
    req.dec.sample_source   = "dac";
#endif

    server_request(pfs->audio_dec, AUDIO_REQ_DEC, &req);

    req.dec.cmd = AUDIO_DEC_START;
    server_request(pfs->audio_dec, AUDIO_REQ_DEC, &req);

    return pfs->audio_dec;
}

static void phone_decode_close(void)
{
    union audio_req req = {0};

    if (!pfs->audio_dec) {
        return;
    }

    req.dec.cmd = AUDIO_DEC_STOP;
    server_request(pfs->audio_dec, AUDIO_REQ_DEC, &req);

    server_close(pfs->audio_dec);

    phone_stream_free();

    pfs->audio_dec = NULL;
}

static int phone_speak_start(int format, int packet_len)
{
    int err = 0;
    union audio_req req = {0};

    pfs->audio_enc = server_open("audio_server", "enc");
    if (!pfs->audio_enc) {
        return -1;
    }

    req.enc.cmd = AUDIO_ENC_OPEN;
    req.enc.channel = 1;
    req.enc.volume = 100;
    req.enc.output_buf = NULL;
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
    req.enc.sample_source = "plnk0";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK1
    req.enc.sample_source = "plnk1";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_IIS0
    req.enc.sample_source = "iis0";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_IIS1
    req.enc.sample_source = "iis1";
#else
    req.enc.sample_source = "mic";
#endif
    req.enc.vfs_ops = &bt_phone_audio_dec_vfs_ops;
#ifdef CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE
    req.enc.channel_bit_map = BIT(CONFIG_PHONE_CALL_ADC_CHANNEL);
#endif

    if (format == BT_AUDIO_FORMAT_MSBC) {
        req.enc.format = "msbc";
        req.enc.sample_rate = 16000;
        req.enc.output_buf_len = 512;
    } else if (format == BT_AUDIO_FORMAT_CVSD) {
        req.enc.format = "cvsd";
        req.enc.sample_rate = 8000;
        req.enc.frame_size = packet_len * 2;
        req.enc.output_buf_len = packet_len * 2 * 10;
    }

#if defined CONFIG_AEC_ENC_ENABLE && !defined CONFIG_FPGA_ENABLE

#define AEC_EN              BIT(0)
#define NLP_EN              BIT(1)
#define ANS_EN              BIT(2)

    /*aec module enable bit define*/
#define AEC_MODE_ADVANCE	(AEC_EN | NLP_EN | ANS_EN)
#define AEC_MODE_REDUCE		(NLP_EN | ANS_EN)
#define AEC_MODE_SIMPLEX	(ANS_EN)

    struct aec_s_attr aec_param = {0};
    aec_param.EnableBit = AEC_MODE_ADVANCE;
    /* aec_param.agc_en = 1; */
    /* aec_param.output_way = 1; */
    req.enc.aec_attr = &aec_param;
    req.enc.aec_enable = 1;

    extern void get_cfg_file_aec_config(struct aec_s_attr * aec_param);
    get_cfg_file_aec_config(&aec_param);

    if (aec_param.EnableBit == 0) {
        req.enc.aec_enable = 0;
        req.enc.aec_attr = NULL;
    }
    if (aec_param.EnableBit != AEC_MODE_ADVANCE) {
        aec_param.output_way = 0;
    }

#if defined CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE && defined CONFIG_AISP_LINEIN_ADC_CHANNEL && defined CONFIG_AEC_LINEIN_CHANNEL_ENABLE
    if (req.enc.aec_enable) {
        aec_param.output_way = 1;
        req.enc.channel_bit_map |= BIT(CONFIG_AISP_LINEIN_ADC_CHANNEL);
        if (CONFIG_AISP_LINEIN_ADC_CHANNEL < CONFIG_PHONE_CALL_ADC_CHANNEL) {
            req.enc.ch_data_exchange = 1;
        }
    }
#endif

    if (req.enc.sample_rate == 16000) {
        aec_param.wideband = 1;
        aec_param.hw_delay_offset = 50;
    } else {
        aec_param.wideband = 0;
        aec_param.hw_delay_offset = 75;
    }

#if CONFIG_AEC_SIMPLEX_ENABLE
#define AEC_SIMPLEX_TAIL 15 /*???????????????0?????????*/
    /*??????????????????CONST_AEC_SIMPLEX_THR,??????????????????????????????????????????????????????????????????????????????*/
#define AEC_SIMPLEX_THR		100000	/*default:260000*/

    aec_param.wn_en = 1;
    aec_param.simplex = 1;
    aec_param.dly_est = 1;
    aec_param.wn_gain = 331;
    aec_param.dst_delay = 50;
    aec_param.EnableBit = AEC_MODE_SIMPLEX;
    aec_param.SimplexTail = req.enc.sample_rate == 8000 ? AEC_SIMPLEX_TAIL / 2 : AEC_SIMPLEX_TAIL;
    aec_param.SimplexThr = AEC_SIMPLEX_THR;
#endif/*TCFG_AEC_SIMPLEX*/

#endif

    err = server_request(pfs->audio_enc, AUDIO_REQ_ENC, &req);

#if defined CONFIG_AEC_ENC_ENABLE && !defined CONFIG_FPGA_ENABLE
    if (aec_param.output_way) {
#ifdef CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE
        extern void adc_multiplex_set_gain(const char *source, u8 channel_bit_map, u8 gain);
        adc_multiplex_set_gain("mic", BIT(CONFIG_AISP_LINEIN_ADC_CHANNEL), CONFIG_AISP_LINEIN_ADC_GAIN * 2);
#endif
    }
#endif

    return err;
}

static int phone_speak_close(void)
{
    union audio_req req = {0};

    if (pfs->audio_enc) {
        req.enc.cmd = AUDIO_ENC_STOP;
        server_request(pfs->audio_enc, AUDIO_REQ_ENC, &req);
        server_close(pfs->audio_enc);
        pfs->audio_enc = NULL;
    }

    return 0;
}

void *phone_call_begin(void *priv, u8 volume)
{
    int err = 0;
    u32 esco_param = *(u32 *)priv;
    int esco_len = esco_param >> 16;
    int codec_type = esco_param & 0x000000ff;

    if (codec_type == 3) {
        log_info(">>sco_format:msbc");
        codec_type = BT_AUDIO_FORMAT_MSBC;
    } else if (codec_type == 2) {
        log_info(">>sco_format:cvsd");
        codec_type = BT_AUDIO_FORMAT_CVSD;
    } else {
        log_error("sco_format:error->please check %d", codec_type);
        return NULL;
    }

    pfs->lmp_open = 1;

    err = phone_speak_start(codec_type, esco_len);
    if (err) {
        goto __err;
    }

    return phone_decode_start(codec_type, volume);

__err:
    phone_call_end();
    return NULL;
}

void phone_call_end(void)
{
    pfs->lmp_open = 0;
    phone_speak_close();
    phone_decode_close();
}

#endif


#ifdef BT_EMITTER_ENABLE

#define BT_EMITTER_CBUF_SIZE	(512 * 4)

typedef struct sbc_param_str {
    unsigned int flags;

    u8 frequency;
    u8 blocks;
    u8 subbands;
    u8 mode;
    u8 allocation;
    u8 bitpool;
    u8 endian;
    void *priv;
    void *priv_alloc_base;
} sbc_t;

struct bt_emitter_vfs_handle {
    volatile u8 open;
    volatile u8 reading;
    volatile u8 wait_stop;
    spinlock_t lock;
    u8 suspend;
    u16 frame_size;
    OS_SEM sem;
    OS_SEM sync_sem;
    void *audio_enc;
    struct audio_cbuf_t *virtual;
    cbuffer_t cbuf;
    void *buf;
};

static struct bt_emitter_vfs_handle bt_emitter_vfs_handle;
#define efs      (&bt_emitter_vfs_handle)

#define BT_EMITTER_SYNC_TIMER JL_TIMER4

static const u8 sbc_mute_encode_data[] = {
    0x9C, 0xB9, 0x35, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x76, 0xDB, 0x6E,
    0xED, 0xB6, 0xDB, 0xBB, 0xB6, 0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDD, 0xB6, 0xDB, 0xBB, 0x6D, 0xB6,
    0xEE, 0xED, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x77, 0x6D, 0xB6, 0xEE, 0xDB, 0x6D, 0xBB, 0xBB, 0x6D,
    0xB7, 0x76, 0xDB, 0x6D, 0xDD, 0xDB, 0x6D, 0xBB, 0xB6, 0xDB, 0x6E, 0xEE, 0xDB, 0x6D, 0xDD, 0xB6,
    0xDB, 0x77, 0x76, 0xDB, 0x6E, 0xED, 0xB6, 0xDB, 0xBB, 0xB6, 0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDD,
    0xB6, 0xDB, 0xBB, 0x6D, 0xB6, 0xEE, 0xED, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x77, 0x6D, 0xB6, 0xEE,
    0xDB, 0x6D, 0xBB, 0xBB, 0x6D, 0xB7, 0x76, 0xDB, 0x6D, 0xDD, 0xDB, 0x6D, 0xBB, 0xB6, 0xDB, 0x6E,
    0xEE, 0xDB, 0x6D, 0xDD, 0xB6, 0xDB,
};

static int bt_emitter_vfs_fwrite(void *file, void *data, u32 len)
{
    cbuffer_t *cbuf = (cbuffer_t *)file;
    efs->frame_size = len;

    while (efs->open) {
        if (cbuf_write(cbuf, data, len) == len) {
            break;
        }
        os_sem_pend(&efs->sem, 0);
    }

    return len;
}

static int bt_emitter_vfs_fclose(void *file)
{
    return 0;
}

static const struct audio_vfs_ops bt_emitter_vfs_ops = {
    .fwrite = bt_emitter_vfs_fwrite,
    .fclose = bt_emitter_vfs_fclose,
};

___interrupt
static void bt_emitter_isr(void)
{
    BT_EMITTER_SYNC_TIMER->CON |= BIT(14);
    os_sem_post(&efs->sync_sem);
    if (efs->suspend) {
        efs->suspend = 0;
        extern void stack_run_loop_resume();
        stack_run_loop_resume();
    }
}

static u32 sbc_read_input(u8 *buf, u32 len)
{
    u32 rlen = 0;

    do {
        spin_lock(&efs->lock);
        if (!efs->open || !efs->virtual || efs->wait_stop) {
            efs->reading = 0;
            spin_unlock(&efs->lock);
            return 0;
        }
        efs->reading = 1;
        spin_unlock(&efs->lock);

        rlen = cbuf_read(efs->virtual->cbuf, buf, len);
        if (rlen == len) {
            break;
        }
        if (efs->virtual->end) {
            efs->virtual->end = 2;
            os_sem_post(efs->virtual->wr_sem);
            efs->reading = 0;
            return 0;
        }
        os_sem_pend(efs->virtual->rd_sem, 0);
    } while (!rlen);

    os_sem_post(efs->virtual->wr_sem);
    efs->reading = 0;

    return rlen;
}

int a2dp_sbc_encoder_init(void *sbc_struct)
{
    int ret = 0;
    union audio_req req = {0};
    sbc_t *sbc = (sbc_t *)sbc_struct;

    if (sbc) {
        log_info("audio bt emitter server open");

#ifdef CONFIG_WIFI_ENABLE
        switch_rf_coexistence_config_table(5);
#endif

        efs->buf = malloc(BT_EMITTER_CBUF_SIZE);
        if (!efs->buf) {
            return -1;
        }

        efs->audio_enc = server_open("audio_server", "enc");
        if (!efs->audio_enc) {
            free(efs->buf);
            efs->buf = NULL;
            return -1;
        }

        cbuf_init(&efs->cbuf, efs->buf, BT_EMITTER_CBUF_SIZE);
        os_sem_create(&efs->sem, 0);
        os_sem_create(&efs->sync_sem, 0);

        req.enc.cmd = AUDIO_ENC_OPEN;
        req.enc.channel = sbc->mode > 0 ? 2 : 1;
        req.enc.frame_size = (sbc->subbands ? 8 : 4) * (4 + (sbc->blocks * 4)) * req.enc.channel * 2;
        req.enc.output_buf_len = 3 * req.enc.frame_size;
        req.enc.format = "sbc";
        req.enc.sample_source = "virtual";
        req.enc.read_input = sbc_read_input;
        req.enc.vfs_ops = &bt_emitter_vfs_ops;
        req.enc.file = (FILE *)&efs->cbuf;
        req.enc.vir_data_wait = 1;
        req.enc.no_auto_start = 1;
        req.enc.bitrate = ((3 - sbc->blocks) << 28) & sbc->bitpool;
        if (sbc->endian) {
            req.enc.bitrate |= BIT(31);
        }
        if (!sbc->subbands) {
            req.enc.bitrate |= BIT(30);
        }
        if (sbc->frequency == SBC_FREQ_48000) {
            req.enc.sample_rate = 48000;
        } else if (sbc->frequency == SBC_FREQ_44100) {
            req.enc.sample_rate = 44100;
        } else if (sbc->frequency == SBC_FREQ_32000) {
            req.enc.sample_rate = 32000;
        } else {
            req.enc.sample_rate = 16000;
        }

        ret = server_request(efs->audio_enc, AUDIO_REQ_ENC, &req);
        if (!ret) {
            efs->open = 1;
        }

        BT_EMITTER_SYNC_TIMER->CON = BIT(14);
        BT_EMITTER_SYNC_TIMER->CNT = 0;
        BT_EMITTER_SYNC_TIMER->PRD = (u64)clk_get("osc") * (req.enc.frame_size * 5 / 2 / req.enc.channel) / req.enc.sample_rate;
        request_irq(IRQ_TIMER4_IDX, 1, bt_emitter_isr, 1);
        BT_EMITTER_SYNC_TIMER->CON = BIT(0) | BIT(3); //osc clk

        return ret;
    } else {
        log_info("audio bt emitter server close");

#ifdef CONFIG_WIFI_ENABLE
        switch_rf_coexistence_config_table(6);
#endif

        efs->open = 0;
        os_sem_set(&efs->sem, 0);
        os_sem_post(&efs->sem);
        BT_EMITTER_SYNC_TIMER->CON = 0;
        BT_EMITTER_SYNC_TIMER->CON = 0;
        os_sem_post(&efs->sync_sem);

        if (efs->audio_enc) {
            req.enc.cmd = AUDIO_ENC_CLOSE;
            server_request(efs->audio_enc, AUDIO_REQ_ENC, &req);
            server_close(efs->audio_enc);
            efs->audio_enc = NULL;
        }
        if (efs->buf) {
            cbuf_clear(&efs->cbuf);
            free(efs->buf);
            efs->buf = NULL;
        }
        efs->frame_size = 0;
        efs->suspend = 0;

        return 0;
    }
}

int a2dp_sbc_encoder_get_data(u8 *packet, u16 buf_len, int *frame_size)
{
    int number = 0;
    u32 rlen = 0;
    u32 data_size = cbuf_get_data_size(&efs->cbuf);

    if (!efs->open) {
        return 0;
    }

    if (0 != os_sem_accept(&efs->sync_sem)) {
        efs->suspend = 1;
        return 0;
    }

    if (!efs->frame_size || data_size < buf_len) {
        if (data_size > 0) {
            *frame_size = efs->frame_size;
            rlen = cbuf_read(&efs->cbuf, packet, data_size);
            while (buf_len - rlen > efs->frame_size) {
                memcpy(packet + rlen, sbc_mute_encode_data, sizeof(sbc_mute_encode_data));
                rlen += sizeof(sbc_mute_encode_data);
            }
            os_sem_set(&efs->sem, 0);
            os_sem_post(&efs->sem);
            putchar('N');
            return rlen;
        }
        number += buf_len / sizeof(sbc_mute_encode_data);
        *frame_size = sizeof(sbc_mute_encode_data);
        for (int i = 0; i < number; i++) {
            memcpy(&packet[sizeof(sbc_mute_encode_data) * i], sbc_mute_encode_data, sizeof(sbc_mute_encode_data));
        }
        return number * sizeof(sbc_mute_encode_data);
    }

    number = buf_len / efs->frame_size;  /*????????????*/
    *frame_size = efs->frame_size;

    rlen = cbuf_read(&efs->cbuf, packet, *frame_size * number);
    if (rlen == 0) {
        putchar('N');
    }
    os_sem_set(&efs->sem, 0);
    os_sem_post(&efs->sem);

    return rlen;
}

void *get_bt_emitter_audio_server(void)
{
    return efs->audio_enc;
}

void set_bt_emitter_virtual_hdl(void *virtual)
{
    spin_lock(&efs->lock);
    efs->wait_stop = virtual ? 0 : 1;
    while (efs->reading) {
        spin_unlock(&efs->lock);
        if (!virtual && efs->virtual && efs->virtual->rd_sem) {
            os_sem_post(efs->virtual->rd_sem);
        }
        os_time_dly(1);
        spin_lock(&efs->lock);
    }
    efs->virtual = virtual;
    spin_unlock(&efs->lock);
}

u8 bt_emitter_stu_get(void)
{
    return efs->open;
}

#endif

#endif
