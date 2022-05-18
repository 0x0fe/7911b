#include "system/includes.h"
#include "device/device.h"
#include "app_config.h"

/*************************************本文件串口测试，可以使能接收和发送进行两个板子对接测试*********************************************************/

const unsigned char file1_table4k[4096]={

0x29,0x23,0xBE,0x84,0xE1,0x6C,0xD6,0xAE,0x52,0x90,0x49,0xF1,0xF1,0xBB,0xE9,0xEB,0xB3,0xA6,0xDB,0x3C,0x87,0x0C,0x3E,0x99,0x24,0x5E,0x0D,0x1C,0x06,0xB7,0x47,0xDE,

0xB3,0x12,0x4D,0xC8,0x43,0xBB,0x8B,0xA6,0x1F,0x03,0x5A,0x7D,0x09,0x38,0x25,0x1F,0x5D,0xD4,0xCB,0xFC,0x96,0xF5,0x45,0x3B,0x13,0x0D,0x89,0x0A,0x1C,0xDB,0xAE,0x32,

0x20,0x9A,0x50,0xEE,0x40,0x78,0x36,0xFD,0x12,0x49,0x32,0xF6,0x9E,0x7D,0x49,0xDC,0xAD,0x4F,0x14,0xF2,0x44,0x40,0x66,0xD0,0x6B,0xC4,0x30,0xB7,0x32,0x3B,0xA1,0x22,

0xF6,0x22,0x91,0x9D,0xE1,0x8B,0x1F,0xDA,0xB0,0xCA,0x99,0x02,0xB9,0x72,0x9D,0x49,0x2C,0x80,0x7E,0xC5,0x99,0xD5,0xE9,0x80,0xB2,0xEA,0xC9,0xCC,0x53,0xBF,0x67,0xD6,

0xBF,0x14,0xD6,0x7E,0x2D,0xDC,0x8E,0x66,0x83,0xEF,0x57,0x49,0x61,0xFF,0x69,0x8F,0x61,0xCD,0xD1,0x1E,0x9D,0x9C,0x16,0x72,0x72,0xE6,0x1D,0xF0,0x84,0x4F,0x4A,0x77,

0x02,0xD7,0xE8,0x39,0x2C,0x53,0xCB,0xC9,0x12,0x1E,0x33,0x74,0x9E,0x0C,0xF4,0xD5,0xD4,0x9F,0xD4,0xA4,0x59,0x7E,0x35,0xCF,0x32,0x22,0xF4,0xCC,0xCF,0xD3,0x90,0x2D,

0x48,0xD3,0x8F,0x75,0xE6,0xD9,0x1D,0x2A,0xE5,0xC0,0xF7,0x2B,0x78,0x81,0x87,0x44,0x0E,0x5F,0x50,0x00,0xD4,0x61,0x8D,0xBE,0x7B,0x05,0x15,0x07,0x3B,0x33,0x82,0x1F,

0x18,0x70,0x92,0xDA,0x64,0x54,0xCE,0xB1,0x85,0x3E,0x69,0x15,0xF8,0x46,0x6A,0x04,0x96,0x73,0x0E,0xD9,0x16,0x2F,0x67,0x68,0xD4,0xF7,0x4A,0x4A,0xD0,0x57,0x68,0x76,

0xFA,0x16,0xBB,0x11,0xAD,0xAE,0x24,0x88,0x79,0xFE,0x52,0xDB,0x25,0x43,0xE5,0x3C,0xF4,0x45,0xD3,0xD8,0x28,0xCE,0x0B,0xF5,0xC5,0x60,0x59,0x3D,0x97,0x27,0x8A,0x59,

0x76,0x2D,0xD0,0xC2,0xC9,0xCD,0x68,0xD4,0x49,0x6A,0x79,0x25,0x08,0x61,0x40,0x14,0xB1,0x3B,0x6A,0xA5,0x11,0x28,0xC1,0x8C,0xD6,0xA9,0x0B,0x87,0x97,0x8C,0x2F,0xF1,

0x15,0x1D,0x9A,0x95,0xC1,0x9B,0xE1,0xC0,0x7E,0xE9,0xA8,0x9A,0xA7,0x86,0xC2,0xB5,0x54,0xBF,0x9A,0xE7,0xD9,0x23,0xD1,0x55,0x90,0x38,0x28,0xD1,0xD9,0x6C,0xA1,0x66,

0x5E,0x4E,0xE1,0x30,0x9C,0xFE,0xD9,0x71,0x9F,0xE2,0xA5,0xE2,0x0C,0x9B,0xB4,0x47,0x65,0x38,0x2A,0x46,0x89,0xA9,0x82,0x79,0x7A,0x76,0x78,0xC2,0x63,0xB1,0x26,0xDF,

0xDA,0x29,0x6D,0x3E,0x62,0xE0,0x96,0x12,0x34,0xBF,0x39,0xA6,0x3F,0x89,0x5E,0xF1,0x6D,0x0E,0xE3,0x6C,0x28,0xA1,0x1E,0x20,0x1D,0xCB,0xC2,0x03,0x3F,0x41,0x07,0x84,

0x0F,0x14,0x05,0x65,0x1B,0x28,0x61,0xC9,0xC5,0xE7,0x2C,0x8E,0x46,0x36,0x08,0xDC,0xF3,0xA8,0x8D,0xFE,0xBE,0xF2,0xEB,0x71,0xFF,0xA0,0xD0,0x3B,0x75,0x06,0x8C,0x7E,

0x87,0x78,0x73,0x4D,0xD0,0xBE,0x82,0xBE,0xDB,0xC2,0x46,0x41,0x2B,0x8C,0xFA,0x30,0x7F,0x70,0xF0,0xA7,0x54,0x86,0x32,0x95,0xAA,0x5B,0x68,0x13,0x0B,0xE6,0xFC,0xF5,

0xCA,0xBE,0x7D,0x9F,0x89,0x8A,0x41,0x1B,0xFD,0xB8,0x4F,0x68,0xF6,0x72,0x7B,0x14,0x99,0xCD,0xD3,0x0D,0xF0,0x44,0x3A,0xB4,0xA6,0x66,0x53,0x33,0x0B,0xCB,0xA1,0x10,

0x5E,0x4C,0xEC,0x03,0x4C,0x73,0xE6,0x05,0xB4,0x31,0x0E,0xAA,0xAD,0xCF,0xD5,0xB0,0xCA,0x27,0xFF,0xD8,0x9D,0x14,0x4D,0xF4,0x79,0x27,0x59,0x42,0x7C,0x9C,0xC1,0xF8,

0xCD,0x8C,0x87,0x20,0x23,0x64,0xB8,0xA6,0x87,0x95,0x4C,0xB0,0x5A,0x8D,0x4E,0x2D,0x99,0xE7,0x3D,0xB1,0x60,0xDE,0xB1,0x80,0xAD,0x08,0x41,0xE9,0x67,0x41,0xA5,0xD5,

0x9F,0xE4,0x18,0x9F,0x15,0x42,0x00,0x26,0xFE,0x4C,0xD1,0x21,0x04,0x93,0x2F,0xB3,0x8F,0x73,0x53,0x40,0x43,0x8A,0xAF,0x7E,0xCA,0x6F,0xD5,0xCF,0xD3,0xA1,0x95,0xCE,

0x5A,0xBE,0x65,0x27,0x2A,0xF6,0x07,0xAD,0xA1,0xBE,0x65,0xA6,0xB4,0xC9,0xC0,0x69,0x32,0x34,0x09,0x2C,0x4D,0x01,0x8F,0x17,0x56,0xC6,0xDB,0x9D,0xC8,0xA6,0xD8,0x0B,

0x88,0x81,0x38,0x61,0x6B,0x68,0x12,0x62,0xF9,0x54,0xD0,0xE7,0x71,0x17,0x48,0x78,0x0D,0x92,0x29,0x1D,0x86,0x29,0x99,0x72,0xDB,0x74,0x1C,0xFA,0x4F,0x37,0xB8,0xB5,

0xB0,0x95,0x57,0xF5,0xDF,0x80,0x6C,0x6D,0x8D,0x74,0xD9,0x8B,0x43,0x65,0x11,0x08,0xA5,0xF6,0x79,0xBD,0xF7,0xEB,0x15,0xB8,0xE0,0xE1,0x60,0x8F,0x6E,0x3C,0x7B,0xF4,

0x5B,0x62,0x8A,0x8A,0x8F,0x27,0x5C,0xF7,0xE5,0x87,0x4A,0x3B,0x32,0x9B,0x61,0x40,0x84,0xC6,0xC3,0xB1,0xA7,0x30,0x4A,0x10,0xEE,0x75,0x6F,0x03,0x2F,0x9E,0x6A,0xEF,

0x10,0x50,0x9B,0xC8,0x81,0x43,0x29,0x28,0x8A,0xF6,0xE9,0x9E,0x47,0xA1,0x81,0x48,0x31,0x6C,0xCD,0xA4,0x9E,0xDE,0x81,0xA3,0x8C,0x98,0x10,0xFF,0x9A,0x43,0xCD,0xCF,

0x57,0xC7,0x50,0x59,0xBF,0xBD,0x1C,0x27,0x03,0x28,0x7F,0x5D,0x89,0x5F,0xB9,0x49,0x34,0x4E,0x60,0x3C,0xE5,0xDE,0x02,0x98,0x42,0xB2,0x0D,0x2B,0xB6,0x14,0xEC,0xBB,

0xB8,0x2F,0x73,0xE2,0x51,0x7E,0x7D,0x1D,0xD8,0x84,0xD3,0x1F,0x01,0xBE,0x50,0x6B,0x16,0xD6,0x43,0x21,0x83,0x19,0x15,0x18,0x98,0x2B,0x2C,0x2E,0x8B,0xF9,0x0E,0xDC,

0xBC,0xF0,0xCA,0x0E,0x3D,0x6D,0x94,0x31,0x92,0x74,0xAF,0x8D,0xB5,0xA4,0x90,0xD5,0x5E,0x6A,0x40,0xFC,0x80,0x76,0x02,0x4B,0x17,0x6B,0x36,0xB1,0x21,0xDB,0x7D,0x5A,

0xEA,0x72,0x1E,0x82,0x8D,0x71,0xA8,0x8C,0xB8,0x5E,0xD9,0x4E,0xAF,0xFA,0xBF,0xB0,0x94,0x74,0x1D,0x75,0xE5,0xDC,0x10,0x58,0x46,0xDA,0xF2,0x5B,0x81,0xA0,0x7F,0x5C,

0xCB,0x1D,0x36,0xE9,0x49,0x74,0x02,0x55,0xD2,0xAC,0x1A,0x0B,0xF7,0xA9,0x26,0x23,0x40,0x5B,0xA3,0x33,0xB9,0x35,0x88,0x68,0xAD,0xE1,0x2A,0xD5,0xB2,0x32,0x5D,0x0A,

0xE5,0x5A,0xDC,0xE9,0x77,0x5D,0xEB,0xB5,0x69,0xC5,0x3A,0x6C,0x93,0x98,0x0D,0x57,0xEB,0x87,0x9A,0xDF,0x04,0x68,0xB2,0xA2,0xD5,0xE6,0xA4,0xC6,0xBC,0x77,0x5F,0x8D,

0xC3,0x8F,0xD6,0x2A,0x21,0x14,0xA9,0xD4,0x04,0x11,0x01,0x18,0x8D,0xAE,0xBB,0x73,0x1C,0x60,0xCA,0x20,0xCF,0x5D,0xD6,0x2F,0x45,0x53,0x29,0xD7,0xA8,0x59,0xCC,0x0D,

0xEA,0x26,0xED,0x55,0x4E,0x80,0x84,0xD9,0x2B,0xF8,0x37,0xB8,0xED,0xD5,0x7A,0xA0,0x5C,0x4E,0xFA,0x9F,0x21,0xFC,0x3C,0x36,0x85,0x8E,0x81,0xB0,0x7D,0xBF,0xEE,0xB1,

0xE4,0x85,0xE9,0x12,0x07,0x8B,0xC6,0xEC,0x66,0xE2,0xA3,0xF3,0xB9,0xF4,0x90,0x06,0x32,0xB9,0xF4,0x04,0x02,0x2D,0x2C,0xE0,0x1E,0x01,0x74,0xF8,0x43,0x90,0x0B,0xA2,

0x37,0x16,0x92,0x08,0x53,0x1C,0xB5,0x37,0x3E,0x37,0x0E,0x72,0xFB,0xF2,0x46,0xCC,0x26,0x09,0x7E,0xF6,0x7A,0xD8,0xED,0x55,0x97,0x12,0xCA,0x57,0x02,0xB6,0x6C,0x08,

0x6E,0x3F,0xB0,0xE0,0x3A,0x1C,0x9B,0xDF,0x3A,0x5F,0x40,0xDD,0xBA,0xB9,0xE4,0x1B,0xC0,0xA6,0x61,0x1D,0x92,0xE5,0xC8,0xBC,0x78,0x2B,0x4A,0x77,0xC3,0x18,0x59,0x0A,

0x0E,0x6A,0x0A,0x42,0xC4,0x71,0xBE,0x0F,0xE3,0xC3,0x01,0xDB,0xFF,0x30,0xB2,0x1A,0x89,0xF8,0x65,0x22,0x51,0x3C,0x05,0x3D,0x4A,0xB3,0xBE,0xFD,0x8D,0x9E,0x19,0xD0,

0xA1,0xFE,0x69,0xD4,0xFA,0x04,0x67,0xED,0xBF,0xC9,0x19,0x14,0xD1,0x3F,0xF8,0xF2,0x08,0x68,0x51,0xAD,0xC0,0xC5,0xEC,0x02,0x94,0x12,0xEC,0x94,0x69,0x30,0xF6,0x83,

0xAF,0x63,0x95,0x41,0xE3,0xBD,0xDD,0xA1,0x58,0xDA,0x4F,0x31,0x38,0xCE,0xFD,0xCA,0xC5,0x5C,0xEF,0x65,0xE5,0x69,0xC4,0x30,0xDE,0xAF,0x9C,0xE2,0x5E,0xB6,0x36,0x4B,

0xBE,0x00,0x56,0x2F,0x87,0x84,0x6A,0x54,0x36,0x5F,0x6C,0xDA,0x3C,0xC5,0x0A,0xCB,0x49,0x3D,0x05,0xF3,0xCA,0x0E,0xD7,0xF2,0xB1,0xF4,0x98,0x8F,0x74,0x18,0x22,0x4F,

0x58,0x3F,0x74,0x47,0xEF,0x42,0x54,0x2E,0xDF,0xBE,0x39,0xB6,0xE6,0x0C,0x67,0x1C,0x1B,0x74,0x5C,0xFE,0x77,0x9D,0x6B,0x6E,0x93,0x48,0xA7,0x44,0xB3,0x3E,0x02,0xB8,

0x04,0x87,0xB6,0x30,0x22,0xDD,0xE4,0x56,0xDD,0x61,0x7B,0x6E,0x3D,0x8C,0x5C,0xE7,0xC4,0x67,0xBC,0x2F,0xF2,0xFF,0xC9,0xCC,0x0F,0x13,0x90,0xA9,0x24,0x11,0x1E,0xAD,

0x4B,0x41,0xE5,0x92,0x28,0x3F,0x62,0xF5,0xB8,0xAE,0xFD,0xA9,0x49,0x2B,0x31,0x51,0xCA,0x80,0xEC,0x2E,0x45,0x1A,0x39,0x35,0xAA,0xBE,0x1C,0x65,0xCE,0x77,0xBD,0x57,

0xB3,0xD3,0xC9,0x17,0x0A,0x4E,0x16,0x32,0xF6,0x0F,0x86,0x10,0x13,0xD2,0x2D,0x84,0xB7,0x26,0xB6,0xA2,0x78,0xD8,0x02,0xD1,0xEE,0xAF,0x13,0x21,0xBA,0x59,0x29,0xDE,

0xC6,0xA6,0x2A,0x65,0xCF,0xF4,0x47,0x36,0x21,0xEB,0xDD,0x4B,0xA2,0x6A,0x99,0xA8,0x12,0xC0,0xE0,0x34,0x91,0x20,0x6E,0xC7,0x62,0x4F,0x3D,0x84,0xEE,0xA0,0xA8,0x69,

0x0C,0x22,0xD0,0x24,0x7F,0x18,0x3F,0x28,0xC0,0xA9,0xCB,0x01,0xFF,0xD9,0xBD,0xE4,0x64,0xB8,0x33,0x8B,0x9A,0xD9,0xC3,0x40,0x8E,0x06,0x61,0x37,0x74,0x32,0x83,0x20,

0x0C,0xAF,0x83,0xFD,0x23,0xA2,0x44,0x31,0x5C,0xB3,0x18,0xDA,0x30,0x09,0xE1,0x61,0x34,0x75,0x77,0x50,0x9A,0xED,0x4A,0x63,0xFB,0x3D,0x49,0xE1,0x54,0xF9,0x02,0x2C,

0x4E,0xB6,0x0A,0x97,0xC2,0x79,0x9F,0x79,0x7C,0x70,0x8C,0x80,0x3F,0xE0,0x4D,0x47,0x0A,0x5F,0x74,0x2A,0x9A,0x43,0x4B,0x59,0x30,0x5A,0xBB,0x2B,0x94,0xDC,0x6C,0xB5,

0x5A,0x9D,0x2E,0x9C,0x64,0x87,0x98,0x27,0xA8,0x48,0xEE,0x98,0x33,0x48,0x48,0xBD,0x6E,0xDE,0xF1,0xC2,0xA1,0xC3,0x0E,0x49,0xB5,0xC7,0x80,0xBC,0x3E,0xC2,0x0B,0xE2,

0xB9,0xCE,0xB7,0xB1,0x11,0xB3,0x76,0x64,0x69,0xA3,0x08,0xCD,0x15,0x28,0x1C,0xEB,0xE9,0x5A,0xB8,0xBF,0xB7,0x55,0xDA,0x5C,0x13,0xEA,0x60,0x3E,0x59,0x95,0x25,0xDC,

0xF2,0xAF,0x6D,0x80,0xD2,0xE5,0x83,0x57,0x45,0xE9,0xA0,0xC4,0xEC,0x67,0x0F,0xFA,0x02,0x3B,0x8F,0xCA,0xE4,0xE1,0xF9,0xB9,0xD1,0x2D,0x22,0x56,0xEE,0x3C,0x03,0xCB,

0x8D,0xAA,0x17,0xB1,0xAE,0x05,0x05,0x29,0xC6,0x82,0x7F,0x28,0xC0,0xEF,0x6A,0x12,0x42,0xE9,0x3F,0x8B,0x31,0x4F,0xB1,0x8A,0x77,0xF7,0x90,0xAE,0x04,0x9F,0xED,0xD6,

0x12,0x26,0x7F,0xEC,0xAE,0xFC,0x45,0x01,0x74,0xD7,0x6D,0x9F,0x9A,0xA7,0x75,0x5A,0x30,0xCD,0x90,0xA9,0xA5,0x87,0x4B,0xF4,0x8E,0xAF,0x70,0xEE,0xA3,0xA6,0x2A,0x25,

0x0A,0x8B,0x6B,0xD8,0xD9,0xB0,0x8B,0x08,0xD6,0x4E,0x32,0xD1,0x81,0x77,0x77,0xFB,0x54,0x4D,0x49,0xCD,0x49,0x72,0x0E,0x21,0x9D,0xBF,0x8B,0xBE,0xD3,0x39,0x04,0xE1,

0xFD,0x40,0xA4,0x1D,0x37,0x0A,0x1F,0x65,0x74,0x50,0x95,0x68,0x7D,0x47,0xBA,0x1D,0x36,0xD2,0x34,0x9E,0x23,0xF6,0x44,0x39,0x2C,0x8E,0xA9,0xC4,0x9D,0x40,0xC1,0x32,

0x71,0xAF,0xF2,0x64,0xD0,0xF2,0x48,0x41,0xD6,0x46,0x5F,0x09,0x96,0xFF,0x84,0xE6,0x5F,0xC5,0x17,0xC5,0x3E,0xFC,0x33,0x63,0xC3,0x84,0x92,0xAB,0x08,0xA3,0xAA,0x3F,

0xF0,0x3F,0x1C,0x55,0xAD,0x51,0x4F,0xC4,0x85,0x96,0x58,0x5E,0xD5,0x88,0x1E,0x81,0x56,0x8C,0xBB,0xE9,0x9F,0x6D,0x25,0xC8,0xEB,0x09,0x0D,0x19,0x1D,0x4A,0x07,0x31,

0x01,0x58,0xEC,0x97,0xD5,0x0D,0x7C,0x15,0x08,0xAA,0x48,0x0F,0x41,0xC8,0xD0,0x14,0xA3,0x91,0xE8,0xB3,0x50,0x2F,0x60,0x90,0x2B,0x85,0xE3,0xB7,0xE3,0x1D,0x20,0x2F,

0x2D,0x62,0x28,0xD3,0x50,0x10,0x17,0x5D,0xE7,0xE8,0xF7,0xC4,0xE2,0xA8,0xE1,0xC8,0xCF,0x3A,0x65,0xCA,0x58,0x2C,0x2D,0xE2,0x0C,0x60,0xDC,0x2C,0x62,0x05,0x3C,0x62,

0xFA,0xC5,0x99,0xB0,0x27,0x40,0x68,0xC3,0xAB,0xBA,0x2D,0x24,0xC1,0x10,0x9B,0xC4,0x61,0xF1,0xFC,0xD8,0xBF,0x4A,0xD3,0xE6,0x15,0x02,0xC0,0x20,0xA2,0xE8,0xA5,0xF2,

0xF2,0xEA,0x07,0xD7,0x61,0x87,0xB7,0x70,0xDB,0x87,0xB1,0xD7,0xE5,0xE9,0x44,0x31,0xE1,0x1D,0x73,0x82,0x8D,0x73,0x9C,0xC6,0xCE,0xD4,0x57,0x3D,0xAC,0xB0,0xA1,0x06,

0x9D,0x37,0x3A,0xEF,0x06,0xCC,0x4B,0x8C,0xFF,0xB6,0x4C,0x86,0x57,0x19,0x25,0x36,0xD7,0x25,0x93,0x72,0xCB,0x8E,0xCE,0xA7,0xBF,0x3C,0x69,0x28,0x87,0x43,0x79,0xC6,

0x82,0x15,0xF9,0xA1,0x1F,0xF7,0x6D,0x3E,0x9F,0xB1,0xC6,0xD9,0x1D,0x8A,0x86,0xFC,0xCC,0x73,0x24,0x50,0x81,0x83,0xB2,0xB4,0x71,0xA3,0xBD,0x8C,0x3B,0x8B,0x75,0x5B,

0x29,0xED,0x0D,0x95,0xB2,0xEF,0x65,0xAE,0x44,0xDF,0xE7,0x77,0x41,0x22,0xAF,0xAA,0x48,0x6E,0xEE,0xC3,0xB5,0x3A,0x90,0x12,0x6B,0x72,0x1C,0x0F,0xD1,0x6E,0xDC,0xED,

0x1A,0x25,0x3F,0x72,0xCA,0x9E,0x7B,0x05,0x75,0xA8,0x76,0x09,0xCA,0xCB,0xE7,0x69,0xD1,0x7E,0xB8,0x74,0x71,0x9A,0xB0,0xEC,0x35,0x0F,0x4E,0x59,0x50,0xD6,0xF8,0xA3,

0xDE,0x27,0x55,0xE0,0x6C,0xEB,0xF7,0x6B,0xBB,0x74,0x3D,0x36,0xC1,0x6C,0x77,0x61,0xF1,0x0B,0x4C,0x0B,0xBC,0x8D,0x59,0x68,0x58,0xE4,0x1B,0x13,0xC0,0xAA,0x0F,0xA6,

0xFC,0x59,0x17,0x89,0xA2,0xBE,0x20,0x07,0x9D,0xAB,0x02,0xA7,0x2D,0xED,0xA7,0xB9,0x2F,0x7D,0x70,0x2F,0x9F,0xFB,0xD4,0xAE,0x5B,0x57,0x4B,0xE5,0x29,0xD2,0x6A,0x1E,

0xFC,0x25,0x4F,0x12,0x73,0xFF,0x3F,0x02,0xA3,0xB5,0x8E,0x03,0x16,0x35,0xBF,0x9A,0x13,0x3D,0xED,0x88,0x20,0xCA,0x6A,0xE7,0xC6,0xD2,0xA5,0x76,0x94,0x35,0x51,0x32,

0x67,0xF2,0xC3,0x26,0xE8,0x96,0x9C,0x83,0x55,0xFB,0xA9,0xF3,0x85,0x2E,0x07,0x2B,0x26,0xB1,0x8B,0xBF,0x4A,0xE3,0x60,0x3B,0x22,0xBC,0xF2,0x6F,0x08,0xBD,0x0B,0x0A,

0xC3,0x27,0x3C,0x6B,0x07,0x6C,0x7F,0xB3,0x3C,0xE3,0x1A,0x1F,0x80,0xBF,0xC7,0x95,0xEF,0x42,0x11,0x7C,0x22,0x2E,0x01,0xD1,0xF5,0x7D,0xFA,0x77,0x8E,0x51,0xE2,0xCF,

0x9B,0x2E,0x82,0x89,0xDA,0x67,0x30,0xBA,0xDF,0xD6,0xAB,0x2E,0x12,0xD0,0x46,0xFF,0xF7,0x58,0x49,0x66,0xB2,0x93,0x94,0xD2,0xCA,0x7D,0x85,0x37,0x2D,0xD9,0x1C,0xA9,

0x74,0x6E,0x5D,0x29,0x69,0x70,0xF6,0xBF,0xC6,0x3D,0x22,0xC9,0x40,0x4A,0xCD,0x92,0xC4,0x5C,0xF9,0x26,0x01,0xFB,0x60,0x65,0x26,0x23,0x5B,0x57,0xED,0x3E,0x02,0xBF,

0xD8,0x4F,0x95,0xF3,0xBA,0x70,0x1B,0xEA,0x7A,0x7E,0x48,0x97,0x14,0x13,0xA4,0x75,0xE0,0xB4,0xEA,0x64,0x17,0x4C,0xAE,0xB3,0x93,0xD9,0x43,0x7E,0xD6,0x66,0xDC,0x3A,

0x4E,0x39,0xF1,0x8E,0xD8,0x4D,0xE5,0x64,0x82,0x03,0xE5,0x41,0x95,0x14,0x13,0xD1,0xD3,0xCA,0xE4,0xC7,0xFD,0x70,0xC6,0xE4,0x98,0x07,0x07,0x54,0xF1,0x3A,0xF2,0x41,

0x5F,0x94,0x3B,0xA3,0xC8,0xF1,0x9D,0x55,0x66,0x32,0xC1,0x6E,0xCB,0x35,0x62,0xCE,0x23,0x05,0xAF,0xF7,0xBA,0x4D,0xF0,0x1F,0xBE,0x13,0x6D,0x83,0x45,0xA2,0x8B,0xFD,

0x91,0xC8,0x39,0xD9,0x93,0x42,0x8A,0xE5,0xAF,0x75,0xA3,0xC7,0xBF,0x5F,0xD8,0x93,0x5A,0xCC,0x12,0x9D,0x56,0xCD,0x74,0x8C,0x8B,0x66,0x3E,0xB0,0xDA,0x87,0xF1,0x96,

0x6F,0x3D,0xB4,0xD9,0x42,0x2A,0xF6,0x3A,0xE4,0x33,0x55,0xF3,0x78,0x78,0xBE,0x49,0xFF,0x89,0xD6,0x61,0xDA,0xD7,0x99,0x54,0x89,0x68,0x41,0x85,0xB9,0xCF,0x6A,0x33,

0x7E,0x5C,0x73,0x4A,0xDD,0x90,0x27,0x7F,0x8D,0xD3,0x9D,0x9B,0xFE,0x69,0x5C,0x17,0x9B,0xA3,0xC3,0xEA,0x4D,0x52,0xA9,0x9F,0x40,0x81,0x40,0xAA,0xE9,0x64,0x3F,0xFC,

0x48,0x8B,0x40,0xD5,0x6A,0x5B,0x66,0xDA,0x33,0xBF,0x44,0x67,0x5A,0x1B,0xFA,0x26,0xB5,0x82,0xA1,0xE1,0xB7,0x28,0xEA,0x94,0x37,0x1A,0x01,0xC7,0x72,0x2C,0xB8,0x1A,

0x54,0x33,0xE1,0x22,0xF3,0x75,0xFB,0x73,0x5D,0x5E,0x12,0xFE,0x93,0x75,0xE0,0x9E,0xD5,0x8E,0x38,0xED,0x20,0x40,0xA5,0x5C,0xF6,0x99,0x4E,0x83,0x5D,0x11,0x1C,0xB5,

0x2A,0xBD,0x1F,0xD8,0x7F,0xC5,0x2E,0x73,0x93,0x18,0xCE,0x09,0xB1,0x5E,0x56,0xA6,0x84,0x2F,0x50,0xB7,0x91,0x82,0x21,0x1E,0x05,0x68,0xED,0x86,0xB1,0xFA,0xB5,0xF4,

0x53,0x8F,0xC2,0x9F,0x17,0x33,0x47,0x02,0x5D,0x55,0x42,0x2F,0xBD,0xC0,0xA3,0x66,0x48,0xCD,0xB0,0xE6,0x11,0xD6,0xA8,0x03,0xED,0xED,0xA6,0x79,0x76,0xCE,0xC9,0x00,

0x56,0x13,0x92,0x21,0xC2,0xA7,0x8D,0x47,0x44,0x7D,0x34,0x19,0xBE,0x82,0x10,0x07,0xAC,0xD0,0x21,0x23,0xA9,0x24,0x80,0x33,0x35,0x92,0x43,0x04,0xB5,0x77,0xA1,0x01,

0xBB,0xB0,0x57,0x03,0x88,0x09,0x49,0x6B,0xCF,0xF8,0x6D,0x6F,0xBC,0x8C,0xE5,0xB1,0x35,0xA0,0x6B,0x16,0x60,0x54,0xF2,0xD5,0x65,0xBE,0x8A,0xCE,0x75,0xDC,0x85,0x1E,

0x0B,0xCD,0xD8,0xF0,0x71,0x41,0xC4,0x95,0x87,0x2F,0xB5,0xD8,0xC0,0xC6,0x6A,0x8B,0x6D,0xA5,0x56,0x66,0x3E,0x4E,0x46,0x12,0x05,0xD8,0x45,0x80,0xBE,0xE5,0xBC,0x7F,

0xCD,0xD4,0xDE,0x8E,0x86,0x38,0x43,0xEE,0xF2,0x88,0xD3,0xFC,0xD0,0x18,0xE6,0xBE,0xDB,0x47,0xAA,0xBC,0x4B,0xFA,0xC4,0x11,0x9E,0x4A,0x3A,0xC1,0x98,0x7A,0x90,0x4D,

0x89,0x2C,0x31,0x85,0xCE,0xD4,0x11,0x9E,0x9A,0x6C,0x91,0x84,0xF7,0x6A,0xA3,0x71,0x07,0xEF,0x2E,0xBF,0x90,0x41,0xB4,0xFB,0xB7,0x7B,0x32,0x3A,0x0C,0x83,0x47,0xB0,

0xC7,0x3D,0x99,0x7E,0x51,0xFE,0x75,0xCC,0x07,0x44,0xB5,0x18,0x3A,0xA4,0xE7,0xCD,0x7A,0x03,0xAB,0x18,0x14,0x09,0x5D,0xF7,0xD9,0xD3,0xF4,0x93,0x21,0xE8,0x2A,0xCF,

0x10,0x6F,0xDE,0x21,0x18,0x9F,0xB6,0xA1,0xBF,0x76,0x08,0x5F,0xA3,0xAE,0xFB,0xFA,0xBB,0xED,0xE9,0x6E,0xDF,0x3C,0x08,0x2E,0x8B,0xBA,0x4A,0x73,0xE0,0x91,0x81,0xD3,

0xEB,0x2A,0xC7,0x15,0x2A,0x9D,0x1D,0x44,0x4C,0x6C,0x52,0x02,0x39,0x70,0x27,0x1F,0x52,0x13,0xB0,0x6A,0xFA,0xC0,0xFD,0xC7,0x55,0x98,0xFA,0x82,0x4F,0x67,0x94,0xE3,

0xE1,0xD6,0x1D,0x03,0x90,0xE2,0xF2,0xDD,0x36,0x8C,0x5B,0xA8,0x04,0xD2,0xB2,0x65,0xC8,0xDF,0xC8,0xB4,0x6C,0x7F,0x84,0xEB,0xBF,0xD5,0xCD,0x69,0x79,0x50,0xAA,0x28,

0x79,0xDB,0xA8,0x92,0x50,0x54,0x7D,0x96,0x03,0x40,0xEA,0xFA,0x0D,0xBD,0xE6,0xF3,0xA4,0xB7,0xF8,0xF3,0x3D,0x5F,0xE5,0xC2,0x52,0xD9,0x8B,0xD0,0x63,0x35,0x0D,0xCA,

0x3B,0xA1,0x30,0x6B,0x74,0xDD,0x05,0x95,0x3D,0xEF,0xC9,0xA0,0x5B,0x17,0x09,0xF2,0x6E,0x05,0x09,0xCF,0x76,0x4A,0x67,0x73,0x95,0x0D,0xFC,0x5E,0x17,0xFF,0x03,0xF0,

0xAF,0x91,0x7D,0x35,0x03,0x64,0xD4,0x02,0x6B,0x00,0xBE,0x41,0xF6,0xCA,0x64,0x89,0xAF,0x30,0xC4,0xF2,0x1E,0x27,0x53,0x27,0x11,0xD7,0xE7,0xBC,0x9C,0x94,0xD5,0xC2,

0x5E,0x11,0x56,0x99,0x06,0xD0,0x30,0x06,0x16,0xDD,0x92,0x85,0xE7,0xBC,0x3F,0xE1,0xED,0xA0,0xEE,0x01,0x3D,0xDE,0xF1,0x05,0x4C,0xA0,0x16,0x92,0xFA,0xDE,0xCB,0x69,

0xCF,0x8A,0x85,0x3F,0x84,0x0B,0x62,0xC9,0xC5,0xED,0x0D,0x16,0x35,0xD7,0xE2,0x21,0xB3,0xBD,0x52,0xA7,0xDC,0x56,0x89,0x36,0xD0,0xD1,0x4F,0x87,0x39,0xC4,0x2C,0x0D,

0x8A,0x65,0xD0,0xCE,0x85,0xFC,0xB1,0x72,0x00,0x98,0xF6,0x99,0xE8,0x01,0x94,0x72,0x86,0xEF,0xB7,0x8A,0x01,0x79,0x63,0xE1,0x25,0xD0,0x5B,0x43,0x62,0x2D,0x42,0xD5,

0x18,0x08,0x00,0xEF,0x12,0x8B,0x67,0x29,0x50,0x46,0x17,0xB9,0x09,0x24,0x9E,0xFC,0xF0,0x9E,0xE4,0x52,0xB7,0x2E,0xC7,0x2F,0xD1,0x07,0x02,0x6F,0x7D,0x03,0x53,0xEA,

0x00,0xDD,0xDD,0x49,0x31,0xA0,0xCB,0x18,0x3B,0x5F,0x36,0x1C,0x9F,0x27,0x48,0xE6,0x78,0x32,0xA2,0xA8,0x03,0x5D,0xFC,0x48,0x5E,0xDC,0x0B,0xB3,0x90,0x2D,0xA8,0x74,

0xCA,0x4A,0x2E,0x85,0xED,0x23,0x24,0x64,0x4B,0x4B,0x1C,0x6A,0xB2,0xF2,0xDA,0x59,0xA7,0x13,0xB9,0x34,0xEF,0xEE,0x4B,0x53,0x54,0xB9,0x40,0xB6,0xA5,0x93,0x89,0x9A,

0xFF,0xB9,0xBD,0x4A,0x4B,0xFC,0xBB,0x38,0x08,0x73,0x91,0x4C,0x4B,0x6D,0x9C,0x7C,0x03,0xA9,0xF1,0x9D,0x82,0xCA,0xFC,0x78,0x39,0x05,0x67,0x21,0xC3,0x1D,0x3D,0x84,

0x26,0x91,0x50,0x41,0x55,0x14,0xD8,0xBA,0xF9,0x3D,0x5C,0x69,0x70,0x80,0xD6,0x78,0x16,0x5D,0x12,0x8B,0xC4,0xD7,0x57,0xE1,0x97,0x28,0x49,0x9B,0xF3,0xB3,0x0E,0x5B,

0xC7,0x3A,0xB0,0x11,0x12,0x51,0xC2,0x12,0xA6,0x12,0x47,0x6B,0x2C,0x13,0xCF,0x74,0x68,0x95,0xE3,0xA8,0xBE,0xFE,0xA3,0xB3,0xF5,0x8A,0xAE,0xCD,0x3C,0x3D,0x42,0x47,

0x6A,0x1C,0xA5,0x63,0x8A,0x9C,0xC3,0x69,0x97,0x5B,0x18,0xF7,0x84,0x0E,0xD0,0x99,0x7F,0xBA,0x2D,0x99,0x77,0x28,0x2A,0x19,0xDC,0x93,0x5E,0x5E,0xA6,0xA3,0x22,0x6F,

0x98,0x9F,0xF6,0xDF,0xC6,0xDE,0x21,0xE7,0x55,0x7E,0x98,0xB8,0x82,0x59,0x21,0x0E,0xE5,0x35,0xB8,0x09,0xF7,0x3B,0x32,0x39,0xD3,0xAB,0x20,0xF7,0x39,0xCD,0xF6,0xFC,

0xD8,0x2B,0x6D,0x2C,0xCD,0xFD,0x25,0xB3,0x67,0xE5,0x8F,0x53,0x2D,0xDC,0x0A,0xFC,0x22,0x6C,0x4C,0x9E,0x47,0x21,0x04,0x3B,0x62,0x3A,0xBD,0x40,0xFE,0xA3,0x06,0x15,

0xB3,0x28,0xD0,0xF3,0xA7,0xE3,0x17,0xF6,0x55,0xF6,0xC5,0x73,0x8D,0x80,0xD3,0x8B,0xBC,0xC9,0xB1,0x00,0x6E,0xC0,0xE8,0x48,0x11,0xA8,0xFE,0xE0,0xFC,0x0E,0x99,0xE3,

0xB0,0xFE,0xE8,0xDB,0x5D,0x76,0x3F,0xD7,0xA8,0x1B,0x01,0xBE,0xAB,0x2B,0xC3,0xE2,0x3D,0xB3,0xAE,0xD8,0x74,0x02,0x25,0x88,0x69,0x5D,0xA8,0x80,0x3B,0xF4,0xF9,0x8E,

0x57,0x15,0x7D,0x8D,0xF6,0xA0,0xE4,0x7F,0xE7,0xBB,0x0D,0xDC,0x8E,0xC6,0x23,0x2A,0x2D,0x92,0x0D,0xCE,0x62,0xCD,0x05,0x22,0xF1,0xC1,0x86,0xC7,0xC4,0x3F,0x6C,0x3D,

0x30,0xD5,0x57,0xB0,0x7A,0x47,0x50,0x15,0x9A,0x3D,0xAF,0x76,0x3E,0x3A,0x3B,0x8A,0x12,0xCD,0x94,0x89,0x3F,0x0B,0xCE,0x3E,0x31,0x3C,0x5F,0x5E,0x9E,0xD5,0x3B,0x18,

0xC4,0xA7,0x3D,0xED,0xF2,0x55,0xC9,0xC2,0x49,0x0B,0xB0,0x34,0xC4,0x6D,0x53,0x2B,0x76,0xCE,0x0C,0xB2,0x13,0xA3,0xC9,0x06,0xB2,0x37,0xFA,0xEC,0xD1,0xA0,0xAE,0x48,

0x9A,0xF1,0xF8,0xEC,0x65,0xB1,0x98,0xAE,0x7D,0x8C,0xD7,0xBD,0x27,0x49,0xB3,0x35,0xE0,0xFC,0x3C,0xF0,0xE7,0x7D,0x3E,0xA0,0xFB,0x18,0x20,0x1A,0x66,0x86,0x0C,0xF5,

0x3A,0x1C,0x51,0x54,0xDB,0x43,0x05,0x00,0xBD,0x28,0xEE,0xBA,0x6F,0xB5,0xA3,0xCF,0xD9,0xBF,0xEE,0xEC,0xC2,0x81,0x75,0x34,0x95,0x49,0x99,0x90,0x64,0x71,0x9F,0x46,

0x2D,0x91,0x0D,0xCE,0x5C,0xF3,0x57,0xE1,0x92,0x47,0xBB,0xD2,0xA5,0x98,0x6A,0x21,0xE8,0x7F,0xE8,0x4E,0xAC,0x97,0xB5,0xEB,0x06,0x30,0x2D,0xF5,0xD3,0x48,0xAD,0x64,

0xFA,0xB7,0xF7,0x01,0xF1,0xA9,0xD8,0x78,0x83,0x51,0x08,0xAE,0xD0,0xDC,0x51,0x54,0x95,0xA4,0xF3,0xBD,0xAD,0xA7,0x48,0xEC,0xD8,0x37,0xA4,0xF2,0xBC,0xF3,0x7F,0x77,

0x29,0xF5,0xD6,0x96,0xA1,0x4D,0xCE,0xEE,0x18,0xAE,0x9B,0xF5,0xF8,0x68,0xA0,0x90,0x68,0x96,0xD7,0xE1,0xCE,0x99,0x74,0x60,0x92,0xC5,0xC5,0x2E,0x26,0x59,0x5D,0xA6,

0x43,0xB5,0x71,0x34,0x75,0xC7,0x82,0x6A,0xD9,0xC7,0x3C,0x50,0x25,0x24,0x9F,0xFC,0xEA,0xBE,0x5C,0x63,0x16,0x55,0x82,0x6F,0xBD,0x41,0x59,0x52,0x19,0x64,0x8E,0x19,

0xCF,0x5E,0x91,0x84,0x73,0xFF,0x3C,0x15,0x4F,0x02,0xB5,0x67,0x60,0xF8,0x95,0xC1,0xA2,0x82,0x4A,0xEB,0x8E,0xC3,0xBA,0x40,0xE0,0x15,0x28,0x05,0x9D,0xFB,0x5C,0xF9,

0x55,0x58,0xFE,0x2D,0xA5,0xDE,0x44,0x16,0x01,0xC8,0xCD,0xE1,0xB0,0xCC,0xCC,0x06,0x18,0x4B,0x68,0x20,0x3C,0xCC,0x63,0xFC,0x83,0xA8,0xFA,0xF0,0xBB,0x06,0x0D,0x6E,

0x5D,0x0A,0x80,0xD8,0x13,0x4A,0xE1,0x96,0x77,0x82,0x4B,0x67,0x1E,0x88,0x8A,0xF4,0xD5,0x82,0x7F,0xAB,0x2A,0x56,0xC6,0xCA,0x2F,0x62,0x97,0xBA,0x7A,0x6E,0xEA,0x9F,

0x70,0xDE,0xDF,0x2D,0xC4,0x2C,0x5C,0xBD,0x3A,0x96,0xF8,0xA0,0xB1,0x14,0x18,0xB3,0x60,0x8D,0x57,0x33,0x60,0x4A,0x2C,0xD3,0x6A,0xAB,0xC7,0x0C,0xE3,0x19,0x3B,0xB5,

0x15,0x3B,0xE2,0xD3,0xC0,0x6D,0xFD,0xB2,0xD1,0x6E,0x9C,0x35,0x71,0x58,0xBE,0x6A,0x41,0xD6,0xB8,0x61,0xE4,0x91,0xDB,0x3F,0xBF,0xEB,0x51,0x8E,0xFC,0xF0,0x48,0xD7,

0xD5,0x89,0x53,0x73,0x0F,0xF3,0x0C,0x9E,0xC4,0x70,0xFF,0xCD,0x66,0x3D,0xC3,0x42,0x01,0xC3,0x6A,0xDD,0xC0,0x11,0x1C,0x35,0xB3,0x8A,0xFE,0xE7,0xCF,0xDB,0x58,0x2E,

0x37,0x31,0xF8,0xB4,0xBA,0xA8,0xD1,0xA8,0x9C,0x06,0xE8,0x11,0x99,0xA9,0x71,0x62,0x27,0xBE,0x34,0x4E,0xFC,0xB4,0x36,0xDD,0xD0,0xF0,0x96,0xC0,0x64,0xC3,0xB5,0xE2,

0xC3,0x99,0x99,0x3F,0xC7,0x73,0x94,0xF9,0xE0,0x97,0x20,0xA8,0x11,0x85,0x0E,0xF2,0x3B,0x2E,0xE0,0x5D,0x9E,0x61,0x73,0x60,0x9D,0x86,0xE1,0xC0,0xC1,0x8E,0xA5,0x1A,

0x01,0x2A,0x00,0xBB,0x41,0x3B,0x9C,0xB8,0x18,0x8A,0x70,0x3C,0xD6,0xBA,0xE3,0x1C,0xC6,0x7B,0x34,0xB1,0xB0,0x00,0x19,0xE6,0xA2,0xB2,0xA6,0x90,0xF0,0x26,0x71,0xFE,

0x7A,0x4C,0xF4,0xD1,0x2D,0xEA,0x32,0x0E,0xCD,0x49,0x9E,0x72,0xF1,0x2F,0x38,0x06,0x4F,0x0C,0xF9,0xF3,0x39,0x78,0x71,0x96,0x68,0xDD,0xAF,0xD7,0xF9,0x71,0x61,0xB7,

0xB5,0x68,0x3C,0x29,0x95,0x67,0x9E,0x23,0x85,0x3B,0x72,0xF4,0x69,0xCB,0x55,0xD8,0x5E,0x4B,0xF6,0xCA,0x42,0xB3,0xC3,0x99,0x76,0x70,0xC2,0x3E,0xE2,0x59,0xBC,0x6D,

0x3A,0xE4,0xA1,0x6A,0x80,0x9A,0x28,0x1E,0xCB,0xC8,0xB6,0x6A,0x46,0x78,0x81,0xBB,0x7B,0x9F,0xF5,0xDF,0xD2,0x98,0x57,0x17,0x54,0xD1,0xA8,0x6D,0x9D,0xCE,0x52,0xAF,

};



