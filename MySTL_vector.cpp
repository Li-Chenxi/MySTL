#include "MySTL_vector.h"
#include "MySTL_uninitialized.h"
#include "MySTL_algobase.h"

template <typename Type,typename Allocator>
void vector<Type, Allocator>::insert_aux(iterator position, const Type &value)
{
	if (finish != end_of_storage)
	{
		construct(finish, *(finish - 1));
		finish++;
		Type value_copy = value;
		copy_backward(position, finish - 2, finish - 1);
		*position = value_copy;
	}
	else
	{
		const size_type old_size = size();
		const size_type len = old_size != 0 ? old_size + 1 : 1;

		iterator new_start = data_allocator::allocate(len);
		iterator new _finish = new_start;
		try
		{
			new_finish = uninitialized_copy(start, position, new_start);
			construct(new_finish, *position);
			++new_finish;
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...)
		{
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}

		desstroy(start, finish);
		data_allocator(start, old_size);
		start = new_start;
		finish = new_finish;
		end_of_storage = start + len;
	}
}

template <typename Type,typename Allocator>
void vector<Type, Allocator>::insert(iterator position, size_type num_elements, const Type &value)
{
	if ((size_type)(end_of_storage - finish) >= num_elements)
	{
		Type value_copy = value;
		const size_type elements_after = finish - position;
		iterator old_finish = finish;
		if (elements_after > num_elements)
		{
			uninitialized_copy(finish - num_elements, finish, finish);
			finish += num_elements;
			copy_backward(position, old_finish - num_elements, old_finish);
			fill(position, position + n, value_copy);
		}
		else
		{
			uninitialized_fill_n(finish, num_elements - elements_after, value_copy);
			finish += num_elements - elements_after;
			uninitialized_copy(position, old_finish, finish);
			finish += elements_after;
			fill(position, old_finish, value_copy);
		}
	}
	else
	{
		const size_type old_size = size();
		const size_type new_size = old_size + max(old_size, n);

		iterator new_start = data_allocator(new_size);
		iterator new_finish = new_start;
		try
		{
			new_finish = uninitialized_copy(start, position, new_start);
			new_finish = uninitialized_fill_n(position, n, value_copy);
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...)
		{
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, new_size);
			throw;
		}

		destroy(start, finish);
		data_allocator::deallocate(start, old_size);
		start = new_start;
		finish = new_finish;
		end_of_storage = start + new_size;
	}
}