#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* Input examples
* 1. (or a1 (not (or (not (or a2 a3)) a4)))
* 1-1. (or a1 (not (or a4 (not (or a2 a3)))))
* 2. (or a1 (not (or (not (or a2 (and a4 a5)) a6))))
* 3. (or a1 (not (or (not (or a2 (or a4 a5)) a6))))
* 4. (or a1 (not (or (not (or (or a2 a3) (and a4 a5)) a6)))) -> CNN X
* 5. (or a1 (not (or (not (or a2 a3 (and a4 a5)) a6)))) -> multi-parameter example
* 6. syntax error
*/

/////////////////////// Tree Structure ///////////////////////

typedef struct _treeNode {
	char key[256];
	struct _treeNode* parent;
	struct _treeNode* left;
	struct _treeNode* right;
} treeNode;

treeNode* makeNode(char* key);
treeNode* grow(treeNode* head, treeNode* node, char* key, int* flag);
void _preorderTraverse(treeNode* node);
void preorderTraverse(treeNode* node);
void countTree(treeNode* node, int* cnt);


/////////////////////// For Converter ///////////////////////

treeNode* makeInitialTree(char* input);
char* getKey(char* ptr, int* cnt);
void detectTypo(char* str);
treeNode* DeMorgan(treeNode* node, char* op);
void makeNNF(treeNode* node);
treeNode* makeCNF(treeNode* node);
char* _makeArrayFromTree(treeNode* node);
char* _getArrayFromTree(treeNode* node, char* arr, int* cnt);
char* getArrayFromTree(treeNode* node, int* cnt);
void printResult(char* arr, int* cnt);


/////////////////////// Main ///////////////////////

int main() {
	char input[256];

	printf("Enter a propositional formula in prefix notation: ");
	scanf("%[^\n]", input);
	// strcpy(input, "(or (not a1 (or (not (or a2 a3)) a4)))");

	treeNode* root = makeInitialTree(input);
	// preorderTraverse(root);
	makeNNF(root);
	// preorderTraverse(root);
	root = makeCNF(root);
	root = root->left;
	// preorderTraverse(root);


	char* list;
	int* cnt = (int*) malloc(sizeof(int));
	*cnt = 1;
	list = getArrayFromTree(root, cnt);

	// printf("[list] ");
	// for(int i = 0; i < *cnt; i++) {
	// 	if(list[i] == '|') printf("| ");
	// 	else printf("%d ", list[i]);
	// }
	// printf("\n");
	printResult(list, cnt);

	return 0;
}


/////////////////////// Tree Structure ///////////////////////

treeNode* makeNode(char* key) {
	treeNode* new = (treeNode*) malloc(sizeof(treeNode));
	strcpy(new->key, key);
	new->parent = NULL;
	new->left = NULL;
	new->right = NULL;
	return new;
}

treeNode* grow(treeNode* head, treeNode* node, char* key, int* flag) {
  if (node == NULL) {
		*flag = 0;
		treeNode* new = makeNode(key);
		new->parent = head;
		return new;
	}

  if(strcmp(node->key, "and") == 0 || strcmp(node->key, "or") == 0) {
		if(*flag) node->left = grow(node, node->left, key, flag);
		if(*flag) node->right = grow(node, node->right, key, flag);
	}
	else if(*flag && (strcmp(node->key, "not") == 0)) {
		node->left = grow(node, node->left, key, flag);
	}
	else {
		return node;
	}

	return node;
}

/*
* This functions print the tree by preorder traverse.
*/
void _preorderTraverse(treeNode* node) {
	if(node == NULL) return;

	printf("%s ", node->key);
	_preorderTraverse(node->left);
	_preorderTraverse(node->right);
}
void preorderTraverse(treeNode* node) {
	printf("[preorder] ");
	_preorderTraverse(node);
	printf("\n");
}

void countTree(treeNode* node, int* cnt) {
	if(node == NULL) return;
	*cnt = *cnt + 1;
	countTree(node->left, cnt);
	countTree(node->right, cnt);
}


/////////////////////// For Converter ///////////////////////

