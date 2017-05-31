/*
TinyGPS - a small GPS library for Arduino providing basic NMEA parsing
Based on work by and "distance_to" and "course_to" courtesy of Maarten Lamers.
Suggestion to add satellites(), course_to(), and cardinal(), by Matt Monson.
Precision improvements suggested by Wayne Holder.
Copyright (C) 2008-2013 Mikal Hart
All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef __cplusplus
extern "C" {
#endif

#define byte unsigned char
#include <stdlib.h>

#define _GPS_VERSION 13 // software version of this library
#define _GPS_MPH_PER_KNOT 1.15077945
#define _GPS_MPS_PER_KNOT 0.51444444
#define _GPS_KMPH_PER_KNOT 1.852
#define _GPS_MILES_PER_METER 0.00062137112
#define _GPS_KM_PER_METER 0.001

#define bool char
#define true 1
#define false 0
// #define _GPS_NO_STATS


#define GPS_INVALID_AGE  0xFFFFFFFF
#define GPS_INVALID_ANGLE  999999999
#define GPS_INVALID_ALTITUDE  999999999
#define GPS_INVALID_DATE  0
#define GPS_INVALID_TIME  0xFFFFFFFF
#define GPS_INVALID_SPEED  999999999
#define GPS_INVALID_FIX_TIME  0xFFFFFFFF 
#define GPS_INVALID_SATELLITES  0xFF
#define GPS_INVALID_HDOP  0xFFFFFFFF

extern const float GPS_INVALID_F_ANGLE;


 // static const float GPS_INVALID_F_ANGLE, GPS_INVALID_F_ALTITUDE, GPS_INVALID_F_SPEED;

  extern void tinygps_init(void);
	
  bool tinygps_encode(char c); // process one character received from GPS

  // lat/long in MILLIONTHs of a degree and age of fix in milliseconds
  // (note: versions 12 and earlier gave lat/long in 100,000ths of a degree.
  extern void tinygps_get_position(long *latitude, long *longitude, unsigned long *fix_age);

  // date as ddmmyy, time as hhmmsscc, and age in milliseconds
  extern void tinygps_get_datetime(unsigned long *date, unsigned long *time, unsigned long *age);

  // signed altitude in centimeters (from GPGGA sentence)
  extern  long tinygps_altitude(void);

  // course in last full GPRMC sentence in 100th of a degree
  extern  unsigned long tinygps_course(void);

  // speed in last full GPRMC sentence in 100ths of a knot
  extern  unsigned long tinygps_speed(void);

  // satellites used in last full GPGGA sentence
  extern  unsigned short tinygps_satellites(void);

  // horizontal dilution of precision in 100ths
  extern  unsigned long tinygps_hdop(void);

  extern void tinygps_f_get_position(float *latitude, float *longitude, unsigned long *fix_age);
  extern void tinygps_crack_datetime(int *year, byte *month, byte *day,
    byte *hour, byte *minute, byte *second, byte *hundredths, unsigned long *fix_age);
  extern float tinygps_f_altitude(void);
  extern float tinygps_f_course(void);
  extern float tinygps_f_speed_knots(void);
  extern float tinygps_f_speed_mph(void);
  extern float tinygps_f_speed_mps(void);
  extern float tinygps_f_speed_kmph(void);

  float tinygps_distance_between (float lat1, float long1, float lat2, float long2);
  float tinygps_course_to (float lat1, float long1, float lat2, float long2);

#ifndef _GPS_NO_STATS
  void tinygps_stats(unsigned long *chars, unsigned short *good_sentences, unsigned short *failed_cs);
#endif

#ifdef __cplusplus
} // extern "C"
#endif
