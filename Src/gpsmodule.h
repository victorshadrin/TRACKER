
#ifndef __GPSMODULE_H
#define __GPSMODULE_H

#include "stm32f3xx_hal.h"

#define SIGNATURE 0xee
#define START_INDEX 5

typedef enum {
    RECORD_BEGIN,
    RECORD_END,
    RECORD_PAUSE,
    RECORD_CONTINUE
} RecordType;

typedef struct{
    RecordType dataType;
    uint8_t year, month, day, hour, minute, second, hundredths;
    float latitude;
    float longitude;
    float ele;
    unsigned long fix_age;
    unsigned long age;
} TrackerData;

extern TrackerData trackerData;
extern uint32_t trackerDataIndex;
extern int missedWritings;

void printout(void);
void readIndex(void);
void writeIndex(void);
void clearData(void);
void writeCurrentData(RecordType recType);




#endif
