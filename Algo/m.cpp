#include <iostream>
#include <fstream>
#include <iterator>
#include "bst.h"
#include "list.h"
#include <string>
#include <algorithm>
#include "util.h"
#include "tick.h"

#include <time.h>

using namespace std;

//#define _CRT_SECURE_NO_WARNINGS

#if 0
struct tm {
int tm_sec;     /* seconds after the minute - [0,59] */
int tm_min;     /* minutes after the hour - [0,59] */
int tm_hour;    /* hours since midnight - [0,23] */
int tm_mday;    /* day of the month - [1,31] */
int tm_mon;     /* months since January - [0,11] */
int tm_year;    /* years since 1900 */
int tm_wday;    /* days since Sunday - [0,6] */
int tm_yday;    /* days since January 1 - [0,365] */
int tm_isdst;   /* daylight savings time flag */
		};
#endif


tick gettk(const char* s){
	struct tm timeinfo={};
	double price;
	int volume;
	char tag[3]={};
	char tstr[20]={};//strlen("01/02/2013 01:20:00") + 1

	//Don't use a precision specifier for sscanf, and use %lf, %f is for floats.
	//(note: this applies to scanf only, not printf).
	sscanf(s,"%d/%d/%d %d:%d:%d,%lf,%d,%s",
		&timeinfo.tm_mon,&timeinfo.tm_mday,&timeinfo.tm_year,
		&timeinfo.tm_hour,&timeinfo.tm_min,&timeinfo.tm_sec,&price,&volume,tag);
	timeinfo.tm_year -= 1900;
	timeinfo.tm_mon -= 1;
	PV t={price,volume};
	time_t tt=mktime(&timeinfo);

	tick tk;
	tk.pvs.push_back(t);
	strncpy(tk.tag,tag,3);
	tk.timestamp=tt;
	tk.vwap=price;
	tk.sumofvol=volume;
	tk.sumofcash=volume*price/1000;
	const char* x=strchr(s,',');
	strncpy(tstr,s,x-s);
	tk.timestr=tstr;
	return tk;
}


int main(int argc,char* argv[]){
	//dclist::test();
	//slist::test();

	fstream ts("1.csv");
	tickdata* ptd= new tickdata("1.HK");
	///"01/02/2013 01:20:00,119.3,23000,U"
	string line;
	int num=0;
	if (ts.is_open()){
		while (ts.good() && !ts.eof()){
			getline(ts,line);
			if (!line.empty()){
				ptd->push(gettk(line.c_str()));
			}
			//cout << ++num << " " << line << endl;
		}
	}
	ptd->update();
	ptd->head();
	return 0;
}

