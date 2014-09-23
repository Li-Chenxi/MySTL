/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef LIST_H
#define LIST_H

#include "MySTL_allocate.h"
#include "MySTL_algobase.h"
#include "MySTL_construct.h"
#include "MySTL_iterator.h"
#include <cstddef>

namespace stupid
{
	template <typename Type>
	struct __list_node
	{
		typedef __list_node<Type> *node_pointer;
		node_pointer prev;
		node_pointer next;
		Type data;
	};

	template <typename Type, typename Ref, typename Ptr>
	struct __list_iterator
	{
		typedef __list_node<Type> *node_pointer;
		typedef __list_iterator self;

		typedef bidirectional_iterator_tag iterator_category;
		typedef Type value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		node_pointer p;

		__list_iterator() :p(0)
		{
		}

		__list_iterator(node_pointer x) :p(x)
		{
		}

		__list_iterator(const self &x) :p(x.p)
		{
		}

		self &operator=(const self &x)
		{
			p = x.p;
			return *this;
		}

		bool operator==(const self &x) const
		{
			return p == x.p;
		}

		bool operator!=(self &x) const
		{
			return p != x.p;
		}

		reference operator*() const
		{
			return ((node_pointer)p)->data;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self &operator++()
		{
			p = ((node_pointer)p)->next;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			p = ((node_pointer)p)->next;
			return tmp;
		}

		self &operator--()
		{
			p = ((node_pointer)p)->prev;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			p = ((node_pointer)p)->prev;
			return tmp;
		}
	};

	template <typename Type, typename Allocator = stupid::allocator>
	class list
	{
	public:
		typedef __list_node<Type> *node_pointer;

		typedef __list_iterator<Type, Type &, Type *> iterator;
		typedef __list_iterator<Type, const Type &, const Type *> const_iterator;

		typedef Type value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef size_t	size_type;
		typedef ptrdiff_t difference_type;
	protected:
		typedef stupid::simple_allocator<__list_node<Type>, Allocator> data_allocator;
		iterator finish;

		node_pointer get_node()
		{
			return data_allocator::allocate();
		}

		void put_node(node_pointer p)
		{
			data_allocator::deallocate(p);
		}

		node_pointer create_node(const Type &value)
		{
			node_pointer p = get_node();
			stupid::construct(&(p->data), value);
			return p;
		}

		void destroy_node(node_pointer p)
		{
			stupid::destroy(&(p->data));
			put_node(p);
		}

		void empty_initialize()
		{
			finish.p = get_node();
			(finish.p)->prev = finish.p;
			(finish.p)->next = finish.p;
		}

		void transfer(iterator position, iterator first, iterator last);
	public:
		list()
		{
			empty_initialize();
		}

		iterator begin()
		{
			return (finish.p)->next;
		}

		const_iterator begin() const
		{
			return (finish.p)->next;
		}

		iterator end()
		{
			return finish.p;
		}

		const_iterator end() const
		{
			return finish.p;
		}

		bool empty() const
		{
			return finish.p == (finish.p->next);
		}

		size_type size() const
		{
			return (size_type)distance(begin(), end());
		}

		size_type max_size() const
		{
			return size_type(-1);
		}

		reference front()
		{
			return *((finish.p)->next);
		}

		const_reference front() const
		{
			return *((finish.p)->next);
		}

		reference back()
		{
			return *((finish.p)->prev);
		}

		const_reference back() const
		{
			return *((finish.p)->prev);
		}

		void push_front(const Type &value)
		{
			insert(begin(), value);
		}

		void push_back(const Type &value)
		{
			insert(end(), value);
		}

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			iterator tmp = end();
			erase(--tmp);
		}

		iterator insert(iterator position, const Type &value);
		iterator erase(iterator position);
		void clear();
		void remove(const Type &value);
		void unique();
		void splice(iterator position, list &x);
		void splice(iterator position, iterator i);
		void splice(iterator position, iterator first, iterator last);
		void merge(list &x);
		void reverse();
		void sort();

		~list()
		{
			clear();
			destroy_node(finish.p);
		}
	};

	template <typename Type, typename Allocator>
	typename list<Type, Allocator>::iterator list<Type, Allocator>::insert(iterator position, const Type &value)
	{
		node_pointer cur = position.p;
		node_pointer prev = cur->prev;
		node_pointer new_node = create_node(value);
		new_node->prev = prev;
		new_node->next = cur;
		prev->next = new_node;
		cur->prev = new_node;
		return new_node;
	}

	template <typename Type, typename Allocator>
	typename list<Type, Allocator>::iterator list<Type, Allocator>::erase(iterator position)
	{
		node_pointer cur = position.p;
		node_pointer prev = cur->prev;
		node_pointer next = cur->next;
		prev->next = next;
		next->prev = prev;
		destroy_node(cur);
		return next;
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::clear()
	{
		iterator cur = begin();
		iterator last = end();
		while (cur != last)
		{
			iterator tmp = cur;
			++cur;
			erase(tmp);
		}
		(last.p)->next = last.p;
		(last.p)->prev = last.p;
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::remove(const Type &value)
	{
		iterator cur = begin();
		iterator last = end();
		while (cur != last)
		{
			if (*cur == value)
			{
				iterator tmp = cur;
				++cur;
				erase(tmp);
			}
			else
				++cur;
		}
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::unique()
	{
		iterator cur = begin();
		iterator last = end();
		iterator trace = cur;
		while ((++trace) != last)
		{
			if (*cur == *trace)
			{
				erase(trace);
				trace = cur;
			}
			else
				cur = trace;
		}
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::transfer(iterator position, iterator first, iterator last)
	{
		if (position != last)
		{
			node_pointer position_prev = (position.p)->prev;
			node_pointer first_prev = (first.p)->prev;
			node_pointer first_next = (first.p)->next;
			node_pointer last_prev = (last.p)->prev;

			first_prev->next = last.p;
			(last.p)->prev = first_prev;

			position_prev->next = first.p;
			(first.p)->prev = position_prev;

			(position.p)->prev = last_prev;
			last_prev->next = position.p;
		}
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::splice(iterator position, list &x)
	{
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::splice(iterator position, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j)
			return;
		transfer(position, i, j);
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::splice(iterator position, iterator first, iterator last)
	{
		if (first != last)
			transfer(position, first, last);
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::merge(list &x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while (first1 != last1&&first2 != last2)
		{
			iterator next;
			if (*first1 <= *first2)
				++first1;
			else
			{
				next = first2;
				++next;
				transfer(first1, first2, next);
				first2 = next;
			}
		}

		if (first2 != last2)
		{
			transfer(last1, first2, last2);
		}
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::reverse()
	{
		if (size() == 0 || size() == 1)
			return;
		iterator first = begin();
		iterator last = end();
		iterator old_first;
		++first;
		while (first != last)
		{
			old_first = first;
			++first;
			transfer(begin(), old_first, first);
		}
	}

	template <typename Type, typename Allocator>
	void list<Type, Allocator>::sort()
	{
		if (size() == 0 || size() == 1)
			return;
		iterator cur = begin();
		iterator last = end();
		iterator prev;
		++cur;
		while (cur != last)
		{
			prev = cur;
			--prev;
			while (prev != last&&*prev > *cur)
				--prev;
			++prev;
			iterator tmp = cur;
			++cur;
			if (prev!=tmp)
				transfer(prev, tmp, cur);
		}
	}
}

#endif