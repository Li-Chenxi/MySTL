#ifndef ALGO_H
#define ALGO_H

#include "MySTL_iterator.h"
#include "MySTL_heap.h"
#include "MySTL_tempbuf.h"
#include <utility>
#include <cstdlib>
#include <ctime>

namespace stupid
{
	template <typename ForwardIterator, typename Type, typename Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const Type &value, Distance *, stupid::forward_iterator_tag)
	{
		Distance len = distance(first, last);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			stupid::advance(middle, half);
			if (*middle < value)
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
			else
				len = half;
		}

		return first;
	}

	template <typename RandomAccessIterator, typename Type, typename Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const Type &value, Distance *, stupid::random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len>0)
		{
			half = len >> 1;
			middle = first + half;
			if (*middle < value)
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
				len = half;
		}

		return first;
	}

	template <typename ForwardIterator, typename Type>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const Type &value)
	{
		return __lower_bound(first, last, value, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename ForwardIterator, typename Type, typename Compare, typename Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const Type &value, Compare comp, Distance *, stupid::forward_iterator_tag)
	{
		Distance len = stupid::distance(first, last);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			stupid::advance(middle, half);
			if (comp(*middle, value))
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
			else
				len = half;
		}

		return first;
	}

	template <typename RandomAccessIterator, typename Type, typename Compare, typename Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const Type &value, Compare comp, Distance *, stupid::random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
			if (comp(*middle, value))
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
				len = half;
		}
		return first;
	}

	template <typename ForwardIterator, typename Type, typename Compare>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator lat, const Type &value, Compare comp)
	{
		return __lower_bound(first, last, value, comp, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename ForwardIterator, typename Type, typename Distance>
	ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const Type &value, Distance *, stupid::forward_iterator_tag)
	{
		Distance len = stupid::distance(first, last);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			stupid::advance(middle, half);
			if (value < *middle)
				len = half;
			else
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
		}
		return last;
	}

	template <typename RandomAccessIterator, typename Type, typename Distance>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const Type &value, Distance *, stupid::random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
			if (value < *middle)
				len = half;
			else
			{
				first = middle;
				len = len - half - 1;
			}
		}
		return last;
	}

	template <typename ForwardIterator, typename Type>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const Type &value)
	{
		return __upper_bound(first, last, value, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename ForwardIterator, typename Type, typename Distance, typename Compare>
	ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const Type &value, Compare comp, Distance *, stupid::forward_iterator_tag)
	{
		Distance len = stupid::distance(first, last);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			stupid::advance(middle, half);
			if (comp(value, *middle))
				len = half;
			else
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
		}
		return last;
	}

	template <typename RandomAccessIterator, typename Type, typename Compare, typename Distance>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const Type &value, Compare comp, Distance *, stupid::random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
			if (comp(value, *middle))
				len = half;
			else
			{
				first = middle;
				len = len - half - 1;
			}
		}
		return last;
	}

	template <typename ForwardIterator, typename Type, typename Compare>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const Type &value, Compare comp)
	{
		return __upper_bound(first, last, value, comp, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		typedef typename stupid::iterator_traits<ForwardIterator1>::difference_type distance_type1;
		typedef typename stupid::iterator_traits<ForwardIterator2>::difference_type distance_type2;

		if (first1 == last1 || first2 == last2)
			return last1;

		distance_type1 distance1 = stupid::distance(first1, last1);
		distance_type2 distance2 = stupid::distance(first2, last2);

		if (distance1 < distance2)
			return last1;

		ForwardIterator1 current1 = first1;
		ForwardIterator2 current2 = first2;

		while (current2 != last2)
		{
			if (*current1 != *current2)
			{
				if (distance1 == distance2)
					return last1;
				else
				{
					current1 = ++first1;
					current2 = first2;
					--distance1;
				}
			}
			else
			{
				++current1;
				++current2;
			}
		}

		return first1;
	}

	template <typename ForwardIterator1, typename ForwardIterator2, typename Compare>
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Compare comp)
	{
		typedef typename stupid::iterator_traits<ForwardIterator1>::difference_type distance_type1;
		typedef typename stupid::iterator_traits<ForwardIterator2>::difference_type distance_type2;

		if (first1 == last1 || first2 == last2)
			return last1;

		distance_type1 distance1 = stupid::distance(first1, last1);
		distance_type2 distance2 = stupid::distance(first2, last2);

		if (distance1 < distance2)
			return last1;

		ForwardIterator1 current1 = first1;
		ForwardIterator2 current2 = first2;

		while (current2 != last2)
		{
			if (!comp(*current1, *current2))
			{
				if (distance1 == distance2)
					return last1;
				else
				{
					current1 = ++first1;
					current2 = first2;
					--distance1;
				}
			}
			else
			{
				++current1;
				++current2;
			}
		}

		return first1;
	}

	template <typename InputIterator, typename Count, typename Type>
	InputIterator search_n(InputIterator first, InputIterator last, Count count, const Type &value)
	{
		if (count <= 0)
			return last;
		first = stupid::find(first, last, value);
		while (first != last)
		{
			InputIterator current = first;
			Count n = 1;
			while ((++current) != last&&n < count&&value == *current)
				++n;
			if (n == count)
				return first;
			else
				first = stupid::find(current, last, value);
		}

		return last;
	}

	template <typename InputIterator, typename Count, typename Type, typename Compare>
	InputIterator search_n(InputIterator first, InputIterator last, Count count, const Type &value, Compare comp)
	{
		if (count <= 0)
			return last;
		first = stupid::find(first, last, value, comp);
		while (first != last)
		{
			InputIterator current = first;
			Count n = 1;
			while ((++current) != last&&comp(*current, value))
				++n;
			if (n == count)
				return first;
			else
				first = stupid::find(current, last, value, comp);
		}

		return last;
	}

	template <typename ForwardIterator>
	ForwardIterator adjcacent_find(ForwardIterator first, ForwardIterator last)
	{
		ForwardIterator current = first;
		while (++current != last)
		{
			if (*current == *first)
				return first;
			first = current;
		}

		return last;
	}

	template <typename ForwardIterator, typename Compare>
	ForwardIterator adjcacent_find(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		ForwardIterator current = first;
		while (++current != last)
		{
			if (comp(*first, *current))
				return first;
			first = current;
		}

		return last;
	}

	template <typename InputIterator, typename Type>
	typename stupid::iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const Type &value)
	{
		typename stupid::iterator_traits<InputIterator>::difference_type result = 0;

		while (first != last)
		{
			if (value == *first)
				++result;
			++first;
		}

		return result;
	}

	template <typename InputIterator, typename Type, typename Compare>
	typename stupid::iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const Type &value, Compare comp)
	{
		typename stupid::iterator_traits<InputIterator>::difference_type result = 0;

		while (first != last)
		{
			if (comp(*first, value))
				++result;
			++first;
		}

		return result;
	}

	template <typename InputIterator, typename Predicate>
	typename stupid::iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Predicate pred)
	{
		typename stupid::iterator_traits<InputIteraot>::difference_type result = 0;

		while (first != last)
		{
			if (pred(*first))
				++result;
			++first;
		}

		return result;
	}

	template <typename InputIterator, typename Type>
	InputIterator find(InputIterator first, InputIterator last, const Type &value)
	{
		while (first != last&&*first != value)
			++first;
		return first;
	}

	template <typename InputIterator, typename Type, typename Compare>
	InputIterator find(InputIterator first, InputIterator last, const Type &value, Compare comp)
	{
		while (first != last &&!(comp(*first, value)))
			++first;
		return first;
	}

	template <typename InputIterator, typename Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
	{
		while (first != last&&!pred(*first))
			++first;
		return first;
	}

	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
		forward_iterator_tag, forward_iterator_tag)
	{
		if (first2 == last2)
			return last1;
		ForwardIterator1 result = last1;
		while (true)
		{
			ForwardIterator1 new_result = stupid::search(first1, last1, first2, last2);
			if (new_result != last1)
			{
				result = new_result;
				first1 = new_result;
				++first1;
			}
			else
				return result;
		}
	}

	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2,
		bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		typedef reverse_iterator<BidirectionalIterator1> rite1;
		typedef reverse_iterator<BidirectionalIterator2> rite2;

		rite1 rlast1(first1);
		rite2 rlast2(first2);

		rite1 rresult = stupid::search(rite1(last1), rlast1, rite(last2), rlast2);
		if (rresult != rlast1)
		{
			BidirectionalIterator1 result = rresult.base();
			stupid::advance(result, -distance(first2, last2));
			return result;
		}
		else
			return last1;
	}

	template <typename InputIterator1, typename InputIterator2>
	InputIterator1 find_end(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		typedef typename stupid::iterator_traits<first1>::iterator_category category1;
		typedef typename stupid::iterator_traits<first2>::iterator_category category2;

		return __find_end(first1, last1, first2, last2, category1(), category2());
	}

	template <typename ForwardIterator1, typename ForwardIterator2, typename Compare>
	ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Compare comp, 
		forward_iterator_tag, forward_iterator_tag)
	{
		if (first2 == last2)
			return last1;
		ForwardIterator1 result = last1;
		while (true)
		{
			ForwardIterator1 new_result = stupid::search(first1, last1, first2, last2, comp);
			if (new_result != last1)
			{
				result = new_result;
				first1 = new_result;
				++first1;
			}
			else
				return result;
		}
	}

	template <typename BidirectionalIterator1, typename BidirectionalIterator2, typename Compare>
	BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, Compare comp, 
		bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		typedef reverse_iterator<BidirectionalIterator1> rite1;
		typedef reverse_iterator<BidirectionalIterator2> rite2;

		rite1 rlast1(first1);
		rite2 rlast2(first2);

		rite1 rresult = stupid::search(rite1(last1), rlast1, rite(last2), rlast2, comp);
		if (rresult != rlast1)
		{
			BidirectionalIterator1 result = rresult.base();
			stupid::advance(result, -distance(first2, last2));
			return result;
		}
		else
			return last1;
	}

	template <typename InputIterator1, typename InputIterator2, typename Compare>
	InputIterator1 find_end(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		typedef typename stupid::iterator_traits<first1>::iterator_category category1;
		typedef typename stupid::iterator_traits<first2>::iterator_category category2;

		return __find_end(first1, last1, first2, last2, category1(), category2(), comp);
	}

	template <typename InputIterator1, typename InputIterator2>
	InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			for (InputIterator2 current = first2; current != last2; ++current)
				if (*first1 == *current)
					return first1;
			++first1;
		}

		return last1;
	}

	template <typename InputIterator1, typename InputIterator2, typename Compare>
	InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1)
		{
			for (InputIterator2 current = first2; current != last2; ++current)
				if (comp(*first1, *current))
					return first1;
			++first1;
		}

		return last1;
	}

	template <typename InputIterator, typename Function>
	Function for_each(InputIterator first, InputIterator last, Function function)
	{
		while (first != last)
		{
			function(*first);
			++first;
		}

		return function;
	}

	template <typename ForwardIterator, typename Generator>
	void generate(ForwardIterator first, ForwardIterator last, Generator gen)
	{
		while (first != last)
		{
			*first = gen();
			++first;
		}
	}

	template <typename OutputIterator, typename Size, typename Generator>
	void generate_n(OutputIterator first, Size n, Generator gen)
	{
		for (; n > 0; --n)
		{
			*first = gen();
			++first;
		}

		return first;
	}

	template <typename InputIterator1, typename InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (*first2 < *first1)
				return false;
			else
			{
				if (*first1 < *first2)
					++first1;
				else
				{
					++first1;
					++first2;
				}
			}
		}

		return first2 == last2;
	}

	template <typename InputIterator1, typename InputIterator2, typename Compare>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (comp(*first2, *first1))
				return false;
			else
			{
				if (comp(*first1, *first2))
					++first1;
				else
				{
					++first1;
					++first2;
				}
			}
		}

		return first2 == last2;
	}

	template <typename InputIterator>
	InputIterator max_element(InputIterator first, InputIterator last)
	{
		if (first == last)
			return first;
		InputIterator result = first;
		while ((++first) != last)
		{
			if (*result < *first)
				result = first;
		}

		return result;
	}

	template <typename InputIterator, typename Compare>
	InputIterator max_element(InputIterator first, InputIterator last, Compare comp)
	{
		if (first == last)
			return first;
		InputIterator result = first;
		while ((++first) != last)
		{
			if (comp(*result, *first))
				result = first;
		}

		return result;
	}

	template <typename InputIterator>
	InputIterator min_element(InputIterator first, InputIterator last)
	{
		if (first == last)
			return first;
		InputIterator result = first;
		while ((++first) != last)
		{
			if (*first < *result)
				result = first;
		}

		return result;
	}

	template <typename InputIterator, typename Compare>
	InputIterator min_element(InputIterator first, InputIterator last, Compare comp)
	{
		if (first == last)
			return first;
		InputIterator result = first;
		while ((++first) != last)
		{
			if (comp(*first, *result))
				result = first;
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2  last2, OutputIterator result)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (*first2 < *first1)
			{
				*result = *first2;
				++result;
				++first2;
			}
			else
			{
				*result = *first1;
				++result;
				++first1;
			}
		}

		return stupid::copy(first1, last1, copy(first2, last2, result));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2  last2, OutputIterator result, Compare comp)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (comp(*first2, *first1))
			{
				*result = *first2;
				++result;
				++first2;
			}
			else
			{
				*result = *first1;
				++result;
				++first1;
			}
		}

		return stupid::copy(first1, last1, copy(first2, last2, result));
	}

	template <typename BidirectionalIterator, typename Predicate>
	BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred)
	{
		while (first != last)
		{
			while (true)
			{
				if (first == last)
					return first;
				if (!pred(*first))
					break;
				++first;
			}

			--last;
			while (true)
			{
				if (first == last)
					return first;
				if (pred(*last))
					break;
				--last;
			}

			stupid::iter_swap(first, last);
			++first;
		}
	}

	template <typename InputIterator, typename OutputIterator, typename Type>
	OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const Type &value)
	{
		while (first != last)
		{
			if (value != *first)
			{
				*result = *first;
				++result;
			}
			++first;
		}

		return result;
	}

	template <typename ForwardIterator, typename Type>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const Type &value)
	{
		first = stupid::find(first, last, value);
		ForwardIterator next;
		return first == last ? first : stupid::remove_copy(++next, last, first, value);
	}

	template <typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
	{
		while (first != last)
		{
			if (!pred(*first))
			{
				*result = *first;
				++result;
			}
			++first;
		}
		return result;
	}

	template <typename ForwardIterator, typename Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred)
	{
		first = stupid::find_if(first, last, pred);
		ForwardIterator next = first;
		return first == last ? first : stupid::remove_copy_if(++next, last, first, pred);
	}

	template <typename ForwardIterator, typename Type>
	void replace(ForwardIterator first, ForwardIterator last, const Type &old_value, const Type &new_value)
	{
		while (first != last)
		{
			if (old_value == *first)
				*first = new_value;
			++first;
		}
	}

	template <typename ForwardIterator, typename Type, typename Compare>
	void replace(ForwardIterator first, ForwardIterator last, const Type &old_value, const Type &new_value, Compare comp)
	{
		while (first != last)
		{
			if (comp(*first, old_value))
				*first = new_value;
			++first;
		}
	}

	template <typename InputIterator, typename OutputIterator, typename Type>
	void replace_copy(InputIterator first, InputIterator last, OutputIterator result, const Type &old_value, const Type &new_value)
	{
		while (first != last)
		{
			if (old_value == *first)
				*result = new_value;
			else
				*result = *first;
			++result;
			++first;
		}
	}

	template <typename InputIterator, typename OutputIterator, typename Type, typename Compare>
	void replace_copy(InputIterator first, InputIterator last, OutputIterator result, const Type &old_value, const Type &new_value, Compare comp)
	{
		while (first != last)
		{
			if (comp(*first, old_value))
				*result = new_value;
			else
				*result = *first;
			++result;
			++first;
		}
	}

	template <typename ForwardIterator, typename Predicate, typename Type>
	void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const Type &new_value)
	{
		while (first != last)
		{
			if (pred(*first))
				*first = new_value;
			++first;
		}
	}

	template <typename InputIterator, typename OutputIterator, typename Predicate, typename Type>
	void replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const Type &new_type)
	{
		while (first != last)
		{
			if (pred(*first))
				*result = *new_value;
			else
				*result = *first;
			++result;
			++first;
		}
	}

	template <typename BidirectionalIterator>
	void __reverse(BidirectionalIterator first, BidirectionalIterator last, stupid::bidirectional_iterator_tag)
	{
		while (true)
		{
			if (first == last || first == --last)
				return;
			else
				stupid::iter_swap(first, last);
		}
	}

	template <typename RandomAccessIterator>
	void __reverse(RandomAccessIterator first, RandomAccessIterator last, stupid::random_access_iterator_tag)
	{
		while (first < last)
		{
			--last;
			stupid::iter_swap(first, last);
			++first;
		}
	}

	template <typename BidirectionalIterator>
	inline void reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		typedef stupid::iterator_traits<BidirectionalIterator>::iterator_category category;
		__reverse(first, last, category());
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator reverse_copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		while (first != last)
		{
			--last;
			*result = *last;
			++result;
		}

		return result;
	}

	template <typename ForwardIterator, typename Distance>
	ForwardIterator __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance *, stupid::forward_iterator_tag)
	{
		for (ForwardIterator i = middle;;)
		{
			stupid::iter_swap(first, i);
			++first;
			++i;

			if (first == middle)
			{
				if (i == last)
					return first;
				else
					middle = i;
			}
			else
				if (i == last)
					i = middle;
		}
	}

	template <typename BidirectionalIterator, typename Distance>
	BidirectionalIterator __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance *, stupid::bidirectional_iterator_tag)
	{
		stupid::reverse(first, middle);
		stupid::reverse(middle, last);
		stupid::reverse(first, last);
		stupid::advance(first, stupid::distance(middle, last));
		return first;
	}

	template <typename EuclideanRingElement>
	EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
	{
		EuclideanRingElement tmp;
		while (n != 0)
		{
			tmp = m;
			m = n;
			n = tmp%n;
		}
		return m;
	}

	template <typename RandomAccessIterator, typename Distance ,typename Type>
	void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, Type *)
	{
		RandomAccessIterator ptr1 = initial;
		Type value = *ptr1;
		RandomAccessIterator ptr2 = ptr1 + shift;
		while (ptr2 != initial)
		{
			*ptr1 = *ptr2;
			ptr1 = ptr2;
			if (last - ptr2 > shift)
				ptr2 = ptr1 + shift;
			else
				ptr1 = first + (shift - (last - ptr2));
		}

		*ptr1 = value;
	}

	template <typename RandomAccessIterator, typename Distance>
	RandomAccessIterator __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance *, stupid::random_access_iterator_tag)
	{
		Distance n = stupid::__gcd(middle - first, last - middle);
		while (n--)
		{
			stupid::__rotate_cycle(first, last, first + n, middle - first, stupid::value_type(first));
		}

		return first + (last - middle);
	}

	template <typename ForwardIterator>
	inline ForwardIterator rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
	{
		if (first == middle || middle == last)
			return middle;
		return __rotate(first, middle, last, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator rotate_copy(InputIterator first, InputIterator middle, InputIterator last, OutputIterator result)
	{
		return stupid::copy(first, middle, stupid::copy(middle, last, result));
	}

	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
	{
		while (first1 != last1)
		{
			iter_swap(first1, first2);
			++first1;
			++first2;
		}

		return first2;
	}

	template <typename InputIterator, typename OutputIterator , typename UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op)
	{
		while (first != last)
		{
			*result = op(*first);
			++first;
			++result;
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperation>
	OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, 
		OutputIterator result, 
		BinaryOperation op)
	{
		while (first1 != last1)
		{
			*result = op(*first1, *first2);
			++first1;
			++first2;
			++result;
		}

		return result;
	}

	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator __unique_copy(InputIterator first, InputIterator last, ForwardIterator result, stupid::forward_iterator_tag)
	{
		*result = *first;
		while ((++first != last))
		{
			if (*first != *result)
			{
				++result;
				*result = *first;
			}
		}

		return ++result;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, stupid::output_iterator_tag)
	{
		typedef stupid::iterator_traits<InputIterator>::value_type value_type;

		value_type value = *first;
		*result = *first;
		while ((++first) != last)
		{
			if (*first != value)
			{
				++result;
				*result = *first;
				value = *first;
			}
		}

		return ++result;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __unique_copy(first, last, result, stupid::iterator_category(result));
	}

	template<typename InputIterator, typename ForwardIterator, typename Compare>
	ForwardIterator __unique_copy(InputIterator first, InputIterator last, ForwardIterator result, Compare comp, stupid::forward_iterator_tag)
	{
		*result = *first;
		while ((++first != last))
		{
			if (!comp(*first, *result))
			{
				++result;
				*result = *first;
			}
		}

		return ++result;
	}

	template <typename InputIterator, typename OutputIterator, typename Compare>
	OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, Compare comp, stupid::output_iterator_tag)
	{
		typedef stupid::iterator_traits<InputIterator>::value_type value_type;

		value_type value = *first;
		*result = *first;
		while ((++first) != last)
		{
			if (!comp(*first, value))
			{
				++result;
				*result = *first;
				value = *first;
			}
		}

		return ++result;
	}

	template <typename InputIterator, typename OutputIterator, typename Compare>
	OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result, Compare comp)
	{
		return __unique_copy(first, last, result, comp, stupid::iterator_category(result));
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator unique(InputIterator first, InputIterator last, OutputIterator result)
	{
		first = stupid::adjcacent_find(first, last);
		return unique_copy(first, last, reuslt);
	}

	template <typename InputIterator, typename OutputIterator, typename Compare>
	OutputIterator unique(InputIterator first, InputIterator last, OutputIterator result, Compare comp)
	{
		first = stupid::adjcacent_find(first, last, comp);
		return unique_copy(first, last, comp);
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else
			{
				if (*first2 < *first1)
				{
					*result = *first2;
					++first2;
					++result;
				}
				else
				{
					*result = *first1;
					++first1;
					++first2;
					++result;
				}
			}
		}

		return stupid::copy(first1, last1, stupid::copy(first2, last2, result));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (comp(*first1, *first2))
			{
				*result = *first1;
				++first1;
				++result;
			}
			else
			{
				if (comp(*first2, *first1))
				{
					*result = *first2;
					++first2;
					++result;
				}
				else
				{
					*result = *first1;
					++first1;
					++first2;
					++result;
				}
			}
		}

		return stupid::copy(first1, last1, stupid::copy(first2, last2, result));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1 &&first2 != last2)
		{
			if (*first1 < *first2)
				++first1;
			else
			{
				if (*first2 < *first1)
					++first2;
				else
				{
					*result = *first1;
					++first1;
					++first2;
					++result;
				}
			}
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp)
	{
		while (first1 != last1 &&first2 != last2)
		{
			if (comp(*first1, *first2))
				++first1;
			else
			{
				if (comp(*first2, *first1))
					++first2;
				else
				{
					*result = *first1;
					++first1;
					++first2;
					++result;
				}
			}
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else
			{
				if (*first2 < *first1)
					++first2;
				else
				{
					++first1;
					++first2;
				}
			}
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (comp(*first1, *first2))
			{
				*result = *first1;
				++first1;
				++result;
			}
			else
			{
				if (comp(*first2, *first1))
					++first2;
				else
				{
					++first1;
					++first2;
				}
			}
		}

		return result;
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++result;
				++first1;
			}
			else
			{
				if (*first2 < *first1)
				{
					*result = *first2;
					++result;
					++first2;
				}
				else
				{
					++first1;
					++first2;
				}
			}
		}

		return stupid::copy(first1, last1, stupid::copy(first2, last2, result));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (comp(*first1, *first2))
			{
				*result = *first1;
				++result;
				++first1;
			}
			else
			{
				if (comp(*first2, *first1))
				{
					*result = *first2;
					++result;
					++first2;
				}
				else
				{
					++first1;
					++first2;
				}
			}
		}

		return stupid::copy(first1, last1, stupid::copy(first2, last2, result));
	}

	template <typename ForwardIterator, typename Type>
	bool binary_search(ForwardIterator first, ForwardIterator last, const Type &value)
	{
		ForwardIterator result = stupid::lower_bound(first, last, value);
		return result != last && !(value < *result);
	}

	template <typename ForwardIterator, typename Type, typename Compare>
	bool binary_search(ForwardIterator first, ForwardIterator last, const Type &value, Compare comp)
	{
		ForwardIterator result = stupid::lower_bound(first, last, value, comp);
		return result != last &&!(value < *result);
	}

	template <typename BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last)
			return false;
		BidirectionalIterator prev = first;
		++prev;
		if (prev == last)
			return false;
		BidirectionalIterator next=last;
		--next;
		prev = next;
		--prev;
		while (true)
		{
			if (*prev < *next)
			{
				BidirectionalIterator tmp = last;
				while (!(*prev < *(--tmp)));
				stupid::iter_swap(prev, tmp);
				stupid::reverse(next, last);
				return true;
			}
			if (prev == first)
			{
				stupid::reverse(first, last);
				return false;
			}
			next = prev;
			--prev;
		}
	}

	template <typename BidirectionalIterator, typename Compare>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
	{
		if (first == last)
			return false;
		BidirectionalIterator prev = first;
		++prev;
		if (prev == last)
			return false;
		BidirectionalIterator next = last;
		--next;
		prev = next;
		--prev;
		while (true)
		{
			if (comp(*prev, *next))
			{
				BidirectionalIterator tmp = last;
				while (!(comp(*prev, *(--tmp))));
				stupid::iter_swap(prev, tmp);
				stupid::reverse(next, last);
				return true;
			}
			if (prev == first)
			{
				stupid::reverse(first, last);
				return false;
			}
			next = prev;
			--prev;
		}
	}

	template <typename BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last)
			return false;
		BidirectionalIterator prev = first;
		++prev;
		if (prev == last)
			return false;
		BidirectionalIterator next = last;
		--next;
		prev = next;
		--prev;

		while (true)
		{
			if (*next < *prev)
			{
				BidirectionalIterator tmp = last;
				while (!(*(--tmp) < *prev));
				stupid::iter_swap(tmp, prev);
				stupid::reverse(next, last);
				return true;
			}
			if (prev == first)
			{
				stupid::reverse(first, last);
				return false;
			}

			next = prev;
			--prev;
		}
	}

	template <typename BidirectionalIterator, typename Compare>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
	{
		if (first == last)
			return false;
		BidirectionalIterator prev = first;
		++prev;
		if (prev == last)
			return false;
		BidirectionalIterator next = last;
		--next;
		prev = next;
		--prev;

		while (true)
		{
			if (comp(*next, *prev))
			{
				BidirectionalIterator tmp = last;
				while (!(comp(*(--tmp), *prev)));
				stupid::iter_swap(tmp, prev);
				stupid::reverse(next, last);
				return true;
			}
			if (prev == first)
			{
				stupid::reverse(first, last);
				return false;
			}

			next = prev;
			--prev;
		}
	}

	template <typename RandomAccessIterator, typename Distance>
	void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance *)
	{
		if (first == last)
			return;
		std::srand(unsigned(time(0)));
		for (RandomAccessIterator i = first + 1; i < last; ++i)
			iter_swap(i, first + Distance(std::rand() % ((i - first) + 1)));
	}

	template <typename RandomAccessIterator>
	inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last)
	{
		return __random_shuffle(first, last, distance_type(first));
	}

	template <typename RandomAccessIterator, typename RandomNumberGenerator>
	void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator &rand)
	{
		if (first == last)
			return;
		for (RandomAccessIterator i = first + 1; i < last; ++i)
			iter_swap(i, first + rand() % ((i - first) + 1));
	}

	template <typename RandomAccessIterator, typename Type>
	void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Type *)
	{
		stupid::make_heap(first, middle);
		for (RandomAccessIterator i = middle; i < last; ++i)
			if (*i < *first)
				stupid::__pop_heap(first, middle, i, Type(*i), stupid::distance_type(first));
		stupid::sort_heap(first, middle);
	}

	template <typename RandomAccessIterator>
	inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
	{
		__partial_sort(first, middle, last, stupid::value_type(first));
	}

	template <typename RandomAccessIterator, typename Type, typename Compare>
	void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp,Type *)
	{
		stupid::make_heap(first, middle, comp);
		for (RandomAccessIterator i = middle; i < last; ++i)
			if (comp(*i, *first))
				stupid::__pop_heap(first, middle, i, Type(*i), stupid::distance_type(first), comp);
		stupid::sort_heap(first, middle, comp);
	}

	template <typename RandomAccessIterator, typename Compare>
	inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp)
	{
		__partial_sort(first, middle, last, stupid::value_type(first), comp);
	}

	template <typename InputIterator, typename RandomAccessIterator, typename Distance, typename Type>
	RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Distance *, Type *)
	{
		if (result_first == result_last)
			return result_last;
		RandomAccessIterator result_real_last = result_first;
		while (first != last&&result_real_last != result_last)
		{
			*result_real_last = *first;
			++result_real_last;
			++first;
		}

		stupid::make_heap(result_first, result_real_last);
		for (; first != last; ++first)
			if (*first < *result_first)
				stupid::__adjust_heap(result_first, Distance(0), Distance(result_real_last - result_first), Type(*first));
		sort_heap(result_first, result_real_last);
		return result_real_last;
	}

	template <typename InputIterator, typename RandomAccessIterator>
	inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last)
	{
		return stupid::__partial_sort_copy(first, last, result_first, result_last, stupid::distance_type(result_first), stupid::value_type(result_first));
	}


	template <typename InputIterator, typename RandomAccessIterator, typename Compare, typename Distance, typename Type>
	RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp, Distance *, Type *)
	{
		if (result_first == result_last)
			return result_last;
		RandomAccessIterator result_real_last = result_first;
		while (first != last&&result_real_last != result_last)
		{
			*result_real_last = *first;
			++result_real_last;
			++first;
		}

		stupid::make_heap(result_first, result_real_last, comp);
		for (; first != last; ++first)
			if (comp(*first, *result_first))
				stupid::__adjust_heap(result_first, Distance(0), Distance(result_real_last - result_first), Type(*first), comp);
		sort_heap(result_first, result_real_last, comp);
		return result_real_last;
	}

	template <typename InputIterator, typename RandomAccessIterator, typename Compare>
	inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp)
	{
		return stupid::__partial_sort_copy(first, last, result_first, result_last, comp, stupid::distance_type(result_first), stupid::value_type(result_first));
	}

	//for sort algorithm

	const int __stl_threshold = 16;

	template <typename RandomAccessIterator, typename Type>
	void __unguarded_linear_insert(RandomAccessIterator last, Type value)
	{
		RandomAccessIterator next = last;
		--next;
		while (value < *next)
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}

	template <typename RandomAccessIterator, typename Type>
	void __linear_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Type *)
	{
		Type value = *last;
		if (value < *first)
		{
			stupid::copy_backward(first, last, last + 1);
			*first = value;
		}
		else
			__unguarded_linear_insert(last, value);
	}

	template <typename RandomAccessIterator>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first == last)
			return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
			stupid::__linear_insertion_sort(first, i, stupid::value_type(first));
	}

	template <typename Type>
	inline const Type &__median(const Type &a, const Type &b, const Type &c)
	{
		if (a < b)
		{
			if (b < c)
				return b;
			else
			{
				if (a < c)
					return c;
				else
					return a;
			}
		}
		else
		{
			if (b < c)
			{
				if (a < c)
					return a;
				else
					return c;
			}
			else
				return b;
		}
	}

	template <typename RandomAccessIterator, typename Type>
	RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, Type pivot)
	{
		while (true)
		{
			while (*first < pivot)
				++first;
			--last;
			while (pivot < *last)
				--last;
			if (first < last)
			{
				stupid::iter_swap(first, last);
				++first;
			}
			else
				return first;
		}
	}

	template <typename Size>
	inline Size __lg(Size n)
	{
		Size k = 0;
		while (n > 1)
		{
			++k;
			n >>= 1;
		}

		return k;
	}

	template <typename RandomAccessIterator, typename Size>
	void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, Size depth)
	{
		typedef typename stupid::iterator_traits<RandomAccessIterator>::difference_type Distance;
		typedef typename stupid::iterator_traits<RandomAccessIterator>::value_type Type;

		Distance len = last - first;
		while (len > stupid::__stl_threshold)
		{
			if (depth == 0)
			{
				partial_sort(first, last, last);
				return;
			}
			else
			{
				Distance half = len >> 1;
				const Type &pivot = ::stupid::__median(*first, *(first + half), *(last - 1));
				RandomAccessIterator middle = stupid::__unguarded_partition(first, last, pivot);
				__introsort_loop(middle, last, --depth);
				last = middle;
				len = last - first;
			}
		}
	}

	template <typename RandomAccessIterator, typename Type>
	void __unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, Type *)
	{
		for (RandomAccessIterator i = first; i != last; ++i)
			stupid::__unguarded_linear_insert(i, Type(*i));
	}

	template <typename RandomAccessIterator>
	inline void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		stupid::__unguarded_insertion_sort_aux(first, last, stupid::value_type(first));
	}

	template <typename RandomAccessIterator>
	void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if ((last - first) > stupid::__stl_threshold)
		{
			stupid::__insertion_sort(first, first + __stl_threshold);
			stupid::__unguarded_insertion_sort(first + __stl_threshold, last);
		}
		else
			__insertion_sort(first, last);
	}

	template <typename RandomAccessIterator>
	inline void sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first != last)
		{
			stupid::__introsort_loop(first, last, stupid::__lg((last - first) * 2));
			stupid::__final_insertion_sort(first, last);
		}
	}

	template <typename RandomAccessIterator, typename Type, typename Compare>
	void __unguarded_linear_insert(RandomAccessIterator last, Type value, Compare comp)
	{
		RandomAccessIterator next = last;
		--next;
		while (comp(value, *next))
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}

	template <typename RandomAccessIterator, typename Compare, typename Type>
	void __linear_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Type *)
	{
		Type value = *last;
		if (comp(value < *first))
		{
			stupid::copy_backward(first, last, last + 1);
			*first = value;
		}
		else
			__unguarded_linear_insert(last, value, comp);
	}

	template <typename RandomAccessIterator, typename Compare>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		if (first == last)
			return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
			stupid::__linear_insertion_sort(first, i, comp, stupid::value_type(first));
	}

	template <typename Type, typename Compare>
	inline const Type &__median(const Type &a, const Type &b, const Type &c, Compare comp)
	{
		if (comp(a, b))
		{
			if (comp(b, c))
				return b;
			else
			{
				if (comp(a, c))
					return c;
				else
					return a;
			}
		}
		else
		{
			if (comp(b, c))
			{
				if (comp(a, c))
					return a;
				else
					return c;
			}
			else
				return b;
		}
	}

	template <typename RandomAccessIterator,  typename Type, typename Compare>
	RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, Type pivot, Compare comp)
	{
		while (true)
		{
			while (comp(*first, pivot))
				++first;
			--last;
			while (comp(pivot, *last))
				--last;
			if (first < last)
			{
				stupid::iter_swap(first, last);
				++first;
			}
			else
				return first;
		}
	}

	template <typename RandomAccessIterator, typename Size, typename Compare>
	void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, Size depth, Compare comp)
	{
		typedef typename stupid::iterator_traits<RandomAccessIterator>::difference_type Distance;
		typedef typename stupid::iterator_traits<RandomAccessIterator>::value_type Type;

		Distance len = last - first;
		while (len > stupid::__stl_threshold)
		{
			if (depth == 0)
			{
				partial_sort(first, last, last, comp);
				return;
			}
			else
			{
				Distance half = len >> 1;
				const Type &pivot = ::stupid::__median(*first, *(first + half), *(last - 1), comp);
				RandomAccessIterator middle = stupid::__unguarded_partition(first, last, pivot, comp);
				__introsort_loop(middle, last, --depth, comp);
				last = middle;
				len = last - first;
			}
		}
	}

	template <typename RandomAccessIterator, typename Compare, typename Type>
	void __unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Type *)
	{
		for (RandomAccessIterator i = first; i != last; ++i)
			stupid::__unguarded_linear_insert(i, Type(*i), comp);
	}

	template <typename RandomAccessIterator, typename Compare>
	inline void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		stupid::__unguarded_insertion_sort_aux(first, last, comp, stupid::value_type(first));
	}

	template <typename RandomAccessIterator, typename Compare>
	void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		if ((last - first) > stupid::__stl_threshold)
		{
			stupid::__insertion_sort(first, first + threshold, comp);
			stupid::__unguarded_insertion_sort(first + threshold, last, comp);
		}
		else
			__insertion_sort(first, last, comp);
	}

	template <typename RandomAccessIterator, typename Compare>
	inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		if (first != last)
		{
			stupid::__introsort_loop(first, last, stupid::__lg((last - first) * 2), comp);
			stupid::__final_insertion_sort(first, last, comp);
		}
	}

	template <typename ForwardIterator, typename Type, typename Distance>
	std::pair<ForwardIterator, ForwardIterator> __equal_range(ForwardIterator first, ForwardIterator last, const Type &value, Distance *, stupid::forward_iterator_tag)
	{
		Distance len = stupid::distance(first, last);
		Distance half;
		ForwardIterator middle;
		ForwardIterator left, right;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			stupid::advance(middle, half);
			if (*middle < value)
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
			else
			{
				if (value < *middle)
					len = half;
				else
				{
					left = stupid::lower_bound(first, middle, value);
					advance(first, len);
					right = stupid::upper_bound(++middle, first, value);
					return std::make_pair(left, right);
				}
			}
		}

		return std::make_pair(first, first);
	}

	template <typename RandomAccessIterator, typename Type, typename Distance>
	std::pair<RandomAccessIterator, RandomAccessIterator> __equal_range(RandomAccessIterator first, RandomAccessIterator last, const Type &value, Distance *, stupid::random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;
		RandomAccessIterator left, right;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
			if (*middle < value)
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
			{
				if (value < *middle)
					len = half;
				else
				{
					left = stupid::lower_bound(first, middle, value);
					right = stupid::upper_bound(middle + 1, first + len, value);
					return std::make_pair(left, right);
				}
			}
		}

		return std::make_pair(first, first);
	}

	template <typename ForwardIterator, typename Type>
	inline std::pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const Type &value)
	{
		return stupid::__equal_range(first, last, value, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename ForwardIterator, typename Type, typename Compare, typename Distance>
	std::pair<ForwardIterator, ForwardIterator> __equal_range(ForwardIterator first, ForwardIterator last, const Type &value, Compare comp, Distance *, stupid::forward_iterator_tag)
	{
		Distance len = stupid::distance(first, last);
		Distance half;
		ForwardIterator middle;
		ForwardIterator left, right;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			stupid::advance(middle, half);
			if (comp(*middle, value))
			{
				first = middle;
				++first;
				len = len - half - 1;
			}
			else
			{
				if (comp(value, *middle))
					len = half;
				else
				{
					left = stupid::lower_bound(first, middle, value, comp);
					advance(first, len);
					right = stupid::upper_bound(++middle, first, value, comp);
					return std::make_pair(left, right);
				}
			}
		}

		return std::make_pair(first, first);
	}

	template <typename RandomAccessIterator, typename Type, typename Compare, typename Distance>
	std::pair<RandomAccessIterator, RandomAccessIterator> __equal_range(RandomAccessIterator first, RandomAccessIterator last, const Type &value, Compare comp, Distance *, stupid::random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;
		RandomAccessIterator left, right;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
			if (comp(*middle, value))
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
			{
				if (comp(value, *middle))
					len = half;
				else
				{
					left = stupid::lower_bound(first, middle, value, comp);
					right = stupid::upper_bound(middle + 1, first + len, value, comp);
					return std::make_pair(left, right);
				}
			}
		}

		return std::make_pair(first, first);
	}

	template <typename ForwardIterator, typename Type, typename Compare>
	inline std::pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const Type &value, Compare comp)
	{
		return stupid::__equal_range(first, last, value, comp, stupid::distance_type(first), stupid::iterator_category(first));
	}

	template <typename BidirectionalIterator, typename Buffer, typename Distance>
	BidirectionalIterator __rotate_adapitive(BidirectionalIterator first,
		BidirectionalIterator middle,
		BidirectionalIterator last,
		Distance len1, Distance len2,
		Buffer buf, Distance buf_size)
	{
		Buffer buf_end;
		if (len2 <= len2&&len2 <= buf_size)
		{
			buf_end = stupid::copy(middle, last, buf);
			stupid::copy_backward(first, middle, last);
			return stupid::copy(buf, buf_end, first);
		}
		else
		{
			if (len1 <= len2&&len1 <= buf_size)
			{
				buf_end = stupid::copy(first, middle, buf);
				stupid::copy(middle, last, first);
				return stupid::copy_backward(buf, buf_end, last);
			}
			else
			{
				stupid::rotate(first, middle, last);
				advance(first, len2);
				return first;
			}
		}
	}

	template <typename BidirectionalIterator1, typename BidirectionalIterator2, typename BidirectionalIterator3>
	BidirectionalIterator3 __merge_backward(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
		BidirectionalIterator2 first2, BidirectionalIterator2 last2,
		BidirectionalIterator3 result)
	{
		if (first1 == last1)
			return stupid::copy_backward(first2, last2, result);
		if (first2 == last2)
			return stupid::copy_backward(first1, last1, result);

		--last1;
		--last2;
		while (true)
		{
			if (*last1 < *last2)
			{
				*(--result) = *last2;
				if (first2 == last2)
					return stupid::copy_backward(first1, last1, result);
				--last2;
			}
			else
			{
				*(--result) = *last1;
				if (first1 == last1)
					return stupid::copy_backward(first2, last2, result);
				--last1;
			}
		}
	}

	template <typename BidirectionalIterator, typename Distance, typename Buffer>
	void __merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance len1, Distance len2, Buffer buf, Distance buf_size)
	{
		if (len1 <= len2&&len1 <= buf_size)
		{
			Buffer buf_end = stupid::copy(first, middle, buf);
			stupid::merge(middle, last, buf, buf_end, first);
		}
		else
		{
			if (len2 <= len1&&len2 <= buf_size)
			{
				Buffer buf_end = stupid::copy(middle, last, buf);
				stupid::__merge_backward(first, middle, buf, buf_end, last);
			}
			else
			{
				BidirectionalIterator first_cut=first;
				BidirectionalIterator second_cut = middle;

				Distance new_len1;
				Distance new_len2;

				if (len1 < len2)
				{
					new_len2 = len2 >> 1;
					stupid::advance(second_cut, new_len2);
					first_cut = stupid::upper_bound(first, middle, *second_cut);
					new_len1 = stupid::distance(first, first_cut);
				}
				else
				{
					new_len1 = len1 >> 1;
					stupid::advance(first_cut, new_len1);
					second_cut = stupid::lower_bound(middle, last, *first_cut);
					new_len2 = stupid::distance(middle, second_cut);
				}

				BidirectionalIterator new_middle = stupid::__rotate_adapitive(first_cut, middle, second_cut,
					len1 - new_len1, new_len2, buf, buf_size);
				
				stupid::__merge_adaptive(first, first_cut, new_middle, new_len1, new_len2, buf, buf_size);
				stupid::__merge_adaptive(new_middle, second_cut, last, len1 - new_len1, len2 - new_len2, buf, buf_size);
			}
		}
	}

	template <typename BidirectionalIterator, typename Distance>
	void __merge_without_buf(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance len1, Distance len2)
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2)
		{
			if (*middle < *first)
				stupid::iter_swap(middle, first);
			return;
		}

		BidirectionalIterator first_cut = first;
		BidirectionalIterator second_cut = middle;

		Distance new_len1;
		Distance new_len2;

		if (len1 < len2)
		{
			new_len2 = len2 >> 1;
			stupid::advance(second_cut, new_len2);
			first_cut = stupid::upper_bound(first, middle, *second_cut);
			new_len1 = stupid::distance(first, first_cut);
		}
		else
		{
			new_len1 = len1 >> 1;
			stupid::advance(first_cut, new_len1);
			second_cut = stupid::upper_bound(middle, last, *first_cut);
			new_len2 = stupid::distance(middle, second_cut);
		}

		BidirectionalIterator new_middle = stupid::rotate(first_cut, middle, second_cut);
		stupid::__merge_without_buf(first, first_cut, new_middle, new_len1, new_len2);
		stupid::__merge_without_buf(new_middle, second_cut, last, len1 - new_len1, len2 - new_len2);

	}

	template <typename BidirectionalIterator, typename Type, typename Distance>
	inline void __inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
		Type *, Distance *)
	{
		Distance len1 = stupid::distance(first, middle);
		Distance len2 = stupid::distance(middle, last);

		stupid::tempbuf<BidirectionalIterator, Type> buf(first, last);
		if (buf.size() == 0)
			stupid::__merge_without_buf(first, middle, last, len1, len2);
		else
			stupid::__merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()));
	}

	template <typename BidirectionalIterator>
	inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
		stupid::__inplace_merge_aux(first, middle, last, stupid::value_type(first), stupid::distance_type(first));
	}

	template <typename BidirectionalIterator1, typename BidirectionalIterator2, typename BidirectionalIterator3, typename Compare>
	BidirectionalIterator3 __merge_backward(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
		BidirectionalIterator2 first2, BidirectionalIterator2 last2,
		BidirectionalIterator3 result, Compare comp)
	{
		if (first1 == last1)
			return stupid::copy_backward(first2, last2, result);
		if (first2 == last2)
			return stupid::copy_backward(first1, last1, result);

		--last1;
		--last2;
		while (true)
		{
			if (comp(*last1, *last2))
			{
				*(--result) = *last2;
				if (first2 == last2)
					return stupid::copy_backward(first1, last1, result);
				--last2;
			}
			else
			{
				*(--result) = *last1;
				if (first1 == last1)
					return stupid::copy_backward(first2, last2, result);
				--last1;
			}
		}
	}

	template <typename BidirectionalIterator, typename Distance, typename Buffer, typename Compare>
	void __merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, 
		Distance len1, Distance len2, Buffer buf, Distance buf_size, Compare comp)
	{
		if (len1 <= len2&&len1 <= buf_size)
		{
			Buffer buf_end = stupid::copy(first, middle, buf);
			stupid::merge(middle, last, buf, buf_end, first, comp);
		}
		else
		{
			if (len2 <= len1&&len2 <= buf_size)
			{
				Buffer buf_end = stupid::copy(middle, last, buf);
				stupid::__merge_backward(first, middle, buf, buf_end, last, comp);
			}
			else
			{
				BidirectionalIterator first_cut = first;
				BidirectionalIterator second_cut = middle;

				Distance new_len1;
				Distance new_len2;

				if (len1 < len2)
				{
					new_len2 = len2 >> 1;
					stupid::advance(second_cut, new_len2);
					first_cut = stupid::upper_bound(first, middle, *second_cut, comp);
					new_len1 = stupid::distance(first, first_cut);
				}
				else
				{
					new_len1 = len1 >> 1;
					stupid::advance(first_cut, new_len1);
					second_cut = stupid::lower_bound(middle, last, *first_cut, comp);
					new_len2 = stupid::distance(middle, second_cut);
				}

				BidirectionalIterator new_middle = stupid::__rotate_adapitive(first_cut, middle, second_cut,
					len1 - new_len1, new_len2, buf, buf_size);

				stupid::__merge_adaptive(first, first_cut, new_middle, new_len1, new_len2, buf, buf_size, comp);
				stupid::__merge_adaptive(new_middle, second_cut, last, len1 - new_len1, len2 - new_len2, buf, buf_size, comp);
			}
		}
	}

	template <typename BidirectionalIterator, typename Distance, typename Compare>
	void __merge_without_buf(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
		Distance len1, Distance len2, Compare comp)
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2)
		{
			if (*middle < *first)
				stupid::iter_swap(middle, first);
			return;
		}

		BidirectionalIterator first_cut = first;
		BidirectionalIterator second_cut = middle;

		Distance new_len1;
		Distance new_len2;

		if (len1 < len2)
		{
			new_len2 = len2 >> 1;
			stupid::advance(second_cut, new_len2);
			first_cut = stupid::upper_bound(first, middle, *second_cut, comp);
			new_len1 = stupid::distance(first, first_cut);
		}
		else
		{
			new_len1 = len1 >> 1;
			stupid::advance(first_cut, new_len1);
			second_cut = stupid::upper_bound(middle, last, *first_cut, comp);
			new_len2 = stupid::distance(middle, second_cut);
		}

		BidirectionalIterator new_middle = stupid::rotate(first_cut, middle, second_cut);
		stupid::__merge_without_buf(first, first_cut, new_middle, new_len1, new_len2, comp);
		stupid::__merge_without_buf(new_middle, second_cut, last, len1 - new_len1, len2 - new_len2, comp);

	}

	template <typename BidirectionalIterator, typename Type, typename Distance, typename Compare>
	inline void __inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
		Type *, Distance *, Compare comp)
	{
		Distance len1 = stupid::distance(first, middle);
		Distance len2 = stupid::distance(middle, last);

		stupid::tempbuf<BidirectionalIterator, Type> buf(first, last);
		if (buf.size() == 0)
			stupid::__merge_without_buf(first, middle, last, len1, len2, comp);
		else
			stupid::__merge_adaptive(first, middle, lasst, len1, len2, buf.begin(), Distance(buf.size()), comp);
	}

	template <typename BidirectionalIterator, typename Compare>
	inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Compare comp)
	{
		stupid::__inplace_merge_aux(first, middle, last, stupid::value_type(first), stupid::distance_type(first), comp);
	}

	template <typename RandomAccessIterator, typename Type>
	void __nth_element_aux(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Type *)
	{
		while (last - first > 3)
		{
			const Type &pivot = stupid::__median(*first, *(first + ((last - first) >> 1)), *(last - 1));
			RandomAccessIterator middle = stupid::__unguarded_partition(first, last, pivot);
			if (middle < nth)
				first = middle;
			else
				last = middle;
		}

		__insertion_sort(first, last);
	}

	template <typename RandomAccessIterator>
	inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
	{
		stupid::__nth_element_aux(first, nth, last, stupid::value_type(first));
	}

	template <typename RandomAccessIterator, typename Compare, typename Type>
	void __nth_element_aux(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare comp, Type *)
	{
		while (last - first > 3)
		{
			const Type &pivot = stupid::__median(*first, *(first + ((last - first) >> 1)), *(last - 1), comp);
			RandomAccessIterator middle = stupid::__unguarded_partition(first, last, pivot, comp);
			if (middle < nth)
				first = middle;
			else
				last = middle;
		}

		__insertion_sort(first, last, comp);
	}

	template <typename RandomAccessIterator, typename Compare>
	inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare comp)
	{
		stupid::__nth_element_aux(first, nth, last, comp, stupid::value_type(first));
	}

	template <typename BidirectionalIterator>
	void mergesort(BidirectionalIterator first, BidirectionalIterator last)
	{
		typename stupid::iterator_traits<BidirectionalIterator>::difference_type dis = stupid::distance(first, last);
		if (dis <= 1)
			return;
		else
		{
			BidirectionalIterator middle = first;
			stupid::advance(middle, dis >> 1);
			mergesort(first, middle);
			mergesort(middle, last);
			inplace_merge(first, middle, last);
		}
	}
}

#endif