#ifndef HASH_MULTISET_H
#define HASH_MULITSET_H

#include "MySTL_hash_func.h"
#include "MySTL_allocate.h"
#include "MySTL_functional.h"
#include "MySTL_hashtable.h"
#include <utility>

namespace stupid
{
	template <typename Value, typename HashFunc = stupid::hash<Value>, typename EqualKey = stupid::equal_to<Value>, typename Allocator = stupid::allocator>
	class hash_multiset
	{
	private:
		typedef stupid::hashtable<Value, Value, HashFunc, stupid::identity<Value>, EqualKey, Allocator> ht;
		ht table;
	public:
		typedef typename ht::size_type size_type;
		typedef typename ht::difference_type difference_type;
		typedef typename ht::iterator iterator;
		typedef typename ht::const_iterator const_iterator;
		typedef typename ht::pointer pointer;
		typedef typename ht::const_pointer const_pointer;
		typedef typename ht::reference reference;
		typedef typename ht::const_reference const_reference;

		typedef typename ht::key_type key_type;
		typedef typename ht::value_type value_type;
		typedef typename ht::hasher hasher;
		typedef typename ht::key_equal key_equal;

		hasher hash_func() const
		{
			return table.hash_func();
		}

		key_equal key_eq() const
		{
			return table.key_eq();
		}

		hash_multiset()
			:table(100, hasher(), key_equal())
		{
		}

		explicit hash_multiset(size_type n)
			:table(n, hasher(), key_equal())
		{
		}

		hash_multiset(size_type n, const hasher &h)
			:table(n, h, key_equal())
		{
		}

		hash_multiset(size_type n, const hasher &h, const key_equal &e)
			:table(n, h, e)
		{
		}

		template <typename InputIterator>
		hash_multiset(InputIterator first, InputIterator last)
			: table(100, hasher(), key_equal())
		{
			table.insert_equal(first, last);
		}

		template <typename InputIterator>
		hash_multiset(InputIterator first, InputIterator last, size_type n)
			: table(n, hasher(), key_equal())
		{
			table.insert_equal(first, last);
		}

		template <typename InputIterator>
		hash_multiset(InputIterator first, InputIterator last, size_type n, const hasher &h)
			: table(n, h, key_equal())
		{
			table.insert_equal(first, last);
		}

		template <typename InputIterator>
		hash_multiset(InputIterator first, InputIterator last, size_type n, const hasher &h, const key_equal &e)
			: table(n, h, e)
		{
			table.insert_equal(first, last);
		}

		size_type size() const
		{
			return table.size();
		}

		size_type max_size() const
		{
			return table.max_size();
		}

		bool empty() const
		{
			return table.empty();
		}

		void swap(hash_multiset &x)
		{
			table.swap(x.table);
		}

		friend bool operator==(const hash_multiset &x, const hash_multiset &y);
		friend bool operator< (const hash_multiset &x, const hash_multiset &y);

		iterator begin()
		{
			return table.begin();
		}

		const_iterator begin() const
		{
			return table.begin();
		}

		iterator end()
		{
			return table.end();
		}

		const_iterator end() const
		{
			return table.end();
		}

		std::pair<iterator, iterator> insert(const value_type &value)
		{
			return table.insert_equal(value);
		}

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			table.insert_equal(first, last);
		}

		std::pair<iterator, iterator> insert_noresize(const value_type &value)
		{
			return table.insert_equal_noresize(value);
		}

		iterator find(const key_type &key)
		{
			return table.find(key);
		}

		const_iterator find(const key_type &key) const
		{
			return table.find(key);
		}

		size_type count(const key_type &key) const
		{
			return table.count(key);
		}

		std::pair<iterator, iterator> equal_range(const key_type &key)
		{
			return table.equal_range(key);
		}

		std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
		{
			return table.equal_range(key);
		}

		size_type erase(const key_type &key)
		{
			return table.erase(key);
		}

		void erase(iterator position)
		{
			return table.erase(position);
		}

		void erase(iterator first, iterator last)
		{
			return table.erase(first, last);
		}

		void clear()
		{
			table.clear();
		}

		void resize(size_type hint)
		{
			table.resize(hint);
		}

		size_type bucket_count() const
		{
			return table.buckets_count();
		}

		size_type max_bucket_count() const
		{
			return table.max_bucket_count();
		}
	};

	template <typename Value, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator==(const hash_multiset<Value, HashFunc, EqualKey, Allocator> &x, const hash_multiset<Value, HashFunc, EqualKey, Allocator> y)
	{
		return x.table == y.table;
	}

	template <typename Value, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator!=(const hash_multiset<Value, HashFunc, EqualKey, Allocator> x, const hash_multiset<Value, HashFunc, EqualKey, Allocator> y)
	{
		return !(x.table == y.table);
	}

	template <typename Value, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator< (const hash_multiset<Value, HashFunc, EqualKey, Allocator> x, const hash_multiset<Value, HashFunc, EqualKey, Allocator> y)
	{
		return x.table < y.table;
	}

	template <typename Value, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator>(const hash_multiset<Value, HashFunc, EqualKey, Allocator> x, const hash_multiset<Value, HashFunc, EqualKey, Allocator> y)
	{
		return y.table < x.table;
	}

	template <typename Value, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator<=(const hash_multiset<Value, HashFunc, EqualKey, Allocator> x, const hash_multiset<Value, HashFunc, EqualKey, Allocator> y)
	{
		return !(x.table > y.table);
	}

	template <typename Value, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator>=(const hash_multiset<Value, HashFunc, EqualKey, Allocator> x, const hash_multiset<Value, HashFunc, EqualKey, Allocator> &y)
	{
		return !(x.table < y.table);
	}
}

#endif