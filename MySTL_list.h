/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef LIST_H
#define LIST_H

#include "MySTL_allocate.h"
#include "MySTL_allocate.h"
#include "MySTL_construct.h"
#include "MySTL_iterator.h"
#include <cstddef>	//for ptrdiff_t

template <typename Type>
struct __list_node
{
	typedef __list_node<Type> *node_pointer;
	node_pointer prev;
	node_pointer next;
	Type data;
};

template <typename Type>
struct __list_iterator
{
	typedef __list_node<Type> *node_pointer;
	typedef __list_iterator<Type> self;
	typedef bidirectional_iterator_tag iterator_category;
	typedef Type *pointer;
	typedef Type &reference;
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

	const self &operator=(const self &x)
	{
		p = x.p;
		return *this;
	}

	bool operator==(self &x) const
	{
		return p == x.p;
	}

	bool operator!=(self &x) const
	{
		return p != x.p;
	}

	reference operator*() const
	{
		return p->data;
	}

	pointer operator->() const
	{
		return &(p->data);
	}

	self &operator++()
	{
		p = (node_pointer)p->next;
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		p = (node_pointer)p->next;
		return self;
	}

	self &operator--()
	{
		p = (node_pointer)p->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		p = (node_pointer)p->prev;
		return tmp;
	}
};

template <typename Type,typename Allocator=allocator>
class list
{
public:
	typedef __list_node<Type>	  list_node;
	typedef __list_iterator<Type> iterator;
	typedef Type				  value_type;
	typedef value_type *		  pointer;
	typedef value_type &		  reference;
	typedef size_t				  size_type;
	typedef ptrdiff_t			  difference_type;
protected:
	typedef simple_allocator<list_node, Allocator> data_allocator;
	iterator finish;

	list_node *get_node()
	{
		return data_allocator::allocate();
	}

	void put_node(list_node *p)
	{
		data_allocator::deallocate(p);
	}

	list_node *create_node(const Type &value)
	{
		list_node *p = get_node;
		construct(&(p->data), value);
		return p;
	}

	void destroy_node(list_node *p)
	{
		destroy(&(p->data));
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

	iterator begin() const
	{
		iterator tmp=end();
		return (--tmp);
	}

	iterator end() const
	{
		return finish;
	}

	bool empty() const
	{
		return begin() == end();
	}

	size_type size() const
	{
		return (size_t)distance(begin(), end());
	}

	reference front()
	{
		return *(begin());
	}

	reference back()
	{
		iterator tmp=end();
		return *(--tmp);
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
	iterator find(iterator first, iterator last, const Type &value);
	void splice(iterator position, list &x);
	void splice(iterator position, iterator i);
	void splice(iterator position, iterator first, iterator last);
	void merge(list &x);
	void reverse();
	void sort();
};

#endif