#define TEST_CRC_SIZE   4096
static u8 buf[2 * TEST_CRC_SIZE] __attribute__((aligned(32)));

#if 0  //发送板子
static void uart_recv_test_task(void *priv)
{
    int send_buf[64];	//需要对齐32位，DMA发送
    int len = sizeof(buf);
    int cnt = 0;
    u8 *recv_buf;
    void *hdl = dev_open("uart2", NULL);

    if (!hdl) {
        printf("open uart err !!!\n");
        return ;
    }
	recv_buf = zalloc(TEST_CRC_SIZE);
	if(!recv_buf){
        printf("malloc uart err !!!\n");
        return ;
	}

	u32 *wcrc = recv_buf + TEST_CRC_SIZE - 4;

    /* 1 . 设置接收数据地址 */
    dev_ioctl(hdl, UART_SET_CIRCULAR_BUFF_ADDR, (int)buf);

    /* 2 . 设置接收数据地址长度 */
    dev_ioctl(hdl, UART_SET_CIRCULAR_BUFF_LENTH, len);

    /* 3 . 设置接收数据为阻塞方式,需要非阻塞可以去掉,建议加上超时设置 */
    dev_ioctl(hdl, UART_SET_RECV_BLOCK, 1);

    u32 parm = 500;
    dev_ioctl(hdl, UART_SET_RECV_TIMEOUT, (u32)parm);

    /* 4 . 使能特殊串口 */
    dev_ioctl(hdl, UART_START, 0);

    printf("uart_test_task running \n");

    while (1) {
        /* 5 . 接收数据 */
read_data:
        len = dev_read(hdl, recv_buf, TEST_CRC_SIZE);
        if (len <= 0) {
            printf("\n recv err len = %d\n", len);
            if (len == -1) {
                printf("\n err buf\n");
                dev_ioctl(hdl, UART_FLUSH, 0);
            }
            os_time_dly(1);
            continue;
        }
		for(int i = 0; i < TEST_CRC_SIZE; i++){
            if(recv_buf[i] != file1_table4k[i]){
                printf("recv err , recv_buf[%d] = 0x%x \n",i, recv_buf[i]);
                os_time_dly(1);
                goto read_data;
            }
		}
		printf("recv ok \n");
		dev_write(hdl, "recv ok", strlen("recv ok"));//发送接收成功命令
        memset(recv_buf, 0, sizeof(recv_buf));
    }
}
static int c_main(void)
{
    os_task_create(uart_recv_test_task, NULL, 10, 1000, 0, "uart_recv_test_task");
    return 0;
}
late_initcall(c_main);
#else //接收板子
static void uart_send_test_task(void *priv)
{
    int send_buf[64];	//需要对齐32位，DMA发送
    int len = sizeof(buf);
    int cnt = 0;
    u8 *recv_buf;
    void *hdl = dev_open("uart2", NULL);

    if (!hdl) {
        printf("open uart err !!!\n");
        return ;
    }
	recv_buf = zalloc(TEST_CRC_SIZE);
	if(!recv_buf){
        printf("malloc uart err !!!\n");
        return ;
	}

	u32 *wcrc = recv_buf + TEST_CRC_SIZE - 4;

    /* 1 . 设置接收数据地址 */
    dev_ioctl(hdl, UART_SET_CIRCULAR_BUFF_ADDR, (int)buf);

    /* 2 . 设置接收数据地址长度 */
    dev_ioctl(hdl, UART_SET_CIRCULAR_BUFF_LENTH, len);

    /* 3 . 设置接收数据为阻塞方式,需要非阻塞可以去掉,建议加上超时设置 */
    dev_ioctl(hdl, UART_SET_RECV_BLOCK, 1);

    u32 parm = 500;
    dev_ioctl(hdl, UART_SET_RECV_TIMEOUT, (u32)parm);

    /* 4 . 使能特殊串口 */
    dev_ioctl(hdl, UART_START, 0);

    printf("uart_test_task running \n");


    while (1) {
        /* 5 . 接收数据 */
send_data:
        memcpy(recv_buf, file1_table4k, TEST_CRC_SIZE);
        dev_write(hdl, recv_buf, TEST_CRC_SIZE);
        u8 cnt = 5;
read_data:
		len = dev_read(hdl, recv_buf, TEST_CRC_SIZE);
        if (len <= 0) {
            printf("\n recv err len = %d\n", len);
            if (len == -1) {
                printf("\n err buf\n");
                dev_ioctl(hdl, UART_FLUSH, 0);
            }
            os_time_dly(1);
            cnt--;
            if(!cnt){//出错超时重发
                goto send_data;
            }
            goto read_data;
        }
        recv_buf[len] = 0;
        if(strstr(recv_buf, "recv ok")){
            printf("send ok , send next packet \n\n");
        }
        memset(recv_buf, 0, sizeof(recv_buf));
    }
}
static int c_main(void)
{
    os_task_create(uart_send_test_task, NULL, 10, 1000, 0, "uart_send_test_task");
    return 0;
}
late_initcall(c_main);
#endif
