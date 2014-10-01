#ifndef TREE_H
#define TREE_H

#include "MySTL_iterator.h"
#include "MySTL_allocate.h"
#include "MySTL_construct.h"
#include "MySTL_algobase.h"
#include <utility>
#include <cstddef>
#include <cassert>

namespace stupid
{
	enum __rb_tree_color_type{ __rb_tree_red = 0, __rb_tree_black = 1 };

	struct __rb_tree_node_base
	{
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base *base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minium(base_ptr x)
		{
			while (0 != x->left)
				x = x->left;
			return x;
		}

		static base_ptr maximum(base_ptr x)
		{
			while (0 != x->right)
				x = x->right;
			return x;
		}
	};

	template <typename Value>
	struct __rb_tree_node :public __rb_tree_node_base
	{
		typedef __rb_tree_node<Value> *link_type;
		Value value_field;
	};

	struct __rb_tree_iterator_base
	{
		typedef __rb_tree_node_base::base_ptr base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		base_ptr node;

		__rb_tree_iterator_base(base_ptr x)
		{
			node = x;
		}

		void increment()
		{
			if (0 != node->right)
			{
				node = node->right;
				while (0 != node->left)
					node = node->left;
			}
			else
			{
				base_ptr parent = node->parent;
				while (node == parent->right)
				{
					node = parent;
					parent = parent->parent;
				}

				if (node->right != parent)
					node = parent;
			}
		}

		void decrement()
		{
			if (__rb_tree_red == node->color&&node->parent->parent == node)
				node = node->right;
			else
			{
				if (0 != node->left)
				{
					node = node->left;
					while (0 != node->right)
						node = node->right;
				}
				else
				{
					base_ptr parent = node->parent;
					while (node == parent->left)
					{
						node = parent;
						parent = parent->parent;
					}

					node = parent;
				}
			}
		}
	};

	template <typename Value, typename Ref, typename Ptr>
	struct __rb_tree_iterator :public __rb_tree_iterator_base
	{
		typedef Value value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __rb_tree_iterator self;
		typedef __rb_tree_node<Value> *link_type;

		__rb_tree_iterator()
			:__rb_tree_iterator_base(0)
		{
		}

		__rb_tree_iterator(base_ptr x)
			:__rb_tree_iterator_base(x)
		{
		}

		__rb_tree_iterator(const self &x)
			:__rb_tree_iterator_base(x)
		{

		}

		reference operator*() const
		{
			return (link_type(node))->value_field;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self &operator++()
		{
			increment();
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			increment();
			return tmp;
		}

		self &operator--()
		{
			decrement();
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			decrement();
			return tmp;
		}

		bool operator==(const self &x)
		{
			return node == x.node;
		}

		bool operator!=(const self &x)
		{
			return node != x.node;
		}
	};

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator = stupid::allocator>
	class rb_tree
	{
	protected:
		typedef void *void_pointer;
		typedef __rb_tree_node_base *base_ptr;
		typedef __rb_tree_node<Value> rb_tree_node;
		typedef __rb_tree_color_type color_type;
		typedef stupid::simple_allocator<rb_tree_node, Allocator> rb_tree_node_allocator;
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef rb_tree_node *link_type;
		typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
		typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;
		typedef stupid::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef stupid::reverse_iterator<iterator> reverse_iterator;
	protected:
		link_type get_node()
		{
			return rb_tree_node_allocator::allocate();
		}

		void put_node(link_type p)
		{
			rb_tree_node_allocator::deallocate(p);
		}

		link_type create_node(const value_type &v)
		{
			link_type result = get_node();
			try
			{
				stupid::construct(&(result->value_field), v);
			}
			catch (...)
			{
				put_node(result);
				throw;
			}

			return result;
		}

		link_type clone_node(link_type x)
		{
			link_type result = create_node(x->value_field);
			result->color = x->color;
			result->left = 0;
			result->right = 0;
			return result;
		}

		void destroy_node(link_type x)
		{
			stupid::destroy(&(x->value_field));
			put_node(x);
		}

	protected:
		size_type node_count;
		link_type header;
		Compare key_compare;

		link_type &root() const
		{
			return (link_type &)(header->parent);
		}

		link_type &leftmost() const
		{
			return (link_type &)(header->left);
		}

		link_type &rightmost() const
		{
			return (link_type &)(header->right);
		}

		static link_type& left(link_type x)
		{
			return (link_type &)(x->left);
		}

		static link_type& right(link_type x)
		{
			return (link_type &)(x->right);
		}

		static link_type& parent(link_type x)
		{
			return (link_type &)(x->parent);
		}

		static reference value(link_type x)
		{
			return x->value_field;
		}

