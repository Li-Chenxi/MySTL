#ifndef QUEUE_H
#define QUEUE_H

#include "MySTL_deque.h"

template <typename Type,typename Container=deque<Type>>
class queue
{
	friend bool operator==(const queue &x, const queue &y);
	friend bool operator<(const queue &x, const queue &y);
public:
	typedef Container::value_type value_type;
	typedef Container::pointer pointer;
	typedef Container::const_pointer const_pointer;
	typedef Container::reference reference;
	typedef Container::const_reference const_reference;
	typedef Container::size_type size_type;
	typedef Container::difference_type difference_type;
protected:
	Container __queue;
public:
	bool empty() const
	{
		return __queue.empty();
	}

	bool size() const
	{
		return __queue.size();
	}

	void push_back(const value_type &value)
	{
		__queue.push_back(value);
	}

	void pop()
	{
		__queue.pop_front();
	}

	reference front()
	{
		return __queue.front();
	}

	const_reference front() const
	{
		return __queue.front();
	}

	reference back()
	{
		return __queue.back();
	}

	const_reference back() const
	{
		return __deque.back();
	}
};

template <typename Type,typename Container>
inline bool operator==(const queue<Type, Container> &x, const queue<Type, Container> &y)
{
	return x.__queue == y.__queue;
}

template <typename Type,typename Container>
inline bool operator<(const queue<Type, Container> &x, const queue<Type, Container> &y)
{
	return x.__queue < y.__queue;
}

#endif