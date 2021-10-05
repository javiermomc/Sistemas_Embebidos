#ifndef RTCCONTROL_H_               /*this goes at the top of the document”*/
#define RTCCONTROL_H_

#define RTC_ADDR 0x68u

#define SECONDS_ADDR 0x00u
typedef union {
    struct  { 
        uint8_t un_seconds  	:4,
         	    dec_seconds    :3,
                RESERVED    :1;
    } field;
    uint8_t reg;
} second_type;

#define MINUTES_ADDR 0x01u
typedef union {
    struct  { 
        uint8_t un_minutes  	:4,
         	    dec_minutes    :3,
                RESERVED    :1;
    } field;
    uint8_t reg;
} minutes_type;

#define HOURS_ADDR 0x02u
#define HOURS_CONFIG24 0x0u
#define HOURS_CONFIG12 0x1u
typedef union {
    struct{
		uint8_t un_hours  	:4,
         		dec_hours   :2,
				en_t24		:1,
				RESERVED    :1;
	} field;
	uint8_t reg;
} t24_type;

typedef union {
    struct{
		uint8_t un_hours  	:4,
         		dec_hours   :1,
				pm_am	    :1,
				en_t24		:1,
				RESERVED    :1;
	} field;
	uint8_t reg;
} t12_type;

typedef struct {
    t12_type t12;
	t24_type t24;
} hour_type;

#define DAY_ADDR 0x03u
typedef union {
    struct  { 
        uint8_t day  	:3,
                RESERVED    :5;
    } field;
    uint8_t reg;
} day_type;

#define DATE_ADDR 0x04u
typedef union {
    struct  { 
        uint8_t un_date  	:4,
         	    dec_date    :2,
                RESERVED    :2;
    } field;
    uint8_t reg;
} date_type;

#define MONTH_ADDR 0x05u
typedef union {
    struct  { 
        uint8_t dec_month  	:4,
         	    un_month    :1,
                RESERVED    :3;
    } field;
    uint8_t reg;
} month_type;

#define YEAR_ADDR 0x06u
typedef union {
    struct  { 
        uint8_t un_year  	:4,
         	    dec_year    :4;
    } field;
    uint8_t reg;
} year_type;


 
#include "RTCControl.c"

#endif /*RTCCONTROL_H_*/          /*this goes at the bottom of the document”*/