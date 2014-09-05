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

template <typename Type>
struct __list_node
{
	typedef __list_node<Type> *node_pointer;
	node_pointer prev;
	node_pointer next;
	Type data;
};

template <typename Type,typename Ref,typename Ptr>
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
		return self;
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

template <typename Type,typename Allocator=allocator>
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
	typedef simple_allocator<node, Allocator> data_allocator;
	iterator finish;

	node_pointer get_node()
	{
		return data_allocator::allocate();
	}

	void put_node(node_pointer *p)
	{
		data_allocator::deallocate(p);
	}

	node_pointer create_node(const Type &value)
	{
		node_pointer p = get_node();
		construct(&(p->data), value);
		return p;
	}

	void destroy_node(node_pointer *p)
	{
		destroy(&(p->data));
		put_node(p);
	}

	void empty_initialize()
	{
		finish = get_node();
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
		return (finish.p)->prev;
	}

	const_iterator begin() const
	{
		return (finish.p)->prev;
	}

	iterator end()
	{
		return finish;
	}

	const_iterator end() const
	{
		return finish;
	}

	bool empty() const
	{
		return finish.p==(finish.p->next);
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
		
	}
};

#endif