#pragma warning(disable: 4996)
#include<stdio.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include<malloc.h>
typedef struct  tree_t{
	int key;
	int number;
	struct tree_t* left;
	struct tree_t* right;
}tree_t;
int reached_k_element = 0;
int k_el = 0;
void MemoryLeaks(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}
tree_t* ReadTree2(FILE* f) 
{
	char ch_l, ch_r;
	char ch1;
	int key_tree = 0;
	int num = 0;
	tree_t* t = NULL;

	fscanf(f, "%d", &key_tree);
	fscanf(f, "%d", &num);
	fscanf(f, "%c", &ch1);
	fscanf(f, "%c", &ch_l);
	fscanf(f, "%c", &ch1);
	fscanf(f, "%c", &ch_r);
	if (!feof(f))
	{
		fscanf(f, "%c", &ch1);
	}
	
	t = (tree_t*)malloc(sizeof(tree_t));
	if (!t)
	{
		return NULL;
	}
	t->key = key_tree;
	t->number = num;
	t->left = NULL;
	t->right = NULL;
	if (ch_l != '-')
		t->left = ReadTree2(f);
	if (ch_r != '-')
		t->right = ReadTree2(f);
	return t;
}
void FreeTree2(tree_t* t)
{
	if (t != NULL)
	{
		FreeTree2(t->left);
		FreeTree2(t->right);
	}
	free(t);
}
tree_t* KMinElement(tree_t* t, int k)
{
	int r;
	if (!t)
	{
		return NULL;
	}
	if (t->left)
	{
		r = t->left->number + 1;
	}
	else
		r = 1;
	if (r != k)
	{
		if (k < r)
			t = KMinElement(t->left, k);
		else
			t = KMinElement(t->right, k - r);
	}
	if (!t)
	{
		return NULL;
	}
	if(t->key%2==0)
		return t;
	else
		return NULL;
}
void PrintTree6(tree_t* t_main, int k, FILE* f)
{
	if (t_main != NULL)
	{
		PrintTree6(t_main->left, k, f);
		k_el++;
		if (k_el < k)
		{
			fprintf(f, "%d ", t_main->key);
		}
		PrintTree6(t_main->right, k, f);
	}
}
void WriteFile(tree_t* t_main, int k) {
	FILE* file = fopen("output.txt", "w");
	if (!file) {
		printf("Error opening file\n");
		exit(1);
	}
	if (reached_k_element == 1)
	{
		PrintTree6(t_main->right, k, file);
	}
	else
	{
		fprintf(file, "%d", 0);
	}
	
	fclose(file);
}
int main()
{
	int k = 0;
	
	tree_t* t = NULL;
	tree_t* t2 = NULL;
	FILE* file = fopen("input.txt", "r");
	if (!file) {
		printf("Error opening input file\n");
		fclose(file);
		return 0;
	}
	if (fscanf(file, "%d", &k))
	{
		t = ReadTree2(file);
		t2 = t;
		t2 = KMinElement(t2, k);
		if (t2 != NULL)
		{
			reached_k_element = 1;
		}
		WriteFile(t, k);
		fclose(file);
		FreeTree2(t);
		MemoryLeaks();
	}
	fclose(file);
	return 0;
}