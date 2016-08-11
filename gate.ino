#include <SoftwareSerial.h>
#include <avr/eeprom.h>
#include <stdint.h> //uint8_t
#include <stdlib.h> // atoi

extern "C" { // obligatoire sinon ça marche pô
#include "id.h"
}

const uint16_t rootLength = 2;
const uint16_t rootIds[] = {2,9};

const int BUFF_SIZE = 50;
const char EOC = '.';

const int RX = 10;
const int TX = 11;
const int spd = 9600;
const int gatePin = 9;

char buf[BUFF_SIZE];

ids i;

//SoftwareSerial bth(RX, TX);

//TODO: EEPROM
bool writeTo(void* dest, void* src, size_t length){
    eeprom_update_block(src, dest, length);
    return true;
}
bool readFrom(void* src, void* dest, size_t length){
    eeprom_read_block (dest, src, length);
    return true;
}

void setup() {
    //Setup IDS
    Serial.begin(spd);
    idsInitialize(&i);
    if(!loadIds(&i))
        clearIds(&i);
    //bth.begin(spd);
    pinMode(gatePin, OUTPUT);
}

void loop()
{
    // TODO: reset Buffer
    Serial.print("Waiting for data\n");
    readBuff();
    treatData();
}



void openGate(){
    digitalWrite(gatePin, HIGH);
    delay(500);
    digitalWrite(gatePin, LOW);
}

void readBuff() {
    for (int i = 0; i < BUFF_SIZE; i++) {
        //while(!bth.available());
        while(!Serial.available());
        //buf[i] = bth.read();
        buf[i] = Serial.read();

        if(buf[i] == EOC){
            buf[i] = '\0';
            Serial.print("Command received: ");
            Serial.print(buf);
            Serial.print("\n");
            return;
        }
    }
}

void treatData()
{
    char* b = buf;
    bool root = false;
    uint16_t c = 0;
    uint16_t id = 0;

    id = atoi(b);

    if(!identificate(id, &i)){
        Serial.print("Failed to authentificate with ID");
        Serial.print(id);
        Serial.print("\n");
        return; //if id don't exist
    }

    root = isRoot(id);

    while(isdigit(*b)) //remove all atoi-ed digits
        b++;

    switch(*b){
        case 'a':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.print(id);
                Serial.print("\n");
                return; //if id don't exist
            }
            b++;
            c = atoi(b);
            if(addId(c, &i)){
                Serial.print("ID ");
                Serial.print(c);
                Serial.print(" added\n");
            }
            else {
                Serial.print("ID ");
                Serial.print(c);
                Serial.print(" NOT added\n");
            }
            break;
        case 'r':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.print(id);
                Serial.print("\n");
                return; //if id don't exist
            }
            b++;
            c = atoi(b);
            if(rmId(c ,&i)){
                Serial.print("ID " );
                Serial.print(c);
                Serial.print(" removed\n");
            }
            else {
                Serial.print("ID ");
                Serial.print(c);
                Serial.print(" NOT removed\n");
            }
            break;
        case 'c':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.print(id);
                Serial.print("\n");
                return; //if id don't exist
            }
            if(clearIds(&i)){
                Serial.print("IDs cleared\n");
            }
            else {
                Serial.print("IDs not cleared\n");
            }
            break;
        case 'd':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.print(id);
                Serial.print("\n");
                return; //if id don't exist
            }
            for (uint16_t j = 0; j < i.length; ++j) {
                Serial.print("ID: ");
                Serial.print(i.ids[j]);
                Serial.print(" - root: ");
                Serial.print(isRoot(i.ids[j]) ? "true\n" : "false\n");
            }
            break;
        case 'o':
            openGate();
            Serial.print("Gate opened\n");
            break;
        case 'e': //Dump EEPROM : for debug purpose only
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.print(id);
                Serial.print("\n");
                return; //if id don't exist
            }
            uint16_t d;
            Serial.print("/!\\ For debug purpose only /!\\\nDumping EEPROM\n");
            Serial.print("Length: ");
            readFrom(0, &d, sizeof(d));
            Serial.print(d);
            Serial.print("\n");

            for (int j = 0; j < i.length; j++) {
                Serial.print("ID: ");
                readFrom(sizeof(i.length) + j*sizeof(*i.ids), &d, sizeof(d));
                Serial.print(d);
            Serial.print("\n");
            }
            break;

        default:
            Serial.print("Command ");
            Serial.print(*b);
            Serial.print(" not found\n");
    }
}
