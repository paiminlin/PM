/*
demo_crc.c
crc demo : CRC 校验

引用 crcLib.h crcLib.c
github 地址 : https://github.com/whik/crc-lib-c

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "crcLib.h"

int main(int argc, char *argv[]) {

    char str[16] = "Hello World!";

    printf("crc4_itu\t \"%s\" = 0x%0X\n", str, crc4_itu((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc5_epc\t \"%s\" = 0x%0X\n", str, crc5_epc((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc5_itu\t \"%s\" = 0x%0X\n", str, crc5_itu((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc5_usb\t \"%s\" = 0x%0X\n", str, crc5_usb((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc6_itu\t \"%s\" = 0x%0X\n", str, crc6_itu((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc7_mmc\t \"%s\" = 0x%0X\n", str, crc7_mmc((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc8\t\t \"%s\" = 0x%0X\n", str, crc8((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc8_itu\t \"%s\" = 0x%0X\n", str, crc8_itu((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc8_rohc\t \"%s\" = 0x%0X\n", str, crc8_rohc((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc8_maxim\t \"%s\" = 0x%0X\n", str, crc8_maxim((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_ibm\t \"%s\" = 0x%0X\n", str, crc16_ibm((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_maxim\t \"%s\" = 0x%0X\n", str, crc16_maxim((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_usb\t \"%s\" = 0x%0X\n", str, crc16_usb((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_modbus\t \"%s\" = 0x%0X\n", str, crc16_modbus((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_ccitt\t \"%s\" = 0x%0X\n", str, crc16_ccitt((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_ccitt_false\t \"%s\" = 0x%0X\n", str, crc16_ccitt_false((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_x25\t \"%s\" = 0x%0X\n", str, crc16_x25((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_xmodem\t \"%s\" = 0x%0X\n", str, crc16_xmodem((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc16_dnp\t \"%s\" = 0x%0X\n", str, crc16_dnp((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc32\t\t \"%s\" = 0x%0X\n", str, crc32((uint8_t *)str, (uint16_t)(strlen(str))));
    printf("crc32_mpeg_2\t \"%s\" = 0x%0X\n", str, crc32_mpeg_2((uint8_t *)str, (uint16_t)(strlen(str))));

    return 0;
}