		static const key_type &key(link_type x)
		{
			return KeyOfValue()(value(x));
		}

		static color_type &color(link_type x)
		{
			return (color_type &)(x->color);
		}

		static link_type& left(base_ptr x)
		{
			return (link_type &)(x->left);
		}

		static link_type& right(base_ptr x)
		{
			return (link_type &)(x->right);
		}

		static link_type& parent(base_ptr x)
		{
			return (link_type &)(x->parent);
		}

		static reference value(base_ptr x)
		{
			return ((link_type)x)->value_field;
		}

		static const key_type & key(base_ptr x)
		{
			return KeyOfValue()(value(x));
		}

		static color_type &color(base_ptr x)
		{
			return (color_type &)(link_type(x)->color);
		}

		static link_type minium(link_type x)
		{
			return link_type(__rb_tree_node_base::minium(base_ptr(x)));
		}

		static link_type maximum(link_type x)
		{
			return link_type(__rb_tree_node_base::maximum(base_ptr(x)));
		}
	private:
		iterator __insert(base_ptr x, base_ptr y, const value_type &v);
		void __rb_tree_balance(base_ptr x, base_ptr &root);
		base_ptr __rb_tree_balance_for_erase(base_ptr z, base_ptr &root, base_ptr &leftmost, base_ptr &rightmost);
		void __rb_tree_rotate_left(base_ptr x, base_ptr &root);
		void __rb_tree_rotate_right(base_ptr x, base_ptr &root);
		link_type __rb_tree_copy(link_type x, link_type y);
		void init()
		{
			header = get_node();
			color(header) = __rb_tree_red;

			root() = 0;
			leftmost() = header;
			rightmost() = header;

			node_count = 0;
		}
	public:
		rb_tree()
		{
			init();
		}

		rb_tree(Compare x)
			:key_compare(x)
		{
			init();
		}

		rb_tree(const rb_tree &x)
		{
			init();
			if (!x.empty())
			{
				root() = __rb_tree_copy(x.root(), header);
				leftmost() = (link_type)__rb_tree_node_base::minium(root());
				rightmost() = (link_type)__rb_tree_node_base::maximum(root());
				node_count = x.node_count;
			}
		}

		rb_tree& operator=(const rb_tree &x)
		{
			if (&x == this)
				return *this;
			clear();
			if (x.empty())
			{
				leftmost() = header;
				rightmost() = header;
				root() = 0;
				node_count = 0;
			}
			else
			{
				root() = __rb_tree_copy(x.root(), header);
				leftmost() = (link_type)__rb_tree_node_base::minium(root());
				rightmost() = (link_type)__rb_tree_node_base::maximum(root());
				node_count = x.node_count;
			}
			return *this;
		}

		Compare key_comp() const
		{
			return key_compare;
		}

		iterator begin()
		{
			return leftmost();
		}

		const_iterator begin() const
		{
			return leftmost();
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		iterator end()
		{
			return header;
		}

		const_iterator end() const
		{
			return header;
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		bool empty() const
		{
			return node_count == 0;
		}

		size_type size() const
		{
			return node_count;
		}

		size_type max_size() const
		{
			return size_type(-1);
		}

		~rb_tree()
		{
			clear();
		}

		std::pair<iterator, bool> insert_unique(const value_type &v);
		template <typename InputIterator>
		void insert_unique(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				insert_unique(*first);
				++first;
			}
		}

		iterator insert_equal(const value_type &v);
		template <typename InputIterator>
		void insert_equal(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				insert_equal(*first);
				++first;
			}
		}

