/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "MySTL_type_traits.h"
#include "MySTL_construct.h"
#include "MySTL_algobase.h"
#include <cstring>	//for memmove

template <typename InputIterator,typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
	return copy(first,last,);
}

template <typename InputIterator,typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
	ForwardIterator cur = result;
	while (first != last)
	{
		construct(&(*cur), *first);
		first++;
		cur++;
	}
	return cur;
}

template <typename InputIterator,typename ForwardIterator,class Type>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, Type *)
{
	typedef typename __type_traits<Type>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <typename InputIterator,typename ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}


template <>
inline char *uninitialized_copy(const char *first, const char *last, char *result)
{
	return (char *)memmove(result, first, last - first);
}

template <>
inline wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
	return (wchar_t *)memmove(result, first, last - first);
}


template <typename ForwardIterator,typename Size,typename Type>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const Type &x, __true_type)
{
	return fill_n(first, n, x);
}

template <typename ForwardIterator,typename Size,typename Type>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const Type &x, __false_type)
{
	ForwardIterator cur = first;
	while (n > 0)
	{
		construct(&(*cur), x);
		n--;
		++cur;
	}
	return cur;
}

template <typename ForwardIterator,typename Size,typename Type>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, Size n, const Type &x, Type*)
{
	typedef typename __type_traits<Type>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <typename ForwardIterator,typename Size,typename Type>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const Type &x)
{
	return __uninitialized_fill_n(first, n, x, value_type(first));
}

template <typename ForwardIterator, typename Type>
inline ForwardIterator
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const Type &x, __true_type)
{
	return fill(first, last, x);
}

template <typename ForwardIterator,typename Type>
inline ForwardIterator
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const Type &x, __false_type)
{
	ForwardIterator cur = first;
	while (cur != last)
	{
		construct(&(*cur), x);
		cur++;
	}
	
	return cur;
}

template <typename ForwardIterator,typename Type>
inline ForwardIterator
__uninitialized_fill(ForwardIterator first, ForwardIterator last, Type &x, Type *)
{
	typedef typename __type_traits<Type>::is_POD_type is_POD;
	return _uninitialized_fill_aux(first, last, x, is_POD());
}

template <typename ForwardIterator,typename Type>
inline ForwardIterator
uninitialized_fill(ForwardIterator first, ForwardIterator last, const Type &x)
{
	return __uninitialized_fill(first, last, x, value_type(first));
}

#endif