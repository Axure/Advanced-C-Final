#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

struct node
{
	struct linked_list_node list;
	int num;
};

int main(int argc, char const *argv[])
{
	int i;
	struct node *tem;
	struct linked_list_node *pos, *n;

	struct node mylist;
	INIT_LINKED_LIST_HEAD(&mylist.list);

	for (i = 0; i < 10; ++i)
	{
		tem = (struct node*)malloc(sizeof(struct node));
		tem->num = i;
		linked_list_add_tail(&(mylist.list), &(tem->list));
	}

	linked_list_for_each(pos, &mylist.list)
	{
		tem = struct_entry(pos, struct node, list);
		printf("%d", tem->num);
	}

	printf("\n");

	linked_list_for_each_safe(pos, n, &mylist.list)
	{
		tem = struct_entry(pos, struct node, list);
		if (tem->num == 5)
		{
			//
		}
	}

	linked_list_for_each_safe(pos, n, &mylist.list)
	{
		tem = struct_entry(pos, struct node, list);

	}

	return 0;
}