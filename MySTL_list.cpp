#include "MySTL_list.h"

template <typename Type,typename Allocator>
list<Type,Allocator>::iterator list<Type, Allocator>::insert(iterator position,const Type &value)
{
	node_pointer cur = position.p;
	node_pointer prev = cur->prev;
	node_pointer new_node = create_node(value);
	new_node->prev = prev;
	new_node->next = cur;
	prev->next = new_node;
	cur->prev = new_node;
	return new_node;
}

template <typename Type,typename Allocator>
list<Type, Allocator>::iterator list<Type, Allocator>::erase(iterator position)
{
	node_pointer cur = position.p;
	node_pointer prev = cur->prev;
	node_pointer next = cur->next;
	prev->next = next;
	next->prev = prev;
	destroy_node(cur);
	return next;
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::clear()
{
	iterator cur = begin();
	iterator last = end();
	while (cur != last)
	{
		iterator tmp = cur;
		++cur;
		erase(tmp);
	}
	(last.p)->next = last.p;
	(last.p)->prev = last.p;
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::remove(const Type &value)
{
	iterator cur = begin();
	iterator last = end();
	while (cur != last)
	{
		if (*cur == value)
		{
			iterator tmp = cur;
			++cur;
			erase(tmp);
		}
		else
			++cur;
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::unique()
{
	iterator cur = begin();
	iterator last = end();
	iterator trace = cur;
	while ((++trace) != last)
	{
		if (*cur == *trace)
		{
			erase(trace);
			trace = cur;
		}
		else
			cur = trace;
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::transfer(iterator position, iterator first, iterator last)
{
	if (position != last)
	{
		node_pointer position_prev = (position.p)->prev;
		node_pointer first_prev = (first.p)->prev;
		node_pointer first_next = (first.p)->next;
		node_pointer last_prev = (last.p)->prev;

		first_prev->next = last.p;
		(last.p)->prev = first_prev;

		position_prev->next = first.p;
		(first.p)->prev = position_prev;

		(position.p)->prev = last_prev;
		last_prev->next = position.p;
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::splice(iterator position, list &x)
{
	if (!x.empty())
		transfer(position, x.begin(), x.end());
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::splice(iterator position, iterator i)
{
	iterator j = i;
	++j;
	if (position == i || position == j)
		return;
	transfer(position, i, j);
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::splice(iterator position, iterator first, iterator last)
{
	if (first != last)
		transfer(position, first, last);
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::merge(list &x)
{
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();

	while (first1 != last1&&kfirst2 != last2)
	{
		iterator next;
		if (*first1 <= *first2)
			++first1;
		else
		{
			next = first2;
			++next;
			transfer(first1, first2, next);
			first2 = next;
		}
	}

	if (first2 != last2)
	{
		transfer(last1, first2, last2);
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::reverse()
{
	if (size() == 0 || size() == 1)
		return;
	iterator first = begin();
	iterator last = end();
	iterator next = first;
	++next;
	while (first != last)
	{
		transfer(begin(), first, next);
		first = next;
		++next;
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::sort()
{
	if (size() == 0 || size() == 1)
		return;
	iterator cur = begin();
	iterator last = end();
	iterator prev;
	++cur;
	while (cur != last)
	{
		prev = cur;
		--prev;
		while (prev != last&&*prev > *cur)
			--prev;
		++prev;
		iterator tmp = cur;
		++cur;
		if (prev!=cur)
			transfer(prev, tmp, cur);
	}
}


