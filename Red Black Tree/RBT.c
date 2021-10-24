#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct RBNode RBNode;

struct RBNode
{
	unsigned long long int key;
	char word[257];
	RBNode* parent;
	RBNode* left;
	RBNode* right;
	char colour;
};

typedef struct RBTree RBTree;
struct RBTree
{
	RBNode* root;
	RBNode* sentinel;
};

RBTree createTree()
{
	RBTree tree;
	tree.sentinel = malloc(sizeof(RBNode));
	if (tree.sentinel == NULL)
		printf("ERROR: Couldn`t create tree\n");
	tree.sentinel->colour = 'b';
	tree.sentinel->parent = NULL;

	tree.root = tree.sentinel;
	return tree;
}

RBNode* createNode(RBNode* parent, unsigned long long int key, char word[], RBNode* sentinel)
{
	RBNode *node = malloc(sizeof(RBNode));
	if (node == NULL)
	{
		printf("ERROR: Cannot create node\n");
		return NULL;
	}
	node->colour = 'r';
	node->key = key;
	strcpy(node->word, word);
	node->parent = parent;
	node->left = sentinel;
	node->right = sentinel;
	return node;
}

void rotateRight(RBNode *node, RBTree* tree)
{
	RBNode *pivot = node->left;
	pivot->parent = node->parent;

	if (node->parent == NULL)
		tree->root = pivot;

	if (node->parent != NULL)
	{
		if (node->parent->left == node)
			node->parent->left = pivot;
		else
			node->parent->right = pivot;
	}

	node->left = pivot->right;
	if (pivot->right != tree->sentinel)
		pivot->right->parent = node;

	node->parent = pivot;
	pivot->right = node;
}

void rotateLeft(RBNode *node, RBTree* tree)
{
	RBNode *pivot = node->right;
	pivot->parent = node->parent;

	if (node->parent == NULL)
		tree->root = pivot;

	if (node->parent != NULL)
	{
		if (node->parent->left == node)
			node->parent->left = pivot;
		else
			node->parent->right = pivot;
	}

	node->right = pivot->left;
	if (pivot->left != tree->sentinel)
		pivot->left->parent = node;

	node->parent = pivot;
	pivot->left = node;
}

int compare(char str1[], char str2[])
{
	//printf("%s str 1, %s str2\n", str1, str2);
	int size = strlen(str1);
	if (size != strlen(str2))
		return 0;
	for (int i = 0; i < size; ++i)
	{
		if (str1[i] != str2[i])
			return 0;
	}
	return 1;
}


RBNode* sibling(RBNode *node)
{
	if (node == node->parent->left)
		return node->parent->right;
	return node->parent->left;
}

RBNode* grandp(RBNode *node)
{
	if (node != NULL && node->parent != NULL)
		return node->parent->parent;
	return NULL;
}

RBNode* uncle(RBNode *node)
{
	RBNode *g = grandp(node);

	if (g == NULL)
	 	return	NULL;
	if (node->parent == g->left)
		return g->right;
	return g->left;
}

void insertcase1(RBNode *node, RBTree* tree);

void insertcase5(RBNode *node, RBTree* tree)
{
	RBNode *g = grandp(node);

	node->parent->colour = 'b';
	g->colour = 'r';
	if ((node == node->parent->left) && (node->parent == g->left))
		rotateRight(g, tree);
	else
		rotateLeft(g, tree);
}

void insertcase4(RBNode *node, RBTree* tree)
{
	RBNode *g = grandp(node);
	if ((node == node->parent->right) && (node->parent == g->left))
	{
		rotateLeft(node->parent, tree);
		node = node->left;
	}
	else if ((node == node->parent->left) && (node->parent == g->right))
	{
		rotateRight(node->parent, tree);
		node = node->right;
	}
	insertcase5(node, tree);
}

void insertcase3(RBNode *node, RBTree* tree)
{
	RBNode *u = uncle(node);
		if ( u != NULL && u->colour == 'r')
		{
			node->parent->colour = 'b';
			u->colour = 'b';
			RBNode *g = grandp(node);
			g->colour = 'r';
			insertcase1(g, tree);
		} else {
			insertcase4(node, tree);
		}
}

void insertcase2(RBNode *node, RBTree* tree)
{
	if (node->parent->colour == 'b')
		return;
	else
		insertcase3(node, tree);
}

void insertcase1(RBNode *node, RBTree* tree)
{
	if (node->parent == NULL)
	{
		node->colour = 'b';
		tree->root = node;
	}
	else
		insertcase2(node, tree);
}

