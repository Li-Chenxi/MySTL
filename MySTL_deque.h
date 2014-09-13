/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef DEQUE_H
#define DEQUE_H

#include "MySTL_iterator.h"
#include "MySTL_allocate.h"
#include "MySTL_algobase.h"
#include "MySTL_construct.h"
#include "MySTL_uninitialized.h"
#include <cstddef>

namespace stupid
{
	inline size_t  __deque_buffer_size(size_t buffer_size, size_t type_size)
	{
		return buffer_size != 0 ? buffer_size : (type_size < 512 ? size_t(512 / type_size) : 1);
	}

	template <typename Type, typename Ref, typename Ptr, size_t Buffer_size>
	struct __deque_iterator
	{
		typedef __deque_iterator self;

		typedef random_access_iterator_tag iterator_category;
		typedef Type value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef value_type **map_pointer;

		value_type *first;
		value_type *last;
		value_type *cur;
		map_pointer node;

		__deque_iterator()
			:first(0),
			last(0),
			cur(0),
			node(0)
		{

		}

		__deque_iterator(value_type *cur_, map_pointer node_)
			:cur(cur_),
			node(node_),
			first(*node_),
			last(*node_ + buffer_size())
		{
		}

		__deque_iterator(const self &x)
			:first(x.first),
			last(x.last),
			cur(x.cur),
			node(x.node)
		{
		}

		self operator=(const self &x)
		{
			first = x.first;
			last = x.last;
			cur = x.cur;
			node = x.node;
			return *this;
		}

		static size_type buffer_size()
		{
			return stupid::__deque_buffer_size(Buffer_size, sizeof(value_type));
		}

		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		reference operator*()
		{
			return *cur;
		}

		pointer operator->()
		{
			return &(operator*());
		}

		difference_type operator-(const self &x) const
		{
			return (difference_type)buffer_size()*(node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}

		self &operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}

