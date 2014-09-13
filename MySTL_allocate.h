/*
	the code of MySTL doesn't support multi_threading and exception handing
	the code of MySTL is really SHIT
*/

#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <new>
#include <cstddef>	//for size_t
#include <cstdlib>
#include <iostream>

namespace stupid
{
	class allocator_1st
	{
	private:
		//for out of memory
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *, size_t);
		static void(*__allocate_oom_handler)();
	public:
		static void *allocate(size_t n)
		{
			void *result = malloc(n);
			if (0 == result)
				result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);
		}

		static void *reallocate(void *p, size_t, size_t new_sz)
		{
			void *result = realloc(p, new_sz);
			if (0 == result)
				result = oom_realloc(p, new_sz);
			return result;
		}

		static void(*set_malloc_handler(void(*new_handler)()))()
		{
			void(*old_handler)() = __allocate_oom_handler;
			__allocate_oom_handler = new_handler;
			return old_handler;
		}
	};

	class allocator_2nd
	{
	private:
		static const int __ALIGN = 8;
		static const int __MAX_BYTES = 128;
		static const int __NFREELISTS = __MAX_BYTES / __ALIGN;

		union obj
		{
			obj* free_list_link;
			char client_data[1];
		};

		static obj *free_lists[__NFREELISTS];

		static size_t free_list_index(size_t bytes)
		{
			return (bytes + __ALIGN - 1) / __ALIGN - 1;
		}

		static size_t round_up(size_t bytes)
		{
			return (bytes + __ALIGN - 1)&~(__ALIGN - 1);
		}

		static void *refill(size_t n);
		static char *chunk_free(size_t size, int &n_nodes);

		static char *start_free;
		static char *end_free;
		static size_t heap_size;
	public:
		static void *allocate(size_t n)
		{
			obj* *my_free_list;
			void *result;
			if (n > __MAX_BYTES)
				return allocator_1st::allocate(n);
			else
			{
				my_free_list = free_lists + free_list_index(n);
				result = *my_free_list;
				if (0 == result)
				{
					void *r = refill(round_up(n));
					return r;
				}

				*my_free_list = (*my_free_list)->free_list_link;
				return result;
			}
		}

		static void deallocate(void *p, size_t n)
		{
			if (n > __MAX_BYTES)
			{
				allocator_1st::deallocate(p, n);
				return;
			}
			else
			{
				obj* *my_list_free = free_lists + free_list_index(n);
				((obj *)p)->free_list_link = *my_list_free;
				*my_list_free = (obj *)p;
			}
		}

		static void *reallocate(void *p, size_t, size_t new_sz)
		{
			allocator_1st::reallocate(p, 0, new_sz);
		}
	};

	typedef allocator_2nd allocator;

	template <typename Type, typename Allocator>
	class simple_allocator
	{
	public:
		static Type *allocate(size_t num)
		{
			return 0 == num ? 0 : (Type *)Allocator::allocate(sizeof(Type)*num);
		}

		static Type *allocate(void)
		{
			return (Type *)Allocator::allocate(sizeof(Type));
		}

		static void deallocate(Type *p, size_t num)
		{
			if (0 != num)
				Allocator::deallocate(p, sizeof(Type)*num);
		}

		static void deallocate(Type *p)
		{
			Allocator::deallocate(p, sizeof(Type));
		}
	};

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
				size_t bytes_to_get = 2 * total_size + round_up(heap_size >> 4);

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
}

#endif