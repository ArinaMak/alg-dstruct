#pragma warning(disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
typedef struct tagList {
	int value;
	struct tagList* next;
} tagList_t;
typedef struct tagGraph {
	int numVertices;
	//int count;
	int* passed;
	tagList_t** verticeNeighbours;
}tagGraph_t;
typedef struct tagQueue {
	int length;
	tagList_t* head;
	tagList_t* tail;
}tagQueue_t;

int success = 0;
void MemoryLeaks(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}
tagGraph_t* CreateGraph(tagGraph_t* graph, int num_vertices)
{
	graph->numVertices = num_vertices;
	graph->passed = NULL;
	graph->verticeNeighbours = NULL;
	graph->passed = (int*)malloc(num_vertices * sizeof(int));
	if (!graph->passed)
	{
		printf("\nError in CreateGraph\n");
		return NULL;
	}
	graph->verticeNeighbours = (tagList_t**)malloc(num_vertices * sizeof(tagList_t*));
	if (!graph->verticeNeighbours)
	{
		printf("\nError in CreateGraph\n");
		return NULL;
	}
	for (int i = 0; i < num_vertices; i++)
	{
		graph->passed[i] = 0;
		graph->verticeNeighbours[i] = NULL;
	}
	return graph;
}
void FreeList(tagList_t* list)
{
	tagList_t* curr = list;
	while (curr != NULL)
	{
		list = list->next;
		free(curr);
		curr = list;
	}
}
void PrintQueue(tagQueue_t* queue)
{
	tagList_t* curr = queue->head;
	while (curr != NULL)
	{
		printf("%d ", curr->value + 1);
		curr = curr->next;
	}
}

void FreeWrongWay(tagList_t* list, tagGraph_t* graph)
{
	tagList_t* curr = list;
	while (curr != NULL)
	{
		list = list->next;
		graph->passed[curr->value] = 0;
		free(curr);
		curr = list;
	}
}

tagList_t* SearchPtrQueue(tagQueue_t* queue, int vertex)
{
	if (!queue || !queue->head)
	{
		printf("Error in SearchPtrQueue");
		return NULL;
	}
	int count = 1;
	tagList_t* curr = queue->head;
	if (curr->next!=NULL)
	{
		while (curr->next->value != vertex)
		{
			count += 1;
			curr = curr->next;
		}
		queue->tail = curr;
		curr = curr->next;
		queue->tail->next = NULL;
		queue->length = count;
	}
	return curr;
}

void FreeAllQueue(tagQueue_t* queue) {
	tagList_t* curr = queue->head;
	while (curr != NULL)
	{
		queue->head = queue->head->next;
		free(curr);
		curr = queue->head;
	}
}

void FreeGraph(tagGraph_t* graph)
{
	for (int i = 0; i < graph->numVertices; i++)
	{
		FreeList(graph->verticeNeighbours[i]);
	}
	free(graph->verticeNeighbours);
	free(graph->passed);
	free(graph);
}

tagList_t* CreateAdjVert(int vertex)
{
	tagList_t* new_vertex = (tagList_t*)malloc(sizeof(tagList_t));
	if (!new_vertex)
	{
		printf("\nError in  CreateAdjVert\n");
		return NULL;
	}
	new_vertex->value = vertex;
	new_vertex->next = NULL;
	return new_vertex;
}

void AddNewVert(tagGraph_t* graph, int vertex1, int vertex2)
{
	tagList_t* new_vertex = CreateAdjVert(vertex2);
	new_vertex->next = graph->verticeNeighbours[vertex1];
	graph->verticeNeighbours[vertex1] = new_vertex;

	new_vertex = CreateAdjVert(vertex1);
	new_vertex->next = graph->verticeNeighbours[vertex2];
	graph->verticeNeighbours[vertex2] = new_vertex;
}

