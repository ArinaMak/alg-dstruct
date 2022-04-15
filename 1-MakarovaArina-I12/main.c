#pragma warning(disable: 4996)
#include<stdio.h>
#include <stdbool.h>
#include <stdlib.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
#define t 2

#define maxindexkey 2*t-2
#define maxquantitykeys 2*t-1
#define maxindexchild 2*t-1
#define maxquantitychildren 2*t
#define not_found -1
/*
void MemoryLeaks(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}
*/
typedef struct btree_t {
	int keys[2 * t - 1];
	struct btree_t* children[2 * t];
	int quantitykeys;
}btree_t;

btree_t* CreateTree();
void DestroyTree(btree_t* tree);
int FindIndexValueInKeys(btree_t* tree, int value);
int FindIndexChildNextWay1(btree_t* tree, int value);
int AddingValueToAnIncompleteArray1(btree_t** tree_ptr, int value);
void SplittingTheTree1(btree_t* tree, btree_t** left_ptr, btree_t** right_ptr, int* hight_key);
void FixingTheTree1(btree_t** tree_ptr, int value);
void AddingValue1(btree_t** tree_ptr, int value);
void LeftRotate(btree_t** tree_ptr, int index);
void RightRotate(btree_t** tree_ptr, int index);
int FindingTheMinimumElementInTheRightSubtree(btree_t* tree, int index_tree);
int FindingTheMaximumElementInTheLeftSubtree(btree_t* tree, int index_tree);
btree_t* FindTreeAndIndex(btree_t* tree, int* index, int value);
void ConnectingLeftAndRightChildren3(btree_t** tree_ptr, int index);
void DeleteFromList3(btree_t** tree_ptr, int value);
void DeleteFromNode3(btree_t** tree_ptr, btree_t* desired_node, int index_desired_node, int value);
void DeleteValue3(btree_t** tree_ptr, int value);
int TestSystemMainCycle(FILE* in, FILE* out);

int main()
{
	TestSystemMainCycle(stdin, stdout);
	//MemoryLeaks();
	return 0;
}

