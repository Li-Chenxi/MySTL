/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>	//for placement new
#include "MySTL_type_traits.h"
#include "MySTL_iterator.h"

namespace stupid
{
	template <typename T1, typename T2>
	inline void construct(T1 *p, T2 &value)
	{
		new(p)T2(value);
	}

	template <typename T1>
	inline void construct(T1 *p)
	{
		stupid::construct(p, T1());
	}


	template <typename T>
	inline void destroy(T *p)
	{
		p->~T();
	}

	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
	{
	}

	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
	{
		for (; first != last; first++)
			stupid::destroy(&(*first));
	}

	template <typename ForwardIterator, typename Type>
	inline void __destroy(ForwardIterator first, ForwardIterator last, Type *)
	{
		typedef typename __type_traits<Type>::has_trivial_destructor trivial_constructor;
		stupid::__destroy_aux(first, last, trivial_constructor());
	}

	template <typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		stupid::__destroy(first, last, value_type(first));
	}

	void destroy(char *, char *)
	{
	}

	void destroy(wchar_t *, wchar_t *)
	{
	}
}

#endif