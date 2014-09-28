#ifndef ALGO_H
#define ALGO_H

#include "MySTL_iterator.h"

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

		while (len>0)
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
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, const Compare &comp)
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

	template <typename ForwardIterator>
	ForwardIterator adjcacent_find(ForwardIterator first, ForwardIterator last)
	{
		ForwardIterator current=first;
		while (++current != last)
		{
			if (*current == *first)
				return first;
			first = current;
		}

		return last;
	}

	template <typename ForwardIterator, typename Compare>
	ForwardIterator adjcacent_find(ForwardIterator first, ForwardIterator last, const Compare &comp)
	{
		ForwardIterator current = first;
		while (++current != last)
		{
			if (comp(*current, *first))
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
		count(InputIterator first, InputIterator last, const Type &value, const Compare &comp)
	{
		typename stupid::iterator_traits<InputIterator>::difference_type result = 0;

		while (first != last)
		{
			if (comp(value == *first))
				++result;
			++first;
		}

		return result;
	}

	template <typename InputIterator, typename Predicate>
	typename stupid::iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, const Predicate &pred)
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
	InputIterator find(InputIterator first, InputIterator last, const Type &value, const Compare &comp)
	{
		while (first != last &&!(comp(*first, value)))
			++first;
		return first;
	}

	template <typename InputIterator, typename Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, const Predicate &pred)
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

		return stupid::copy(first1, last1, stupid::copy(first2, last2));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, const Compare &comp)
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

		return stupid::copy(first1, last1, stupid::copy(first2, last2));
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
		OutputIterator result, const Compare &comp)
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
		OutputIterator result, const Compare &comp)
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

		return stupid::copy(first1, last1, stupid::copy(first2, last2));
	}

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, const Compare &comp)
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

		return stupid::copy(first1, last1, stupid::copy(first2, last2));
	}
}

#endif