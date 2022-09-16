// binary representation
// attribute size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#ifndef _LE_NET_CFG_DUI_H
#define _LE_NET_CFG_DUI_H

#include <stdint.h>
// #include <le_user.h>
//
// gatt profile include file, generated by jieli gatt_inc_generator.exe
//

static const uint8_t profile_data[] = {
    //////////////////////////////////////////////////////
    //
    // 0x0001 PRIMARY_SERVICE  1800
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18,

    /* CHARACTERISTIC,  2a00, READ | DYNAMIC, */
    // 0x0002 CHARACTERISTIC 2a00 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a,
    // 0x0003 VALUE 2a00 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, 0x2a,

    /* CHARACTERISTIC,  2a01, READ | DYNAMIC, */
    // 0x0004 CHARACTERISTIC 2a01 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x04, 0x00, 0x03, 0x28, 0x02, 0x05, 0x00, 0x01, 0x2a,
    // 0x0005 VALUE 2a01 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x05, 0x00, 0x01, 0x2a,

    //////////////////////////////////////////////////////
    //
    // 0x0006 PRIMARY_SERVICE  1801
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x28, 0x01, 0x18,

    /* CHARACTERISTIC,  2a05, INDICATE, */
    // 0x0007 CHARACTERISTIC 2a05 INDICATE
    0x0d, 0x00, 0x02, 0x00, 0x07, 0x00, 0x03, 0x28, 0x20, 0x08, 0x00, 0x05, 0x2a,
    // 0x0008 VALUE 2a05 INDICATE
    0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x05, 0x2a,
    // 0x0009 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x09, 0x00, 0x02, 0x29, 0x00, 0x00,

    //////////////////////////////////////////////////////
    //
    // 0x000a PRIMARY_SERVICE  1111
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x00, 0x28, 0x11, 0x11,

    /* CHARACTERISTIC,  2222, NOTIFY | WRITE | DYNAMIC, */
    // 0x000b CHARACTERISTIC 2222 NOTIFY | WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0b, 0x00, 0x03, 0x28, 0x18, 0x0c, 0x00, 0x22, 0x22,
    // 0x000c VALUE 2222 NOTIFY | WRITE | DYNAMIC
    0x08, 0x00, 0x18, 0x01, 0x0c, 0x00, 0x22, 0x22,
    // 0x000d CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x0d, 0x00, 0x02, 0x29, 0x00, 0x00,
    // 0x000d CHARACTERISTIC_USER_DESCRIPTION
    0x0a, 0x00, 0x0a, 0x01, 0x0e, 0x00, 0x01, 0x29, 0x00, 0x00,

    // END
    0x00, 0x00,
};
//
// characteristics <--> handles
//
#define ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_2a01_01_VALUE_HANDLE 0x0005
#define ATT_CHARACTERISTIC_2a05_01_VALUE_HANDLE 0x0008
#define ATT_CHARACTERISTIC_2a05_01_CLIENT_CONFIGURATION_HANDLE 0x0009
#define ATT_CHARACTERISTIC_2222_01_VALUE_HANDLE 0x000c
#define ATT_CHARACTERISTIC_2222_01_CLIENT_CONFIGURATION_HANDLE 0x000d
#define ATT_CHARACTERISTIC_2222_01_USER_DESCRIPTION_HANDLE 0x000e

#endif
