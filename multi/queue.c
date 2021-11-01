#include "main.h"

void	push(Queue **head, Queue **new_node)
{
	(*new_node)->next = NULL;
	if (*head)
	{
		 (*head)->last->next = *new_node;
		 (*head)->last = *new_node;
		 (*new_node)->last = *head;
	}
	else
	{
		*head = *new_node;
		(*head)->last = *head;
	}
}

Queue	*pop(Queue **head) // i will change it to retutn first elmenet from void to Queue*
{
	Queue *tmp = NULL;
	if (*head)
	{
		tmp = *head; // to save last pointer
		*head = (*head)->next; // move to the next
		if (*head) {
			(*head)->last = tmp->last; // change that last 
			(*head)->last->last = *head; // here update the last node to pointing to the new head
		}
		// else means that queue is empty
	}
	return tmp;
}

int	empty(Queue *head)
{
	if (!head)
		return 1;
	return 0;
}

Queue	*back(Queue *head)
{
	return head->last;
}
