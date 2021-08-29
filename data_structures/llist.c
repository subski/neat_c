#include "data_structures/llist.h"

#include "tools/malloc_dbg.h"
#include "tools/utils.h"

void llpush(llist** head, void* data_ptr)
{
	if (head == NULL) {
		fprintf(stderr, "List pointer is NULL.");
		return;
	}

	llist* new = malloc(sizeof(llist));
	MCHECK(new, );

	new->next = *head;
	new->data = data_ptr;
	*head = new;
}

void llappend(llist* node, void* data_ptr)
{
	if (node == NULL) {
		fprintf(stderr, "List pointer is NULL.");
		return;
	}

	while (node->next != NULL)
	{
		node = node->next;
	}

	llist* new = malloc(sizeof(llist));
	MCHECK(new, );

	new->data = data_ptr;
	new->next = NULL;
	node->next = new;
}

void lldestroy_list(llist** head)
{
	llist* current = *head;
	llist* next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}