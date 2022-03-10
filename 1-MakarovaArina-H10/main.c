#pragma warning(disable: 4996)
#include<stdio.h>
#include <stdlib.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

typedef struct tree_t
{
	int key;
	int height;
	struct tree_t* left;
	struct tree_t* right;
}tree_t;
/*
void MemoryLeaks(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}
*/
tree_t* CreateTree()
{
	tree_t* tree = NULL;
	tree = (tree_t*)malloc(sizeof(tree_t));
	if (tree == NULL)
	{
		//printf("error in Add\n");
		exit(1);
	}
	tree->height = 1;
	tree->key = -1;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}
int FindHeight(tree_t* tree)
{
	if (!tree) return 0;
	int height_l = 0;
	int height_r = 0;
	if (tree->left) height_l = tree->left->height;
	if (tree->right) height_r = tree->right->height;
	if (height_l > height_r) return height_l + 1;
	else return height_r + 1;
}
void HeightLeftRight(tree_t* tree, int* height_l, int* height_r)
{
	*height_l = 0;
	*height_r = 0;
	if (tree)
	{
		if (tree->left) *height_l = tree->left->height;
		if (tree->right) *height_r = tree->right->height;
	}
}
tree_t* LeftRotate(tree_t* tree)
{
	tree_t* tmp_x = tree;
	tree_t* tmp_y = tmp_x->right;
	tmp_x->right = tmp_y->left;
	tmp_y->left = tmp_x;
	tmp_x->height = FindHeight(tmp_x);
	tmp_y->height = FindHeight(tmp_y);
	return tmp_y;
}
tree_t* RightRotate(tree_t* tree)
{
	tree_t* tmp_y = tree;
	tree_t* tmp_x = tmp_y->left;
	tmp_y->left = tmp_x->right;
	tmp_x->right = tmp_y;
	tmp_y->height = FindHeight(tmp_y);
	tmp_x->height = FindHeight(tmp_x);
	return tmp_x;
}
tree_t* BalancingSubtree(tree_t* tree)
{
	int height_l = 0;
	int height_r = 0;
	HeightLeftRight(tree, &height_l, &height_r);

	if (height_l - height_r == 2)
	{
		int hl_l = 0;
		int hl_r = 0;
		HeightLeftRight(tree->left, &hl_l, &hl_r);
		if (hl_l - hl_r < 0) tree->left = LeftRotate(tree->left);
		return RightRotate(tree);
	}
	if (height_r - height_l == 2)
	{
		int hr_l = 0;
		int hr_r = 0;
		HeightLeftRight(tree->right, &hr_l, &hr_r);
		if (hr_r - hr_l < 0) tree->right = RightRotate(tree->right);
		return LeftRotate(tree);
	}
	return tree;
}
void AddNew(tree_t** tree_ptr, int x)
{
	tree_t* tree1;
	if (tree_ptr == NULL)
	{
		//printf("\nERROR\n");
		exit(1);
	}
	tree1 = (*tree_ptr);
	if (tree1 == NULL)
	{
		tree1 = CreateTree();
		tree1->key = x;
		(*tree_ptr) = tree1;
	}
	else
	{
		if (x < (*tree_ptr)->key)
		{
			AddNew(&((*tree_ptr)->left), x);
		}
		else
		{
			if (x > (*tree_ptr)->key) AddNew(&((*tree_ptr)->right), x);
		}
	}
	(*tree_ptr)->height = FindHeight(*tree_ptr);
	(*tree_ptr) = BalancingSubtree(*tree_ptr);
}
void RemovingAndFindingTheMinimumElementInTheRightSubtree(tree_t** tree_ptr, tree_t** tree_min_r_ptr)
{
	if ((*tree_ptr)->left)
	{
		RemovingAndFindingTheMinimumElementInTheRightSubtree(&((*tree_ptr)->left), tree_min_r_ptr);
	}
	else
	{
		(*tree_min_r_ptr) = (*tree_ptr);
		(*tree_ptr) = (*tree_ptr)->right;
	}


	if (*tree_ptr)
	{
		(*tree_ptr)->height = FindHeight(*tree_ptr);
		(*tree_ptr) = BalancingSubtree(*tree_ptr);
	}
}
void RemoveElement2(tree_t** tree_ptr, int x)
{
	if (*tree_ptr)
	{
		if (x == (*tree_ptr)->key)
		{
			tree_t* tmp = (*tree_ptr);
			tree_t* tmp_l = (*tree_ptr)->left;
			tree_t* tmp_r = (*tree_ptr)->right;
			if (tmp->height == 1)
			{
				free(tmp);
				(*tree_ptr) = NULL;
			}
			else
			{
				if (!tmp_r)
				{
					(*tree_ptr) = (*tree_ptr)->left;
					free(tmp);
				}
				else
				{
					tree_t* new_tree_ptr = (*tree_ptr);
					RemovingAndFindingTheMinimumElementInTheRightSubtree(&((*tree_ptr)->right), &new_tree_ptr);
					tree_t* tmp_l2 = (*tree_ptr)->left;
					tree_t* tmp_r2 = (*tree_ptr)->right;
					new_tree_ptr->left = tmp_l2;
					new_tree_ptr->right = tmp_r2;
					new_tree_ptr->height = FindHeight(new_tree_ptr);
					new_tree_ptr = BalancingSubtree(new_tree_ptr);
					(*tree_ptr) = new_tree_ptr;
					free(tmp);
				}
			}
		}
		else
		{
			if (x < (*tree_ptr)->key) RemoveElement2(&((*tree_ptr)->left), x);
			else RemoveElement2(&((*tree_ptr)->right), x);
			(*tree_ptr)->height = FindHeight(*tree_ptr);
			(*tree_ptr) = BalancingSubtree(*tree_ptr);
		}
	}
}
void DestroyTree(tree_t* tree)
{
	if (tree)
	{
		DestroyTree(tree->left);
		DestroyTree(tree->right);
		free(tree);
	}
}
int SearchElement(tree_t* tree, int x)
{
	tree_t* tmp = tree;
	while (tmp)
	{
		if (x == tmp->key) return 1;
		if (x < tmp->key) tmp = tmp->left;
		else tmp = tmp->right;
	}
	return 0;
}
int Solution(FILE* f_in, FILE* f_out)
{
	char buf[10] = { 0 };
	tree_t** tree_ptr = (tree_t**)malloc(sizeof(tree_t*));
	(*tree_ptr) = NULL;
	char ch;
	int value;
	while (fgets(buf, 10, f_in))
	{
		sscanf(buf, "%c %i", &ch, &value);
		if (value < 0)
		{
			DestroyTree(*tree_ptr);
			free(tree_ptr);
			return 0;
		}
		switch (ch) {
		case 'a':
			AddNew(tree_ptr, value);
			break;
		case 'r':
			RemoveElement2(tree_ptr, value);
			break;
		case 'f':
			if (SearchElement(*tree_ptr, value))
				fprintf(f_out, "yes\n");
			else
				fprintf(f_out, "no\n");
			break;
		default:
			DestroyTree(*tree_ptr);
			free(tree_ptr);
			return 0;
		}
	}
	DestroyTree(*tree_ptr);
	free(tree_ptr);
	return 0;
}

int main()
{
	Solution(stdin, stdout);
	//MemoryLeaks();
	return 0;
}
