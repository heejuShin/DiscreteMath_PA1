#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct _treeNode {
	char *key;
	struct _treeNode* parent;
	struct _treeNode* left;
	struct _treeNode* right;
} treeNode;

int count = 0; 
int *countPointer = &count;
int n1=0, n2=0;
treeNode* makeNode(){
    treeNode *new= (treeNode*)malloc(sizeof(treeNode));
    new->parent= NULL;
    new->key = NULL;
    new->right = NULL;
    new->left = NULL;
    return new;
}
treeNode* makeInitialTree(treeNode* root, char * input){
    treeNode *new= (treeNode*)malloc(sizeof(treeNode));
    new->parent = root;
    root->left = new;
    new->key = input;
    new->left = NULL;
    new->right = NULL;
    return new;
}
int syntaxchecker(char *input){
    int count=0;
    for(int i=0; i<strlen(input); i++){
        if(input[i]=='(') count++;
        if(input[i]==')')count--;
    }
    return (count!=0) ? 0 : 1;
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
treeNode* makeNodes(char* key) {
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
		treeNode* new = makeNodes(key);
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
treeNode* makeInitial(char* input) {
	treeNode* root = NULL;
	treeNode* head = makeNodes("head");
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
treeNode* make_DNF(treeNode* node) {
	if(node == NULL) return node;

	if(strcmp(node->key, "and") == 0) {
		// if left node is"and"
		if(strcmp(node->left->key, "or") == 0) {
			treeNode* temp = node;
			// link head and next node(and)
			if(node->parent->left == node) node->parent->left = node->left;
			else if(node->parent->right == node) node->parent->right = node->left;
			node->left->parent = node->parent;
			// make new_left and new_right node
			// copy temp to new nodes.
			treeNode* new_left = makeNodes(temp->key);
			treeNode* new_right = makeNodes(temp->key);
			if(temp->right != NULL) {
				new_left->right = makeNodes(temp->right->key);
				new_right->right = makeNodes(temp->right->key);
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
		else if(strcmp(node->right->key, "or") == 0) {
			treeNode* temp = node;
			// link head and next node(and)
			if(node->parent->left == node) { node->parent->left = node->right; }
			else if(node->parent->right == node) { node->parent->right = node->right; }
			node->right->parent = node->parent;
			// make new_left and new_right node
			// copy temp to new nodes.
			treeNode* new_left = makeNodes(temp->key);
			treeNode* new_right = makeNodes(temp->key);
			if(temp->left != NULL) {
				new_left->left = makeNodes(temp->left->key);
				new_right->left = makeNodes(temp->left->key);
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
	make_DNF(node->left);
	make_DNF(node->right);
	return node;
}

int eleToInt(char* number){
    char num[strlen(number)-1];
    int count=0;
    if(number[0]=='n'){
        for(int i=2; i<strlen(number); i++)
            num[count++]=number[i];
        return atoi(num)*-1;
    }
    else{
        for(int i=1; i<strlen(number); i++)
            num[count++]=number[i];
        return atoi(num);
    }
}
void next(treeNode* ptr, int**result){
    if (ptr==NULL) return;
    if(strcmp(ptr->key, "o")==0){
        next(ptr->left, result);
        if(strcmp(ptr->left->key, "o")!=0){
            n1+=1;
            n2=0;
        }
        next(ptr->right, result);
        if(strcmp(ptr->right->key, "o")!=0){
            n1+=1;
            n2=0;
        }
    }
    else if (strcmp(ptr->key, "a")==0){
        next(ptr->left, result);
        next(ptr->right, result);
    }
    else{
        result[n1][n2]=eleToInt(ptr->key);
        n2+=1;
    }
}

void add (treeNode* ptr,char* data, int option){
    treeNode *add=(treeNode*)malloc(sizeof(treeNode));
    if(option==1)
        ptr->left=add;
    if(option==2)
        ptr->right=add;
    add->parent = ptr;
    add->key= data;
    add->left= add->right;
    add->right=NULL;
}
void delete(treeNode* ptr, treeNode* pa){
    if(ptr->right==NULL){
        if(pa->right==ptr) 
            pa->right=ptr->left;
        else if(pa->left==ptr)
            pa->left=ptr->left;
        ptr->left->parent=pa;
        free(ptr);
        return;
    }
    else if(ptr->left==NULL){
        if(pa->right==ptr) pa->right=ptr->right;
        else if(pa->left==ptr) pa->left=ptr->right; 
        ptr->right->parent=pa;
        free(ptr);
        return;
    }
}
void checkOp(treeNode* node,treeNode* pa){
    char not[4]="n";
    if(node==NULL) return;
    if(strcmp(node->key,"o")==0) node->key="a";
    else if(strcmp(node->key,"a")==0) node->key="o";
    else if(strcmp(node->key,"n")==0) {
        delete(node,pa);
        return;
    }
    else if(node->key[0]=='n'){
       int i=0;
       while(1){
           if(node->key[i]=='\0') break;
           node->key[i]=node->key[i+1];
           i++;
       }
    }
    else{
     strcat(not,node->key);
     node->key=(char*)malloc(strlen(node->key)+1);
     strcpy(node->key,not);
    }
    checkOp(node->left,node);
    checkOp(node->right,node);
    return;
}
void search(treeNode *ptr, char* data){
    if(ptr==NULL) return;
    if(strcmp(ptr->key, data)==0){
        checkOp (ptr->left, ptr);
        checkOp (ptr->right, ptr);
        delete(ptr, ptr->parent);
    }
    else{
        search(ptr->left, data);
        search(ptr->right, data);
    }
    return;
}

void clear(treeNode* node) {
    if (node -> left != NULL) clear(node -> left);
    if (node -> right != NULL) clear(node -> right);
    free(node);
}
void makenew(treeNode* root, treeNode* new){
    if(root == NULL) return;
    new->key = root->key;
    if(root->left != NULL){
        treeNode* newL=(treeNode*)malloc(sizeof(treeNode)); 
        new->left =newL; 
        newL->parent = new;
        makenew(root->left, newL);
    }
    if(root->right != NULL){
        treeNode* newR=(treeNode*)malloc(sizeof(treeNode));   
        new->right =newR; 
        newR->parent =new;
        makenew(root->right, newR);
    }
    return;
}
void changer(treeNode* ptr, treeNode *child, int option){
    treeNode* new = (treeNode*)malloc(sizeof(treeNode));
    new->key = ptr->key;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;

    treeNode* new2 =(treeNode*)malloc(sizeof(treeNode));
    new2->parent = NULL;
    new2->left = NULL;
    new2->right = NULL;

    if(option==1)
        new->left = new2;
    if(option==2)
        new->right = new2;
    new2->parent = new;
    if(option==1){
        makenew(ptr->left, new2);
        if(ptr->parent!=NULL){
            if(ptr->parent->left == ptr)
                ptr->parent->left = child;
            else
                ptr->parent->right = child;
        }   
        ptr->right = child->right;
        new->right = child->left;
        child->left = ptr;
        child->right = new;
        new->parent = child;
    }//option1
    if(option==2){
        makenew(ptr->right, new2);
        if(ptr->parent!=NULL){
            if(ptr->parent->right == ptr){
            ptr->parent->right = child;
            }
            else{
                ptr->parent->left = child;
            }
        }
        ptr->left = child->right;
        new->left = child->left;
        child->right = ptr;
        child->left = new;
        new->parent = child;
    }//option2
    child->parent = ptr->parent; 
    child->right->parent = ptr;
    ptr->parent = child;
    child->left->parent = new;
}
void checker(treeNode* ptr){
    if(ptr==NULL) return;
    if(strcmp(ptr->key,"a")==0){
        if(strcmp(ptr->left->key,"o")==0){
         changer(ptr,ptr->left,2);
         return;   
        }
        else if(strcmp(ptr->right->key,"o")==0){
        
        changer(ptr,ptr->right,1);
    
         return;
        }
    }
    checker(ptr->left);
    checker(ptr->right);
}

int DNF(treeNode*ptr){
    if(ptr==NULL) return 0;
    if(strcmp(ptr->key,"a")==0){
        if(strcmp(ptr->left->key,"o")==0){
         return 1;   
        }
        else if(strcmp(ptr->right->key,"o")==0){
         return 1;
        }
    }
    int num1=DNF(ptr->left);
    int num2=DNF(ptr->right);
    if(num1+num2==0) return 0;
    else return 1;
}

void makeDNF(treeNode* root){
    treeNode* node = root->left;
    while(1){
        node = root->left;
        if(DNF(node)==0) break;
        else checker(node);
    }
}

int change(char* str){
    int count=0, count2=0;
    for(int i=0; i< strlen(str); i++){
        while(str[i] !=' ' ){
            if(str[i] == '(') count++;
            else if(str[i] == ')') count--;
            if(i==strlen(str)-1) break;
            i++;
        }
        if(count==0){
            if(str[i] == ' ') str[i] = '-';
            count2++;
        }
    }//for
    return count2;
}
void plus(treeNode* treeNode, char* operator, char* elements,int count);
void makeTree(treeNode* treeNode, char* str){

    char *ptr;
    if(str == NULL) return; 
    int count = 0;
    if(str[strlen(str)-1] == ')'){
        str[strlen(str)-1] = '\0';
        count = change(&str[1]);
        ptr = strtok(&str[1],"-");
    }
    else if ((str[strlen(str)-2] == ')')&&(str[strlen(str)-1] == '\n')){
        str[strlen(str)-2] = ' '; str[strlen(str)-1] = '\0';
        count = change(&str[1]);
        ptr = strtok(&str[1],"-");
    }
    else ptr = str;
    if(count == 0) return;
    else if(count >3){
        if(strlen(ptr)>1){
            if(strcmp(ptr,"not")==0)
                treeNode->key = "n";        
            else if(strcmp(ptr,"or")==0)
                treeNode->key = "o";        
            else if(strcmp(ptr,"and")==0)
                treeNode->key = "a";        
        }
        else treeNode->key = ptr;
        ptr = strtok(NULL,"-");
        add(treeNode,ptr,1);    
        ptr = strtok(NULL,"-");
        plus(treeNode, treeNode->key, ptr, count);

    }
    else{
        if(strlen(ptr)>1){
            if(strcmp(ptr,"not")==0)
                treeNode->key = "n";        
            else if(strcmp(ptr,"or")==0)
                treeNode->key = "o";        
            else if(strcmp(ptr,"and")==0)
                treeNode->key = "a";        
        }
        else treeNode->key = ptr;
        ptr = strtok(NULL,"-");

        add(treeNode, ptr,1);
        if(count == 2){
            makeTree(treeNode->left, treeNode->left->key);
            return;
        }
        ptr = strtok(NULL,"-");
        add(treeNode, ptr,2);
    }
    makeTree(treeNode->left, treeNode->left->key);
    makeTree(treeNode->right, treeNode->right->key);
}

void plus(treeNode* treeNode, char* operator, char* elements,int count){
    add(treeNode, operator,2);
    treeNode = treeNode->right;
    if(count==4){
        add(treeNode, elements,1);
        elements = strtok(NULL,"-");
        add(treeNode, elements,2);
         makeTree(treeNode->left, treeNode->left->key);
        makeTree(treeNode->right, treeNode->right->key);
        return;
    }
    else{
        add(treeNode, elements,1);
        elements = strtok(NULL,"-");
        plus(treeNode, operator,elements,count-1 );
    }
    makeTree(treeNode->left,treeNode->left->key);
    makeTree(treeNode->right,treeNode->right->key);
}

void printOrder(treeNode* ptr, int *count){
    if(ptr==NULL) return;
    if(strcmp(ptr->key,"o")==0){
        printOrder(ptr->left,count);
        if(strcmp(ptr->left->key,"o")!=0) printf("\n");
        printOrder(ptr->right,count);
        if(strcmp(ptr->right->key,"o")!=0) printf("\n");
    }
    else if(strcmp(ptr->key,"a")==0){
        printOrder(ptr->left,count);
        printOrder(ptr->right,count);
    }
    else{
        int num=eleToInt(ptr->key);
        printf("%d ",num);
        *count+=1;
    }
}
void printAnswer(treeNode* root, int count){
    printf("0\n");
    int **result = (int**)malloc((count) * sizeof (int*));
    for (int i=0; i<count; i++)
        result[i]=(int*)malloc((count)*sizeof(int));
    int * answer = (int*)malloc(count*sizeof(int));
    next(root->left, result);
    int complete = 0, num = 0;
    int len = 0;
    int ok = 0;
    for (int i =0; i<count;i++)
        if(result[num][i]!=0) ok++;
    for (int i =0; i<count; i++)
        for (int j=0; j<count; j++)
            if(abs(result[i][j])>len) len = abs(result[i][j]);
    for(int i=0; i< count; i++){
        if(result[num][i]<0)
            answer[(result[num][i])*-1-1] = result[num][i];
        else
            answer[result[num][i]-1]=result[num][i];
    }
    for (int i=0; i<len; i++){
        if(answer[i] == 0) answer[i] = i+1;
        printf("%d ", answer[i]);
    }
    for (int i=0; i<count; i++)
        free(result[i]);
    free(result);
    free(answer);
    printf("\n");
}
int main(){
	char input[256];
	printf("Enter a propositional formula in prefix notation: ");
	scanf("%[^\n]", input);
    // strcpy(input, "(or (not a1 (or (not (or a2 a3)) a4)))");
    if (!syntaxchecker(input)){
        printf("SYNTAX ERROR\n");
        return 0;
    }
    treeNode *root= (treeNode*)malloc(sizeof(treeNode));
    root = makeNode();
    //root=makeInitial(input);
    treeNode *new= (treeNode*)malloc(sizeof(treeNode));
    new=makeInitialTree(root, input);
    makeTree(new, input);
    search(new,"n");
    makeDNF(root);
    printOrder(root->left,countPointer);
    printAnswer(root, count);
    clear(root);
    return 0;
}