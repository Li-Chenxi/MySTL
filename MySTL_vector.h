/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef VECTOR_H
#define VECTOR_H

#include "MySTL_allocate.h"
#include "MySTL_uninitialized.h"
#include "MySTL_algobase.h"
#include "MySTL_construct.h"
#include <cstddef>

template <typename Type,typename Allocator=allocator>
class vector
{
public:
	typedef Type value_type;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef value_type *iterator;
	typedef const value_type *const_iterator;
protected:
	typedef simple_allocator<value_type, Allocator> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storage;

	void deallocate()
	{
		if (start)
			data_allocator::deallocate(start, end_of_storage - start);
	}

	iterator allocate_and_fill(size_type n, const Type &value)
	{
		iterator result = data_allocator(n);
		uninitialized_fill_n(result, n, value);
		return result;
	}

	void fill_initialize(size_type num_elements, const Type *value)
	{
		start = allocate_and_fill(num_elements, value);
		finish = start + num_elements;
		end_of_storage = finish;
	}

	void insert_aux(iterator position, const Type &x);
public:
	vector() :start(0), finish(0), end_of_storage(0)
	{
	}

	vector(size_type n, const Type &value)
	{
		fill_initialize(n, value);
	}

	vector(int n, const Type &value)
	{
		fill_initialize(n, value);
	}

	vector(long n, const Type &value)
	{
		fill_initialize(n, value);
	}

	explicit vector(size_type n)
	{
		fill_initialize(n, Type());
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

	size_type size() const
	{
		return (size_type)(finish - start);
	}

	size_type capacity() const
	{
		return (size_type)(end_of_stotage - start);
	}

	bool empty() const
	{
		return start == finish;
	}

	reference operator[] (size_type n)
	{
		return *(start + n);
	}

	const_reference operator[](size_type n) const
	{
		return *(start + n);
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

	void push_back(const Type &value)
	{
		if (finish != end_of_storage)
		{
			construct(finish, value);
			++finish;
		}
		else
			insert_aux(end(), value);
	}

	void pop_back()
	{
		if (!empty())
		{
			--finish;
			destroy(finish);
		}
	}

	iterator erase(iterator position)
	{
		if (position + 1 != finish)
			copy(position + 1, finish, position);
		--finish;
		destroy(finish);
		return position;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator i = copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}

	void resize(size_type new_sz, const Type &value)
	{
		if (new_sz < size())
			erase(start + new_sz, finish);
		else
			insert(finish, new_sz - size(), value);
	}

	void resize(size_type new_sz)
	{
		resize(new_sz, Type());
	}

	void clear()
	{
		erase(start, finish);
	}

	void insert(iterator position, size_type n, const Type &value);

	~vector()
	{
		destroy(start, finish);
		deallocate();
	}
};

#endif