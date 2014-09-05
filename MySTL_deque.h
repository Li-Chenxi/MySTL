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

inline size_t  __deque_buffer_size(size_t buffer_size, size_t type_size)
{
	return buffer_size != 0 ? buffer_size : (type_size < 512 ? size_t(512 / type_size) : 1);
}

template <typename Type,typename Ref,typename Ptr,size_t Buffer_size>
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

	__deque_iterator(value_type *_cur, map_pointer _node)
		:cur(_cur),
		node(_node)
		first(*_node),
		last(*_node + buffer_size())
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

template <typename Type,typename Allocator=allocator,size_t Buffer_size=0>
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

	static size_type buffer_size() const
	{
		return __deque_buffer_size(Buffer_size, sizeof(Type));
	}

	void *allocate_node()
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
		return start;
	}

	iterator end()
	{
		return finish;
	}

	const_iterator end() const
	{
		return finish;
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

	const_reference back()
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
			destroy(finish.cur);
		}
		else
			pop_back_aux();
	}

	void pop_front()
	{
		if (start.cur != start.last - 1)
		{
			destroy(start.cur);
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
				return insert_aux(position,value);
		}
	}

	void clear();
	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);

	~deque()
	{
		if (!empty())
			destroy(start, finish);
		map_allocator::deallocate(map, map_size);
	}
};

template <typename Type,typename Allocator,size_t Buffer_size>
inline bool operator==(const deque<Type, Allocator, Buffer_size> &x, const deque<Type, Allocator, Buffer_size> &y)
{
	return ((x.size() == y.size()) && equal(x.begin(), x.end(), y.begin()));
}

template <typename Type, typename Allocator, size_t Buffer_size>
inline bool operator<(const deque<Type, Allocator, Buffer_size> &x, const deque<Type, Allocator, Buffer_size> &y)
{
	return lexicographical(x.begin(), x.end(), y.begin(), y.end());
}

#endif