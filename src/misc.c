#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "misc.h"

int compare(struct tm from_date,struct tm to_date,struct tm current_date,int flag)
{

	/**
	 * 0:	no time range provided
	 * 1:	time1 -> time2
	 * 2:	year1 -> year2
	 * 3:	time1,year1  --> time2,year2
	 */

	double diff1;
	double diff2;

	switch(flag)
	{
		case 0:
			return 1;
		case 1:
			from_date.tm_mday = to_date.tm_mday = current_date.tm_mday;
			from_date.tm_mon  = to_date.tm_mon  = current_date.tm_mon;
			from_date.tm_year = to_date.tm_year = current_date.tm_year;
			break;
		case 2:
			from_date.tm_hour = to_date.tm_hour = current_date.tm_hour;
			from_date.tm_min  = to_date.tm_min  = current_date.tm_min;
			break;
		case 3:
			break;
	}


	/*Disabling Daylight Saving */
	from_date.tm_isdst = to_date.tm_isdst = current_date.tm_isdst=0;
	diff1=difftime(mktime(&current_date),mktime(&from_date));
	diff2=difftime(mktime(&current_date),mktime(&to_date));

	return diff1>0 &&   diff2<0;
}
