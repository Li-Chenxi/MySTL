#ifndef HASH_MULTIMAP_H
#define HASH_MULTIMAP_H

#include "MySTL_allocate.h"
#include "MySTL_hash_func.h"
#include "MySTL_hashtable.h"
#include "MySTL_functional.h"
#include <utility>

namespace stupid
{
	template <typename Key, typename Data, typename HashFunc = stupid::hash<Key>, typename EqualKey = stupid::equal_to<Key>, typename Allocator = stupid::allocator>
	class hash_multimap
	{
	private:
		typedef hashtable<std::pair<const Key, Data>, Key, HashFunc, select1st<std::pair<const Key, Data>>, EqualKey, Allocator> ht;
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
		typedef Data data_type;
		typedef Data mapped_type;
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

		hash_multimap()
			:table(100, hasher(), key_equal())
		{
		}

		explicit hash_multimap(size_type n)
			:table(n, hasher(), key_equal())
		{
		}

		hash_multimap(size_type n, const hasher &h)
			:table(n, h, key_equal())
		{
		}

		hash_multimap(size_type n, const hasher &h, const key_equal &e)
			:table(n, h, e)
		{
		}

		template <typename InputIterator>
		hash_multimap(InputIterator first, InputIterator last)
			: table(100, hasher(), key_equal())
		{
		}

		template <typename InputIterator>
		hash_multimap(InputIterator first, InputIterator last, size_type n)
			: table(n, hahser(), key_equal())
		{
			table.insert_equal(first, last);
		}

		template <typename InputIterator>
		hash_multimap(InputIterator first, InputIterator last, size_type n, const hasher &h)
			: table(n, h, key_equal())
		{
			table.insert_equal(first, last);
		}

		template <typename InputIterator>
		hash_multimap(InputIterator first, InputIterator last, size_type n, const hasher &h, const key_equal &e)
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

		size_type buckets_count() const
		{
			return table.buckets_count();
		}

		size_type max_bucket_count() const
		{
			return table.max_bucket_count();
		}

		bool empty() const
		{
			return table.empty();
		}

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

		std::pair<iterator, bool> insert(const value_type &value)
		{
			return table.insert_equal(value);
		}

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			table.insert_equal(first, last);
		}

		std::pair<iterator, bool> insert_noresize(const value_type &value)
		{
			return table.insert_equal_noresize(value);
		}

		size_type erase(const key_type &key)
		{
			return table.erase(key);
		}

		void erase(iterator position)
		{
			table.erase(position);
		}

		template <typename InputIterator>
		void erase(InputIterator first, InputIterator last)
		{
			table.erase(first, last);
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

		void resize(size_type hint)
		{
			table.resize(hint);
		}

		friend bool operator==(const hash_multimap &x, const hash_multimap &y);
		friend bool operator< (const hash_multimap &x, const hash_multimap &y);
	};

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator==(const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return x.table == y.table;
	}

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator< (const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return x.table < y.table;
	}

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator!=(const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return !(x == y);
	}

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator>(const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return y < x;
	}

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator>=(const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return !(x < y);
	}

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator<=(const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_multimap<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return !(x > y);
	}
}

#endif