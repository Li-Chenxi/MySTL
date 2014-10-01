#ifndef MAP_H
#define MAP_H

#include "MySTL_functional.h"
#include "MySTL_allocate.h"
#include "MySTL_tree.h"
#include <utility>

namespace stupid
{
	 template <typename Key, typename Data ,typename Compare=less<Key>, typename Allocator=stupid::allocator>
	 class map
	 {
	 public:
		 typedef Key key_type;
		 typedef Data data_type;
		 typedef Data mapped_type;
		 typedef std::pair<key_type, data_type> value_type;
		 typedef Compare key_compare;

		 class value_compare :public binary_function < value_type , value_type, bool >
		 {
			 friend class map < Key, Data, Compare, Allocator > ;
		 protected:
			 Compare comp;
			 value_compare(const Compare &c)
				 :comp(c)
			 {
			 }
		 public:
			 bool operator()(const value_type &x, const value_type &y)
			 {
				 return comp(x.first, y.first);
			 }
		 };
	 private:
		 typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Allocator> rep_type;
		 rep_type tree;
	 public:
		 typedef typename rep_type::pointer pointer;
		 typedef typename rep_type::const_pointer const_pointer;
		 typedef typename rep_type::reference reference;
		 typedef typename rep_type::const_reference const_reference;
		 typedef typename rep_type::iterator iterator;
		 typedef typename rep_type::const_iterator const_iterator;
		 typedef typename rep_type::reverse_iterator reverse_iterator;
		 typedef typename rep_type::const_reverse_iterator const_reverse_iterator;

		 typedef typename rep_type::size_type size_type;
		 typedef typename rep_type::difference_type difference_type;

		 map()
			 :tree(Compare())
		 {
		 }

		 explicit map(const Compare &c)
			 :tree(c)
		 {
		 }

		 template <typename InputIterator>
		 map(InputIterator first, InputIterator last)
			 : tree(Compare())
		 {
			 tree.insert_unique(first, last);
		 }

		 template <typename InputIterator>
		 map(InputIterator first, InputIterator last, const Compare &c)
			 : tree(c)
		 {
			 tree.inset_unique(first, last);
		 }

		 map(const map &x)
			 :tree(x.tree)
		 {
		 }

		 map &operator=(const map&x)
		 {
			 tree = x.tree;
			 return *this;
		 }

		 key_compare key_comp() const
		 {
			 return tree.key_comp();
		 }

		 value_compare value_comp() const
		 {
			 return value_compare(t.key_compare());
		 }

		 iterator begin()
		 {
			 return tree.begin();
		 }

		 const_iterator begin() const
		 {
			 return tree.begin();
		 }

		 reverse_iterator rbegin()
		 {
			 return tree.rbegin();
		 }

		 const_reverse_iterator rbegin() const
		 {
			 return tree.rbegin();
		 }

		 iterator end()
		 {
			 return tree.end();
		 }

		 const_iterator end() const
		 {
			 return tree.end();
		 }

		 reverse_iterator rend()
		 {
			 return tree.rend();
		 }

		 const_reverse_iterator rend() const
		 {
			 return tree.rend();
		 }

		 bool empty() const
		 {
			 return tree.empty();
		 }

		 size_type size() const
		 {
			 return tree.size();
		 }

		 size_type max_size() const
		 {
			 return tree.max_size();
		 }
		 
		 Data &operator[](const key_type &k)
		 {
			 iterator i = lower_bound(k);
			 if (i == end() || key_compare()(k, (*i).first))
				 return (*(insert(value_type(k, Data()))).first).second;
			 else
				 return (*i).second;
		 }

		 void swap(map &x)
		 {
			 tree.swap(x.tree);
		 }

		 std::pair<iterator, bool> insert(const value_type &x)
		 {
			 return tree.insert_unique(x);
		 }

		 template <typename InputIterator>
		 void insert(InputIterator first, InputIterator last)
		 {
			 tree.insert_unique(first, last);
		 }

		 void erase(iterator pos)
		 {
			 tree.erase(pos);
		 }

		 size_type erase(const key_type &k)
		 {
			 return tree.erase(k);
		 }

		 void clear()
		 {
			 tree.clear();
		 }

		 iterator find(const key_type &k)
		 {
			 return tree.find(k);
		 }

		 const_iterator find(const key_type &k) const
		 {
			 return tree.find(k);
		 }

		 iterator lower_bound(const key_type &k)
		 {
			 return tree.lower_bound(k);
		 }

		 const_iterator lower_bound(const key_type &k) const
		 {
			 return tree.lower_bound(k);
		 }
		 
		 iterator upper_bound(const key_type &k)
		 {
			 return tree.upper_bound(k);
		 }

		 const_iterator upper_bound(const key_type &k) const
		 {
			 return tree.upper_bound(k);
		 }

		 std::pair<iterator, iterator> equal_range(const key_type &k)
		 {
			 return equal_range(k);
		 }

		 std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const
		 {
			 return equal_range(k);
		 }

		 friend bool operator==<>(const map &x, const map &y);
		 friend bool operator!=<>(const map &x, const map &y);
		 friend bool operator< <>(const map &x, const map &y);
		 friend bool operator> <>(const map &x, const map &y);
		 friend bool operator<=<>(const map &x, const map &y);
		 friend bool operator>=<>(const map &x, const map &y);
	 };

	 template <typename Key, typename Value, typename Compare, typename Allocator>
	 bool operator==(const map<Key, Value, Compare, Allocator> &x, const map<Key, Value, Compare, Allocator> &y)
	 {
		 return x.tree == y.tree;
	 }

	 template <typename Key, typename Value, typename Compare, typename Allocator>
	 bool operator!=(const map<Key, Value, Compare, Allocator> &x, const map<Key, Value, Compare, Allocator> &y)
	 {
		 return x.tree != y.tree;
	 }

	 template <typename Key, typename Value, typename Compare, typename Allocator>
	 bool operator< (const map<Key, Value, Compare, Allocator> &x, const map<Key, Value, Compare, Allocator> &y)
	 {
		 return x.tree < y.tree;
	 }

	 template <typename Key, typename Value, typename Compare, typename Allocator>
	 bool operator> (const map<Key, Value, Compare, Allocator> &x, const map<Key, Value, Compare, Allocator> &y)
	 {
		 return x.tree > y.tree;
	 }

	 template <typename Key, typename Value, typename Compare, typename Allocator>
	 bool operator<=(const map<Key, Value, Compare, Allocator> &x, const map<Key, Value, Compare, Allocator> &y)
	 {
		 return x.tree <= y.tree;
	 }

	 template <typename Key, typename Value, typename Compare, typename Allocator>
	 bool operator>= (const map<Key, Value, Compare, Allocator> &x, const map<Key, Value, Compare, Allocator> &y)
	 {
		 return x.tree >= y.tree;
	 }
}

#endif