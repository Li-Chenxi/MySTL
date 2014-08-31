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

inline size_t  __deque_buffer_size(size_t n, size_t sz)
{
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : 1);
}

template <typename Type,typename Ref,typename Ptr,size_t Buffer_size>
struct __deque_iterator
{
	typedef __deque_iterator<Type, Type &, Type *, Buffer_size> iterator;
	typedef __deque_iterator<Type, const Type &, const Type *, Buffer_size> const_iterator;
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

	static size_type buffer_size() const
	{
		return __deque_buffer_size(Buffer_size, sizeof(value_type));
	}

	void set_node(map_pointer new_node)
	{
		node = new_node;
		fisrt = *new_node;
		last = first + difference_type(buffer_size());
	}

	__deque_iterator()
		:first(0),
		last(0),
		cur(0),
		node(0)
	{
	}

	reference operator*()
	{
		return *cur;
	}

	pointer operator->()
	{
		return &(operator*());
	}

	difference_type operator-(self &x)
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

	self &operator+=(difference_type n)
	{
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else
		{
			difference_type node_offset = offset>0 ? offset / difference_type(buffer_size()) :
				-difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + offset - difference_type(buffer_size())*node_offset;
		}
		return *this;
	}

	self operator+(difference_type n)
	{
		self tmp = *this;
		return tmp += n;
	}

	self &operator-=(difference_type n)
	{
		return (*this) += -n;
	}

	self operator-(difference_type n)
	{
		self tmp = *this;
		return tmp -= n;
	}

	reference operator[](difference_type n)
	{
		return *(*this + n);
	}

	bool operator==(const self &x)
	{
		return cur == x.cur;
	}

	bool operator!=(const self &x)
	{
		return cur != x.cur;
	}

	bool operator<(const self &x)
	{
		return node == x.node ? cur < x.cur : node < x.node;
	}
};

template <typename Type,typename Allocator=allocator,size_t Buffer_size=0>
class deque
{
public:
	typedef Type value_type;
	typedef value_type *pointer;
	typedef value_type &reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __deque_iterator<Type, Type &, Type *, Buffer_size> iterator;
	typedef typename iterator::const_iterator const_iterator;
protected:
	typedef value_type **map_pointer;

	typedef simple_allocator<Type, Allocator> data_allocator;
	typedef simple_allocator<map_pointer, Allocator> map_allocator;

	iterator start;
	iterator finish;
	map_pointer map;
	size_type map_size;

	static size_type buffer_size() const
	{
		return iterator::buffer_size();
	}

	void *allocate_node();
	void deallocate_node(map_pointer *p);

	void create_map_and_nodes(size_type num_elements);
	void fill_initialize(size_type n, value_type &value);

	void push_back_aux(const value_type &value);
	void push_front_aux(const value_type &value);

	void reserve_map_at_back(size_type nodes_to_add = 1)
	{
		if (nodes_to_add > map_size - (finish.node - start.node))
			reallocate_map(nodes_to_add, false);
	}

	void reserve_map_at_front(size_type nodes_to_add = 1)
	{
		if (node_to_add > start.node - map)
			reallocate_map(nodes_to_add, true);
	}

	void reallocate_map(size_type nodes_to_add, bool add_at_front);
public:
	deque(int n, const value_type &value)
		:start(),
		finish(),
		map(0),
		map_size(0)
	{
	}

	iterator begin()
	{
		return start;
	}

	iterator end()
	{
		return finish;
	}

	reference front()
	{
		return *start;
	}

	reference back()
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

	bool empty()
	{
		return start == finish;
	}

	void push_back(const value_type &value)
	{
		if (finish.cur != finish.last - 1)
		{
			contruct(finish.cur, value);
			++finish.cur;
		}
		else
			push_back_aux(value);
	}

	void push_front(const value_type &value)
	{
		if (start.cur != start.start)
		{
			construct(start.cur, value);
			--start.cur;
		}
		else
			push_front_aux(value);
	}
};



#endif