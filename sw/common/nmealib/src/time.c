/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: time.c 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*! \file time.h */

#include "nmea/time.h"

void nmea_time_now(nmeaTIME *stm)
{
    stm->year = 0;
    stm->mon  = 0;
    stm->day  = 0;
    stm->hour = 0;
    stm->min  = 0;
    stm->sec  = 0;
    stm->hsec = 0;
}

