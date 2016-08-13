#include <avr/eeprom.h>

extern "C" { // obligatoire sinon ça marche pô
#include "id.h"
}

const uint16_t rootLength = 2;
const uint16_t rootIds[] = {2,9};

const int spd = 9600;

ids i;

bool writeTo(void* dest, void* src, size_t length){
    eeprom_update_block(src, dest, length);
    return true;
}
bool readFrom(void* src, void* dest, size_t length){
    eeprom_read_block (dest, src, length);
    return true;
}

void setup() {
    Serial.begin(spd);

    Serial.println("Creating EEPROM structure...");
    idsInitialize(&i);
    clearIds(&i);
    Serial.println("Structure created");
    Serial.println("Please upload gate.ino on Arduino");
}

void loop(){}
