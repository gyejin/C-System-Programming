//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section E - Binary Trees Questions
Purpose: Implementing the required functions for Question 2 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////
typedef struct _btnode{
	int item;
	struct _btnode *left;
	struct _btnode *right;
} BTNode;   // You should not change the definition of BTNode

/////////////////////////////////////////////////////////////////////////////////

typedef struct _stackNode{
    BTNode *btnode;
    struct _stackNode *next;
}StackNode;

typedef struct _stack{
    StackNode *top;
}Stack;

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
int maxHeight(BTNode *node);

BTNode *createBTNode(int item);

BTNode *createTree();
void push( Stack *stk, BTNode *node);
BTNode* pop(Stack *stk);

void printTree(BTNode *node);
void removeAll(BTNode **node);

///////////////////////////// main() /////////////////////////////////////////////

int main()
{
    int c;
    char e;
	c = 1;

    BTNode *root;
    root = NULL;

    printf("1: Create a binary tree.\n");
    printf("2: Find the maximum height of the binary tree.\n");
    printf("0: Quit;\n");

    while(c != 0){
        printf("\nPlease input your choice(1/2/0): ");
        if(scanf("%d", &c) > 0)
        {
            switch(c)
            {
            case 1:
                removeAll(&root);
                root = createTree();
                printf("The resulting binary tree is: ");
                printTree(root);
                printf("\n");
                break;
            case 2:
                c = maxHeight(root);
                printf("The maximum height of the binary tree is: %d\n",c);
                removeAll(&root);
                break;
            case 0:
                removeAll(&root);
                break;
            default:
                printf("Choice unknown;\n");
                break;
            }
		}
        else
        {
            scanf("%c",&e);
        }

    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

int maxHeight(BTNode *node)
{
    if (node == NULL){      //트리가 비었으면
        return -1;
    }

    //int size = 0;
    //size++;
    int left_height = maxHeight(node->left);    //재귀 왼쪽 서브트리로 계속 파고들기
    int right_height = maxHeight(node->right);      //재귀 오른쪽 서브트리로 계속 파고 들기
    
    if (left_height > right_height){        //왼쪽꺼가 오른쪽보다 크면
        return left_height +1;      //왼쪽에 +1 해서 반환
    }
    else{       //오른쪽꺼가 왼쪽보다 크면
        return right_height +1;     //오른쪽에 +1 해서 반환
    }
}

///////////////////////////////////////////////////////////////////////////////////

BTNode *createBTNode(int item){
    BTNode *newNode = malloc(sizeof(BTNode));
    newNode->item = item;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


//////////////////////////////////////////////////////////////////////////////////

BTNode *createTree()
{
    Stack stk;
    BTNode *root, *temp;
    char s;
    int item;

    stk.top = NULL;
    root = NULL;

    printf("Input an integer that you want to add to the binary tree. Any Alpha value will be treated as NULL.\n");
    printf("Enter an integer value for the root: ");
    if(scanf("%d",&item) > 0)
    {
        root = createBTNode(item);
        push(&stk,root);
    }
    else
    {
        scanf("%c",&s);
    }

    while((temp =pop(&stk)) != NULL)
    {

        printf("Enter an integer value for the Left child of %d: ", temp->item);

        if(scanf("%d",&item)> 0)
        {
            temp->left = createBTNode(item);
        }
        else
        {
            scanf("%c",&s);
        }

        printf("Enter an integer value for the Right child of %d: ", temp->item);
        if(scanf("%d",&item)>0)
        {
            temp->right = createBTNode(item);
        }
        else
        {
            scanf("%c",&s);
        }

        if(temp->right != NULL)
            push(&stk,temp->right);
        if(temp->left != NULL)
            push(&stk,temp->left);
    }
    return root;
}

void push( Stack *stk, BTNode *node){
    StackNode *temp;

    temp = malloc(sizeof(StackNode));
    if(temp == NULL)
        return;
    temp->btnode = node;
    if(stk->top == NULL){
        stk->top = temp;
        temp->next = NULL;
    }
    else{
        temp->next = stk->top;
        stk->top = temp;
    }
}

BTNode* pop(Stack *stk){
   StackNode *temp, *top;
   BTNode *ptr;
   ptr = NULL;

   top = stk->top;
   if(top != NULL){
        temp = top->next;
        ptr = top->btnode;

        stk->top = temp;
        free(top);
        top = NULL;
   }
   return ptr;
}

void printTree(BTNode *node){
    if(node == NULL) return;

    printTree(node->left);
    printf("%d ",node->item);
    printTree(node->right);
}

void removeAll(BTNode **node){
    if(*node != NULL){
        removeAll(&((*node)->left));
        removeAll(&((*node)->right));
        free(*node);
        *node = NULL;
    }
}
