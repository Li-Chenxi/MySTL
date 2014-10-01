/*
	tempbuf is NOT included in standard STL, it is a extension of STL.
*/

#ifndef TEMPBUF_H
#define TEMPBUF_H

#include "MySTL_iterator.h"
#include "MySTL_uninitialized.h"
#include <cstdlib>
#include <climits>

namespace stupid
{
	template <typename ForwardIterator, typename Type=typename stupid::iterator_traits<ForwardIterator>::value_type>
	class tempbuf
	{
	private:
		typedef typename stupid::iterator_traits<ForwardIterator>::difference_type difference_type;

		Type *buf;
		difference_type len;
		difference_type origin_len;

		void allocate_buffer()
		{
			origin_len = len;

			if (len > INT_MAX / (sizeof(Type)))
				len = INT_MAX / (sizeof(Type));

			while (len > 0)
			{
				buf = (Type *)malloc(len*sizeof(Type));
				if (0 != buf)
					break;
				len >>= 1;
			}
		}

		void initialize_buf(const Type &, stupid::__true_type)
		{
		}

		void initialize_buf(const Type &value, stupid::__false_type)
		{
			stupid::uninitialized_fill_n(buf, buf + len, value);
		}
		
	public:
		tempbuf()
		{
			buf = (Type *)0;
			len = 0;
		}

		tempbuf(ForwardIterator first, ForwardIterator last)
		{
			typedef typename stupid::__type_traits<Type>::is_POD_type is_POD_type;

			try
			{
				len = stupid::distance(first, last);
				allocate_buffer();
				if (len > 0)
					initialize_buf(*first, is_POD_type());
			}
			catch (...)
			{
				std::free(buf);
				buf = (Type *)0;
				len = 0;
			}
		}

		difference_type size() const
		{
			return len;
		}

		difference_type requested_size() const
		{
			return origin_len;
		}

		Type *begin()
		{
			return buf;
		}

		Type *end()
		{
			return buf + len;
		}
		
		~tempbuf()
		{
			if (buf)
				free(buf);
		}
	private:
		tempbuf(const tempbuf &);
		tempbuf &operator=(const tempbuf &);
	};
}

#endif