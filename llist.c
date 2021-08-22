#include "llist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

void llpush(llist** head, void* data_ptr)
{
	if (head == NULL) {
		fprintf(stderr, "List pointer is NULL.");
		return;
	}

	llist* new = malloc(sizeof(llist));

	if (new == NULL) {
		fprintf(stderr, "Allocation error.");
		return;
	}
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

	if (new == NULL) {
		fprintf(stderr, "Allocation error.");
		return;
	}

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