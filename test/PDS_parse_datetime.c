#include <tinypds.h>
#include "test.h"

#define TINY_PDS_IMPL
#include <tinypds.h>
    
typedef struct
{
    int ret;
    PDS_datetime t;
} expected_t;

int main()
{
    static const expected_t values[] =
    {
        { 1, {0,1977, 24, 7, 0, 0, 0,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,1985,320, 0, 0, 0, 0,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,   0,  0, 0,12, 7,31,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,   0,  0, 0, 8,15, 1,  2450,-8,-15, PDS_ZONED_TIME } },
        { 1, {0,2015, 14, 7,11,49,57,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,2016,  3, 1,18,21,49,129900, 2, 10, PDS_ZONED_TIME } },
        { 1, {0,1977,  5, 9,12,56, 0,     0, 0,  0, PDS_UTC_TIME   } },
        { 1, {0,1997, 15,10, 8,43, 0,     0, 1,  0, PDS_ZONED_TIME } },
        { 0, {0,   0,  0, 0, 0, 0, 0,     0, 0,  0, PDS_LOCAL_TIME } }, 
    };

    begin_test_data(expected_t)
        test_data( "1977-07-24 /* single date */", 10, values[0], PDS_OK ),
        test_data( "1985-320   /* day of year */",  8, values[1], PDS_OK ),
        test_data( "12:07:31                    ",  8, values[2], PDS_OK ),
        test_data( "08:15:1.00245-08:15         ", 19, values[3], PDS_OK ),
        test_data( "2015-07-14T11:49:57\r\n     ", 19, values[4], PDS_OK ),
        test_data( "2016-01-3T18:21:49.1299+2:10", 28, values[5], PDS_OK ),
        test_data( "1977-09-05T12:56:000.000Z\t ", 25, values[6], PDS_OK ),
        test_data( "1997-10-15T08:43:00+1       ", 21, values[7], PDS_OK ),
        test_data( "11-11-11          /* year */",  0, values[8], PDS_INVALID_RANGE ),
        test_data( "1989-53-11       /* month */",  0, values[8], PDS_INVALID_RANGE ),
        test_data( "1975-04-34         /* day */",  0, values[8], PDS_INVALID_RANGE ),
        test_data( "1976-11-11T /* empty time */",  0, values[8], PDS_INVALID_VALUE ),
        test_data( "1977-12-24T07 /*incomplete*/",  0, values[8], PDS_INVALID_VALUE ),
        test_data( "1977-12-24T07:15:00-44      ",  0, values[8], PDS_INVALID_RANGE ),
        test_data( "1977-12-24T07:15:00+12:75   ",  0, values[8], PDS_INVALID_RANGE ),
    end_test_data()

    PDS_parser parser;
    parser.error = dummy_error;

    size_t i;
    test_foreach(i)
    {
        parser.line_num = 1;
        parser.first    = test_str(i);
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;
        
        int ret = PDS_parse_datetime(&parser);
        
        check(test_expected(i).ret == ret);
        check(test_end(i) == parser.current);
        check(test_status(i) == parser.status);
        if(ret)
        {
            PDS_datetime *date_time = &parser.scalar.date_time;
            check(PDS_DATE_TIME_VALUE == parser.scalar.type);
            check(test_expected(i).t.year == date_time->year);
            check(test_expected(i).t.day == date_time->day);
            check(test_expected(i).t.month == date_time->month);
            check(test_expected(i).t.hour == date_time->hour);
            check(test_expected(i).t.minute == date_time->minute);
            check(test_expected(i).t.second == date_time->second);
            check(test_expected(i).t.microsecond == date_time->microsecond);
            check(test_expected(i).t.hour_offset == date_time->hour_offset);
            check(test_expected(i).t.minute_offset == date_time->minute_offset);
            check(test_expected(i).t.time_type == date_time->time_type);
        }
    }
    return EXIT_SUCCESS;
}
