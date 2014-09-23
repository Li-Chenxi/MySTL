#ifndef SLIST_H
#define SLIST_H

#include "MySTL_iterator.h"
#include "MySTL_allocate.h"
#include "MySTL_construct.h"

namespace stupid
{
	struct __slist_node_base
	{
		__slist_node_base *next;
	};

	template <typename Type>
	struct __slist_node :public __slist_node_base
	{
		Type data;
	};

	inline __slist_node_base *__slist_make_link(__slist_node_base *prev_node, __slist_node_base *new_node)
	{
		new_node->next = prev_node->next;
		prev_node->next = new_node;
		return new_node;
	}

	template <typename Type>
	size_t __slist_size(__slist_node_base *node)
	{
		size_t result = 0;
		while (node != 0)
		{
			++result;
			node = node->next;
		}
	}

	struct __slist_iterator_base
	{
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef forward_iterator_tag iterator_category;

		__slist_node_base *node_pointer;

		__slist_iterator_base(__slist_node_base *x)
			:node_pointer(x)
		{
		}

		bool operator==(const __slist_iterator_base &x)
		{
			return node_pointer == x.node_pointer;
		}

		bool operator!=(const __slist_iterator_base &x)
		{
			return node_pointer != x.node_pointer;
		}

		void increase()
		{
			node_pointer = node_pointer->next;
		}
	};

	template <typename Type, typename Ref, typename Ptr>
	struct __slist_iterator :public __slist_iterator_base
	{
		typedef Type value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __slist_node<Type> slist_node;

		typedef __slist_iterator<Type, Type &, Type *> iterator;
		typedef __slist_iterator<Type, Type &, Type *> const_iterator;
		typedef __slist_iterator self;

		__slist_iterator(__slist_node_base *x)
			:__slist_iterator_base(x)
		{
		}

		__slist_iterator()
			:__slist_iterator_base(0)
		{
		}

		__slist_iterator(const iterator &x)
			:__slist_iterator_base(x.node_pointer)
		{
		}

		reference operator*()
		{
			return ((slist_node *)node_pointer)->data;
		}

		pointer operator->()
		{
			return &(operator*());
		}

		self &operator++()
		{
			increase();
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			increase();
			return tmp;
		}
	};


	template <typename Type, typename Allocator = stupid::allocator>
	class slist
	{
	public:
		typedef Type value_type;
		typedef Type &reference;
		typedef const Type &const_reference;
		typedef Type *pointer;
		typedef const Type *const_pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef __slist_iterator<Type, Type &, Type *> iterator;
		typedef __slist_iterator<Type, const Type &, const Type *> const_iterator;
	private:
		typedef __slist_node_base slist_node_base;
		typedef __slist_node<Type> slist_node;
		typedef __slist_iterator_base iterator_base;
		typedef stupid::simple_allocator<__slist_node<Type>, Allocator> data_allocator;

		static slist_node *create_node(const value_type &value)
		{
			slist_node *result = data_allocator::allocate();
			try
			{
				construct(&(result->data), value);
				result->next = 0;
			}
			catch (...)
			{
				data_allocator::deallocate(result);
				throw;
			}
			return result;
		}

		static void destroy_node(slist_node *node)
		{
			destroy(&(node->data));
			data_allocator::deallocate(node);
		}

		slist_node_base head;
	public:
		slist()
		{
			head.next = 0;
		}

		iterator begin()
		{
			return head.next;
		}

		const_iterator begin() const
		{
			return head.next;
		}

		iterator end()
		{
			return iterator(0);
		}

		const_iterator end() const
		{
			return const_iterator(0);
		}

		bool empty() const
		{
			return head.next == 0;
		}

		size_type size() const
		{
			return __slist_size(head.node);
		}

		void swap(slist &x)
		{
			slist_node_base *tmp = head.next;
			head.next = x.head.next;
			x.head.next = tmp;
		}

		reference front()
		{
			return ((slist_node *)head)->data;
		}

		const_reference front() const
		{
			return ((slist_node *)head)->data;
		}

		void push_front(const value_type &value)
		{
			__slist_make_link(&head, create_node(value));
		}

		void pop_front()
		{
			slist_node *tmp = (slist_node *)(head.next);
			head = head.next;
			destroy_node(tmp);
		}

		void clear()
		{
			slist_node_base *to_delete = head.next;
			slist_node_base *tmp;
			while (0 != to_delete)
			{
				tmp = to_delete;
				to_delete = to_delete->next;
				destroy_node((slist_node *)tmp);
			}
			head.next = 0;
		}

		~slist()
		{
			clear();
		}
	};
}

#endif