#include<stdio.h>
#include <stdlib.h>
#pragma warning(disable: 4996)

typedef struct node_t{
	int num_node;
	struct node_t* next_node;
}node_t;

typedef struct {
	int passed;
	node_t* head;
	node_t* tail;
}list_t;

typedef struct {
	int num_node;
	struct queue_t* next_num_node;
}queue_t;

void FreeList(list_t* list) {
	node_t* curr_node = list->head;
	while (curr_node != NULL)
	{
		list->head = list->head->next_node;
		free(curr_node);
		curr_node = list->head;
	}
}

void FreeQueue(queue_t* queue) {
	queue_t* curr_num = queue;
	while (curr_num != NULL)
	{
		queue = queue->next_num_node;
		free(curr_num);
		curr_num = queue;
	}
}

int ReadList(list_t list[], int number_of_vertices)
{
	if (number_of_vertices <= 0)
	{
		return 0;
	}

	for (int i = 0; i < number_of_vertices; i++)
	{
		char ch_tmp = 0;
		int num_tmp = 0;
		int num_main_vert = 0;
		int num_addi_vert = 0;
		int i_const = 0;

		scanf("%d%c", &num_tmp, &ch_tmp);

		while (ch_tmp == ' ')
		{
			//Если вершина имеет смежные вершины

			scanf("%d%c", &num_addi_vert, &ch_tmp);
			num_main_vert = i;

			for (int v = 0; v < 2; v++)
			{
				int replacing_vertices = 0;

				if (!list[num_main_vert].head)
				{
					list[num_main_vert].head = (node_t*)malloc(sizeof(node_t));
					if (!list[num_main_vert].head)
					{
						for (int j = 0; j < number_of_vertices; j++)
						{
							FreeList(&list[j]);
						}
						return 0;
					}
					list[num_main_vert].head->num_node = num_addi_vert;
					list[num_main_vert].head->next_node = NULL;

					list[num_main_vert].tail = (node_t*)malloc(sizeof(node_t));
					if (!list[num_main_vert].tail)
					{
						for (int j = 0; j < number_of_vertices; j++)
						{
							FreeList(&list[j]);
						}
						return 0;
					}
					list[num_main_vert].tail = list[num_main_vert].head;

				}
				else
				{
					//Т.к. цикл запускаем дважды, при этом меняя вершины местами,
					//то необходимо добавлять смежные вершины так, чтобы они шли по возрастанию
					if (list[num_main_vert].head->num_node > num_addi_vert)
					{
						node_t* new_num_node = (node_t*)malloc(sizeof(node_t));
						if (!new_num_node)
						{
							return 0;
						}
						new_num_node->num_node = num_addi_vert;
						new_num_node->next_node = list[num_main_vert].head;
						list[num_main_vert].head = new_num_node;
					}
					else
					{
						node_t* tmp_node = NULL;
						tmp_node = list[num_main_vert].head;
						while (tmp_node != NULL)
						{
							if (tmp_node->num_node < num_addi_vert)
							{
								if (tmp_node->next_node)
								{
									if (tmp_node->next_node->num_node > num_addi_vert)
									{
										node_t* new_num_node = (node_t*)malloc(sizeof(node_t));
										if (!new_num_node)
										{
											for (int j = 0; j < number_of_vertices; j++)
											{
												FreeList(&list[j]);
											}
											return 0;
										}
										new_num_node->num_node = num_addi_vert;
										new_num_node->next_node = tmp_node->next_node;
										tmp_node->next_node = new_num_node;
										break;
									}
								}
								else
								{
									tmp_node->next_node = (node_t*)malloc(sizeof(node_t));
									if (!tmp_node->next_node)
									{
										for (int j = 0; j < number_of_vertices; j++)
										{
											FreeList(&list[j]);
										}
										return 0;
									}
									tmp_node->next_node->num_node = num_addi_vert;
									tmp_node->next_node->next_node = NULL;
									list[num_main_vert].tail = tmp_node->next_node;
									break;
								}

							}
							tmp_node = tmp_node->next_node;
						}
					}

				}

				//меняем вершины местами и снова в цикл
				replacing_vertices = num_main_vert;
				num_main_vert = num_addi_vert;
				num_addi_vert = replacing_vertices;
			}
		}
	}
	return 1;
}

int BFS(list_t list[], int number_of_vertices)
{
	int i = 0;
	int items_added_to_the_queue = 0;
	int num_tmp = 0;
	int num_ver_tmp = 0;
	queue_t* number_of_nodes_traversed = NULL;
	node_t* curr_node;
	queue_t* queue = NULL;
	queue_t* queue_head = NULL;

	queue = (queue_t*)malloc(sizeof(queue_t));
	if (!queue)
	{
		return 0;
	}

	queue->num_node = i;
	queue->next_num_node = NULL;
	queue_head = queue;

	printf("%d", i);
	curr_node = list[i].head;
	list[i].passed = 1;

	number_of_nodes_traversed = queue;
	items_added_to_the_queue = 1;

	while (items_added_to_the_queue < number_of_vertices)
	{
		num_ver_tmp = number_of_nodes_traversed->num_node;
		curr_node = list[num_ver_tmp].head;
		while (curr_node)
		{
			num_tmp = curr_node->num_node;
			if (list[num_tmp].passed==0)
			{
				queue->next_num_node = (queue_t*)malloc(sizeof(queue_t));
				if (!queue->next_num_node)
				{
					FreeQueue(queue_head);
					return 0;
				}
				queue = queue->next_num_node;
				queue->num_node = num_tmp;
				queue->next_num_node = NULL;

				printf(" %d", num_tmp);

				list[num_tmp].passed = 1;
				items_added_to_the_queue++;
			}
			curr_node = curr_node->next_node;
		}
		if (!number_of_nodes_traversed->next_num_node)
		{
			break;
		}
		number_of_nodes_traversed = number_of_nodes_traversed->next_num_node;
	}
	FreeQueue(queue_head);
	return 1;
}

int main()
{
	int size = 0;
	int success = 0;
	list_t* list = NULL;
	scanf("%d", &size);
	
	list = (list_t*)malloc(sizeof(list_t) * size);
	if (!list)
	{
		return 0;
	}
	for (int i = 0; i < size; i++)
	{
		list[i].head = NULL;
		list[i].tail = NULL;
		list[i].passed = 0;
	}
	
	success = ReadList(list, size);
	if (success == 0)
	{
		free(list);
		return 0;
	}
	success = 0;
	success = BFS(list, size);
	if (success == 0)
	{
		free(list);
		return 0;
	}
	for (int j = 0; j < size; j++)
	{
		FreeList(&list[j]);
	}
	free(list);

	return 0;
}