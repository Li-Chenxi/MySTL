#ifndef QUEUE_H
#define QUEUE_H

#include "MySTL_deque.h"

namespace stupid
{
	template <typename Type, typename Container = deque<Type>>
	class queue
	{
		friend bool operator==(const queue &x, const queue &y);
		friend bool operator<(const queue &x, const queue &y);
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::pointer pointer;
		typedef typename Container::const_pointer const_pointer;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef typename Container::size_type size_type;
		typedef typename Container::difference_type difference_type;
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

		void pop_front()
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

	template <typename Type, typename Container>
	inline bool operator==(const queue<Type, Container> &x, const queue<Type, Container> &y)
	{
		return x.__queue == y.__queue;
	}

	template <typename Type, typename Container>
	inline bool operator<(const queue<Type, Container> &x, const queue<Type, Container> &y)
	{
		return x.__queue < y.__queue;
	}
}

#endif