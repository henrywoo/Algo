#ifndef __STACKWITHMIN__
#define __STACKWITHMIN__

#include <iostream>
using namespace std;

namespace wufuheng{

	// stack as a linked list
	template <typename T>
	struct stacknode{
		typedef stacknode<T> * stacknodepointer;
		stacknodepointer prev;
		stacknodepointer next;
		T data;
	};


	template <typename T>
	class stack{
		typedef stacknode<T> * stacknodepointer;
	private:
		stacknodepointer topnode;

		T getdata(int i) const{
			if (topnode==NULL){
				return NULL;
			}
			stacknodepointer tmp=topnode;
			while(i>0){
				tmp = tmp->next;
				i--;
			}
			return tmp->data;
		}

	protected:
		size_t st;
	public:
		stack():topnode(NULL),st(0){}

		~stack(){
			while(topnode){
				stacknodepointer tmp=topnode;
				topnode=topnode->next;
				delete tmp;
			}
		}

		bool isempty(){
			return topnode==NULL;
		}

		// O(1)
		bool push(const T& i){
			stacknodepointer tmp=new stacknode<T>;
			tmp->data = i;
			tmp->next = topnode;
			tmp->prev = NULL;
			if (topnode!=NULL){
				topnode->prev = tmp;
			}
			topnode = tmp;
			st++;
			return true;
		}

		// O(1)
		T pop(){
			stacknode<T> tmp(*topnode);
			topnode = topnode->next;
			topnode->prev=topnode;
			st--;
			return tmp->data;
		}

		// O(1)
		T top() const{
			return topnode->data;
		}

		size_t size() const{
			return st;
		}

	};

	///@brief augmented stack
	//////////////////////////////////////////////////////////////////////////
	///You  can  implement  this  by  having  each  node  in  the \
	stack  keep  track  of  the  minimum  be-neath itself   Then,\
	to find the min, you just look at what the top element thinks\
	is the min.
	///wufuheng::stackwithminmax<double>::test();
	template <typename T>
	struct stackwithminnode: public stacknode<T>{
		T localmin;
		T localmax;
	};

	template <typename T>
	class stackwithminmax{
		stackwithminnode<T>* head;
		stackwithminnode<T>* tail;
		size_t st;
	public:
		stackwithminmax():head(NULL),tail(NULL),st(0){}
		~stackwithminmax(){
			while(head){
				stackwithminnode<T>* tmp = head;
				// dynamic_cast requires virtual function - polymorphisms, so here we must use static_cast
				head = static_cast<stackwithminnode<T>*>(head->next);
				delete tmp;
			}
		}

		bool push(const T& x){
			stackwithminnode<T> * tmp=new stackwithminnode<T>;
			tmp->data = x;
			if (head==NULL){
				tmp->prev=NULL;
				tmp->next=NULL;
				tmp->localmin = x;
				tmp->localmax = x;
				head = tmp;
				tail=tmp;
			}else{
				tmp->next = head;
				head->prev= tmp;

				tmp->prev=NULL;

				tmp->localmin = std::min(head->localmin,x);
				tmp->localmax = std::max(head->localmax,x);
				
				head=tmp;
			}
			st++;
			return true;
		}

		void pop(){
			if (st>0)
			{
				stackwithminnode<T> * tmp=head;
				head=static_cast<stackwithminnode<T>*>(tail->next);
				delete tmp;
				st--;
			}
			
		}


		T min(){
			if (head==NULL){
				return INT_MAX;
			}else{
				return head->localmin;
			}
		}

		T max(){
			if (head==NULL){
				return INT_MIN;
			}else{
				return head->localmax;
			}
		}

		static void test(){
			stack<double> s;
			s.push(3.14);
			s.push(0.1);
			s.push(7.1);
			s.push(1.1);

			double d=s.top();
			cout << d << endl;
			cout << s.size() << endl;

			stackwithminmax<double> swm;
			swm.push(1.3);
			cout << swm.min() << " " << swm.max() << endl;
			swm.push(13.1);
			cout << swm.min() << " " << swm.max() << endl;
			swm.push(4.9);
			cout << swm.min() << " " << swm.max() << endl;
			swm.push(0.1);
			cout << swm.min() << " " << swm.max() << endl;
			swm.push(7.5);
			cout << swm.min() << " " << swm.max() << endl;
			swm.pop();
			cout << swm.min() << " " << swm.max() << endl;
			swm.pop();
			cout << swm.min() << " " << swm.max() << endl;
			swm.pop();
			cout << swm.min() << " " << swm.max() << endl;
			swm.pop();
			cout << swm.min() << " " << swm.max() << endl;
			swm.pop();
			cout << swm.min() << " " << swm.max() << endl;
			swm.pop();
			cout << swm.min() << " " << swm.max() << endl;

			
		}
	};

	

}

#endif
