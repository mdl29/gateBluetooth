#include <stdint.h> //uint16_t
#include <stdlib.h> // atoi
#include <stdbool.h>
#include <string.h> // memcpy
#include <ctype.h> // isdigit

#include "id.h"

#define MAX_USERS 20
#define F_IDS "ids"

int16_t findPos(uint16_t, ids*);
bool setID(int16_t pos, uint16_t id, bool, ids* i);

bool setID(int16_t pos, uint16_t id, bool lengthChanged, ids* i){
    if(pos >= MAX_USERS)
        return false;
    if(pos > i->length)
        return false; //disallow fragmentation

    //set in volatile memory
    i->ids[pos] = id;

    //and in persistant memory
    if (lengthChanged){
        if(!writeTo(0,
                    &i->length,
                    sizeof(i->length))
                )
            return false;
    }


    if (!writeTo((void *)(sizeof(i->length) + pos*sizeof(*i->ids)), //where to write
                 &id, //what to write
                 sizeof(id)) //how much du write
        )
        return false;

    return true;
}

bool addId(uint16_t id, ids* i){
    if(i->length == MAX_USERS)
        return false;

    if(id == 0) //disallow 0 id
        return false;

    if(identificate(id, i)) //id already exist
        return false;

    i->length++;

    return setID(i->length - 1 , id, true, i);
}

// Find pos of id to delete return -1 if no ID founded
int16_t findPos(uint16_t id, ids* i){
    for(int16_t j = 0; j < i->length; j++) {
        if(i->ids[j] == id)
            return j;
    }
    return -1;
}

bool rmId(uint16_t id, ids* i){
    int16_t pos = findPos(id, i);

    //if no id founded
    if(pos == -1)
        return false;

    //move the queue to the id to delete and reduce length
    i->length--;
    return setID(pos, i->ids[i->length], true, i);
}

/*
   Delete all Id except root's id
 */
bool clearIds(ids* i){
    memcpy(i->ids, rootIds, sizeof(*i->ids) * rootLength); //set new ones
    i->length = rootLength;

    return saveIds(i);
}

bool identificate(uint16_t id, ids* i){
    return findPos(id,i) != -1 ? true : false ;
}

bool loadIds(ids* ids){

    if(!readFrom((void*) 0,
              &ids->length,
              sizeof(ids->length))
            )
        return false;

    if(!readFrom((void*)(sizeof(ids->length)),
                ids->ids,
                sizeof(*ids->ids) * ids->length) //read ids * length
            )
        return false;

    return true;
}

bool saveIds(ids* ids){
     if(!writeTo(0,
              &ids->length,
              sizeof(ids->length))
            )
        return false;

    if(!writeTo((void *)(sizeof(ids->length)),
                ids->ids,
                sizeof(*ids->ids) * ids->length) //read ids * length
            )
        return false;

    return true;
}

void idsInitialize(ids* i){
    i->length = 0;
}

bool isRoot(uint16_t id){
    for (uint16_t i = 0; i < rootLength ; ++i) {
        if(rootIds[i] == id)
            return true;
    }
    return false;
}