void ReadFromFile2(tagGraph_t* graph, int* s, int* t, int* k)
{
	char ch = -1;
	int tmp_vertices = 0;
	FILE* file = fopen("input.txt", "r");
	if (!file) {
		printf("Error opening input file\n");
		fclose(file);
		return 0;
	}
	if (fscanf(file, "%d%d%d%d", &tmp_vertices, s, t, k) < 0)
	{
		printf("Error reading file\n");
		fclose(file);
		return 0;
	}
	graph = CreateGraph(graph, tmp_vertices);

	if (fscanf(file, "%c", &ch) < 0)
	{
		printf("Error reading file\n");
		fclose(file);
		return 0;
	}
	while (ch != '\0')
	{
		int vertex = 0;
		int vertex2 = 0;
		if (fscanf(file, "%d", &vertex) < 0)
		{
			printf("Error reading file\n");
			fclose(file);
			return 0;
		}
		if (fscanf(file, "%c", &ch) < 0)
		{
			printf("Error reading file\n");
			fclose(file);
			return 0;
		}
		while (ch != '\n')
		{
			if (fscanf(file, "%d", &vertex2) < 0)
			{
				printf("Error reading file\n");
				fclose(file);
				return 0;
			}
			AddNewVert(graph, vertex - 1, vertex2 - 1);
			if (fscanf(file, "%c", &ch) < 0)
			{
				printf("Error reading file\n");
				fclose(file);
				return 0;
			}
		}
	}
	fclose(file);
}

void PrintGraph(tagGraph_t* graph)
{
	for (int i = 0; i < graph->numVertices; i++)
	{
		tagList_t* curr = NULL;
		curr = graph->verticeNeighbours[i];
		while (curr != NULL)
		{
			curr = curr->next;
		}
	}
}

void TheLongestWayFinal(tagGraph_t* graph, tagQueue_t* queue, int vertex, int k, int t)
{
	tagList_t* adjList = graph->verticeNeighbours[vertex];
	tagList_t* curr = adjList;
	tagQueue_t* tmp = NULL;
	graph->passed[vertex] = 1;
	if (queue->head == NULL)
	{
		queue->head = (tagList_t*)malloc(sizeof(tagList_t));
		if (!queue->head)
		{
			printf("Error");
			return 0;
		}
		queue->head->value = vertex;
		queue->head->next = NULL;
		queue->tail = queue->head;
		queue->length = 1;
	}
	else
	{
		queue->tail->next = (tagList_t*)malloc(sizeof(tagList_t));
		if (!queue->tail->next)
		{
			FreeAllQueue(queue);
			return NULL;
		}
		queue->length += 1;
		queue->tail->next->value = vertex;
		queue->tail = queue->tail->next;
		queue->tail->next = NULL;
	}
	while (curr != NULL)
	{
		int curr_value = curr->value;
		if (graph->passed[curr_value] == 0)
		{
			if (curr_value == t)
			{
				graph->passed[curr_value] = 1;
				queue->tail->next = (tagList_t*)malloc(sizeof(tagList_t));
				if (!queue->tail->next)
				{
					FreeAllQueue(queue);
					return NULL;
				}
				queue->length += 1;
				queue->tail->next->value = curr_value;
				queue->tail = queue->tail->next;
				queue->tail->next = NULL;
				if (queue->length - 1 >= k)
				{
					success = 1;
					return 0;
				}

			}
			else
			{
				TheLongestWayFinal(graph, queue, curr_value, k, t);
				if (success == 1)
				{
					return 0;
				}
			}
			tmp = SearchPtrQueue(queue, curr->value);
			FreeWrongWay(tmp, graph);
		}
		curr = curr->next;
	}
}
void WriteFile(tagQueue_t* queue) {
	FILE* file = fopen("output.txt", "w");
	if (!file) {
		printf("Error opening file\n");
		exit(1);
	}
	if (success == 0)
	{
		fprintf(file, "%d", 0);
	}
	else
	{
		tagList_t* curr = queue->head;
		while (curr != NULL)
		{
			fprintf(file, "%d ", curr->value + 1);
			curr = curr->next;
		}
		FreeAllQueue(queue);
	}
	fclose(file);
}
int main()
{
	tagGraph_t* graph;
	tagQueue_t* queue = NULL;
	int s, t, k;

	graph = (tagGraph_t*)malloc(sizeof(tagGraph_t));
	if (!graph)
	{
		return 0;
	}
	queue = (tagQueue_t*)malloc(sizeof(tagQueue_t));
	if (!queue)
	{
		free(graph);
		return 0;
	}
	ReadFromFile2(graph, &s, &t, &k);
	PrintGraph(graph);
	queue->head = NULL;
	TheLongestWayFinal(graph, queue, s - 1, k, t - 1);
	PrintQueue(queue);
	WriteFile(queue);
	FreeGraph(graph);
	free(queue);
	MemoryLeaks();
	return 0;
}