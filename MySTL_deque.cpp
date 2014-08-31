#include "MySTL_deque.h"
#include "MySTL_algobase.h"

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
		for (; cur >= node_start; cur--)
			deallocate_node(*cur);
		throw;
	}

	start.set_node(node_start);
	finish.set_node(node_finish);
	start.cur = start.first;
	finish.cur = finish.first + num_elements%buffer_size();
}

template <typename Type,typename Allocator,size_t Buffer_size>
void deque<Type, Allocator, Buffer_size>::fill_initialize(size_type num_elements,value_t)
{
	create_map_and_nodes(n);

	map_pointer cur;
	try
	{
		for (cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), value);
	}
	catch (...)
	{
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
		finish.cur = finish.start;
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
	size_type old_num_nodes = finish.node - start.node;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;

	map_pointer new_start_node;
	if (map_size > 2 * new_num_nodes)
	{
		new_start_node = map + (map_size - new_num_nodes) + add_at_front ? nodes_to_add : 0;
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