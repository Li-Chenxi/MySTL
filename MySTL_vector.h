/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef VECTOR_H
#define VECTOR_H

#include "MySTL_iterator.h"
#include "MySTL_allocate.h"
#include "MySTL_uninitialized.h"
#include "MySTL_algobase.h"
#include "MySTL_construct.h"
#include <cstddef>

namespace stupid
{
	template <typename Type, typename Allocator = stupid::allocator>
	class vector
	{
	public:
		typedef Type value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef value_type *iterator;
		typedef const value_type *const_iterator;
		typedef stupid::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef stupid::reverse_iterator<iterator> reverse_iterator;
	protected:
		typedef stupid::simple_allocator<value_type, Allocator> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;

		void deallocate()
		{
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}

		iterator allocate_and_fill(size_type num_elements, const Type &value)
		{
			iterator result = data_allocator::allocate(num_elements);
			stupid::uninitialized_fill_n(result, num_elements, value);
			return result;
		}

		template<typename InputIterator, typename Distance>
		iterator allocate_and_copy(InputIterator first, InputIterator last, Distance num_elements)
		{
			iterator result = data_allocator::allocate(num_elements);
			stupid::uninitialized_copy(first, last, result);
			return result;
		}

		void fill_initialize(size_type num_elements, const Type &value)
		{
			start = allocate_and_fill(num_elements, value);
			finish = start + num_elements;
			end_of_storage = finish;
		}


		template<typename InputIterator>
		void range_initialize(InputIterator first, InputIterator last)
		{
			typedef typename iterator_traits<InputIterator>::difference_type difference_type;
			difference_type num_elements = stupid::distance(first, last);
			start = allocate_and_copy(first, last, num_elements);
			finish = start + num_elements;
			end_of_storage = finish;
		}

		void insert_aux(iterator position, const Type &x);
	public:
		vector() :start(0), finish(0), end_of_storage(0)
		{
		}

		vector(size_type n, const Type &value)
		{
			fill_initialize(n, value);
		}

		vector(int n, const Type &value)
		{
			fill_initialize(n, value);
		}

		vector(long n, const Type &value)
		{
			fill_initialize(n, value);
		}

		explicit vector(size_type n)
		{
			fill_initialize(n, Type());
		}

		template <typename InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			range_initialize(first, last);
		}

		iterator begin()
		{
			return start;
		}

		const_iterator begin() const
		{
			return start;
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return reverse_iterator(end());
		}

		iterator end()
		{
			return finish;
		}

		const_iterator end() const
		{
			return finish;
		}


		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		size_type size() const
		{
			return (size_type)(finish - start);
		}

		size_type capacity() const
		{
			return (size_type)(end_of_storage - start);
		}

		bool empty() const
		{
			return start == finish;
		}

		reference operator[] (size_type n)
		{
			return *(start + n);
		}

		const_reference operator[](size_type n) const
		{
			return *(start + n);
		}

		reference front()
		{
			return *start;
		}

		const_reference front() const
		{
			return *start;
		}

		reference back()
		{
			return *(finish - 1);
		}

		const_reference back() const
		{
			return *(finish - 1);
		}

		void push_back(const Type &value)
		{
			if (finish != end_of_storage)
			{
				stupid::construct(finish, value);
				++finish;
			}
			else
				insert_aux(finish, value);
		}

		void pop_back()
		{
			if (!empty())
			{
				--finish;
				stupid::destroy(finish);
			}
		}

		iterator erase(iterator position)
		{
			if (position + 1 != finish)
				stupid::copy(position + 1, finish, position);
			--finish;
			stupid::destroy(finish);
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator i = stupid::copy(last, finish, first);
			stupid::destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		void resize(size_type new_sz, const Type &value)
		{
			if (new_sz < size())
				erase(start + new_sz, finish);
			else
				insert(finish, new_sz - size(), value);
		}

		void resize(size_type new_sz)
		{
			resize(new_sz, Type());
		}

		void reserve(size_type new_cp)
		{
			if (capacity() < new_cp)
			{
				iterator new_start = data_allocator::allocate(new_cp);
				try
				{
					stupid::uninitialized_copy(start, finish, new_start);
				}
				catch (...)
				{
					data_allocator::deallocate(new_start);
					throw;
				}

				size_type old_size = finish - start;
				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = start + old_size;
				end_of_storage = start+new_cp;
			}
		}

		void clear()
		{
			erase(start, finish);
		}

		void swap(vector &x)
		{
			stupid::swap(start, x.start);
			stupid::swap(finish, x.finish);
			stupid::swap(end_of_storage, x.end_of_storage);
		}

		void insert(iterator position, size_type n, const Type &value);

		~vector()
		{
			stupid::destroy(start, finish);
			deallocate();
		}
	};

	template <typename Type, typename Allocator>
	void vector<Type, Allocator>::insert_aux(iterator position, const Type &value)
	{
		if (finish != end_of_storage)
		{
			stupid::construct(finish, *(finish - 1));
			finish++;
			Type value_copy = value;
			stupid::copy_backward(position, finish - 2, finish - 1);
			*position = value_copy;
		}
		else
		{
			const size_type old_size = size();
			const size_type len = old_size != 0 ? old_size + 1 : 1;

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try
			{
				new_finish = stupid::uninitialized_copy(start, position, new_start);
				stupid::construct(new_finish, value);
				++new_finish;
				new_finish = stupid::uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				stupid::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}

			stupid::destroy(start, finish);
			data_allocator::deallocate(start, old_size);
			start = new_start;
			finish = new_finish;
			end_of_storage = start + len;
		}
	}

	template <typename Type, typename Allocator>
	void vector<Type, Allocator>::insert(iterator position, size_type num_elements, const Type &value)
	{
		Type value_copy = value;
		if ((size_type)(end_of_storage - finish) >= num_elements)
		{
			const size_type elements_after = finish - position;
			iterator old_finish = finish;
			if (elements_after > num_elements)
			{
				stupid::uninitialized_copy(finish - num_elements, finish, finish);
				finish += num_elements;
				stupid::copy_backward(position, old_finish - num_elements, old_finish);
				stupid::fill(position, position + num_elements, value_copy);
			}
			else
			{
				stupid::uninitialized_fill_n(finish, num_elements - elements_after, value_copy);
				finish += num_elements - elements_after;
				stupid::uninitialized_copy(position, old_finish, finish);
				finish += elements_after;
				stupid::fill(position, old_finish, value_copy);
			}
		}
		else
		{
			const size_type old_size = size();
			const size_type new_size = old_size + max(old_size, num_elements);

			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try
			{
				new_finish = stupid::uninitialized_copy(start, position, new_start);
				new_finish = stupid::uninitialized_fill_n(position, num_elements, value_copy);
				new_finish = stupid::uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				stupid::destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}

			stupid::destroy(start, finish);
			data_allocator::deallocate(start, old_size);
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}
}

#endif