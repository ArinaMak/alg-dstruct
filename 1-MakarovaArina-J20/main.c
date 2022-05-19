#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

#define fail -1
#define hash_table_size 100000
typedef struct node_t
{
	char* data_string;
	struct node_t* next;
}node_t;
/*
void MemoryLeaks(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}
*/
unsigned int AssigningIndexToHashTable(char* str)
{
	if (!str) return fail;
	unsigned int counting_index = 0;
	for (int i = 0; str[i] != '\0'; i++) counting_index = counting_index * 16 + str[i];
	return counting_index % hash_table_size;
}
int FindString(node_t** table, char* str)
{
	unsigned int index = AssigningIndexToHashTable(str);
	node_t* curr_node = table[index];
	while (curr_node)
	{
		if (strcmp(str, curr_node->data_string) == 0) return 1;
		else curr_node = curr_node->next;
	}
	return 0;
}
int AddString(node_t** table, char* str)
{
	if (!str || !table) return 0;
	//If found, it is not necessary to add, the function succeeds
	if (FindString(table, str)) return 1;

	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	if (!new_node) return 0;

	new_node->data_string = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	if (!new_node->data_string) return 0;

	strcpy(new_node->data_string, str);
	unsigned int index = AssigningIndexToHashTable(str);
	new_node->next = table[index];
	table[index] = new_node;

	return 1;
}
int RemoveString(node_t** table, char* str)
{
	if (!str || !table) return 0;
	//If we didn't find it, we don't need to delete it, the function succeeds
	if (!FindString(table, str)) return 1;

	unsigned int index = AssigningIndexToHashTable(str);
	node_t* tmp_node = table[index];
	node_t* prev_node = tmp_node;
	while (tmp_node)
	{
		if (strcmp(str, tmp_node->data_string) == 0) break;
		prev_node = tmp_node;
		tmp_node = tmp_node->next;
	}
	if (prev_node == tmp_node) table[index] = tmp_node->next;
	else prev_node->next = tmp_node->next;
	free(tmp_node->data_string);
	free(tmp_node);
}
void DestroyTable(node_t** table)
{
	if (table)
	{
		for (int i = 0; i < hash_table_size; i++)
		{
			if (table[i])
			{
				//remove list
				node_t* tmp = table[i];
				while (table[i])
				{
					table[i] = tmp->next;
					free(tmp->data_string);
					free(tmp);
					tmp = table[i];
				}
			}
		}
	}
}
int MainInput(FILE* in, FILE* out)
{
	node_t* table[hash_table_size] = { 0 };
	char buf[16] = { 0 };
	char c = 0;
	char str[10000] = { 0 };
	int index = -1;
	while (fgets(buf, 16, in)) {
		sscanf(buf, "%c %s", &c, &str);
		switch (c) {
		case 'a':
			AddString(table, str);
			break;
		case 'r':
			RemoveString(table, str);
			break;
		case 'f':
			fprintf(out, "%s\n", FindString(table, str) ? "yes" : "no");
			break;
		default:
			DestroyTable(table);
			return 0;
		}
	}
	DestroyTable(table);
	return 0;
}
int main()
{
	MainInput(stdin, stdout);
	//MemoryLeaks();
	return 0;
}