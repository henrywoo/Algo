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
	tick();
};


class tickdata
{
public:
	tickdata(const string& s);
	~tickdata();
	void push(tick t);
	void update();
	void head();
	void printbycolumn();
	static tick gettk(const char* s);
	static void test();
//private:
	list<tick> ts;
	string symbol;
};