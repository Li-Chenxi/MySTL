#ifndef SET_H
#define SET_H

#include "MySTL_tree.h"
#include "MySTL_functional.h"
#include "MySTL_allocate.h"
#include <utility>

namespace stupid
{
	template <typename Key, typename Compare = stupid::less<Key>, typename Allocator=stupid::allocator>
	class set
	{
	public:
		typedef Key key_type;
		typedef Key value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
	private:
		typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Allocator> rep_type;
		rep_type tree;
	public:
		typedef typename rep_type::pointer pointer;
		typedef typename rep_type::const_pointer const_pointer;
		typedef typename rep_type::reference reference;
		typedef typename rep_type::const_reference const_reference;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::const_iterator const_iterator;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::difference_type difference_type;

		set()
			:tree(Compare())
		{
		}

		explicit set(const Compare &x)
			:tree(x)
		{
		}

		template <typename InputIterator>
		set(InputIterator first, InputIterator last)
			: tree(Compare())
		{
			tree.insert_unique(first, last);
		}

		template <typename InputIterator>
		set(InputIterator first, InputIterator last, const Compare &x)
			: tree(x)
		{

		}

		set(const set &x) :
			tree(x.tree)
		{
		}

		set &operator=(const set &x)
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
			return tree.key_comp();
		}

		iterator begin()
		{
			return tree.begin();
		}

		const_iterator begin() const
		{
			return tree.begin();
		}

		iterator end()
		{
			return tree.end();
		}

		const_iterator end() const
		{
			return tree.end();
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

		void swap(set &x)
		{
			tree.swap(x.tree);
		}

		std::pair<iterator, bool> insert(const key_type &k)
		{
			return tree.insert_unique(k);
		}

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			tree.insert_unique(first, last);
		}

		void erase(iterator position)
		{
			tree.erase(position);
		}

		void erase(const key_type &k)
		{
			tree.erase(k);
		}

		void erase(iterator first, iterator last)
		{
			tree.erase(first, last);
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
			return tree.equal_range(k);
		}

		std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const
		{
			return tree.equal_range(k);
		}

		friend bool operator==<>(const set &x, const set &y);
		friend bool operator!=<>(const set &x, const set &y);
		friend bool operator< <>(const set &x, const set &y);
		friend bool operator> <>(const set &x, const set &y);
		friend bool operator<=<>(const set &x, const set &y);
		friend bool operator>=<>(const set &x, const set &y);
	};

	template <typename Key, typename Compare, typename Allocator>
	bool operator==(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y)
	{
		return x.tree == y.tree;
	}

	template <typename Key, typename Compare, typename Allocator>
	bool operator!=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y)
	{
		return x.tree != y.tree;
	}

	template <typename Key, typename Compare, typename Allocator>
	bool operator< (const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y)
	{
		return x.tree < y.tree;
	}

	template <typename Key, typename Compare, typename Allocator>
	bool operator>(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y)
	{
		return x.tree > y.tree;
	}

	template <typename Key, typename Compare, typename Allocator>
	bool operator<=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y)
	{
		return x.tree <= y.tree;
	}

	template <typename Key, typename Compare, typename Allocator>
	bool operator>= (const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y)
	{
		return x.tree >= y.tree;
	}
}

#endif