#include <string>
#include <list>


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
		time_t time;
	} changes;
	
	int lastupdate;	
	void update();
	tick();
};


///@brief is-implemented-in-terms-of list<tick>
class tickseries
{
public:
	tickseries(const string& s);
	~tickseries();

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