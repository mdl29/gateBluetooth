#ifndef GATE_H
#define GATE_H

#include <stdint.h> //uint16_t
#include <stddef.h> //size_t

#define MAX_USERS 20

/* To define in main.ino */
extern const  uint16_t rootLength;
extern const uint16_t rootIds[];

extern bool writeTo(void*, void*, size_t length);
extern bool readFrom(void*, void*, size_t length);

typedef struct{
    int16_t length;
    uint16_t ids[MAX_USERS];
} ids;

bool clearIds(ids*);
bool addId(uint16_t, ids*);
bool rmId(uint16_t, ids*);
bool loadIds(ids*);
bool saveIds(ids*);
bool identificate(uint16_t, ids*);
void idsInitialize(ids*);
bool isRoot(uint16_t);


#endif /* ifndef GATE_H */
