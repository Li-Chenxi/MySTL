#include "MySTL_deque.h"
#include "MySTL_algobase.h"
#include "MySTL_construct.h"
#include "MySTL_uninitialized.h"

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::create_map_and_nodes(size_type num_elements)
{
	size_type num_nodes = num_elements / buffer_size() + 1;

	map_size = max(8, num_nodes + 2);
	map = map_allocator::allocate(map_size);

	map_pointer node_start = map + (map_size - num_nodes) / 2;
	map_pointer node_finish = node_start + num_nodes - 1;

	map_pointer cur;
	try
	{
		for (cur = node_start; cur <= node_finish; ++cur)
			*cur = allocate_node();
	}
	catch (...)
	{
		for (; cur >= node_start; --cur)
			deallocate_node(*cur);
		throw;
	}

	start.set_node(node_start);
	finish.set_node(node_finish);
	start.cur = start.first;
	finish.cur = finish.first + difference_type(num_elements%buffer_size());
}

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::fill_initialize(size_type num_elements,const value_type &value)
{
	create_map_and_nodes(num_elements);

	map_pointer cur;
	try
	{
		for (cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), value);
		uninitialized_fill(finish.first, finish.cur, value);
	}
	catch (...)
	{
		destroy(start, iterator(*cur, cur));
		throw;
	}
}

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::push_back_aux(const value_type &value)
{
	value_type value_copy = value;
	reserve_map_at_back();
	*(finish.node + 1) = allocate_node();

	try
	{
		construct(finish.cur, value_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	catch (...)
	{
		deallocate_node(*(finish.node + 1));
		throw;
	}
}

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::push_front_aux(const value_type &value)
{
	value_type value_copy = value;
	reserve_map_at_front();
	*(start.node - 1) = allocate_node();

	try
	{
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, value);
	}
	catch (...)
	{
		start.set_node(start.node + 1);
		start.cur = start.first;
		deallocate_node(*(start.node - 1));
		throw;
	}
}

template<typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
	size_type old_num_nodes = finish.node - start.node + 1;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;

	map_pointer new_start_node;
	if (map_size > 2 * new_num_nodes)
	{
		new_start_node = map + (map_size - new_num_nodes) / 2 + add_at_front ? nodes_to_add : 0;
		if (new_start_node < start.node)
			copy(start.node, finish.node + 1, new_start_node);
		else
			copy_backward(start.node, finish.node + 1, new_start_node + old_num_nodes);
	}
	else
	{
		size_type new_map_size = max_size + max(max_size, new_num_nodes);
		map_pointer new_map = map_allocator::allocate(new_map_size);

		new_node_start = new_map + (new_map_size - new_num_nodes) / 2 + add_at_front ? nodes_to_add : 0;
		copy(start.node, finish.node + 1, new_node_start);
		map_allocator::deallocate(map);

		map_size = new_map_size;
		map = new_map;
	}

	start.set_node(new_start_node);
	finish.set_node(new_start_node + old_num_nodes - 1);
}

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::pop_back_aux()
{
	deallocate_node(finish.first);
	finish.set_node(finish.node - 1);
	finish.cur = finish.last-1;
	destroy(finish.cur);
}

template <typename Type, typename Allocator, size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::pop_front_aux()
{
	destroy(start.cur);
	deallocate_node(start.first);
	start.set_node(start.node + 1);
	start.cur = start.first;
	
}

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::clear()
{
	for (map_pointer node = start.node + 1; node < finish.node; node++)
	{
		destroy(*node, *node + (difference_type)buffer_size());
		deallocate_node(*node);
	}

	if (start.node != finish.node)
	{
		destroy(start.cur, start.last);
		destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first);
	}
	else
		destroy(start.cur, finish.cur);

	finish = start;
}

template <typename Type,typename Allocator,size_t Buffer_size>
deque<Type, Allocator, Buffer_size>::iterator deque<Type, Allocator, Buffer_size>::erase(deque<Type,Allocator,Buffer_size>::iterator pos)
{
	iterator next = pos;
	++next;
	difference_type index = pos - start;
	if (index < (size() >> 1))
	{
		copy_backward(start, pos, next);
		pop_front();
	}
	else
	{
		copy(next, finish, pos);
		pop_back();
	}
	return start + index;
}

template <typename Type,typename Allocator,size_t Buffer_size>
deque<Type, Allocator, Buffer_size>::iterator deque<Type, Allocator, Buffer_size>::erase(iterator first, iterator last)
{
	if (first == start&&last == first)
	{
		clear();
		return finish;
	}
	else
	{
		difference_type n = last - first;
		difference_type elements_before = first - start;
		if (elements_before < ((size() - n) >> 1))
		{
			copy_backward(start, first, last);
			iterator new_start = start + n;
			destroy(start, new_start);
			for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				deallocate_node(*cur);
			start = new_start;
		}
		else
		{
			copy(last, finish, first);
			iterator new_finish = finish - n;
			destroy(new_finish, finish);
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
				deallocate_node(*cur);
			finish = new_finish;
		}
		return start + elements_before;
	}
}

template <typename Type,typename Allocator,size_t Buffer_size>
deque<Type, Allocator, Buffer_size>::iterator deque<Type, Allocator, Buffer_size>::insert_aux(iterator position,const value_type &value)
{
	difference_type index = position - start;
	value_type value_copy = value;
	if (index < (size() << 1))
	{
		push_front(front());
		iterator front1 = start;
		++front1;
		iterator front2 = front1;
		++front2;
		position = start + index;
		iterator pos1 = position;
		++pos1;
		copy(front2, pos1, front1);
	}
	else
	{
		push_back(back());
		iterator back1 = finish;
		--back1;
		iterator back2 = back1;
		--back2;
		position = start + index;
		copy_backward(position, back2, back1);
	}
	*position = value_copy;
	return pos;
}



