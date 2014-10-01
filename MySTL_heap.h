#ifndef HEAP_H
#define HEAP_H

#include "MySTL_iterator.h"

namespace stupid
{
	template <typename RandomAccessIterator, typename Distance, typename Type>
	void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Type value)
	{
		Distance parent = (holeIndex - 1) / 2;
		while (parent >= topIndex&&*(first + parent) < value)
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			if (holeIndex == topIndex)
				break;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template <typename RandomAccessIterator, typename Distance, typename Type, typename Compare>
	void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Type value, const Compare &comp)
	{
		Distance parent = (holeIndex - 1) / 2;
		while (parent >= topIndex&&comp(*(first + parent), value))
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			if (holeIndex == topIndex)
				break;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template <typename RandomAccessIterator, typename Distance, typename Type>
	inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance *, Type *)
	{
		__push_heap(first, Distance(last - first - 1), Distance(0), Type(*(last - 1)));
	}

	template <typename RandomAccessIterator, typename Distance, typename Type, typename Compare>
	inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance *, Type *, const Compare &comp)
	{
		__push_heap(first, Distance(last - first - 1), Distance(0), Type(*(last - 1)), comp);
	}

	template <typename RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__push_heap_aux(first, last, stupid::distance_type(first), stupid::value_type(first));
	}

	template <typename RandomAccessIterator, typename Compare>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		__push_heap_aux(first, last, stupid::distance_type(first), stupid::value_type(first), comp);
	}

	template <typename RandomAccessIterator, typename Distance, typename Type>
	void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Type value)
	{
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;

		while (secondChild < len)
		{
			if (*(first + secondChild) < *(first + secondChild - 1))
				--secondChild;
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * holeIndex + 2;
		}
		if (secondChild == len)
		{
			*(first + holeIndex) = *(first + secondChild - 1);
			holeIndex = secondChild - 1;
		}

		__push_heap(first, holeIndex, topIndex, value);
	}

	template <typename RandomAccessIterator, typename Distance, typename Type, typename Compare>
	void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, Type value, const Compare &comp)
	{
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;

		while (secondChild < len)
		{
			if (comp(*(first + secondChild), *(first + secondChild - 1)))
				--secondChild;
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * holeIndex + 2;
		}
		if (secondChild == len)
		{
			*(first + holeIndex) = *(first + secondChild - 1);
			holeIndex = secondChild - 1;
		}

		__push_heap(first, holeIndex, topIndex, value, comp);
	}

	template <typename RandomAccessIterator, typename Distance, typename Type>
	inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Type value, Distance *)
	{
		*result = *first;
		__adjust_heap(first, Distance(0), Distance(last - first), value);
	}

	template <typename RandomAccessIterator, typename Distance, typename Type, typename Compare>
	inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Type value, Distance *, const Compare &comp)
	{
		*result = *first;
		__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
	}

	template <typename RandomAccessIterator, typename Type>
	inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Type *)
	{
		__pop_heap(first, last - 1, last - 1, Type(*(last - 1)), distance_type(first));
	}

	template <typename RandomAccessIterator, typename Type, typename Compare>
	inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Type *, const Compare &comp)
	{
		__pop_heap(first, last - 1, last - 1, Type(*(last - 1)), distance_type(first), comp);
	}


	template <typename RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__pop_heap_aux(first, last, value_type(first));
	}

	template <typename RandomAccessIterator, typename Compare>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		__pop_heap_aux(first, last, value_type(first), comp);
	}

	template <typename RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		while (last - first >= 1)
			pop_heap(first, last--);
	}

	template <typename RandomAccessIterator, typename Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		while (last - first >= 1)
			pop_heap(first, last--, comp);
	}

	template <typename RandomAccessIterator, typename Distance, typename Type>
	void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance *, Type *)
	{
		Distance len = last - first;
		if (len < 2)
			return;
		Distance holeIndex = (len - 2) / 2;
		while (true)
		{
			__adjust_heap(first, holeIndex, len, Type(*(first + holeIndex)));
			if (0 == holeIndex)
				return;
			--holeIndex;
		}
	}

	template <typename RandomAccessIterator, typename Distance, typename Type, typename Compare>
	void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance *, Type *, const Compare &comp)
	{
		Distance len = last - first;
		if (len < 2)
			return;
		Distance holeIndex = (len - 2) / 2;
		while (true)
		{
			__adjust_heap(first, holeIndex, len, Type(*(first + holeIndex)), comp);
			if (0 == holeIndex)
				return;
			--holeIndex;
		}
	}

	template <typename RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__make_heap(first, last, distance_type(first), value_type(first));
	}

	template <typename RandomAccessIterator, typename Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		__make_heap(first, last, distance_type(first), value_type(first), comp);
	}
}

#endif