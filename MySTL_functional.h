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
	struct minus: public binary_function < Type, Type, Type >
	{
		Type operator()(const Type &x, const Type &y) const
		{
			return x - y;
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
	struct divides :public binary_function < Type, Type, Type >
	{
		Type operator()(const Type &x, const Type &y) const
		{
			return x / y;
		}
	};

	template <typename Type>
	struct modulus :public binary_function < Type, Type, Type >
	{
		Type operator()(const Type &x, const Type &y) const
		{
			return x%y;
		}
	};

	template <typename Type>
	struct negate :public unary_function < Type, Type >
	{
		Type operator()(const Type &x)
		{
			return -x;
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
	struct less_equal :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x <= y;
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
	struct greater_than :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x >= y;
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
	struct not_equal_to :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x != y;
		}
	};

	template <typename Type>
	struct logical_and :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x && y;
		}
	};

	template <typename Type>
	struct logical_or :public binary_function < Type, Type, bool >
	{
		bool operator()(const Type &x, const Type &y) const
		{
			return x || y;
		}
	};

	template <typename Type>
	struct logical_not :public unary_function < Type, bool >
	{
		bool operator()(const Type &x) const
		{
			return !x;
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
		typedef typename Pair::first_type first_type;
		const first_type &operator()(const Pair &x) const
		{
			return x.first;
		}
	};

	template <typename Pair>
	struct select2nd :public unary_function < Pair, typename Pair::second_type >
	{
		typedef typename Pair::second_type second_type;
		const second_type &operator()(const Pair &x) const
		{
			return x.first;
		}
	};

	template <typename Predicate>
	class unary_negate :public unary_function < typename Predicate::argument_type, bool >
	{
	protected:
		Predicate pred;
	public:
		explicit unary_negate(const Predicate &_pred)
			:pred(_pred)
		{
		}

		bool operator()(const typename Predicate::argument_type &x) const
		{
			return !pred(x);
		}
	};

	template <typename Predicate>
	inline stupid::unary_negate<Predicate> not1(const Predicate &pred)
	{
		return stupid::unary_negate<Predicate>(pred);
	}

	template <typename Predicate>
	class binary_negate :public binary_function < typename Predicate::first_argument_type, 
		typename Predicate::second_argument_type, bool >
	{
	protected:
		Predicate pred;
	public:
		explicit binary_negate(const Predicate &_pred)
			:pred(_pred)
		{
		}

		bool operator()(const typename Predicate::first_argument_type &x,
			const typename Predicate::second_argument_type &y) const
		{
			return !pred(x, y);
		}
	};

	template <typename Predicate>
	inline stupid::binary_negate<Predicate> not2(const Predicate &pred)
	{
		return stupid::binary_negate<Predicate>(pred);
	}

	template <typename Operation>
	class binder1st:public unary_function<typename Operation::second_argument_type, typename Operation::result_type>
	{
	protected:
		Operation op;
		typename Operation::first_argument_type para;
	public:
		binder1st(const Operation &_op, const typename Operation::first_argument_type _para)
			:op(_op),
			_para(_para)
		{
		}

		typename Operation::result_type operator()(const typename Operation::second_argument_type &y) const
		{
			return op(para, y);
		}
	};

	template <typename Operation, typename Type>
	inline stupid::binder1st<Operation> bind1st(const Operation *op, const Type &x)
	{
		typedef typename Operation::first_argument_type first_argument_type;
		return stupid::binder1st(op, first_argument_type(x));
	}

	template <typename Operation>
	class binder2nd :public unary_function<typename Operation::first_argument_type, typename Operation::result_type>
	{
	protected:
		Operation op;
		typename Operation::second_argument_type para;
	public:
		binder2nd(const Operation &_op, const typename Operation::second_argument_type _para)
			:op(_op),
			_para(_para)
		{
		}

		typename Operation::result_type operator()(const typename Operation::first_argument_type &x) const
		{
			return op(x, para);
		}
	};

	template <typename Operation, typename Type>
	inline stupid::binder2nd<Operation> bind2nd(const Operation *op, const Type &y)
	{
		typedef typename Operation::second_argument_type second_argument_type;
		return stupid::binder2nd(op, second_argument_type(y));
	}

	template <typename Operation1, typename Operation2>
	class unary_compose :public unary_function < typename Operation2::argument_type, typename Operation1::result_type >
	{
	protected:
		Operation1 op1;
		Operation2 op2;
	public:
		unary_compose(const Operation1 &_op1, const Operation2 &_op2)
			:op1(_op1),
			op2(_op2)
		{
		}

		typename Operation1::result_type operator()(const typename Operation2::argument_type &x) const
		{
			return op1(op2(x));
		}
	};

	template <typename Operation1, typename Operation2>
	inline stupid::unary_compose<Operation1, Operation2> compose1(const Operation1 &op1, const Operation2 &op2)
	{
		return stupid::unary_compose<Operation1, Operation2>(op1, op2);
	}

	template <typename Operation1, typename Operation2, typename Operation3>
	class binary_compose :public unary_function < typename Operation2::argument_type, typename Operation1::result_type >
	{
	protected:
		Operation1 op1;
		Operation2 op2;
		Operation3 op3;
	public:
		binary_compose(const Operation1 &_op1, const Operation2 &_op2, const Operation3 &_op3)
			:op1(_op1),
			op2(_op2),
			op3(_op3)
		{
		}

		typename Operation1::result_type operator()(const typename Operation2::argument_type &x) const
		{
			return op1(op2(x), op3(x));
		}
	};


	template <typename Operation1, typename Operation2, typename Operation3>
	inline stupid::binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1 &op1, const Operation2 &op2, const Operation3 &op3)
	{
		return stupid::binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
	}

	template <typename Arg, typename Result>
	class pointer_to_unary_function :public unary_function < Arg, Result >
	{
	protected:
		Result(*p)(Arg);
	public:
		explicit pointer_to_unary_function(Result(*_p)(Arg))
			:p(_p)
		{
		}

		Result operator()(Arg x) const
		{
			return (*p)(x);
		}
	};

	template <typename Arg, typename Result>
	inline stupid::pointer_to_unary_function<Arg, Result> ptr_fun(Result(*p)(Arg))
	{
		return stupid::pointer_to_unary_function<Arg, Result>(p);
	}
	
	template <typename Arg1, typename Arg2, typename Result>
	class pointer_to_binary_function :public binary_function < Arg1, Arg2, Result >
	{
	protected:
		Result(*p)(Arg1, Arg2);
	public:
		explicit pointer_to_binary_function(Result(*_p)(Arg1, Arg2))
			:p(_p)
		{
		}

		Result operator()(Arg1 x, Arg2 y) const
		{
			return (*p)(x, y);
		}
	};

	template <typename Arg1, typename Arg2, typename Result>
	inline stupid::pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result(*p)(Arg1, Arg2))
	{
		return stupid::pointer_to_binary_function<Arg1, Arg2, Result>(p);
	}

	template <typename S, typename T>
	class mem_fun_t :public unary_function < T*, S >
	{
	private:
		S(T::*p)();
	public:
		explicit mem_fun_t(S(T::*_p)())
			:p(_p)
		{
		}

		S operator()(T *x) const
		{
			return (x->*p)();
		}
	};

	template <typename S, typename T>
	class const_mem_fun_t :public unary_function < T*, S >
	{
	private:
		S(T::*p)() const;
	public:
		explicit const_mem_fun_t(S(T::*_p)() const)
			:p(_p)
		{
		}

		S operator()(const T *x) const
		{
			return (x->*p)();
		}
	};

	template <typename S, typename T>
	class mem_fun_ref_t :public unary_function < T, S >
	{
	private:
		S(T::*p)();
	public:
		explicit mem_fun_ref_t(S(T::*_p)())
			:p(_p)
		{
		}

		S operator()(T &x) const
		{
			return (x.(*p))();
		}
	};


	template <typename S, typename T>
	class const_mem_fun_ref_t :public unary_function < T, S >
	{
	private:
		S(T::*p)() const;
	public:
		explicit const_mem_fun_ref_t(S(T::*_p)() const)
			:p(_p)
		{
		}

		S operator()(const T &x) const
		{
			return (x.(*p))();
		}
	};

	template <typename S, typename T, typename A>
	class mem_fun1_t :public binary_function < T*, A, S >
	{
	private:
		S(T::*p)(A);
	public:
		explicit mem_fun1_t(S(T::*_p)(A))
			:p(_p)
		{
		}

		S operator()(T *x, A y) const
		{
			return (x->*p)(y);
		}
	};

	template <typename S, typename T, typename A>
	class const_mem_fun1_t :public binary_function < T*, A, S >
	{
	private:
		S(T::*p)(A) const;
	public:
		explicit const_mem_fun1_t(S(T::*_p)(A) const)
			:p(_p)
		{
		}

		S operator()(const T *x, A y) const
		{
			return (x->*p)(y);
		}
	};

	template <typename S, typename T, typename A>
	class mem_fun1_ref_t :public binary_function < T, A, S>
	{
	private:
		S(T::*p)(A);
	public:
		explicit mem_fun1_ref_t(S(T::*_p)(A))
			:p(_p)
		{
		}

		S operator()(T &x, A y) const
		{
			return (x.(*p))(y);
		}
	};


	template <typename S, typename T, typename A>
	class const_mem_fun1_ref_t :public binary_function < T, A, S >
	{
	private:
		S(T::*p)(A) const;
	public:
		explicit const_mem_fun1_ref_t(S(T::*_p)(A) const)
			:p(_p)
		{
		}

		S operator()(const T &x ,A y) const
		{
			return (x.(*p))(y);
		}
	};

	template <typename S, typename T>
	inline stupid::mem_fun_t<S, T> mem_fun(S(T::*p)())
	{
		return stupid::mem_fun_t<S, T>(p);
	}

	template <typename S, typename T>
	inline stupid::const_mem_fun_t<S, T> mem_fun(S(T::*p)() const)
	{
		return stupid::const_mem_fun_t<S, T>(p);
	}

	template <typename S, typename T>
	inline stupid::mem_fun_ref_t<S, T> mem_fun_ref(S(T::*p)())
	{
		return stupid::mem_fun_ref_t<S, T>(p);
	}

	template <typename S, typename T>
	inline stupid::const_mem_fun_ref_t<S, T> mem_fun_ref(S(T::*p)() const)
	{
		return stupid::const_mem_fun_ref_t<S, T>(p);
	}

	template <typename S, typename T, typename A>
	inline stupid::mem_fun1_t<S, T, A> mem_fun(S(T::*p)(A))
	{
		return stupid::mem_fun1_t<S, T, A>(p);
	}

	template <typename S, typename T, typename A>
	inline stupid::const_mem_fun1_t<S, T, A> mem_fun(S(T::*p)(A) const)
	{
		return stupid::const_mem_fun1_t<S, T, A>(p);
	}

	template <typename S, typename T, typename A>
	inline stupid::mem_fun1_ref_t<S, T, A> mem_fun_ref(S(T::*p)(A))
	{
		return stupid::mem_fun1_ref_t<S, T, A>(p);
	}

	template <typename S, typename T, typename A>
	inline stupid::const_mem_fun1_ref_t<S, T, A> mem_fun_ref(S(T::*p)(A) const)
	{
		return stupid::const_mem_fun1_ref_t<S, T, A>(p);
	}
}
#endif