/*
* This function translate given input to a tree structure.
*/
treeNode* makeInitialTree(char* input) {
	treeNode* root = NULL;
	treeNode* head = makeNode("head");
	int* parenthesesCnt = (int*) malloc(sizeof(int));
	*parenthesesCnt = 0;

	char* ptr = strtok(input, " ");
	while(ptr != NULL) {
		char* key = getKey(ptr, parenthesesCnt);
		detectTypo(key);
		int* flag = (int*) malloc(sizeof(int));
		*flag = 1;
		root = grow(head, root, key, flag);
		head->left = root;
		free(flag);
		ptr = strtok(NULL, " ");
	}

	if(*parenthesesCnt != 0) {
		printf("syntax error: the number of parentheses does not fit.\n");
		exit(-1);
	}

	return root;
}

/*
* This function get a key from given string.
* delete '(' or ')'
* we need to handle multi-param formula. ex) (or a1 a2 a3 a4...)
*/
char* getKey(char* ptr, int* cnt) {
	char* str = (char*) malloc(sizeof(char) * 128);
	// copy string <- if we use original string, information after '\0' will be lose.
	strcpy(str, ptr);

	// delete '('
	char* temp = strchr(str, '(');
	char* prev;
	if(temp != NULL) {
		*cnt = *cnt + 1;
		while (temp != NULL) {
			prev = temp+1;
			temp = strchr(prev, '(');
		}
	 	str = prev;
	}

	// delete ')'
	char* end = strchr(str, ')');
	temp = end;
	if(temp != NULL) {
		// *cnt = *cnt - 1;
		while(temp != NULL) {
			*cnt = *cnt - 1;
			temp = strchr(temp+1, ')');
		}
	}
	if(end != NULL) *end = '\0';

	return str;
}

void detectTypo(char* str) {
	int flag = 0;
	if(strcmp(str, "and") == 0 || strcmp(str, "or") == 0 || strcmp(str, "not") == 0) flag = 1;
	else if(str[0] == 'a')
		for(unsigned long i = 1; i < strlen(str); i++) {
			if(isdigit(str[i])) flag = 1;
			else { flag = 0; break; }
		}
	if(!flag) {
		printf("syntax error: typo\n");
		exit(-1);
	}
}

/*
* This function implements De Morgan's law to make NNF formula.
* 0. Preorder traverse
* 1. find "not"
* 1-1. if not->left->key is "and" or "or"
*	1-2. else if not->left->key is "not"
* 1-3. else
*/
treeNode* DeMorgan(treeNode* node, char* op) {
	// change key
	strcpy(node->left->key, op);
	// delete not node
	treeNode* temp = node;
	node->left->parent = node->parent;
	if(node->parent->left == node) { node->parent->left = node->left; node = node->left; }
	else if(node->parent->right == node) { node->parent->right = node->left; node = node->left; }
	free(temp);
	// add not nodes
	treeNode* left_not = makeNode("not");
	treeNode* right_not = makeNode("not");
	// link not and child
	node->left->parent = left_not;
	left_not->left = node->left;
	node->right->parent = right_not;
	right_not->left = node->right;
	// link not and node
	left_not->parent = node;
	node->left = left_not;
	right_not->parent = node;
	node->right = right_not;
	// return parent
	// node = node->parent;

	return node;
}

void makeNNF(treeNode* node) {
	if(node == NULL) return;

	if(strcmp(node->key, "not") == 0) {
		if(strcmp(node->left->key, "and") == 0) {
			node = DeMorgan(node, "or");
		}
		else if(strcmp(node->left->key, "or") == 0) {
			node = DeMorgan(node, "and");
		}
		else if(strcmp(node->left->key, "not") == 0) {
			// delete two not
			treeNode* temp = node;
			treeNode* temp2 = node->left;
			node->left->left->parent = node->parent;
			if(node->parent->left == node) { node->parent->left = node->left->left; node = node->left->left; }
			else if(node->parent->right == node) { node->parent->right = node->left->left; node = node->left->left; }
			// node = node->parent;
			free(temp);
			free(temp2);
		}
	}

	makeNNF(node->left);
	makeNNF(node->right);
	return;
}

