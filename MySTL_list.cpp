#include "MySTL_list.h"

template <typename Type,typename Allocator>
list<Type,Allocator>::iterator list<Type, Allocator>::insert(iterator position,const Type &value)
{
	list_node *new_node = create_node(value);
	new_node->prev = (position.p)->prev;
	new_node->next = position.p;
	(position.p)->prev->next = new_node;
	(position.p)->prev = new_node;
	return new_node;
}

template <typename Type,typename Allocator>
list<Type, Allocator>::iterator list<Type, Allocator>::erase(iterator position)
{
	iterator result = position;
	++result;
	(position.p)->prev->next = (position.p)->next;
	(position.p)->next->prev = (position.p)->prev;
	destroy(position.p);
	return result;
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
			erase(trace);
		else
			cur = trace;
		trace = cur;
	}
}

template <typename Type,typename Allocator>
list<Type, Allocator>::iterator list<Type, Allocator>::find(iterator first, iterator last, const Type &value)
{
	iterator cur = first;
	while (cur != last)
	{
		if (*cur == value)
			return cur;
	}
	return cur;
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::transfer(iterator position, iterator first, iterator last)
{
	if (position != last)
	{
		iterator tmp = last;
		--tmp;
		(first.p)->prev->next = (tmp.p)->next;
		(tmp.p)->next->prev = (first.p)->prev;
		(position.p)->prev->next = first.p;
		(first.p)->prev = (position.p)->prev;
		(tmp.p)->next = position.p;
		(position.p)->prev = tmp.p;
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::splice(iterator position, list &x)
{
	if (!x.empty)
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

	while (first != last1&&kfirst2 != last2)
	{
		iterator next;
		if (*first1 <= *last1)
			first1++;
		else
		{
			next = first2;
			++next;
			transfer(next, first2, next);
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
	iterator old;
	++first;
	while (first != last)
	{
		old = first;
		first++;
		transfer(begin(), old, first);
	}
}

template <typename Type,typename Allocator>
void list<Type, Allocator>::sort()
{

}


