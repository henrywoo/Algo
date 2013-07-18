#include "tick.h"
#include <iostream>
#include <fstream>
#include <iomanip> //setw
#include "util.h"
#include <memory>

using namespace std;

tick::tick():vwap(0),sumofcash(0),sumofvol(0){
	memset(tag,'\0',3);
	changes.vwap=0;
	changes.vol=0;
	changes.cash=0;
	changes.time=0;
}

void tick::update(){
	list<PV>::reverse_iterator i=pvs.rbegin();
	while(lastupdate-- > 0){
		sumofcash += (i->price * i->volume)/1000;
		sumofvol += i->volume;
		++i;
	}
	vwap=sumofcash*1000/sumofvol;
}

//////////////////////////////////////////////////////////////////////////
void tickseries::push(tick t){
	if (ts.empty()){
		ts.push_back(t);
	}else{
		list<tick>::iterator i= ts.end(); i--;
		tick& r=static_cast<tick&>(*i);// must be a reference
		tick* tp=&r;
		if (r.timestamp==t.timestamp){
			r.pvs.insert(r.pvs.end(),t.pvs.begin(),t.pvs.end());
			r.lastupdate=t.pvs.size();
			r.update();
		}else{
			ts.push_back(t);
		}
	}
}

void tickseries::update(){
	list<tick>::iterator i=ts.begin();
	while (strcmp(i->tag,"U")==0){
		i++;
	}
	list<tick>::iterator tmp=i;
	for (i++;i!=ts.end();i++){
		i->changes.vwap = i->vwap - tmp->vwap;
		i->changes.cash = i->sumofcash - tmp->sumofcash;
		i->changes.time = i->timestamp - tmp->timestamp;
		i->changes.vol  = i->sumofvol - tmp->sumofvol;
		tmp=i;
	}
}

void tickseries::head(){
	cout << symbol << endl;
	cout.precision(5);
	cout<< left << std::setw(30) << "timestr"
		<< left << std::setw(10) << "vwap"
		<< left << std::setw(10) << "sumofvol"
		<< left << std::setw(10) << "sumofcash"
		<< left << std::setw(10) << "pvs.size"
		<< left << std::setw(10) << "c.time"
		<< left << std::setw(20) << "c.vwap"
		<< left << std::setw(10) << "c.vol"
		<< left << std::setw(10) << "c.cash" << endl;
	int n=0;
	for (list<tick>::const_iterator i=ts.begin();i!=ts.end();i++,n++){
		if (n>50){
			break;
		}
		cout<< left << std::setw(30) << i->timestr << ""
			<< left << std::setw(10) << i->vwap<< ""
			<< left << std::setw(10) << i->sumofvol<< ""
			<< left << std::setw(10) << i->sumofcash<< ""
			<< left << std::setw(10) << i->pvs.size()<< ""
			<< left << std::setw(10) << i->changes.time<< ""
			<< left << std::setw(20) << i->changes.vwap<< ""
			<< left << std::setw(10) << i->changes.vol<< ""
			<< left << std::setw(10) << i->changes.cash << endl;

	}
}

void tickseries::printbycolumn(){
	size_t len=ts.size();
	shared_ptr<double> pc(new double[len],array_deleter<double>());
	//double *pc=new double[len];
	double *tmp=pc.get();
	for (list<tick>::const_iterator i=ts.begin();i!=ts.end();++i){
		//cout << i->changes.vwap<< endl;
		*tmp++ = i->changes.vwap;
	}

	/// statistics: max, min, mean
	double ma=INT_MIN,mi=INT_MAX;
	double *tmp2=pc.get()+len;
	tmp=pc.get();
	while (tmp!=tmp2){
		if(*tmp<mi){mi=*tmp;}
		if(*tmp>ma){ma=*tmp;}
		tmp++;
	}
	double mean=getmean(pc.get(),len);
	cout << mean << endl;

	//get the max sum sub array
	double * head=NULL, * tail=NULL;
	double maxsum=maxsubarray(pc.get(),len,head,tail);
	copy(head,tail+1,ostream_iterator<double>(cout," "));
	cout<< endl << maxsum << "size of the result:" << tail-head+1 <<endl;

	double* tmphead=head;
	double tmpsum=0;
	while (tmphead<=tail)
	{
		tmpsum+=*tmphead;
		tmphead++;
	}
	cout << tmpsum << endl;

	//delete [] pc;
}

tick tickseries::gettk(const char* s){
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

void tickseries::test(){
		fstream ts("1.csv");
		tickseries* ptd= new tickseries("1.HK");
		/*std::string s1, s2, s3, s4, s5;
		if (ts >> s1 >> s2 >> s3 >> s4 >> s5){}*/
		///"01/02/2013 01:20:00,119.3,23000,U"
		string line;
		const int linebufsize=256;
		char ln[linebufsize]={};
		int num=0;
		if (ts.is_open()){
			while (ts.good() && !ts.eof()){
				ts.getline(ln,linebufsize,'\n');
				if (*ln){
					ptd->push(gettk(ln));
				}
			}
		}
		ptd->update();
		//ptd->head();

		ptd->printbycolumn();
	}

tickseries::tickseries(const string& s):symbol(s)
{
}

tickseries::~tickseries()
{
}

