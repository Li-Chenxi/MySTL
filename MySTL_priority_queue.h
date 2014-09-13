#ifndef PRIORITY_QUEUE_H	
#define PRIORITY_QUEUE_H

#include "MySTL_functional.h"
#include "MySTL_vector.h"
#include "MySTL_heap.h"

namespace stupid
{
	template <typename Type, typename Container = vector<Type>, typename Compare = less<Container::value_type>>
	class priority_queue
	{
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::pointer pointer;
		typedef typename Container::const_pointer const_pointer;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef typename Container::size_type size_type;
		typedef typename Container::difference_type difference_type;
	protected:
		Container __priority_queue;
		Compare cmp;
	public:
		priority_queue()
			:__priority_queue()
		{
		}

		explicit priority_queue(const Compare &x)
			:__priority_queue(),
			cmp(x)
		{
		}

		template <typename InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: __priority_queue(first, last)
		{
			make_heap(__priority_queue.begin(), __priority_queue.end(), cmp);
		}

		template<typename InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare &x)
			: __priority_queue(first, last),
			cmp(x)
		{
			make_heap(__priority_queue.begin(), __priority_queue.end(), cmp);
		}

		bool empty() const
		{
			return __priority_queue.empty();
		}

		size_type size() const
		{
			return __priority_queue.size();
		}

		const_reference top() const
		{
			return __priority_queue.front();
		}

		void push(const Type &x)
		{
			try
			{
				__priority_queue.push_back(x);
				push_heap(__priority_queue.begin(), __priority_queue.end(), cmp);
			}
			catch (...)
			{
				__priority_queue.clear();
				throw;
			}
		}

		void pop()
		{
			try
			{
				pop_heap(__priority_queue.begin(), __priority_queue.end(), cmp);
				__priority_queue.pop_back();
			}
			catch (...)
			{
				__priority_queue.clear();
				throw;
			}
		}
	};
}

#endif