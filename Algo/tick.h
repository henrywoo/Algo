#include <string>
#include <list>
#include <iomanip> //setw
using namespace std;

///@brief price and volume
struct PV
{
	double price;
	int volume;
};

class tick{
public:
	time_t timestamp;
	string timestr;
	list<PV> pvs;
	char tag[3];

	double vwap;
	int sumofvol;
	double sumofcash;//unit of 1K
	
	struct {
		double vwap;
		double vol;
		double cash;
		double time;
	} changes;
	
	int lastupdate;	
	void update();
	tick():vwap(0),sumofcash(0),sumofvol(0){
		memset(tag,'\0',3);
		changes.vwap=0;
		changes.vol=0;
		changes.cash=0;
		changes.time=0;
	}
};

void tick::update(){
	list<PV>::reverse_iterator i=pvs.rbegin();
	while(lastupdate-- > 0){
		sumofcash += (i->price * i->volume)/1000;
		sumofvol += i->volume;
		++i;
	}
	vwap=sumofcash*1000/sumofvol;
}

class tickdata
{
public:
	tickdata(const string& s);
	~tickdata();

	void push(tick t){
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

	void update(){
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

	void head(){
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

private:
	list<tick> ts;
	string symbol;
};

tickdata::tickdata(const string& s):symbol(s)
{
}

tickdata::~tickdata()
{
}