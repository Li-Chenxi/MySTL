#ifndef ALGOBASE_H
#define ALGOBASE_H

#include "MySTL_iterator.h"
#include "MySTL_type_traits.h"
#include <utility>
#include <cstring>
#include <cstddef>

namespace stupid
{
	template <typename InputIterator>
	bool equal(InputIterator first1, InputIterator last1, InputIterator first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
				return false;
		}
		return true;
	}

	template <typename InputIterator, typename Compare>
	bool equal(InputIterator first1, InputIterator last1, InputIterator first2, Compare comp)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!Compare(*first1, *first2))
				return false;
		}
		return true;
	}

	template <typename ForwardIterator, typename Type>
	ForwardIterator fill(ForwardIterator first, ForwardIterator last, Type value)
	{
		for (; first != last; ++first)
			*first = value;
		return first;
	}

	template <typename OutputIterator, typename Size, typename Type>
	OutputIterator fill_n(OutputIterator first, Size n, Type value)
	{
		for (; n > 0; --n)
			*(first++) = value;
		return first;
	}

	template <typename ForwardIterator1, typename ForwardIterator2, typename Type>
	inline void __iter_swap(ForwardIterator1 iterator1, ForwardIterator2 iterator2, Type *)
	{
		Type tmp = *iterator1;
		*iterator1 = *iterator2;
		*iterator2 = tmp;
	}

	template <typename ForwardIterator1, typename ForwardIterator2>
	inline void iter_swap(ForwardIterator1 iterator1, ForwardIterator2 iterator2)
	{
		__iter_swap(iterator1, iterator2, value_type(iterator1));
	}

	template <typename InputIterator1, typename InputIterator2>
	bool lexicalgraphical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first1 > *first2)
				return false;
		}

		return (first1 == last1) && (first2 != last2);
	}

	template <typename InputIterator1, typename InputIterator2, typename Compare>
	bool lexicalgraphical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
		{
			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, first1))
				return false;
		}

		return (first1 == last1) && (first2 != last2);
	}

	template <typename Type>
	inline const Type &max(const Type &x, const Type &y)
	{
		return x < y ? y : x;
	}

	template <typename Type, typename Compare>
	inline const Type &max(const Type &x, const Type &y, Compare comp)
	{
		return comp(x, y) ? y : x;
	}

	template <typename Type>
	inline const Type &min(const Type &x, const Type &y)
	{
		return x < y ? x : y;
	}

	template <typename Type, typename Compare>
	inline const Type &min(const Type &x, const Type &y, Compare comp)
	{
		return comp(x, y) ? x : y;
	}

	template <typename InputIterator1, typename InputIterator2>
	std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while ((first1 != last1) && (*first1 == *first2))
		{
			++first1;
			++first2;
		}

		return std::pair<InputIterator1, InputIterator2>(first1, first2);
	}

	template <typename InputIterator1, typename InputIterator2, typename Compare>
	std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, Compare comp)
	{
		while ((first1 != last1) && (comp(*first1, *first2)))
		{
			++first1;
			++first2;
		}

		return std::pair<InputIterator1, InputIterator2>(first1, first2);
	}

	template <typename Type>
	inline void swap(Type &x, Type &y)
	{
		Type tmp = x;
		x = y;
		y = tmp;
	}

	template <typename InputIterator, typename OutputIterator, typename Distance>
	OutputIterator __copy_d(InputIterator first, InputIterator last, OutputIterator result, Distance *)
	{
		for (Distance n = last - first; n > 0; --n, ++first, ++result)
			*result = *first;
		return result;
	}

	template <typename Type>
	inline Type *__copy_t(const Type *first, const Type *last, Type *result, __true_type)
	{
		memmove(result, first, sizeof(Type)*(last - first));
		return result + (last - first);
	}

	template <typename Type>
	inline Type *__copy_t(const Type *first, const Type *last, Type *result, __false_type)
	{
		return __copy_d(first, last, result, (ptrdiff_t *)0);
	}

	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d(first, last, result, distance_type(first));
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}

	template <typename InputIterator, typename OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			return __copy(first, last, result, iterator_category(first));
		}
	};

	template <typename Type>
	struct __copy_dispatch < Type *, Type * >
	{
		Type * operator()(Type *first, Type *last, Type *result)
		{
			typedef typename __type_traits<Type>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template <typename Type>
	struct __copy_dispatch < const Type *, Type * >
	{
		Type *operator()(const Type *first, const Type *last, Type *result)
		{
			typedef typename __type_traits<Type>::has_trival_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	inline char *copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}

	template <typename BidirectionalIterator1,typename BidirectionalIterator2,typename Distance>
	BidirectionalIterator2 __copy_backward_d(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, Distance *)
	{
		for (Distance n = last - first; n > 0; --n)
			*(--result) = *(--last);
		return result;
	}

	template <typename Type>
	Type *__copy_backward_t(const Type *first, const Type *last, Type *result, __true_type)
	{
		result = result - (last - first);
		memmove(result, first, sizeof(Type)*(last - first));
		return result;
	}

	template <typename Type>
	Type *__copy_backward_t(const Type *first, const Type *last, Type *result, __false_type)
	{
		return __copy_backward_d(first, last, result, (ptrdiff_t *)(0));
	}

	template <typename BidirectionalIterator1,typename BidirectionalIterator2>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, random_access_iterator_tag)
	{
		return __copy_backward_d(first, last, result, distance_type(first));
	}

	template <typename BidirectionalIterator1,typename BidirectionalIterator2>
	BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, bidirectional_iterator_tag)
	{
		for (; first != last;)
			*(--result) = *(--last);
		return result;
	}
	
	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	struct __copy_backward_dispatch
	{
		inline BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
		{
			return __copy_backward(first, last, result, iterator_category(first));
		}
	};

	template <typename Type>
	struct __copy_backward_dispatch < Type *, Type * >
	{
		inline Type *operator()(Type *first, Type *last, Type *result)
		{
			typedef typename __type_traits<Type>::has_trivial_assignment_operator t;
			return __copy_backward_t(first, last, result, t());
		}
	};

	template <typename Type>
	struct __copy_backward_dispatch < const Type *, Type * >
	{
		inline Type *operator()(const Type *first, const Type *last, Type *result)
		{
			typedef typename __type_traits<Type>::has_trivial_assignment_operator t;
			return __copy_backward_t(first, last, result, t());
		}
	};

	template <typename BidirectionalIterator1,typename BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		return __copy_backward_dispatch< BidirectionalIterator1, BidirectionalIterator2 >() (first, last, result);
	}

	inline char *copy_backward(const char *first,const char *last,char *result)
	{
		result = result - (last - first);
		memmove(result, first, last - first);
		return result;
	}

	inline wchar_t *copy_backward(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		result = result - (last - first);
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}
}

#endif