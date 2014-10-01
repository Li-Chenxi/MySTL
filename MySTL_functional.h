#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <utility>

namespace stupid
{
	template <typename Arg, typename Result>
	struct unary_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

	template <typename Arg1, typename Arg2, typename Result>
	struct binary_function
	{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	template<typename Type>
	struct plus :public binary_function < Type, Type, Type >
	{
		Type operator()(const Type &x, const Type &y) const
		{
			return x + y;
		}
	};

	template <typename Type>
	struct multiplies :public binary_function < Type, Type, Type >
	{
		Type operator()(const Type &x, const Type &y) const
		{
			return x*y;
		}
	};

	template <typename Type>
	struct less :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x < y;
		}
	};

	template <typename Type>
	struct greater :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x > y;
		}
	};

	template <typename Type>
	struct equal_to :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x == y;
		}
	};

	template <typename Type>
	Type identity_element(plus<Type>)
	{
		return Type(0);
	}

	template <typename Type>
	Type identity_element(multiplies<Type>)
	{
		return Type(1);
	}

	template <typename Type>
	struct identity :public unary_function < Type, Type >
	{
		const Type &operator()(const Type &x) const
		{
			return x;
		}
	};

	template <typename Pair>
	struct select1st :public unary_function < Pair, typename Pair::first_type >
	{
		const typename Pair::first_type &operator()(const Pair &x) const
		{
			return x.first;
		}
	};
}
#endif