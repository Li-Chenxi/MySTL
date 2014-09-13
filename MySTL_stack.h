#ifndef STACK_H
#define STACK_H

#include "MySTL_deque.h"

namespace stupid
{
	template <typename Type, typename Container = deque<Type>>
	class stack
	{
		friend bool operator==(const stack &x, const stack &y);
		friend bool operator<(const stack &x, const stack &y);
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::pointer pointer;
		typedef typename Container::const_pointer const_pointer;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef typename Container::size_type size_type;
		typedef typename Container::difference_type difference_type;
	protected:
		Container __stack;
	public:
		bool empty() const
		{
			return __stack.empty();
		}

		size_type size() const
		{
			return __stack.size();
		}

		void push(const value_type &value)
		{
			__stack.push_back(value);
		}

		void pop()
		{
			__stack.pop_back();
		}

		reference top()
		{
			return __stack.back();
		}

		const_reference top() const
		{
			return __stack.back();
		}
	};

	template <typename Type, typename Container>
	inline bool operator==(const stack<Type, Container> &x, const stack<Type, Container> &y)
	{
		return x.__stack == y.__stack;
	}

	template <typename Type, typename Container>
	inline bool operator<(const stack<Type, Container> &x, const stack<Type, Container> &y)
	{
		return x.__stack < y.__stack;
	}
}

#endif