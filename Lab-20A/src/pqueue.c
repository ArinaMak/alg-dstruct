#include <stdio.h>
#include <stdlib.h>
#include "pqueueinfo.h"
Queue_t* CreateQueue()
{
	Queue_t* tmp_queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!tmp_queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	List_t* element = (List_t*)malloc(sizeof(List_t));
	element->data = 0;
	element->priority = 0;
	element->next = NULL;
	tmp_queue->start = element;
	return tmp_queue;
}
int IsEmpty(Queue_t* queue)
{
	int a = 1;
	if (!queue)
	{
		a = 0;
		printf("Queue is empty\n");
	}
	return a;
}
void PushQueue(Queue_t* queue, int value, int priority)
{
	if ((!queue) || (!value) || (!priority))
	{
		printf("Error queue or value or priority is empty\n");
		exit(1);
	}
	if (queue->start->priority == 0)
	{
		queue->start->data = value;
		queue->start->priority = priority;
		return queue;
	}
	List_t* element = (List_t*)malloc(sizeof(List_t));
	if (!element)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	element->data = value;
	element->priority = priority;
	element->next = NULL;
	List_t* tmp_element = (List_t*)malloc(sizeof(List_t));
	List_t* tmp_element2 = (List_t*)malloc(sizeof(List_t));
	if ((!tmp_element) || (!tmp_element2))
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	tmp_element = queue->start;
	tmp_element->data = queue->start->data;
	tmp_element->next = queue->start->next;
	tmp_element->priority = queue->start->priority;
	tmp_element2 = queue->start->next;
	while (tmp_element != NULL)
	{
		//Предполагается, что самый большой приоритет имеет 1
		if (tmp_element2 != NULL)
		{
			//Если в очереди больше одного элемента
			if (tmp_element2->priority > element->priority)
			{
				element->next = tmp_element->next;
				tmp_element->next = element;
				break;
			}
			else
			{
				//Если новый элемент оказывается с самым маленьким приоритетом
				if (tmp_element2->next == NULL)
				{
					element->next = tmp_element2->next;
					tmp_element2->next = element;
					break;
				}
			}
			tmp_element = tmp_element->next;
			tmp_element2 = tmp_element2->next;
		}
		//Если в очереди один элемент
		else
		{
			if (tmp_element->priority > element->priority)
			{
				element->next = tmp_element;
				queue->start = element;
				break;
			}
			else
			{
				element->next = tmp_element->next;
				tmp_element->next = element;
				break;
			}
		}

	}
}

int PopQueue(Queue_t* queue)
{
	if (!queue)
	{
		printf("Error queue is empty\n");
		exit(1);
	}
	int a = queue->start->data;
	queue->start = queue->start->next;
	return a;
}


void DestroyQueue(Queue_t* queue)
{
	List_t* element = queue->start;
	List_t* tmp = NULL;
	while (tmp != NULL)
	{
		tmp = element;
		free(tmp);
		element = element->next;
	}
	free(queue);
}