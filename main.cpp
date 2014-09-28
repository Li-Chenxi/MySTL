#include "MySTL_vector.h"
#include "MySTL_list.h"
#include "MySTL_algobase.h"
#include "MySTL_deque.h"
#include "MySTL_priority_queue.h"
#include "MySTL_numeric.h"
#include "MySTL_slist.h"
#include "MySTL_stack.h"
#include "MySTL_queue.h"
#include "MySTL_tree.h"
#include "MySTL_functional.h"
#include "MySTL_map.h"
#include "MySTL_set.h"
#include "MySTL_hashtable.h"
#include "MySTL_hash_set.h"
#include "MySTL_hash_multiset.h"
#include "MySTL_hash_map.h"
#include "MySTL_hash_multimap.h"
#include "MySTL_hash_func.h"
#include <iostream>
#include <string>
#include <utility>

using namespace stupid;

struct eqstr
{
	bool operator()(const char *s, const char *t) const 
	{
		return strcmp(s, t) == 0;
	}
};

void lookup(const hash_set<const char *, hash<const char *>, eqstr > &x, const char * key)
{
	using std::cout;
	using std::endl;
	
	cout << (((x.find(key))==x.end()) ? "no" : "yes")<< endl;
}

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;
	using std::string;
	using std::pair;


	int a[8];
	istream_iterator<int> i(cin),of;
	int k = 0;
	while (i!=of)
	{
		a[k++] = *i;
		++i;
	}


	for (auto i : a)
	{
		cout << i << ' ';
	}
	cout << endl;


	/*int a[] = { 1, 2, 3, 4, 7, 5, 6 };
	vector<int> v(a,a+sizeof(a)/(sizeof(*a)));

	for (auto i = v.rbegin(); i != v.rend(); ++i)
		cout << *i << ' ';
	cout << endl;*/

	/*hash_map<const char *, int, hash<const char *>, eqstr> map;

	map["january"] = 31;
	map["february"] = 28;
	map["march"] = 31;
	map["april"] = 30;
	map["may"] = 31;
	map["june"] = 30;
	map["july"] = 31;
	map["august"] = 31;
	map["september"] = 30;
	map["october"] = 31;
	map["november"] = 30;
	map["december"] = 31;

	cout << "sep:" << map["september"] << endl;
	cout << "dec:" << map["december"] << endl;

	for (auto i : map)
	{
		cout << i.first << ' ' << i.second << endl;
	}
	*/

	/*hash_set<const char *, hash<const char *>, eqstr> set;
	set.insert("kiwi");
	set.insert("plum");
	set.insert("apple");
	set.insert("mango");
	set.insert("apricot");
	set.insert("banana");

	lookup(set, "mango");
	lookup(set, "apple");
	lookup(set, "durian");
	
	for (auto i = set.begin(); i != set.end(); ++i)
	{
		cout << *i << ' ';
	}
	cout << endl;*/

	/*map<string, int> m;
	m.insert(std::pair<string, int>("AMD", 2));

	m["IBM"] = 3;
	m["Sun"] = 4;

	cout << m["AMD"] << endl;
	m.insert(pair<string, int>("Intel", 1));

	for (auto i = m.begin(); i != m.end(); ++i)
		cout << (*i).first << (*i).second << endl;*/

	/*vector<int> for_insert;
	for (int i = 400; i <= 404; ++i)
		for_insert.push_back(i);
	set<int> s(for_insert.begin(), for_insert.end());
	s.insert(1);
	s.insert(15);
	for (auto i = s.begin(); i != s.end(); ++i)
		cout << *i<< ' ';
	cout << endl;*/

	/*cout << s.size() << endl;
	s.clear();
	cout << s.size() << endl;
	cout << *(--s.begin()) << endl;*/

	/*rb_tree<int, int, identity<int>, less<int>> my_tree;
	cout << my_tree.size() << endl;

	my_tree.insert_unique(12);
	my_tree.insert_unique(1);
	my_tree.insert_unique(9);
	my_tree.insert_unique(2);
	my_tree.insert_unique(0);
	my_tree.insert_unique(11);
	my_tree.insert_unique(7);
	my_tree.insert_unique(19);
	my_tree.insert_unique(4);

	my_tree.insert_unique(15);
	my_tree.insert_unique(18);
	my_tree.insert_unique(5);
	my_tree.insert_unique(14);
	my_tree.insert_unique(13);
	my_tree.insert_unique(10);
	my_tree.insert_unique(16);
	my_tree.insert_unique(6);
	my_tree.insert_unique(3);

	my_tree.insert_unique(8);
	my_tree.insert_unique(17);

	cout.setf(std::ios_base::boolalpha);
	auto result = my_tree.insert_unique(18);
	cout << result.second << endl;


	cout << my_tree.size() << endl;

	rb_tree<int, int, identity<int>, less<int>> tree_copy = my_tree;
	rb_tree<int, int, identity<int>, less<int>> tree_copy2 = tree_copy;
	tree_copy2 = my_tree;

	cout.setf(std::ios_base::boolalpha);
	cout << (tree_copy == my_tree) << endl;
	cout << my_tree.size() << endl;
	cout << tree_copy.size() << endl;

	auto i = tree_copy.find(3);
	cout << *i << endl;

	tree_copy.insert_equal(18);
	for (auto i = tree_copy2.begin(); i != tree_copy2.end(); ++i)
	{
		cout << *i << ' ' << endl;
	}

	cout << (tree_copy == tree_copy2) << endl;*/

	/*queue<int> st;
	st.push_back(2);

	st.push_back(3);
	st.push_back(4);
	st.push_back(5);
	st.push_back(6);

	while (!st.empty())
	{
	cout << st.front() << endl;
	st.pop_front();
	}*/

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
	cout << (x == s);
	*/

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