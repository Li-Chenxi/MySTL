/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
#include <cstddef>	//for ptrdiff_t

namespace stupid
{
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag :public input_iterator_tag{};
	struct bidirectional_iterator_tag :public forward_iterator_tag{};
	struct random_access_iterator_tag :public bidirectional_iterator_tag{};

	template <typename Category,
		typename Type,
		typename Distance = ptrdiff_t,
		typename Pointer = Type*,
		typename Reference = Type&>
	struct iterator
	{
		typedef Category  iterator_category;
		typedef Type	  value_type;
		typedef Distance  difference_type;
		typedef Pointer   pointer;
		typedef Reference reference;
	};

	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type		 value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer			 pointer;
		typedef typename Iterator::reference		 reference;
	};

	template <typename T>
	struct iterator_traits < T * >
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T						   value_type;
		typedef ptrdiff_t				   difference_type;
		typedef T *						   pointer;
		typedef T &						   reference;
	};

	template <typename T>
	struct iterator_traits < const T * >
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T						   value_type;
		typedef ptrdiff_t				   difference_type;
		typedef const T *				   pointer;
		typedef const T &				   reference;
	};

	template <typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator &)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	template <typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type * 
		distance_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
	}

	template <typename Iterator>
	inline typename iterator_traits<Iterator>::value_type *
		value_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
	}


	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		__distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		iterator_traits<InputIterator>::difference_type distance = 0;
		while (first != last)
		{
			++first;
			++distance;
		}
		return distance;
	}

	template <typename RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}

	template <typename InputIterator, typename Distance>
	inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
	{
		while (n--)
			++i;
	}

	template <typename BidirectionalIterator, typename Distance>
	inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
		{
			while (n--)
				++i;
		}
		else
		{
			while (n++)
				--i;
		}
	}

	template <typename RandomAccessIterator, typename Distance>
	inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template < typename InputIterator, typename Distance>
	inline void advance(InputIterator &i, Distance n)
	{
		__advance(i, n, iterator_category(i));
	}

	template <typename Iterator>
	class reverse_iterator
	{
	protected:
		Iterator current;
	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef typename iterator_traits<Iterator>::reference reference;

		typedef Iterator iterator_type;
		typedef reverse_iterator self;

		explicit reverse_iterator(Iterator iterator)
			:current(iterator)
		{
		}

		reverse_iterator(const self &x)
			:current(x.current)
		{
		}

		iterator_type base() const
		{
			return current;
		}

		reference operator*() const
		{
			iterator_type tmp = current;
			--tmp;
			return *tmp;
		}

		pointer operator&() const
		{
			return &(operator*());
		}

		self &operator++()
		{
			--current;
			return *this;
		}

		self operator++(int)
		{
			iterator_type tmp = current;
			--current;
			return tmp;
		}

		self &operator--()
		{
			++current;
			return *this;
		}

		self operator--(int)
		{
			iterator_type tmp = current;
			++current;
			return tmp;
		}

		self &operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}

		self &operator-=(difference_type n)
		{
			current += n;
			return *this;
		}

		self operator+(difference_type n)
		{
			return self(current - n);
		}

		self operator-(difference_type n)
		{
			return self(current_type + n);
		}

		reference operator[](difference_type n)
		{
			return *(*this + n);
		}

		bool operator==(const self &x) const
		{
			return current == x.current;
		}

		bool operator<(const self &x) const
		{
			return current < x.current;
		}

		bool operator!=(const self &x) const
		{
			return current != x.current;
		}

		bool operator>(const self &x) const
		{
			return current > x.current;
		}

		bool operator<=(const self &x) const
		{
			return current <= x.current;
		}

		bool operator>=(const self &x) const
		{
			return current >= x.current;
		}
	};

	template<typename Type, typename Distance=ptrdiff_t>
	class istream_iterator
	{
	protected:
		std::istream *stream;
		Type value;
		bool end_marker;
		void read()
		{
			end_marker = (*stream) ? true : false;
			if (end_marker)
				*stream >> value;
			end_marker = (*stream) ? true : false;
		}

	public:
		typedef stupid::input_iterator_tag iterator_category;
		typedef Type value_type;
		typedef Distance difference_type;
		typedef const Type *pointer;
		typedef const Type &reference;

		istream_iterator()
		{
			stream = &std::cin;
			end_marker = false;
		}

		istream_iterator(std::istream &is)
		{
			stream = &is;
			read();
		}

		reference operator*() const
		{
			return value;
		}

		reference operator->() const
		{
			return &(operator*());
		}

		istream_iterator &operator++()
		{
			read();
			return *this;
		}

		istream_iterator operator++(int)
		{
			istream_iterator tmp = *this;
			read();
			return tmp;
		}

		bool operator==(const istream_iterator &x)
		{
			return (end_marker == x.end_marker && (end_marker == false || stream == x.stream));
		}

		bool operator!=(const istream_iterator &x)
		{
			return !(*this == x);
		}
	};

	template<typename Type, typename Distance = ptrdiff_t>
	class ostream_iterator
	{
	protected:
		std::ostream *stream;
		const char *separator;
	public:
		typedef stupid::output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

		ostream_iterator()
		{
			stream = &cout;
			separator = "";
		}

		ostream_iterator(std::ostream &os, const char *s)
		{
			stream = &os;
			separator = s;
		}

		ostream_iterator &operator=(const Type &value) const
		{
			stream << value << separator;
		}

		ostream_iterator &operator*()
		{
			return *this;
		}

		ostream_iterator &operator++()
		{
			return *this;
		}

		ostream_iterator &operator++(int)
		{
			return *this;
		}
	};

	template <typename Container>
	class back_insert_iterator
	{
	protected:
		Container *c;
	public:
		typedef stupid::output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

		back_insert_iterator()
			:c(0)
		{
		}

		explicit back_insert_iterator(Container &_c)
			:c(&_c)
		{
		}

		back_insert_iterator &operator=(const typename Container::value_type &value)
		{
			c->push_back(value);
			return *this;
		}

		back_insert_iterator &operator*() const
		{
			return *this;
		}

		back_insert_iterator &operator++()
		{
			return *this;
		}

		back_insert_iterator &operator++(int)
		{
			return *this;
		}
	};

	template <typename Container>
	stupid::back_insert_iterator<Container> back_inserter(Container &c)
	{
		return stupid::back_insert_iterator<Container>(c);
	}

	template <typename Container>
	class front_insert_iterator
	{
	protected:
		Container *c;
	public:
		typedef stupid::output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

		front_insert_iterator()
			:c(0)
		{
		}

		explicit front_insert_iterator(Container &_c)
			:c(&_c)
		{
		}

		front_insert_iterator &operator=(const typename Container::value_type &value)
		{
			c->push_front(value);
			return *this;
		}

		front_insert_iterator &operator*() const
		{
			return *this;
		}

		front_insert_iterator &operator++()
		{
			return *this;
		}

		front_insert_iterator &operator++(int)
		{
			return *this;
		}
	};

	template <typename Container>
	stupid::front_insert_iterator<Container> front_inserter(Container &c)
	{
		return stupid::front_insert_iterator<Container>(c);
	}

	template <typename Container>
	class insert_iterator
	{
	protected:
		Container *c;
		typename Container::iterator iterator;
	public:
		typedef stupid::output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

		insert_iterator(Container &_c, typename Container::iterator &ite)
			:c(&_c),
			iterator(ite)
		{
		}

		insert_iterator &operator=(const typename Container::value_type &value)
		{
			c->insert(iterator, value);
			++iterator;
			return *this;
		}

		insert_iterator &operator*() const
		{
			return *this;
		}

		insert_iterator &operator++()
		{
			return *this;
		}

		insert_iterator &operator++(int)
		{
			return *this;
		}
	};

	template <typename Container>
	stupid::insert_iterator<Container> inserter(Container &c, typename Container::iterator ite)
	{
		return stupid::insert_iterator<Container>(c, ite);
	}
}

#endif