void deleteCase1(RBNode* node, RBTree* tree);

void deleteCase6(RBNode* node, RBTree* tree)
{
	//printf("At deletecase6\n");
	RBNode *s = sibling(node);

	s->colour = node->parent->colour;
	node->parent->colour = 'b';

	if (node == node->parent->left)
	{
		s->right->colour = 'b';
		rotateLeft(node->parent, tree);
	} else {
		s->left->colour = 'b';
		rotateRight(node->parent, tree);
	}
}

void deleteCase5(RBNode* node, RBTree* tree)
{
	RBNode *s = sibling(node);

	if (s->colour == 'b')
	{
		if ((node == node->parent->left) &&
			(s->left->colour == 'r') &&
			(s->right->colour == 'b'))
		{
			s->colour = 'r';
			s->left->colour = 'b';
			rotateRight(s, tree);
		}
		else if ((node == node->parent->right) &&
			(s->right->colour == 'r') &&
			(s->left->colour == 'b'))
		{
			s->colour = 'r';
			s->right->colour = 'b';
			rotateLeft(s, tree);
		}
	}
	deleteCase6(node, tree);
}

void deleteCase4(RBNode* node, RBTree* tree)
{
	RBNode *s = sibling(node);

	if (node->parent->colour == 'r' && s->colour == 'b'
		&& s->left->colour == 'b' && s->right->colour == 'b')
	{
		s->colour = 'r';
		node->parent->colour = 'b';
	} else
		deleteCase5(node, tree);
}

void deleteCase3(RBNode* node, RBTree* tree)
{
	RBNode *s = sibling(node);

	if (node->parent->colour == 'b' && s->colour == 'b'
		&& s->left->colour == 'b' && s->right->colour == 'b')
	{
		s->colour = 'r';
		deleteCase1(node->parent, tree);
	} else
		deleteCase4(node, tree);
}

void deleteCase2(RBNode* node, RBTree* tree)
{
	RBNode *s = sibling(node);

	if (s->colour == 'r')
	{
		node->parent->colour = 'r';
		s->colour = 'b';
		if (node == node->parent->left)
			rotateLeft(node->parent, tree);
		else
			rotateRight(node->parent, tree);
	}
	deleteCase3(node, tree);
}

void deleteCase1(RBNode* node, RBTree* tree)
{
	if (node->parent != NULL)
	{
		deleteCase2(node, tree);
		return;
	}
	tree->root = node;
}

RBNode* findNode(RBTree tree, char word[])
{
	RBNode *current = tree.root;

	while (current != tree.sentinel)
	{
		if (compare(current->word, word))
			return current;
		else if (strcmp(word, current->word) < 0)
			current = current->left;
		else
			current = current->right;
	}
	return NULL;
}

RBNode* findLowest(RBNode* root, RBNode* sentinel)
{
	if (root->right != sentinel)
		return findLowest(root->right, sentinel);
	return root;
}

void printResult(int result)
{
	if (result == 0)
		printf("OK\n");
	if (result == 1)
		printf("Exist\n");
	if (result == 2)
		printf("ERROR: Cant allocate memory\n");
}
void print(RBNode* node, RBTree tree, int lvl)
{
	if (node == tree.sentinel)
		return;
	for (int i = 0; i < lvl; ++i)
		printf("--");
	printf("word - %s, key - %llu, colour = %c\n", node->word, node->key, node->colour);
	for (int i = 0; i < lvl; ++i)
		printf("--");
	printf("going left\n");
	print(node->left, tree, lvl + 1);
	for (int i = 0; i < lvl; ++i)
		printf("--");
	printf("going right\n");
	print(node->right, tree, lvl + 1);
}

int insert(RBTree* tree, unsigned long long int key, char word[])
{
	RBNode *parent = NULL, *current = tree->root;
	while (current != tree->sentinel)
	{
		parent = current;

		if (strcmp(word, current->word) < 0)
		{
			current = current->left;
		}	
		else if (strcmp(word, current->word) > 0)
		{
			current = current->right;
		}
		else 
		{
			return 1;
		}
	}
	if (parent == NULL)
	{
		tree->root = createNode(parent, key, word, tree->sentinel);
		if (tree->root == NULL)
		{
			return 2;
		}

		tree->root->colour = 'b';

		return 0;
	}
	RBNode *node = createNode(parent, key, word, tree->sentinel);
		if (node == NULL)
		{
			return 2;
		}

	if (strcmp(word, parent->word) < 0)
		parent->left = node;
	else
		parent->right = node;
	insertcase1(node, tree);

	return 0;
}