btree_t* CreateTree()
{
	btree_t* tree = (btree_t*)(malloc(sizeof(btree_t)));
	if (!tree) return NULL;
	tree->quantitykeys = 0;
	for (int i = 0; i < 2 * t - 1; i++)
		tree->children[i] = NULL;
	return tree;
}
btree_t** head_btree_ptr = NULL;
void DestroyTree(btree_t* tree)
{
	if (tree)
	{
		for (int i = 0; i < tree->quantitykeys + 1; i++)
		{
			DestroyTree(tree->children[i]);
		}
		free(tree);
	}
}
int FindIndexValueInKeys(btree_t* tree, int value)
{
	for (int i = 0; i < tree->quantitykeys; i++)
	{
		if (value == tree->keys[i]) return i;
	}
	return not_found;
}
//The function finds the index of the child where to go down
int FindIndexChildNextWay1(btree_t* tree, int value)
{
	if (!tree) return not_found;
	if (tree->children[0])
	{
		int index_child = 0;
		while (index_child < tree->quantitykeys && tree->keys[index_child] < value)
		{
			index_child++;
		}
		return index_child;
	}
	return not_found;
}
//The function runs through the values in the array and inserts an element
//The output is the index of the inserted element
int AddingValueToAnIncompleteArray1(btree_t** tree_ptr, int value)
{
	if ((*tree_ptr)->quantitykeys < maxquantitykeys)
	{
		int index = (*tree_ptr)->quantitykeys;
		(*tree_ptr)->children[index + 1] = (*tree_ptr)->children[index];
		while (index > 0 && (*tree_ptr)->keys[index - 1] > value)
		{
			(*tree_ptr)->keys[index] = (*tree_ptr)->keys[index - 1];
			(*tree_ptr)->children[index] = (*tree_ptr)->children[index - 1];
			index--;
		}
		(*tree_ptr)->keys[index] = value;
		(*tree_ptr)->quantitykeys++;
		return index;
	}
	return -1;
}
void SplittingTheTree1(btree_t* tree, btree_t** left_ptr, btree_t** right_ptr, int* hight_key)
{
	if (tree->quantitykeys == maxquantitykeys)
	{
		btree_t* new_tree = CreateTree();
		btree_t* new_tree_left = CreateTree();
		int index = t - 1; //the number of the item that will go up
		for (int i = 0; i < t - 1; i++)
		{
			new_tree_left->keys[i] = tree->keys[i];
			new_tree_left->children[i] = tree->children[i];
			tree->children[i] = NULL;
			tree->keys[i] = 0;
		}
		new_tree_left->children[t - 1] = tree->children[t - 1];
		tree->children[t - 1] = NULL;
		for (int i = t; i <= maxindexkey; i++)
		{
			new_tree->keys[i - t] = tree->keys[i];
			new_tree->children[i - t] = tree->children[i];
			tree->children[i] = NULL;
			tree->keys[i] = 0;
		}
		new_tree_left->quantitykeys = t - 1;
		new_tree->children[t - 1] = tree->children[maxindexchild];
		tree->children[maxindexchild] = NULL;
		new_tree->quantitykeys = t - 1;
		(*hight_key) = tree->keys[index];
		tree->keys[index] = 0;
		(*left_ptr) = new_tree_left;
		(*right_ptr) = new_tree;
	}
}
//The function corrects the node
void FixingTheTree1(btree_t** tree_ptr, int value)
{
	btree_t* tree = (*tree_ptr);
	btree_t** left_ptr = (btree_t**)(malloc(sizeof(btree_t*)));
	btree_t** right_ptr = (btree_t**)(malloc(sizeof(btree_t*)));
	(*left_ptr) = NULL;
	(*right_ptr) = NULL;
	int hight_key = not_found;
	//if the main node in the tree
	if (head_btree_ptr == tree_ptr)
	{
		if ((*tree_ptr)->quantitykeys == maxquantitykeys)
		{
			SplittingTheTree1(tree, left_ptr, right_ptr, &hight_key);
			btree_t* left = (*left_ptr);
			btree_t* right = (*right_ptr);
			(*head_btree_ptr)->children[0] = left;
			(*head_btree_ptr)->children[1] = right;
			(*head_btree_ptr)->keys[0] = hight_key;
			(*head_btree_ptr)->quantitykeys = 1;
		}
		if (tree->children[0])
		{
			int index_child = FindIndexChildNextWay1((*tree_ptr), value);
			FixingTheTree1(&((*tree_ptr)->children[index_child]), value);
		}
	}
	if (!tree || tree->quantitykeys < 0) return;
	if (tree->children[0])
	{
		int index_child = FindIndexChildNextWay1((*tree_ptr), value);
		btree_t* tree_child = tree->children[index_child];
		if (tree_child->quantitykeys == maxquantitykeys)
		{
			SplittingTheTree1(tree_child, left_ptr, right_ptr, &hight_key);
			free(tree_child);
			int index = AddingValueToAnIncompleteArray1(tree_ptr, hight_key);
			(*tree_ptr)->children[index] = (*left_ptr);
			(*tree_ptr)->children[index + 1] = (*right_ptr);
		}
	}
	free(left_ptr);
	free(right_ptr);
}
void AddingValue1(btree_t** tree_ptr, int value)
{
	if (!(*tree_ptr))
	{
		(*tree_ptr) = CreateTree();
		(*tree_ptr)->keys[0] = value;
		(*tree_ptr)->quantitykeys = 1;
	}
	else
	{
		int idx = -1;
		btree_t* tmp1 = FindTreeAndIndex((*tree_ptr), &idx, value);
		if (idx != not_found) return;
		FixingTheTree1(tree_ptr, value);
		if ((*tree_ptr)->children[0])
		{
			int index_child = FindIndexChildNextWay1((*tree_ptr), value);
			AddingValue1(&((*tree_ptr)->children[index_child]), value);
		}
		else AddingValueToAnIncompleteArray1(tree_ptr, value);
	}
}
void LeftRotate(btree_t** tree_ptr, int index)
{
	//delete from the left node
	btree_t* left = (*tree_ptr)->children[index];
	btree_t* right = (*tree_ptr)->children[index + 1];
	left->keys[left->quantitykeys] = (*tree_ptr)->keys[index];
	left->children[left->quantitykeys + 1] = right->children[0];
	left->quantitykeys++;
	(*tree_ptr)->keys[index] = right->keys[0];
	int i = 0;
	for (i = 0; i < right->quantitykeys - 1; i++)
	{
		right->keys[i] = right->keys[i + 1];
		right->children[i] = right->children[i + 1];
	}
	right->keys[i] = 0;
	right->children[i] = right->children[i + 1];
	right->quantitykeys--;
}
void RightRotate(btree_t** tree_ptr, int index)
{
	//delete from the right node
	btree_t* left = (*tree_ptr)->children[index];
	btree_t* right = (*tree_ptr)->children[index + 1];
	int i = 0;
	right->children[right->quantitykeys + 1] = right->children[right->quantitykeys];
	for (i = right->quantitykeys; i >= 1; i--)
	{
		right->keys[i] = right->keys[i - 1];
		right->children[i] = right->children[i - 1];
	}
	right->keys[0] = (*tree_ptr)->keys[index];
	right->children[0] = left->children[left->quantitykeys + 1];
	right->quantitykeys++;
	(*tree_ptr)->keys[index] = left->keys[left->quantitykeys - 1];
	left->quantitykeys--;
}
int FindingTheMinimumElementInTheRightSubtree(btree_t* tree, int index_tree)
{
	tree = tree->children[index_tree + 1];
	while (tree->children[0]) tree = tree->children[0];
	return tree->keys[0];
}
int FindingTheMaximumElementInTheLeftSubtree(btree_t* tree, int index_tree)
{
	tree = tree->children[index_tree];
	while (tree->children[0]) tree = tree->children[tree->quantitykeys];
	return tree->keys[tree->quantitykeys - 1];
}
btree_t* FindTreeAndIndex(btree_t* tree, int* index, int value)
{
	int index_value = -1;
	if (!tree || tree->quantitykeys < 0)
	{
		(*index) = not_found;
		return NULL;
	}
	while (tree->children[0])
	{
		index_value = FindIndexValueInKeys(tree, value);
		int index_child = FindIndexChildNextWay1(tree, value);
		if (index_value != not_found)
		{
			(*index) = index_value;
			return tree;
		}
		tree = tree->children[index_child];
	}
	index_value = FindIndexValueInKeys(tree, value);
	if (index_value != not_found)
	{
		(*index) = index_value;
		return tree;
	}
	else return NULL;
}
void ConnectingLeftAndRightChildren3(btree_t** tree_ptr, int index)
{
	if (!tree_ptr) return;
	btree_t* tree = (*tree_ptr);
	if (!tree) return;
	//The extra element has already been removed
	btree_t* left = tree->children[index];
	btree_t* right = tree->children[index + 1];
	if (left->quantitykeys + right->quantitykeys + 1 <= maxquantitykeys)
	{
		int i = 0;
		left->keys[left->quantitykeys] = tree->keys[index];
		for (i = left->quantitykeys + 1; i < right->quantitykeys + left->quantitykeys + 1; i++)
		{
			left->keys[i] = right->keys[i - left->quantitykeys - 1];
			left->children[i] = right->children[i - left->quantitykeys - 1];
		}
		left->children[i] = right->children[i - left->quantitykeys - 1];
		left->quantitykeys += right->quantitykeys + 1;
		int j = index;
		tree->children[index] = left;
		for (j = index; j < tree->quantitykeys - 1; j++)
		{
			tree->keys[j] = tree->keys[j + 1];
			tree->children[j + 1] = tree->children[j + 2];
		}
		tree->quantitykeys--;
		free(right);
		tree->children[j + 1] = NULL;

		if (tree_ptr == head_btree_ptr)
		{
			if (tree->quantitykeys == 0)
			{
				tree->quantitykeys = left->quantitykeys;
				int k = 0;
				for (k = 0; k < left->quantitykeys; k++)
				{
					tree->keys[k] = left->keys[k];
					tree->children[k] = left->children[k];
				}
				tree->children[k] = left->children[k];
				free(left);
			}
		}
	}
}
void DeleteFromList3(btree_t** tree_ptr, int value)
{
	btree_t* tree = (*tree_ptr);
	int index_child = FindIndexChildNextWay1(tree, value);
	btree_t* tree_tmp = NULL;
	int index_key = -1;
	if (index_child == not_found)
	{
		index_key = FindIndexValueInKeys(tree, value);
		tree_tmp = tree;
	}
	else
	{
		index_key = FindIndexValueInKeys(tree->children[index_child], value);
		tree_tmp = tree->children[index_child];
	}
	if (index_key == not_found)
	{
		// If not a sheet, we go deep into the recursion
		if (tree_tmp->children[0]) DeleteFromList3(&(tree_tmp), value);
		//If it's a leaf, then we haven't found anything
		else return;
	}
	else
	{
		//If you found a value, but it is not in the sheet
		if (tree_tmp->children[0]) exit(1);
		else
		{
			//If we find a value in the sheet, delete it and shift the array of values
			for (int i = index_key; i < tree_tmp->quantitykeys - 1; i++)
			{
				tree_tmp->keys[i] = tree_tmp->keys[i + 1];
			}
			tree_tmp->quantitykeys--;
		}
	}
	if (tree_tmp->quantitykeys < t - 1)
	{
		if (index_child != not_found)
		{
			if (index_child == 0)
			{
				if ((tree->children[index_child]->quantitykeys) + (tree->children[index_child + 1]->quantitykeys) + 1 <= maxquantitykeys)
				{
					ConnectingLeftAndRightChildren3(tree_ptr, index_child);
				}
				else LeftRotate(&tree, index_child);
			}
			else
			{
				if ((tree->children[index_child]->quantitykeys) + (tree->children[index_child - 1]->quantitykeys) + 1 <= maxquantitykeys)
				{
					ConnectingLeftAndRightChildren3(tree_ptr, index_child - 1);
				}
				else
				{
					if ((*head_btree_ptr) == tree_tmp)
					{
						if (tree_tmp->quantitykeys == 0)
						{
							free(tree_tmp);
							(*tree_ptr) = NULL;
						}
					}
					else RightRotate(&tree, index_child - 1);
					/*
					if (tree_tmp->quantitykeys == 0)
					{
						free(tree_tmp);
						(*tree_ptr) = NULL;
					}
					else RightRotate(&tree, index_child - 1);
					*/
				}
			}
		}
		else
		{
			if ((*head_btree_ptr) == tree_tmp)
			{
				if (tree_tmp->quantitykeys == 0)
				{
					free(tree_tmp);
					(*tree_ptr) = NULL;
				}
			}
		}

	}
}
void DeleteFromNode3(btree_t** tree_ptr, btree_t* desired_node, int index_desired_node, int value)
{
	if ((*tree_ptr)->quantitykeys == 0)
	{
		free(*tree_ptr);
		(*tree_ptr) = NULL;
		return;
	}
	if (!(*tree_ptr)->children[0])
	{
		DeleteFromList3(tree_ptr, value);
	}
	//Checking the left son
	if (desired_node->children[index_desired_node]->quantitykeys > t - 1)
	{
		int max_key = FindingTheMaximumElementInTheLeftSubtree(desired_node, index_desired_node);
		DeleteFromList3(tree_ptr, max_key);
		desired_node->keys[index_desired_node] = max_key;
	}
	else
	{
		//Checking the right son
		if (desired_node->children[index_desired_node + 1]->quantitykeys > t - 1)
		{
			int min_key = FindingTheMinimumElementInTheRightSubtree(desired_node, index_desired_node);
			DeleteFromList3(tree_ptr, min_key);
			desired_node->keys[index_desired_node] = min_key;
		}
		//If the keys in the left and right sons are less than or equal to t - 1
		//We can combine
		else
		{
			btree_t** desired_node_ptr = &desired_node;
			ConnectingLeftAndRightChildren3(desired_node_ptr, index_desired_node);
			desired_node = FindTreeAndIndex((*tree_ptr), &index_desired_node, value);

			if (index_desired_node == not_found) return;
			if (!desired_node) return;
			if (desired_node->children[0])
			{
				DeleteFromNode3(tree_ptr, desired_node, t - 1, value);
			}
			else
			{
				DeleteFromList3(tree_ptr, value);
			}
		}
	}
}
void DeleteValue3(btree_t** tree_ptr, int value)
{
	int index_desired_node = -1;
	btree_t* desired_node = NULL;
	desired_node = FindTreeAndIndex((*tree_ptr), &index_desired_node, value);
	if (index_desired_node == not_found) return;
	if (desired_node->children[0])
	{
		DeleteFromNode3(tree_ptr, desired_node, index_desired_node, value);
	}
	else
	{
		DeleteFromList3(tree_ptr, value);
	}
}
int TestSystemMainCycle(FILE* in, FILE* out) 
{
	head_btree_ptr = (btree_t**)(malloc(sizeof(btree_t*)));
	(*head_btree_ptr) = NULL;
	char buf[16] = { 0 };
	char c = 0;
	int val = 0;
	int index = -1;
	while (fgets(buf, 16, in)) {
		sscanf(buf, "%c %d", &c, &val);
		switch (c) {
		case 'a':
			AddingValue1(head_btree_ptr, val);
			break;
		case 'r':
			DeleteValue3(head_btree_ptr, val);
			break;
		case 'f':
			fprintf(out, "%s\n", FindTreeAndIndex((*head_btree_ptr), &index, val) ? "yes" : "no");
			break;
		default:
			DestroyTree(*head_btree_ptr);
			free(head_btree_ptr);
			return 0;
		}
	}
	DestroyTree(*head_btree_ptr);
	free(head_btree_ptr);
	return 0;
}