		iterator find(const key_type &k);
		const_iterator find(const key_type &k) const;
		iterator lower_bound(const key_type &k);
		const_iterator lower_bound(const key_type &k) const;
		iterator upper_bound(const key_type &k);
		const_iterator upper_bound(const key_type &k) const;
		std::pair<iterator, iterator> equal_range(const key_type &k);
		std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const;
		size_type count(const key_type &k) const;
		size_type erase(const key_type &k);
		void erase(iterator first, iterator last);
		void erase(iterator position);
		void swap(rb_tree &x);
		void clear();
	};

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_equal(const value_type &v)
	{
		link_type y = header;
		link_type x = root();
		while (0 != x)
		{
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
		}

		return __insert(x, y, v);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator, bool> 
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_unique(const value_type &v)
	{
		link_type y = header;
		link_type x = root();
		bool comp = true;

		while (0 != x)
		{
			y = x;
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}

		iterator j = iterator(y);
		if (comp)
		{
			if (j == begin())
				return std::pair <iterator, bool>(__insert(x, y, v), true);
			else
				--j;
		}

		if (key_compare(key(j.node), KeyOfValue()(v)))
			return std::pair<iterator, bool>(__insert(x, y, v), true);

		return std::pair<iterator, bool>(j, false);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator 
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__insert(base_ptr x_, base_ptr y_, const value_type &v)
	{
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z = create_node(v);

		if (y == header || key_compare(KeyOfValue()(v), key(y)))
		{
			left(y) = z;
			if (y == header)
			{
				root() = z;
				rightmost() = z;
			}
			else
			{
				if (y == leftmost())
					leftmost() = z;
			}
		}
		else
		{
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;
		}

		parent(z) = y;
		left(z) = 0;
		right(z) = 0;

		__rb_tree_balance(z, header->parent);
		++node_count;
		return iterator(z);
	}

	template <typename Key,typename Value,typename KeyOfValue,typename Compare,typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__rb_tree_balance(base_ptr x, base_ptr &root)
	{
		x->color = __rb_tree_red;
		while (x != root&&__rb_tree_red == x->parent->color)
		{
			if (x->parent == x->parent->parent->left)
			{
				base_ptr u = x->parent->parent->right;
				if (0 != u&&__rb_tree_red == u->color)
				{
					x->parent->color = __rb_tree_black;
					u->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->right)
					{
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}

					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else
			{
				base_ptr u = x->parent->parent->left;
				if (0 != u&&__rb_tree_red == u->color)
				{
					x->parent->color = __rb_tree_black;
					u->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = __rb_tree_black;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__rb_tree_rotate_left(base_ptr x, base_ptr &root)
	{
		base_ptr y = x->right;
		x->right = y->left;
		if (0 != y->left)
			y->left->parent = x;

		y->parent = x->parent;

		if (x == root)
			root = y;
		else
			if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;

		y->left = x;
		x->parent = y;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__rb_tree_rotate_right(base_ptr x, base_ptr &root)
	{
		base_ptr y = x->left;
		x->left = y->right;
		if (0 != y->right)
			y->right->parent = x;

		y->parent = x->parent;
		if (x == root)
			root = y;
		else
			if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
		
		y->right = x;
		x->parent = y;
	}

	template <typename Key,typename Value,typename KeyOfValue,typename Compare,typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator 
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::find(const key_type &k)
	{
		link_type y = header;
		link_type x = root();

		while (0 != x)
		{
			if (!key_compare(key(x), k))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}

		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator 
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::find(const key_type &k) const
	{
		link_type y = header;
		link_type x = root();

		while (0 != x)
		{
			if (!key_compare(key(x), k))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}

		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::lower_bound(const key_type &k)
	{
		link_type y = header;
		link_type x = root();

		while (0 != x)
		{
			if (!key_compare(key(x), k))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}

		return iterator(y);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::lower_bound(const key_type &k) const
	{
		link_type y = header;
		link_type x = root();

		while (0 != x)
		{
			if (!key_compare(key(x), k))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}

		return iterator(y);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key,Value,KeyOfValue,Compare,Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::upper_bound(const key_type &k)
	{
		link_type y = header;
		link_type x = root();

		while (0 != x)
		{
			if (key_compare(k, key(x)))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}

		return iterator(y);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::upper_bound(const key_type &k) const
	{
		link_type y = header;
		link_type x = root();

		while (0 != x)
		{
			if (key_compare(k, key(x)))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}

		return iterator(y);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator, typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::equal_range(const key_type &k)
	{
		return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator, typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::equal_range(const key_type &k) const
	{
		return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::size_type 
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::count(const key_type &k) const
	{
		const_iterator first = lower_bound(k);
		const_iterator last = upper_bound(k);
		return distance(first, last);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::size_type rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(const key_type &k)
	{
		std::pair<iterator, iterator> range = equal_range(k);
		size_type n = distance(range.first, range.second);
		erase(range.first, range.second);
		return n;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(iterator first, iterator last)
	{
		if (first == begin() && last == end())
			clear();
		else
		{
			while (first != last)
				erase(first++);
		}
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(iterator position)
	{
		link_type y = (link_type)__rb_tree_balance_for_erase(position.node, header->parent, header->left, header->right);
		destroy_node(y);
		--node_count;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::clear()
	{
		iterator first = begin();
		iterator last = end();
		while (first != last)
			erase(first++);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::base_ptr
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__rb_tree_balance_for_erase(base_ptr z,
		base_ptr &root,
		base_ptr &leftmost,
		base_ptr &rightmost)
	{
		base_ptr y = z;
		base_ptr x = 0;
		base_ptr x_parent = 0;

		if (0 == y->left)
			x = y->right;
		else
		{
			if (0 == y->right)
				x = y->left;
			else
			{
				y = y->right;
				while (0 != y->left)
					y = y->left;
				x = y->right;
			}
		}

		if (y != z)
		{
			z->left->parent = y;
			y->left = z->left;
			if (y != z->right)
			{
				x_parent = y->parent;
				if (0 != x)
					x->parent = y->parent;
				y->parent->left = x;
				z->right->parent = y;
				y->right = z->right;
			}
			else
				x_parent = y;

			if (root == z)
				root = y;
			else
			{
				if (z->parent->left == z)
					z->parent->left = y;
				else
					z->parent->right = y;
			}
			y->parent = z->parent;
			stupid::swap(y->color, z->color);
			y = z;
		}
		else
		{
			x_parent = y->parent;
			if (0 != x)
				x->parent = y->parent;
			if (root == z)
				root = x;
			else
			{
				if (y->parent->left == y)
					y->parent->left = x;
				else
					y->parent->right = x;
			}

			if (leftmost == z)
			{
				if (0 == z->right)
					leftmost = z->parent;
				else
					leftmost = (link_type)__rb_tree_node_base::minium(x);
			}

			if (rightmost == z)
			{
				if (0 == z->left)
					rightmost = z->parent;
				else
					rightmost = (link_type)__rb_tree_node_base::maximum(x);
			}	
		}
		
		if (__rb_tree_red != y->color)
		{
			while (x != root && (0 == x || __rb_tree_black == x->color))
			{
				if (x == x_parent->left)
				{
					base_ptr u = x_parent->right;

					if (0 != u &&__rb_tree_red == u->color)
					{
						u->color = __rb_tree_black;
						x_parent->color = __rb_tree_red;
						__rb_tree_rotate_left(x_parent, root);
						u = x_parent->right;
					}

					if ((0 == u->left || __rb_tree_black == u->left->color) && (0 == u->right || __rb_tree_black == u->right->color))
					{
						u->color = __rb_tree_red;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else
					{
						if ((0 == u->right || __rb_tree_black == u->right->color))
						{
							if (0 != u->left)
								u->left->color = __rb_tree_black;
							u->color = __rb_tree_red;
							__rb_tree_rotate_right(u, root);
							u = x_parent->right;
						}
						u->color = x_parent->color;
						x_parent->color = __rb_tree_black;
						if (0 != u->right)
							u->right->color = __rb_tree_black;
						__rb_tree_rotate_left(x_parent, root);
						break;
					}
				}
				else
				{
					base_ptr u = x_parent->left;
					if (0 != u&&__rb_tree_red == u->color)
					{
						x_parent->color = __rb_tree_red;
						u->color = __rb_tree_black;
						__rb_tree_rotate_right(x_parent, root);
						u = x_parent->left;
					}

					if ((0 == u->left || __rb_tree_black == u->left->color) && (0 == u->right || __rb_tree_black == u->right->color))
					{
						u->color = __rb_tree_red;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else
					{
						if (0 == u->left || __rb_tree_black == u->left->color)
						{
							if (0 != u->right)
								u->right->color = __rb_tree_black;
							u->color = __rb_tree_red;
							__rb_tree_rotate_left(u, root);
							u = x_parent->left;
						}
						u->color = x_parent->color;
						x_parent->color = __rb_tree_black;
						if (0 != u->left)
							u->left->color = __rb_tree_black;
						__rb_tree_rotate_right(x_parent, root);
						break;
					}
				}
			}
			if (0 != x)
				x->color = __rb_tree_black;
		}
		return y;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::link_type rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__rb_tree_copy(link_type x, link_type y)
	{
		link_type top = clone_node(x);
		top->parent = y;

		if (0 != x->right)
			top->right = __rb_tree_copy(right(x), top);
		y = top;
		x = left(x);

		while (0 != x)
		{
			link_type p = clone_node(x);
			p->parent = y;
			y->left = p;
			if (0 != x->right)
				p->right = __rb_tree_copy(right(x), p);
			y = p;
			x = left(x);
		}

		return top;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::swap(rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x)
	{
		stupid::swap(header->parent, (x.header)->parent);
		stupid::swap(header->left, (x.header)->left);
		stupid::swap(header->right, (x.header)->right);
		stupid::swap(node_count, x.node_count);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &y)
	{
		return ((x.size() == y.size()) && stupid::equal(x.begin(), x.end(), y.begin()));
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &y)
	{
		return !(x == y);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &y)
	{
		return stupid::lexicalgraphical_compare(x.begin(), x.end(), y.begin(), y.end());
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool operator>(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &y)
	{
		return y < x;
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool operator<=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &y)
	{
		return !(x > y);
	}

	template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool operator>=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &y)
	{
		return !(x < y);
	}
}

#endif