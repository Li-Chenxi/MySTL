#ifndef NUMERIC_H
#define NUMERIC_H

#include "MySTL_iterator.h"
#include "MySTL_functional.h"

namespace stupid
{
	template <typename InputIterator, typename Type>
	Type accumulate(InputIterator first, InputIterator last, Type init)
	{
		for (; first != last; ++first)
			init = init + *first;
		return init;
	}

	template <typename InputIterator, typename Type, typename BinaryOperator>
	Type accumulate(InputIterator first, InputIterator last, Type init, BinaryOperator op)
	{
		for (; first != last; ++first)
			init = op(init, *first);
		return init;
	}

	template <typename InputIterator, typename OutputIterator, typename Type>
	OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, Type *)
	{
		Type value = *first;
		while ((++first) != last)
		{
			Type tmp = *first;
			*(++result) = tmp - value;
			value = tmp;
		}
		return result;
	}

	template <typename InputIterator, typename OutputIterator, typename BinaryOperator, typename Type>
	OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op, Type *)
	{
		Type value = *first;
		while ((++first) != last)
		{
			Type tmp = *first;
			*(++result) = op(tmp, value);
			value = tmp;
		}
		return result;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == last)
			return result;
		*result = *first;
		return __adjacent_difference(first, last, result, value_type(first));
	}

	template <typename InputIterator, typename OutputIterator, typename BinaryOperator>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op)
	{
		if (first == last)
			return result;
		__adjacent_difference(first, last, result, op, value_type(first));
	}


	template <typename InputIterator, typename Type>
	Type inner_product(InputIterator first1, InputIterator last1, InputIterator first2, Type init)
	{
		for (; first1 != last1; ++first1, ++first2)
			init = init + (*first1)*(*first2);
		return init;
	}

	template <typename InputIterator, typename Type, typename BinaryOperator1, typename BinaryOperator2>
	Type inner_product(InputIterator first1, InputIterator last1, InputIterator first2, Type init, BinaryOperator1 op1, BinaryOperator2 op2)
	{
		for (; first1 != last1!; ++first1, ++first2)
			init = op1(init, op2(*first1, *first2));
		return init;
	}

	template <typename InputIterator, typename OutputIterator, typename Type>
	OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, Type *)
	{
		Type value = *first;
		while ((++first) != last)
		{
			Type tmp = *first;
			value = value + tmp;
			*(++result) = value;
		}
		return result;
	}

	template <typename InputIterator, typename OutputIterator, typename BinaryOperator, typename Type>
	OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op, Type *)
	{
		Type value = *first;
		while ((++first) != last)
		{
			Type tmp = *first;
			value = op(value, op);
			*(++result) = value;
		}
		return result;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == last)
			return result;
		*result = *first;
		return __partial_sum(first, last, result, value_type(first));
	}

	template <typename InputIterator, typename OutputIterator, typename BinaryOperator>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op)
	{
		if (first == last)
			return result;
		*result = *first;
		return __partial_sum(first, last, result, op, value_type(first));
	}

	template <typename Type, typename Integer>
	inline Type power(Type x,Integer y)
	{
		return power(x, y, multiplies<Type>());
	}

	template <typename Type,typename Integer,typename MonoidOperator>
	Type power(Type x, Integer y, MonoidOperator op)
	{
		if (y == 0)
			return identity_element(op);
		else
		{
			while ((y & 1) == 0)
			{
				x = op(x, x);
				y >>= 1;
			}

			Type result = x;
			y >>= 1;
			while (y != 0)
			{
				x = op(x, x);
				if ((y & 1) != 0)
					result = op(result, x);
				y >>= 1;
			}
			return result;
		}
	}

	template <typename ForwardIterator,typename Type>
	void iota(ForwardIterator first, ForwardIterator last, Type value)
	{
		while (first != last)
			*(first++) = value++;
	}
}

#endif