		self &operator--()
		{
			if (cur == first)
			{
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		self &operator+=(const difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur += n;
			else
			{
				difference_type node_offset = offset>0 ? offset / difference_type(buffer_size()) :
					difference_type((offset + 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + offset - difference_type(buffer_size())*node_offset;
			}
			return *this;
		}

		self operator+(const difference_type n) const
		{
			self tmp = *this;
			return tmp += n;
		}

		self &operator-=(const difference_type n)
		{
			return (*this) += -n;
		}

		self operator-(const difference_type n) const
		{
			self tmp = *this;
			return tmp -= n;
		}

		reference operator[](difference_type n) const
		{
			return *(*this + n);
		}

		bool operator==(const self &x) const
		{
			return cur == x.cur;
		}

		bool operator!=(const self &x) const
		{
			return cur != x.cur;
		}

		bool operator<(const self &x) const
		{
			return node == x.node ? cur < x.cur : node < x.node;
		}

		bool operator>(const self &x) const
		{
			return x < *this;
		}

		bool operator<=(const self &x) const
		{
			return !(*this > x);
		}

		bool operator>=(const self &x) const
		{
			return !(*this < x);
		}
	};

	template <typename Type, typename Allocator = allocator, size_t Buffer_size = 0>
	class deque
	{
	public:
		typedef Type value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef __deque_iterator<Type, Type &, Type *, Buffer_size> iterator;
		typedef __deque_iterator<Type, const Type &, const Type *, Buffer_size> const_iterator;
	protected:
		typedef value_type **map_pointer;

		typedef simple_allocator<Type, Allocator> data_allocator;
		typedef simple_allocator<Type *, Allocator> map_allocator;

		iterator start;
		iterator finish;
		map_pointer map;
		size_type map_size;

		static size_type buffer_size()
		{
			return stupid::__deque_buffer_size(Buffer_size, sizeof(Type));
		}

		Type *allocate_node()
		{
			return data_allocator::allocate(buffer_size());
		}
		void deallocate_node(Type *p)
		{
			data_allocator::deallocate(p, buffer_size());
		}

		void reserve_map_at_back(size_type nodes_to_add = 1)
		{
			if (nodes_to_add > map_size - (finish.node - map + 1))
				reallocate_map(nodes_to_add, false);
		}

		void reserve_map_at_front(size_type nodes_to_add = 1)
		{
			if (nodes_to_add > size_type(start.node - map))
				reallocate_map(nodes_to_add, true);
		}

		void create_map_and_nodes(size_type num_elements);
		void fill_initialize(size_type num_elements, const value_type &value);
		void push_back_aux(const value_type &value);
		void push_front_aux(const value_type &value);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void pop_back_aux();
		void pop_front_aux();
		iterator insert_aux(iterator position, const value_type &value);
	public:
		deque()
			:start(),
			finish(),
			map(0),
			map_size(0)
		{
			create_map_and_nodes(0);
		}

		deque(int n, const value_type &value)
			:start(),
			finish(),
			map(0),
			map_size(0)
		{
			fill_initialize(n, value);
		}

		iterator begin()
		{
			return start;
		}

		const_iterator begin() const
		{
			return const_iterator(start.cur, start.node);
		}

		iterator end()
		{
			return finish;
		}

		const_iterator end() const
		{
			return const_iterator(finish.cur, finish.node);
		}

		reference front()
		{
			return *start;
		}

		const_reference front() const
		{
			return *start;
		}

		reference back()
		{
			return *(finish - 1);
		}

		const_reference back() const 
		{
			return *(finish - 1);
		}

		size_type size() const
		{
			return finish - start;
		}

		size_type max_size() const
		{
			return size_type(-1);
		}

		reference operator[](size_type n)
		{
			return *(start + n);
		}

		const_reference operator[](size_type n) const
		{
			return *(start + n);
		}

		bool empty() const 
		{
			return start == finish;
		}

		void push_back(const value_type &value)
		{
			if (finish.cur != finish.last)
			{
				construct(finish.cur, value);
				++finish.cur;
			}
			else
				push_back_aux(value);
		}

		void push_back()
		{
			push_back(Type());
		}

		void push_front(const value_type &value)
		{
			if (start.cur != start.first)
			{
				--start.cur;
				construct(start.cur, value);
			}
			else
				push_front_aux(value);
		}

		void push_front()
		{
			push_front(Type());
		}

		void pop_back()
		{
			if (finish.cur != finish.first)
			{
				--finish.cur;
				stupid::destroy(finish.cur);
			}
			else
				pop_back_aux();
		}

		void pop_front()
		{
			if (start.cur != start.last - 1)
			{
				stupid::destroy(start.cur);
				++start.cur;
			}
			else
				pop_front_aux();
		}

		iterator insert(iterator position, const value_type &value)
		{
			if (position.cur == start.cur)
			{
				push_front(value);
				return start;
			}
			else
			{
				if (position.cur == finish.cur)
				{
					push_back(value);
					iterator tmp = finish;
					--tmp;
					return tmp;
				}
				else
					return insert_aux(position, value);
			}
		}

		void clear();
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);

		~deque()
		{
			clear();
			map_allocator::deallocate(map, map_size);
		}
	};

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::create_map_and_nodes(size_type num_elements)
	{
		size_type num_nodes = num_elements / buffer_size() + 1;

		map_size = stupid::max((size_type)8, num_nodes + 2);
		map = map_allocator::allocate(map_size);

		map_pointer node_start = map + (map_size - num_nodes) / 2;
		map_pointer node_finish = node_start + num_nodes - 1;

		map_pointer cur;
		try
		{
			for (cur = node_start; cur <= node_finish; ++cur)
				*cur = allocate_node();
		}
		catch (...)
		{
			for (; cur >= node_start; --cur)
				deallocate_node(*cur);
			throw;
		}

		start.set_node(node_start);
		finish.set_node(node_finish);
		start.cur = start.first;
		finish.cur = finish.first + difference_type(num_elements%buffer_size());
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::fill_initialize(size_type num_elements, const value_type &value)
	{
		create_map_and_nodes(num_elements);

		map_pointer cur;
		try
		{
			for (cur = start.node; cur < finish.node; ++cur)
				stupid::uninitialized_fill(*cur, *cur + buffer_size(), value);
			stupid::uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...)
		{
			stupid::destroy(start, iterator(*cur, cur));
			throw;
		}
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::push_back_aux(const value_type &value)
	{
		value_type value_copy = value;
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();

		try
		{
			finish.set_node(finish.node + 1);
			stupid::construct(finish.first, value);
			finish.cur = finish.first + 1;
		}
		catch (...)
		{
			deallocate_node(*(finish.node + 1));
			throw;
		}
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::push_front_aux(const value_type &value)
	{
		value_type value_copy = value;
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();

		try
		{
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			stupid::construct(start.cur, value);
		}
		catch (...)
		{
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}

	template<typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_start_node;
		if (map_size > 2 * new_num_nodes)
		{
			new_start_node = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_start_node < start.node)
				stupid::copy(start.node, finish.node + 1, new_start_node);
			else
				stupid::copy_backward(start.node, finish.node + 1, new_start_node + old_num_nodes);
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, new_num_nodes);
			map_pointer new_map = map_allocator::allocate(new_map_size);

			new_start_node = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			stupid::copy(start.node, finish.node + 1, new_start_node);
			map_allocator::deallocate(map);

			map_size = new_map_size;
			map = new_map;
		}

		start.set_node(new_start_node);
		finish.set_node(new_start_node + old_num_nodes - 1);
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		stupid::destroy(finish.cur);
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::pop_front_aux()
	{
		stupid::destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;

	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	void deque<Type, Allocator, Buffer_size>::clear()
	{
		for (map_pointer node = start.node + 1; node < finish.node; node++)
		{
			stupid::destroy(*node, *node + (difference_type)buffer_size());
			deallocate_node(*node);
		}

		if (start.node != finish.node)
		{
			stupid::destroy(start.cur, start.last);
			stupid::destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first);
		}
		else
			destroy(start.cur, finish.cur);

		finish = start;
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	typename deque<Type, Allocator, Buffer_size>::iterator deque<Type, Allocator, Buffer_size>::erase(iterator pos)
	{
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if ((size_type)index < (size() >> 1))
		{
			stupid::copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			stupid::copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	typename deque<Type, Allocator, Buffer_size>::iterator deque<Type, Allocator, Buffer_size>::erase(iterator first, iterator last)
	{
		if (first == start&&last == first)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;
			difference_type elements_before = first - start;
			if (elements_before < ((size() - n) >> 1))
			{
				stupid::copy_backward(start, first, last);
				iterator new_start = start + n;
				stupid::destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					deallocate_node(*cur);
				start = new_start;
			}
			else
			{
				stupid::copy(last, finish, first);
				iterator new_finish = finish - n;
				stupid::destroy(new_finish, finish);
				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
					deallocate_node(*cur);
				finish = new_finish;
			}
			return start + elements_before;
		}
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	typename deque<Type, Allocator, Buffer_size>::iterator deque<Type, Allocator, Buffer_size>::insert_aux(iterator position, const value_type &value)
	{
		difference_type index = position - start;
		value_type value_copy = value;
		if (index < (size() >> 1))
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			position = start + index;
			iterator pos1 = position;
			++pos1;
			stupid::copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + index;
			stupid::copy_backward(position, back2, back1);
		}
		*position = value_copy;
		return pos;
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	inline bool operator==(const deque<Type, Allocator, Buffer_size> &x, const deque<Type, Allocator, Buffer_size> &y)
	{
		return ((x.size() == y.size()) && equal(x.begin(), x.end(), y.begin()));
	}

	template <typename Type, typename Allocator, size_t Buffer_size>
	inline bool operator<(const deque<Type, Allocator, Buffer_size> &x, const deque<Type, Allocator, Buffer_size> &y)
	{
		return lexicographical(x.begin(), x.end(), y.begin(), y.end());
	}
}

#endif