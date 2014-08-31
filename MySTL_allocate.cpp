#include "MySTL_allocate.h"

void(*allocator_1st::__allocate_oom_handler)() = 0;

void *allocator_1st::oom_malloc(size_t n)
{
	void(*my_allocate_oom_handler)();
	void *result;

	while (true)
	{
		my_allocate_oom_handler = __allocate_oom_handler;
		if (0 == my_allocate_oom_handler)
		{
			std::cerr << "out of memory!" << std::endl;
			exit(EXIT_FAILURE);
		}
		(*my_allocate_oom_handler)();
		result = malloc(n);
		if (result)
			return result;
	}
}

void *allocator_1st::oom_realloc(void *p, size_t n)
{
	void(*my_allocate_oom_handler)();
	void *result;

	while (true)
	{
		my_allocate_oom_handler = __allocate_oom_handler;
		if (0 == my_allocate_oom_handler)
		{
			std::cerr << "out of memory!" << std::endl;
			exit(EXIT_FAILURE);
		}
		(*my_allocate_oom_handler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}

char *allocator_2nd::start_free = 0;
char *allocator_2nd::end_free = 0;
size_t allocator_2nd::heap_size = 0;
allocator_2nd::obj *allocator_2nd::free_lists[allocator_2nd::__NFREELISTS] = { 0 };

void *allocator_2nd::refill(size_t n)
{
	int n_node = 20;

	char *chunk = chunk_free(n, n_node);
	obj *result;

	if (n_node == 1)
	{
		return (obj *)chunk;
	}
	else
	{
		obj *current_obj, *next_obj;
		obj* *my_free_list = free_lists + free_list_index(n);
		result = (obj *)chunk;
		*my_free_list = next_obj = (obj *)((char *)chunk + n);

		for (int i = 1;; i++)
		{
			current_obj = next_obj;
			next_obj = (obj *)((char *)next_obj + n);
			if (n_node - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else
				current_obj->free_list_link = next_obj;
		}

		return result;
	}
}

char *allocator_2nd::chunk_free(size_t size, int &n_nodes)
{
	char *result;
	size_t total_size = size*n_nodes;
	size_t left_free = end_free - start_free;

	if (left_free >= total_size)
	{
		result = start_free;
		start_free += total_size;
		return result;
	}
	else
	{
		if (left_free >= size)
		{
			n_nodes = left_free / size;
			total_size = n_nodes*size;
			result = start_free;
			start_free += total_size;
			return result;
		}
		else
		{
			size_t bytes_to_get = 2 * total_size+ round_up(heap_size >> 4);

			if (left_free > 0)
			{
				obj* *my_free_list = free_lists + free_list_index(left_free);
				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj *)start_free;
			}

			start_free = (char *)malloc(bytes_to_get);
			if (0 == start_free)
			{
				obj* *my_free_list, *p;
				for (int i = size; i <= __MAX_BYTES; size += __ALIGN)
				{
					my_free_list = free_lists + free_list_index(i);
					p = *my_free_list;
					if (0 != p)
					{
						start_free = (char *)p;
						end_free = start_free + i;
						*my_free_list = p->free_list_link;
						return chunk_free(size, n_nodes);
					}
				}
				end_free = 0;
				start_free = (char *)allocator_1st::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_free(size, n_nodes);
		}
	}
}