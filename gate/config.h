#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <avr/eeprom.h>

const uint16_t rootLength = 2;
const uint16_t rootIds[] = {2,9};

bool writeTo(void* dest, void* src, size_t length){
    eeprom_update_block(src, dest, length);
    return true;
}
bool readFrom(void* src, void* dest, size_t length){
    eeprom_read_block (dest, src, length);
    return true;
}

#endif /* ifndef CONFIG_H */
