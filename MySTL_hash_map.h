#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "MySTL_allocate.h"
#include "MySTL_hash_func.h"
#include "MySTL_hashtable.h"
#include "MySTL_functional.h"
#include <utility>

namespace stupid
{
	template <typename Key, typename Data, typename HashFunc = stupid::hash<Key>, typename EqualKey=stupid::equal_to<Key>, typename Allocator = stupid::allocator>
	class hash_map
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

		hash_map()
			:table(100, hasher(), key_equal())
		{
		}

		explicit hash_map(size_type n)
			:table(n, hasher(), key_equal())
		{
		}

		hash_map(size_type n, const hasher &h)
			:table(n, h, key_equal())
		{
		}

		hash_map(size_type n, const hasher &h, const key_equal &e)
			:table(n, h, e)
		{
		}

		template <typename InputIterator>
		hash_map(InputIterator first, InputIterator last)
			: table(100, hasher(), key_equal())
		{
		}

		template <typename InputIterator>
		hash_map(InputIterator first, InputIterator last, size_type n)
			: table(n, hahser(), key_equal())
		{
			table.insert_unique(first, last);
		}

		template <typename InputIterator>
		hash_map(InputIterator first, InputIterator last, size_type n, const hasher &h)
			: table(n, h, key_equal())
		{
			table.insert_unique(first, last);
		}

		template <typename InputIterator>
		hash_map(InputIterator first, InputIterator last, size_type n, const hasher &h, const key_equal &e)
			: table(n, h, e)
		{
			table.insert_unique(first, last);
		}

		hash_map(const hash_map &x)
		{
			table(x.table);
		}

		hash_map &operator=(const hash_map &x)
		{
			if (&x != this)
				table = x.table;
			return *this;
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
			return table.insert_unique(value);
		}

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			table.insert_unique(first, last);
		}

		std::pair<iterator, bool> insert_noresize(const value_type &value)
		{
			return table.insert_unique_noresize(value);
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

		data_type &operator[](const key_type &key)
		{
			return (*(insert(std::pair<key_type, data_type>(key, data_type())).first)).second;
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

		friend bool operator==(const hash_map &x, const hash_map &y);
		friend bool operator!=(const hash_map &x, const hash_map &y);
	};

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator==(const hash_map<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_map<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return x.table == y.table;
	}

	template <typename Key, typename Data, typename HashFunc, typename EqualKey, typename Allocator>
	bool operator!=(const hash_map<Key, Data, HashFunc, EqualKey, Allocator> &x, const hash_map<Key, Data, HashFunc, EqualKey, Allocator> &y)
	{
		return x.table == y.table;
	}
}

#endif