bool isLeaf(RBNode* node, RBNode* sentinel)
{
	if (node == sentinel)
		return true;
	return false;
}

void nodeReplace(RBNode *node, RBNode *child, RBTree* tree)
{
	if (node->parent != NULL)
	{
		if (node == node->parent->left)
			node->parent->left = child;
		else
			node->parent->right = child;
	}
	else
	{
		tree->root = child;
	}
	child->parent = node->parent;
}

void deleteOneChild(RBNode* node, RBTree* tree)
{
	RBNode *child = isLeaf(node->left, tree->sentinel) ? node->right : node->left;
	nodeReplace(node, child, tree);

	if (node->colour == 'b')
	{
		if (child->colour == 'r')
			child->colour = 'b';
		else
			deleteCase1(child, tree);
	}

	free(node);
}

void destroy(RBNode* root, RBNode* sentinel)
{
	if (root == sentinel)
		return;

	destroy(root->left, sentinel);
	destroy(root->right, sentinel);

	free(root);
}

int delete(RBTree* tree, char word[])
{
	RBNode *nodeToExchange = findNode(*tree, word), *child;

	if (nodeToExchange == NULL)
	{
		printf("NoSuchWord\n");
		return 4;
	}

	char colour = nodeToExchange->colour;

	if (nodeToExchange->left == tree->sentinel)
	{
		child = nodeToExchange->right;
		nodeReplace(nodeToExchange, child, tree);
		free(nodeToExchange);
	}
	else if (nodeToExchange->right == tree->sentinel)
	{
		child = nodeToExchange->left;
		nodeReplace(nodeToExchange, child, tree);
		free(nodeToExchange);
	}
	else
	{
		RBNode *newNode = findLowest(nodeToExchange->left, tree->sentinel);
		
		nodeToExchange->key = newNode->key;
		strcpy(nodeToExchange->word, newNode->word);

		colour = newNode->colour;
		child = newNode->left;
		nodeReplace(newNode, child, tree);
		free(newNode);
	}

	if (colour == 'b')
	{
		deleteCase1(child, tree);
	}

	return 0;
}

void dictWrite(RBNode* node, FILE* ptrWrite, RBTree tree)
{
	if (node == tree.sentinel)
		return;
	int temp = strlen(node->word);

	fwrite(&temp, sizeof(int), 1, ptrWrite);
	fwrite(node->word, sizeof(char), temp, ptrWrite);
	fwrite(&(node->key), sizeof(unsigned long long int), 1, ptrWrite);

	dictWrite(node->left, ptrWrite, tree);
	dictWrite(node->right, ptrWrite, tree);
}

void createDict(RBNode* node, FILE* ptrWrite, RBTree tree)
{
	char buffer[] = "Red Black Tree v3.14";
	fwrite(buffer, sizeof(char), strlen(buffer), ptrWrite);

	dictWrite(node, ptrWrite, tree);
	printf("OK\n");
}

int secretCheck(char str1[], char str2[])
{
	for (int i = 0; i < 20; ++i)
	{
		if (str1[i] != str2[i])
			return 1;
	}
	return 0;
}

void loadDict(RBNode* node, FILE* ptrRead , RBTree* tree)
{
	char buffer[257];
	unsigned long long int tempKey;
	int bufSize;
	RBTree tempTree = createTree();

	if (fread(buffer, sizeof(char), 20, ptrRead) != 20 * sizeof(char))
	{
		printf("ERROR: Wrong file format\n");
		free(tempTree.sentinel);
		return;
	}
	buffer[20] = '\0';
	bufSize = compare(buffer, "Red Black Tree v3.14");
	if (bufSize == 0)
		{
			printf("ERROR: Bad FILE\n");
			free(tempTree.sentinel);
			return;
		}

	while (fread(&bufSize, sizeof(int), 1, ptrRead) == 1)
	{
			if (feof(ptrRead))
			{
				printf("GOT CHA\n");
				free(tempTree.sentinel);
				return;
			} else if (ferror(ptrRead))
			{
				printf("GOT CHA 2\n");
				free(tempTree.sentinel);
				return;
			}
		if (fread(buffer, sizeof(char), bufSize, ptrRead) != bufSize * sizeof(char))
		{
			if (feof(ptrRead))
			{
				printf("GOT CHA\n");
				free(tempTree.sentinel);
				return;
			} else if (ferror(ptrRead))
			{
				printf("GOT CHA 2\n");
				free(tempTree.sentinel);
				return;
			}
			printf("ERROR: bad word format\n");
			destroy(tempTree.root, tempTree.sentinel);
			free(tempTree.sentinel);
			return;
		}
		if (fread(&tempKey, sizeof(unsigned long long int), 1, ptrRead) != 1)
		{
			printf("ERROR: bad number format\n");
			destroy(tempTree.root, tempTree.sentinel);
			free(tempTree.sentinel);
			return;
		}
		buffer[bufSize] = '\0';
		insert(&tempTree, tempKey, buffer);
	}
	destroy(tree->root, tree->sentinel);
	free(tree->sentinel);
	tree->root = tempTree.root;
	tree->sentinel = tempTree.sentinel;
	printf("OK\n");
}

