
#include <stdio.h>

#include "gpsmodule.h"
#include "main.h"
#include "eeprom.h"
#include "program.h"

TrackerData trackerData;
uint32_t trackerDataIndex;
int missedWritings;

void printout() {
    printf("Printing...\n");
    uint32_t printIndex = START_INDEX;
    TrackerData readData;
    while (printIndex < trackerDataIndex) {
        eeprom_read(&hi2c1, printIndex, &readData, sizeof(readData));
        char buffer[20];        
        sprintf(buffer, "20%02d-%02d-%02dT%02d:%02d:%02dZ", readData.year, readData.month, readData.day, readData.hour, readData.minute, readData.second);
        if (readData.dataType == RECORD_BEGIN) {
            printf("\n<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<gpx version=\"1.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" \n");
            printf("xmlns=\"http://www.topografix.com/GPX/1/1\"\n  xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\" \n");
            printf("xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1\">\n<trk>\n <name><![CDATA[%s]]></name>", (char* )&buffer);            
            printf("<time>%s</time>\n<trkseg>\n", (char*)&buffer);
        } 
        if (readData.dataType == RECORD_END) {
            printf("</trkseg>\n</trk>\n</gpx>\n");
        } else {
            printf("<trkpt lat=\"%2.9f\" lon=\"%2.9f\"><ele>%f</ele><time>%s</time></trkpt>\n", readData.latitude, readData.longitude, readData.ele, (char* )&buffer);
        }        
        printIndex += sizeof(trackerData);
    }
    printf("Ready\n");
}

void readIndex() {
    uint8_t firstByte;
    eeprom_read(&hi2c1, (uint32_t) 0, &firstByte, 1);
    if (firstByte == SIGNATURE) {
        eeprom_read(&hi2c1,(uint32_t) 1, &trackerDataIndex, 4);
    } else {
        printf("Wrong signature. Initializing.\n");
        eeprom_write_8(&hi2c1,(uint32_t) 0, SIGNATURE);
        trackerDataIndex = START_INDEX;
        eeprom_write_32(&hi2c1,(uint32_t) 1, trackerDataIndex);
    }    
}

void writeIndex() {
    eeprom_write_32(&hi2c1, (uint32_t) 1, trackerDataIndex);
    HAL_Delay(50);
}

void writeCurrentData(RecordType recType) {
    trackerData.dataType = recType;
    eeprom_write(&hi2c1, (uint32_t)(trackerDataIndex), &trackerData, (uint32_t) sizeof(trackerData));
    trackerDataIndex += sizeof(trackerData);
    missedWritings = 0;
}

void clearData() {
    trackerDataIndex = START_INDEX;
    writeIndex();
}

