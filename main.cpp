#include "MySTL_vector.h"
#include "MySTL_list.h"
#include "MySTL_algobase.h"
#include "MySTL_deque.h"
#include "MySTL_priority_queue.h"
#include "MySTL_numeric.h"
#include "MySTL_slist.h"
#include "MySTL_stack.h"
#include "MySTL_queue.h"
#include <iostream>
#include <string>

using namespace stupid;

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;


	queue<int> st;
	st.push_back(2);

	st.push_back(3);
	st.push_back(4);
	st.push_back(5);
	st.push_back(6);

	while (!st.empty())
	{
	cout << st.front() << endl;
	st.pop_front();
	}

	/*stack<int> st;
	st.push(2);

	st.push(3);
	st.push(4);
	st.push(5);
	st.push(6);

	while (!st.empty())
	{
		cout << st.top() << endl;
		st.pop();
	}*/



	/*int a[5] = { 1, 2, 3, 4, 5 };
	stupid::adjacent_difference(a, a + 5, a);
	stupid::partial_sum(a, a + 5, a);*/

	/*slist<int> s;
	for (int i = 0; i < 140; ++i)
		s.push_front(i);

	for (slist<int>::iterator i = s.begin(); i != s.end(); ++i)
		cout << *i << endl;*/

	/*deque<std::string> s;
	s.push_back("Hello");
	s.push_back("My");
	s.push_back("STL");
	s.push_front("Good Afternoon");

	deque<std::string> x;
	x.push_back("Hello");
	x.push_back("My");
	x.push_back("STL");
	x.push_front("Good Afternoon");
	x[2] = "NVIDIA";

	for (deque<std::string>::size_type i = 0; i < s.size(); ++i)
		cout << s[i]<<' '<<x[i] << endl;

	cout.setf(std::ios_base::boolalpha);
	cout << (x == s);*/
	

	/*priority_queue<int> q;

	q.push(8);
	q.push(9);
	q.push(6);
	q.push(7);
	q.push(1);
	q.push(14);

	priority_queue<int>::size_type i = q.size();
	cout << "size=" << i << endl;

	for (priority_queue<int>::size_type t = 0; t < i; ++t)
	{
		cout << q.top() << endl;
		q.pop();
	}
	*/

	/*deque<int> x(20,9);
	cout << "size" << x.size()<<endl;

	for (deque<int>::size_type i = 0; i < x.size(); ++i)
		x[i] = i;

	for (deque<int>::size_type i = 0; i < x.size(); ++i)
		cout << x[i] << ' ';
	cout << endl;

	for (int i = 0; i < 50; ++i)
		x.push_back(i);
	
	for (deque<int>::size_type i = 0; i < x.size(); ++i)
		cout << x[i] << ' ';
	cout << endl;
	cout << "size" << x.size() << endl;

	for (int i = 100; i < 120; ++i)
		x.push_front(i);

	for (deque<int>::size_type i = 0; i < x.size(); ++i)
		cout << x[i] << ' ';
	cout << endl;
	cout << "size" << x.size() << endl;

	deque<int>::iterator ite = stupid::find(x.begin(), x.end(), 23);
	if (ite != x.end())
		x.erase(ite);

	for (deque<int>::size_type i = 0; i < x.size(); ++i)
		cout << x[i] << ' ';
	cout << endl;
	cout << "size" << x.size() << endl;*/



	/*vector<int> iv(2, 9);
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	iv.push_back(1);
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	iv.push_back(2);
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	iv.push_back(3);
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	iv.push_back(4);
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	for (vector<int>::size_type i = 0; i < iv.size(); ++i)
		cout << iv[i] << ' ';
	cout << endl;

	iv.push_back(5);
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	for (vector<int>::size_type i = 0; i < iv.size(); ++i)
		cout << iv[i] << ' ';
	cout << endl;

	iv.pop_back();
	iv.pop_back();

	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;


	iv.pop_back();
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	vector<int>::iterator ivite = stupid::find(iv.begin(), iv.end(), 1);
	if (ivite != iv.end())
		iv.erase(ivite);

	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	for (vector<int>::size_type i = 0; i < iv.size(); ++i)
		cout << iv[i] << ' ';
	cout << endl;


	ivite = stupid::find(iv.begin(), iv.end(), 2);
	if (ivite != iv.end())
		iv.insert(ivite, 3, 7);

	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;

	for (vector<int>::size_type i = 0; i < iv.size(); ++i)
		cout << iv[i] << ' ';
	cout << endl;

	iv.clear();
	cout << "size=" << iv.size() << endl;
	cout << "capatity=" << iv.capacity() << endl;*/

	/*list<int> ilist;
	cout << "size=" << ilist.size() << endl;


	ilist.push_back(0);
	ilist.push_back(9);
	ilist.push_back(2);
	ilist.push_back(4);
	ilist.push_back(7);
	ilist.push_back(5);
	cout << "size=" << ilist.size() << endl;

	list<int> xlist;
	xlist.push_back(18);
	xlist.push_back(14);
	xlist.push_back(27);
	xlist.push_back(47);
	xlist.push_back(31);
	xlist.push_back(11);

	ilist.sort();
	xlist.sort();

	ilist.splice(ilist.begin(),xlist);

	ilist.erase(stupid::find(ilist.begin(), ilist.end(), 31));

	list<int>::iterator ite;
	for (ite = ilist.begin(); ite != ilist.end(); ++ite)
		cout << *ite << ' ';
	cout << endl;

	ite = stupid::find(ilist.begin(), ilist.end(), 3);
	if (ite != ilist.end())
		ilist.insert(ite, 99);


	cout << "size=" << ilist.size() << endl;
	cout << *ite << endl;

	for (ite = ilist.begin(); ite != ilist.end(); ++ite)
		cout << *ite << ' ';
	cout<< endl;

	ite = stupid::find(ilist.begin(), ilist.end(), 1);
	if (ite != ilist.end())
		cout << *(ilist.erase(ite)) << endl;

	for (ite = ilist.begin(); ite != ilist.end(); ++ite)
		cout << *ite<<' ';
	cout <<endl;*/

}