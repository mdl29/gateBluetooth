#include <SoftwareSerial.h>
#include <stdint.h> //uint(8,16,32,etc.)_t
#include <stdlib.h> // atoi

extern "C" { // obligatoire sinon ça marche pô
#include "id.h"
}

#include "config.h"


const int BUFF_SIZE = 50;
const char EOC = '.';

const int TX = 10;
const int RX = 11;
const int spd = 9600;
const int gatePin = 9;

char buf[BUFF_SIZE];

ids i;

SoftwareSerial bth(RX, TX);

void setup() {
    //Setup IDS
    Serial.begin(spd);
    idsInitialize(&i);
    if(!loadIds(&i))
        clearIds(&i);

    bth.begin(spd);
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
        //Allow bluetooth and Serial connections
        while(true){
            if(bth.available())
                buf[i] = bth.read();
            else if(Serial.available())
                buf[i] = Serial.read();
            else continue;
            break;
        }
        if(buf[i] == EOC){
            buf[i] = '\0';
            Serial.print("Command received: ");
            Serial.println(buf);
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
        Serial.print("Failed to authentificate with ID ");
        Serial.println(id);
        return; //if id don't exist
    }

    root = isRoot(id);

    while(isdigit(*b)) //remove all atoi-ed digits
        b++;

    switch(*b){
        case 'a':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.println(id);
                return; //if id don't exist
            }
            b++;
            c = atoi(b);
            if(addId(c, &i)){
                Serial.print("ID ");
                Serial.print(c);
                Serial.println(" added");
            }
            else {
                Serial.print("ID ");
                Serial.print(c);
                Serial.println(" NOT added");
            }
            break;
        case 'r':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.println(id);
                return; //if id don't exist
            }
            b++;
            c = atoi(b);
            if(rmId(c ,&i)){
                Serial.print("ID " );
                Serial.print(c);
                Serial.println(" removed");
            }
            else {
                Serial.print("ID ");
                Serial.print(c);
                Serial.println(" NOT removed");
            }
            break;
        case 'c':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.println(id);
                return; //if id don't exist
            }
            if(clearIds(&i)){
                Serial.println("IDs cleared");
            }
            else {
                Serial.println("IDs not cleared");
            }
            break;
        case 'd':
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.println(id);
                return; //if id don't exist
            }
            for (int j = 0; j < rootLength ; ++j) { // Root Ids
                Serial.print("ID: ");
                Serial.print(rootIds[j]);
                Serial.print(" - root: ");
                Serial.println("true");
            }
            for (int j = 0; j < i.length; ++j) { // Normal Ids
                Serial.print("ID: ");
                Serial.print(i.ids[j]);
                Serial.print(" - root: ");
                Serial.println("false");
            }
            break;
        case 'o':
            openGate();
            Serial.println("Gate opened");
            break;
        case 'e': //Dump EEPROM : for debug purpose only
            if (!root){
                Serial.print("Failed to authentificate as root with ID ");
                Serial.println(id);
                return; //if id don't exist
            }
            uint16_t d;
            Serial.print("/!\\ For debug purpose only /!\\\nDumping EEPROM\n");
            Serial.print("Length: ");
            readFrom(0, &d, sizeof(d));
            Serial.println(d);

            for (int j = 0; j < i.length; j++) {
                Serial.print("ID: ");
                readFrom((void *)(sizeof(i.length) + j*sizeof(*i.ids)), &d, sizeof(d));
                Serial.println(d);
            }
            break;

        default:
            Serial.print("Command ");
            Serial.print(*b);
            Serial.println(" not found");
    }
}