/*
* This function makes CNF.
* 0. Preorder Traverse.
* 1. find "or"
* 1-1. if or->next(left, right) is "and"
*/
treeNode* makeCNF(treeNode* node) {
	if(node == NULL) return node;

	if(strcmp(node->key, "or") == 0) {
		// if left node is "and"
		if(strcmp(node->left->key, "and") == 0) {
			treeNode* temp = node;
			// link head and next node(and)
			if(node->parent->left == node) node->parent->left = node->left;
			else if(node->parent->right == node) node->parent->right = node->left;
			node->left->parent = node->parent;
			// make new_left and new_right node
			// copy temp to new nodes.
			treeNode* new_left = makeNode(temp->key);
			treeNode* new_right = makeNode(temp->key);
			if(temp->right != NULL) {
				new_left->right = makeNode(temp->right->key);
				new_right->right = makeNode(temp->right->key);
			}
			else {
				new_left->right = NULL;
				new_right->right = NULL;
			}

			// make new nodes.
			new_left->left = temp->left->left;
			new_right->left = temp->left->right;
			// link news to "and"
			node = node->left;
			node->left = new_left;
			node->right = new_right;
			new_left->parent = node;
			new_right->parent = node;
			free(temp);
			node = node->parent;
		}
		// if right node is "and"
		else if(strcmp(node->right->key, "and") == 0) {
			treeNode* temp = node;
			// link head and next node(and)
			if(node->parent->left == node) { node->parent->left = node->right; }
			else if(node->parent->right == node) { node->parent->right = node->right; }
			node->right->parent = node->parent;
			// make new_left and new_right node
			// copy temp to new nodes.
			treeNode* new_left = makeNode(temp->key);
			treeNode* new_right = makeNode(temp->key);
			if(temp->left != NULL) {
				new_left->left = makeNode(temp->left->key);
				new_right->left = makeNode(temp->left->key);
			}
			else {
				new_left->left = NULL;
				new_right->left = NULL;
			}
			// make new nodes.
			new_left->right = temp->right->left;
			new_right->right = temp->right->right;
			// link news to "and"
			node = node->right;
			// printf("node: %s\n", node->key);
			// printf("node->parent: %s\n", node->parent->key);
			node->left = new_left;
			node->right = new_right;
			new_left->parent = node;
			new_right->parent = node;
			node = node->parent;
		}
	}

	// preorderTraverse(node);
	makeCNF(node->left);
	makeCNF(node->right);
	return node;
}


/*
* This functions print the result.
*/
char* _getArrayFromTree(treeNode* node, char* arr, int* cnt) {
	if(node == NULL) return arr;

	// arr = (char*) realloc(arr, sizeof(char) * (*cnt + 1));
	if(strcmp(node->key, "and") == 0) {
		if(arr[*cnt-1] != '|') arr[*cnt] = '|';
		*cnt = *cnt + 1;
	}
	else if(strcmp(node->key, "or") == 0) ;
	else if(strcmp(node->key, "not") == 0) {
		arr[*cnt] = -1;
		*cnt = *cnt + 1;
	}
	else if(node->key[0] == 'a') {
		char key = atoi(&node->key[1]);
		if(arr[*cnt-1] == -1) {
			*cnt = *cnt - 1;
			key *= -1;
		}
		arr[*cnt] = key;
		*cnt = *cnt + 1;
	}
	else {
		printf("error: _getArrayFromTree\n");
		exit(-1);
	}

	arr = _getArrayFromTree(node->left, arr, cnt);
	if(strcmp(node->key, "and") == 0) {
		if(arr[*cnt-1] != '|') arr[*cnt] = '|';
		*cnt = *cnt + 1;
	}
	arr = _getArrayFromTree(node->right, arr, cnt);
	if(strcmp(node->key, "and") == 0) {
		if(arr[*cnt-1] != '|') arr[*cnt] = '|';
		*cnt = *cnt + 1;
	}
	return arr;
}

char* getArrayFromTree(treeNode* node, int* cnt) {
	char* result = (char*) malloc(sizeof(char) * 1024);
	result = _getArrayFromTree(node, result, cnt);
	return result;
}

void printResult(char* arr, int* cnt) {
	for(int i = 2; i < *cnt; i++) {
		if(arr[i] == '|') printf("\n");
		else printf("%d ", arr[i]);
	}
}
