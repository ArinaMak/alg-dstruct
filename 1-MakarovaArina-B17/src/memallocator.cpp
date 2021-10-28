#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include<stdlib.h>
#include "memallocator.h"

typedef struct descriptor_t descriptor_t;

struct descriptor_t {
	//Если - то блок свободен, если +, то занят
	int size;
	descriptor_t* next;
	descriptor_t* prev;
};

int overall_size = 0;
//направление - если 0, то поиск первого подходящего элемента происходит с начала, если 1 - то с конца
int direction = 0;
descriptor_t* head = NULL;
descriptor_t* tail = NULL;

int memgetminimumsize() {
	return sizeof(descriptor_t);
}
int memgetblocksize() {
	return sizeof(descriptor_t);
}

int meminit(void* pMemory, int size) {
	if ((size <= memgetminimumsize()) || (!pMemory)) {
		return 0;
	}

	head = (descriptor_t*)pMemory;
	head->size = -(size - memgetblocksize());
	head->next = NULL;
	head->prev = NULL;
	tail = head;

	overall_size = size;
	return 1;
}

void* memalloc(int size) {

	if (size <= 0 || !head)
	{
		return 0;
	}

	descriptor_t* first_suitable_ptr = NULL;
	descriptor_t* current = head;
	descriptor_t* new_ptr = NULL;
	int entire_size_of_the_first_suitable_free_block = 0;
	void* ptr;
	//Если ищем с конца
	if (direction == 1)
	{
		current = tail;
	}
	//Если ищем с начала
	else
	{
		current = head;
	}
	while (current)
	{
		//Если находится свободный блок подходящего размера
		if ((size <= abs(current->size)) && (current->size < 0))
		{
			first_suitable_ptr = current;
			entire_size_of_the_first_suitable_free_block = -first_suitable_ptr->size;

			if (first_suitable_ptr->next == NULL)
			{
				tail = first_suitable_ptr;
			}
			//Если найденый блок помимо введённого размера может вместить и доп.инф.
			if (entire_size_of_the_first_suitable_free_block - size > memgetblocksize())
			{
				//Делим найденый блок на два - один занят, другой свободен

				new_ptr = (descriptor_t*)((char*)first_suitable_ptr + size + memgetblocksize());
				new_ptr->prev = first_suitable_ptr;
				new_ptr->next = first_suitable_ptr->next;
				// "-" т.к. блок свободен
				new_ptr->size = -(entire_size_of_the_first_suitable_free_block - size - memgetblocksize());
				first_suitable_ptr->next = new_ptr;
				first_suitable_ptr->size = size;

				if (new_ptr->next == NULL)
				{
					tail = new_ptr;
				}
			}
			else
			{
				first_suitable_ptr->size = -first_suitable_ptr->size;
			}
			ptr = (void*)((char*)first_suitable_ptr + memgetblocksize());
			return ptr;
		}
		if (direction == 1)
		{
			current = current->prev;
		}
		else
		{
			current = current->next;
		}
	}
	if (direction == 1)
	{
		direction = 0;
	}
	else
	{
		direction = 1;
	}
	return 0;
}

void memfree(void* p) {
	if (!p || !head)
	{
		exit(1);
	}
	descriptor_t* free_tmp = NULL;
	free_tmp = (descriptor_t*)((char*)p - memgetblocksize());
	if (!((head <= free_tmp) && (free_tmp <= head + overall_size)))
	{
		exit(1);
	}
	//Если дискриптор занят 
	if (free_tmp->size > 0)
	{
		free_tmp->size = -free_tmp->size;
		//Проверка - свободен ли дискриптор слева
		if (free_tmp->prev && free_tmp->prev->size < 0)
		{
			descriptor_t* free_left_tmp = NULL;
			free_left_tmp = free_tmp->prev;
			//Минус учтён
			free_left_tmp->size = free_left_tmp->size + (free_tmp->size - memgetblocksize());

			if (free_left_tmp->next)
			{
				if (free_left_tmp->next->next)
				{
					free_left_tmp->next->next->prev = free_left_tmp;
					free_left_tmp->next = free_left_tmp->next->next;
				}
				else
				{
					tail = free_left_tmp->next;
					free_left_tmp->next = NULL;
				}
			}
			else
			{
				tail = free_left_tmp;
			}
			free_tmp = free_left_tmp;
		}
		//Проверка - свободен ли дискриптор справа
		if (free_tmp->next && free_tmp->next->size < 0)
		{
			descriptor_t* free_right_tmp = NULL;

			free_right_tmp = free_tmp->next;
			//Минус учтён
			free_tmp->size = free_tmp->size + (free_tmp->next->size - memgetblocksize());
			if (free_tmp->next)
			{
				if (free_tmp->next->next)
				{
					free_tmp->next->next->prev = free_tmp;
					free_tmp->next = free_tmp->next->next;
				}
				else
				{
					tail = free_tmp->next;
					free_tmp->next = NULL;
				}
			}
			else
			{
				tail = free_tmp;
			}
		}

	}
}

void memdone() {
	descriptor_t* curr = head;
	while (curr)
	{
		if (curr->size > 0)
		{
			printf("Memory leak");
		}
		curr = curr->next;
	}

	curr = NULL;
	head = NULL;
	overall_size = 0;
}

#ifdef __cplusplus
}
#endif