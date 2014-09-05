#ifndef HEAP_H
#define HEAP_H

#include "MySTL_iterator.h"

template <typename RandomAccessIterator,typename Distance,typename Type>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, Type value)
{

}


template <typename RandomAccessIterator,typename Distance,typename Type>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance *, Type *)
{
	__push_heap(first, Distance(last - first), Distance(0), *(last - 1));
}

template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__push_heap_aux(first, last, distance_type(first), value_type(first);
}

#endif