int parser(char buffer[])
{
	int leng = strlen(buffer);
	if (leng == 1)
	{
		if (buffer[0] == '+')
			return 1;
		if (buffer[0] == '-')
			return 2;
		if (buffer[0] == '!')
			return 3;
	}

	return 4;
}

void lowerCase(char *word)
{
	int leng = strlen(word);

	for (int i = 0; i < leng; ++i)
	{
		if (word[i] < 91 && word[i] > 64)
			word[i] += 32;
	}
	word[leng] = '\0';
}

void loadOrSave(RBTree* tree, char buffer[])
{
	int flag;
	if (compare(buffer, "Save"))
	{
		flag = scanf("%s", buffer);
		if (flag != 1)
		{
			printf("ERROR: Write path to file or file name in directory\n");
			return;
		}
		FILE *ptrWrite = fopen(buffer, "wb");
		if (ptrWrite == NULL)
		{
			printf("ERROR: Couldn't create file\n");
			return;
		}
		createDict(tree->root, ptrWrite, *tree);

		fclose(ptrWrite);
		return;
	}

	if (compare(buffer, "Load"))
	{
		flag = scanf("%s", buffer);
		if (flag != 1)
		{
			printf("ERROR: Write path to file or file name in directory\n");
			return;
		}
		FILE *ptrRead = fopen(buffer, "rb");
		if (ptrRead == NULL)
		{
			printf("ERROR: Couldn't open file\n");
			return;
		}
		loadDict(tree->root, ptrRead, tree);

		fclose(ptrRead);
		return;
	}

	printf("ERROR: Wrong command\n");
	return;
}

void pnn(RBNode* node, RBTree tree, int lvl)
{

	if (node == tree.sentinel)
		return;

	printf("word - %s\n", node->word);

	pnn(node->left, tree, lvl + 1);

	pnn(node->right, tree, lvl + 1);
}

int main()
{
	unsigned long long int tempKey;
	char buffer[257];
	int state;
	RBTree tree = createTree();
	if (tree.sentinel == NULL)
	{
		return 0;
	}
	RBNode *tempPtr;
	int flag = scanf("%s", buffer);
	while (flag != EOF)
	{
		lowerCase(buffer);
		state = parser(buffer);
		switch(state)
		{
			case 1:
				flag = scanf("%s %llu", buffer, &tempKey);
				if (flag == EOF)
				{
					printf("ERROR: bad input\n");
					break;
				}
				if (flag != 2)
				{
					printf("ERROR: bad format\n");
					break;
				}
				lowerCase(buffer);
				printResult(insert(&tree, tempKey, buffer));
				break;

			case 2:
				flag = scanf("%s", buffer);
				if (flag == EOF)
				{
					printf("ERROR: bad input\n");
					break;
				}
				if (flag != 1)
				{
					printf("ERROR: bad format\n");
					break;
				}
				lowerCase(buffer);
				printResult(delete(&tree, buffer));
				break;

			case 3:
				flag = scanf("%s", buffer);
				if (flag == EOF)
				{
					printf("ERROR: bad input\n");
					break;
				}
				if (flag != 1)
				{
					printf("ERROR: bad format\n");
					break;
				}
				loadOrSave(&tree, buffer);
				break;
				
			default:
				tempPtr = findNode(tree, buffer);
				(tempPtr == NULL) ? printf("NoSuchWord\n"): printf("OK: %llu\n", tempPtr->key);
				break;
		}
		flag = scanf("%s", buffer);
	}

	if (tree.root != NULL || tree.sentinel != NULL)
	{	
		destroy(tree.root, tree.sentinel);
		free(tree.sentinel);
	}
	return 0;
}
