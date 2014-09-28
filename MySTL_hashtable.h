#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "MySTL_iterator.h"
#include "MySTL_allocate.h"
#include "MySTL_vector.h"
#include "MySTL_algo.h"
#include "MySTL_algobase.h"
#include "MySTL_construct.h"
#include <cstddef>
#include <utility>

namespace stupid
{
	static const int __stl_num_primes = 28;
	static const unsigned long __stl_prime_list[__stl_num_primes] =
	{
		53, 97, 193, 389, 769,
		1543, 3079, 6151, 12289, 24593,
		49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457,
		1610612741, 3221225473UL, 429496729UL
	};

	inline unsigned long __stl_next_prime(unsigned long n)
	{
		const unsigned long *first = __stl_prime_list;
		const unsigned long *last = __stl_prime_list + __stl_num_primes;
		const unsigned long *pos = stupid::lower_bound(first, last, n);
		if (pos != last)
			return *pos;
		else
			return *(last - 1);
	}

	template <typename Value>
	struct __hashtable_node
	{
		__hashtable_node *next;
		Value value_field;
	};

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator = stupid::allocator>
	class hashtable;

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Alloctor>
	struct __hashtable_iterator;

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	struct __hashtable_const_iterator;

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	struct __hashtable_iterator
	{
		typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> hashtable;
		typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef __hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Value &reference;
		typedef Value *pointer;

		node *cur;
		hashtable *ht;
		
		__hashtable_iterator()
			:cur(0),
			ht(0)
		{
		}

		__hashtable_iterator(node *_cur, hashtable *_ht)
			:cur(_cur),
			ht(_ht)
		{
		}

		reference operator*() const
		{
			return cur->value_field;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		iterator &operator++();
		iterator operator++(int);
		bool operator==(const iterator &x) const
		{
			return cur == x.cur;
		}

		bool operator!=(const iterator &x) const
		{
			return cur != x.cur;
		}
	};

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::iterator
		&__hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::operator++()
	{
		const node *old = cur;
		cur = cur->next;
		if (0 == cur)
		{
			size_type n = ht->bkt_num(old->value_field);
			while (0 == cur && ((++n) < ht->buckets.size()))
				cur = ht->buckets[n];
		}

		return *this;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::iterator
		__hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::operator++(int)
	{
		iterator tmp = *this;
		++(*this);
		return tmp;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	struct __hashtable_const_iterator
	{
		typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> hashtable;
		typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef __hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef const Value &reference;
		typedef const Value *pointer;

		const node *cur;
		const hashtable *ht;

		__hashtable_const_iterator()
			:cur(0),
			ht(0)
		{
		}

		__hashtable_const_iterator(const node *_cur,const hashtable *_ht)
			:cur(_cur),
			ht(_ht)
		{
		}

		reference operator*() const
		{
			return cur->value_field;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		const_iterator &operator++();
		const_iterator operator++(int);
		bool operator==(const const_iterator &x) const
		{
			return cur == x.cur;
		}

		bool operator!=(const const_iterator &x) const
		{
			return cur != x.cur;
		}
	};

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename __hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::const_iterator
		&__hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::operator++()
	{
		const node *old = cur;
		cur = cur->next;
		if (0 == cur)
		{
			size_type n = ht->bkt_num(old->value_field);
			while (0 == cur && ((++n) < ht->buckets.size()))
				cur = ht->buckets[n];
		}

		return *this;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename __hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::const_iterator
		__hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::operator++(int)
	{
		const_iterator tmp = *this;
		++(*this);
		return tmp;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	class hashtable
	{
		friend struct __hashtable_iterator < Value, Key, HashFunc, ExtractKey, EqualKey, Allocator > ;
		friend struct __hashtable_const_iterator < Value, Key, HashFunc, ExtractKey, EqualKey, Allocator > ;
	public:
		typedef Value value_type;
		typedef Key key_type;
		typedef HashFunc hasher;
		typedef EqualKey key_equal;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type &reference;
		typedef const value_type &const_reference;

		typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> const_iterator;
	private:
		hasher hash;
		key_equal equals;
		ExtractKey get_key;

		typedef __hashtable_node<Value> node;
		typedef simple_allocator<node, Allocator> node_allocator;

		vector<node *, Allocator> buckets;
		size_type num_elements;

		node *new_node(const value_type &value)
		{
			node *result = node_allocator::allocate();
			try
			{
				stupid::construct(&(result->value_field), value);
			}
			catch (...)
			{
				node_allocator::deallocate(result);
				throw;
			}

			return result;
		}

		void delete_node(node *p)
		{
			stupid::destroy(&(p->value_field));
			node_allocator::deallocate(result);
		}

		void initialize_buckets(size_type n)
		{
			const size_type n_buckets = __stl_next_prime(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(), n_buckets, (node *)0);
			num_elements = 0;
		}
	public:
		hashtable(size_type n, const hasher &_hasher, const key_equal &_equals)
			:hash(_hasher),
			equals(_equals)
		{
			initialize_buckets(n);
		}

		iterator begin()
		{
			for (size_type i = 0; i < buckets.size(); ++i)
				if (0 != buckets[i])
					return iterator(buckets[i], this);
			return end();
		}

		const_iterator begin() const
		{
			for (size_type i = 0; i < buckets.size(); ++i)
				if (0 != buckets[i])
					return const_iterator(buckets[i], this);
			return end();
		}

		iterator end()
		{
			return iterator(0, this);
		}

		const_iterator end() const
		{
			return const_iterator(0, this);
		}

		size_type size() const
		{
			return num_elements;
		}

		bool empty() const
		{
			return num_elements == 0;
		}

		void swap(hashtable &x)
		{
			buckets.swap(x.buckets);
			stupid::swap(hash, x.hash);
			stupid::swap(equals, x.equals);
			stupid::swap(get_key, x.get_key);
			stupid::swap(num_elements, x.num_elements);
		}

		hasher hash_func() const
		{
			return hash;
		}

		key_equal key_eq() const
		{
			return equals;
		}

		size_type buckets_count() const
		{
			return buckets.size();
		}

		size_type max_bucket_count() const
		{
			return __stl_prime_list[__stl_num_primes - 1];
		}

		size_type max_size() const
		{
			return size_type(-1);
		}

		void resize(size_type num_elements_hint);

		std::pair<iterator, bool> insert_unique(const value_type &value)
		{
			resize(num_elements + 1);
			return insert_unique_noresize(value);
		}

		template <typename InputIterator>
		void insert_unique(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				insert_unique(*first);
				++first;
			}
		}

		iterator insert_equal(const value_type &value)
		{
			resize(num_elements + 1);
			return insert_equal_noresize(value);
		}

		template <typename InputIterator>
		void insert_equal(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				insert_equal(*first);
				++first;
			}
		}

		std::pair<iterator, bool> insert_unique_noresize(const value_type &value);
		iterator insert_equal_noresize(const value_type &value);

		void erase(iterator first, iterator last)
		{
			if (first == begin() && last == end())
				clear();
			else
			{
				while (first != last)
					erase(first++);
			}
		}

		void erase(iterator pos);
		size_type erase(const key_type &key);

		size_type bkt_num(const value_type &value, size_t n) const
		{
			return bkt_num_key(get_key(value), n);
		}

		size_type bkt_num(const value_type &value) const
		{
			return bkt_num_key(get_key(value));
		}

		size_type bkt_num_key(const key_type &key) const
		{
			return bkt_num_key(key, buckets.size());
		}

		size_type bkt_num_key(const key_type &key, size_t n) const
		{
			return hash(key) % n;
		}

		iterator find(const key_type &key);
		const_iterator find(const key_type &key) const;
		size_type count(const key_type &key) const;
		std::pair<iterator, iterator> equal_range(const key_type &key);
		std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

		void clear();
		void copy_from(const hashtable &x);
	};

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::resize(size_type num_elements_hint)
	{
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = __stl_next_prime(num_elements_hint);
			vector<node *, Allocator> tmp(n, (node *)0);

			for (size_type i = 0; i < old_n; ++i)
			{
				node *first = buckets[i];
				while (first)
				{
					const size_type new_pos = bkt_num(first->value_field, num_elements_hint);
					buckets[i] = first->next;
					first->next = tmp[new_pos];
					tmp[new_pos] = first;
					first = buckets[i];
				}
			}

			buckets.swap(tmp);
		}
	}
	
	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::iterator, bool>
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::insert_unique_noresize(const value_type &value)
	{
		const size_type n = bkt_num(value);
		node *first = buckets[n];

		while (first)
		{
			if (equals(get_key(value), get_key(first->value_field)))
				return std::pair<iterator, bool>(iterator(first, this), false);
			first = first->next;
		}

		node *result = new_node(value);
		result->next = buckets[n];
		buckets[n] = result;
		++num_elements;
		return std::pair<iterator, bool>(iterator(result, this), true);
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey ,Allocator>::iterator
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::insert_equal_noresize(const value_type &value)
	{
		const size_type n = bkt_num(value);
		node *result = new_node(value);
		result->next = buckets[n];
		buckets[n] = node;
		++num_elements;
		return iterator(result, this);
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::clear()
	{
		for (size_type i = 0; i < buckets.size(); ++i)
		{
			node *cur = buckets[i];
			node *next;
			while (0 != cur)
			{
				next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = (node *)0;
		}
		num_elements = 0;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::copy_from(const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> &x)
	{
		clear();
		buckets.clear();
		size_type n = x.buckets.size();
		buckets.reserve(n);
		buckets.insert(buckets.end(), n, (node *)0);

		for (size_type i = 0; i < n; ++i)
		{
			node *cur = x.buckets[i];
			node *copy;
			while (0 != cur)
			{
				copy = new_node(cur->value_field);
				copy->next = buckets[i];
				buckets[i] = copy;
				cur = cur->next;
			}
		}

		num_elements = x.num_elements;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::iterator
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::find(const key_type &key)
	{
		const size_type bucket_num = bkt_num_key(key);
		node *cur = buckets[bucket_num];
		
		while (0 != cur)
		{
			if (true == equals(get_key(cur->value_field), key))
				return iterator(cur, this);
			cur = cur->next;
		}

		return end();
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::const_iterator
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::find(const key_type &key) const
	{
		const size_type bucket_num = bkt_num_key(key);
		node *cur = buckets[bucket_num];

		while (0 != cur)
		{
			if (true == equals(get_key(cur->value_field), key))
				return const_iterator(cur, this);
			cur = cur->next;
		}

		return end();
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::size_type
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::count(const key_type &key) const
	{
		size_type result = 0;
		const size_type bucket_num = bkt_num_key(key);
		node *cur = buckets[bucket_num];

		while (0 != cur)
		{
			if (equals(get_key(cur->value_field), key))
				++result;
			cur = cur->next;
		}

		return result;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::iterator, typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::iterator>
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::equal_range(const key_type &key)
	{
		typedef std::pair<iterator, iterator> pair_ii;
		size_type bucket_num = bkt_num_key(key);
		node *first=buckets[bucket_num];

		while (0 != first)
		{
			if (true == equals(get_key(first->value_field), key))
			{
				for (node *cur = first->next; 0 != cur; cur = cur->next)
				{
					if (false == equals(get_keys(cur->value_field), key))
						return pair_ii(iterator(first, this), iterator(cur, this));
				}
				for (size_type next_bucket = bucket_num + 1; n < buckets.size(); ++next_bucket)
				{
					if (0 != buckets[next_bucket])
						return pair_ii(iterator(first, this), iterator(buckets[next_bucket], this));
				}

				return pair_ii(iterator(first, this), end());
			}
			first = first->next;
		}

		return pair_ii(end(), end());
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::const_iterator, typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::const_iterator>
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::equal_range(const key_type &key) const
	{
		typedef std::pair<const_iterator, const_iterator> pair_ii;
		size_type bucket_num = bkt_num_key(key);
		node *first = buckets[bucket_num];

		while (0 != first)
		{
			if (true == equals(get_key(first->value_field), key))
			{
				for (node *cur = first->next; 0 != cur; cur = cur->next)
				{
					if (false == equals(get_keys(cur->value_field), key))
						return pair_ii(const_iterator(first, this), const_iterator(cur, this));
				}
				for (size_type next_bucket = bucket_num + 1; n < buckets.size(); ++next_bucket)
				{
					if (0 != buckets[next_bucket])
						return pair_ii(const_iterator(first, this), const_iterator(buckets[next_bucket], this));
				}

				return pair_ii(const_iterator(first, this), end());
			}
			first = first->next;
		}

		return pair_ii(end(), end());
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::size_type
		hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::erase(const key_type &key)
	{
		const size_type bucket_num = bkt_num_key(key);
		node *first = buckets[bucket_num];
		size_type result = 0;

		if (0 != first)
		{
			node *cur = first;
			node *next = cur->next;
			while (next)
			{
				if (equals(get_key(next->value_field), key))
				{
					cur->next = cur->next;
					delete_node(next);
					++result;
					--num_elements;
				}
				else
				{
					cur = next;
					next = next->next;
				}
			}

			if (equals(get_key(first->value_field), key))
			{
				buckets[bucket_num] = first->next;
				delete_node(first);
				++result;
				--num_elements;
			}
		}

		return result;
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator>::erase(iterator position)
	{
		if (0 != position.cur)
		{
			const size_type bucket_num = bkt_num((position.cur)->value_field);
			node *cur = buckets[bucket_num];
			node *p = position.cur;

			if (cur == position.cur)
			{
				buckets[bucket_num] = p->next;
				delete_node(p);
				--num_elements;
			}
			else
			{
				node* next = cur->next;
				while (next)
				{
					if (next == p)
					{
						cur->next = next->next;
						delete_node(p);
						--num_elements;
					}
					else
					{
						cur = next;
						next = next->next;
					}
				}
			}
		}
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	bool operator==(const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> &x,
		const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> &y)
	{
		return (x.size() == y.size() && stupid::equal(x.begin(), x.end(), y.begin()));
	}

	template <typename Value, typename Key, typename HashFunc, typename ExtractKey, typename EqualKey, typename Allocator>
	bool operator< (const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> &x,
		const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Allocator> &y)
	{
		return stupid::lexicalgraphical_compare(x.begin(), x.end(), y.begin(), y.end());
	}
	
}

#endif