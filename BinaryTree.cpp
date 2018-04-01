#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct BiTNode {
       char data;
       struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;

typedef struct BiThrNode {
       char data;
       struct BiThrNode *lchild,*rchild;
	   int Ltag, Rtag;
} BiThrNode, *BiThrTree;

void CreateBiTree(BiTree &T) //扩展先序遍历序列为输入，创建二叉链表的递归算法
{
	char ch;

	scanf("%c", &ch);
	if(ch == '.')
		T = NULL;
	else{
		T = (BiTree)malloc(sizeof(BiTNode));
		T->data = ch;
		CreateBiTree(T->lchild);
		CreateBiTree(T->rchild);
	}
}

void PreOrderTraverse(BiTree T)
{
	if(T!=NULL){
		printf("%c", T->data);      //访问根结点
		PreOrderTraverse(T->lchild);//先序遍历根的左子树
		PreOrderTraverse(T->rchild);//先序遍历根的右子树
	}
}

void InOrderTraverse(BiTree T)
{
	if(T!=NULL){
		InOrderTraverse(T->lchild);//中序遍历根的左子树
		printf("%c", T->data);     //访问根结点
		InOrderTraverse(T->rchild);//中序遍历根的右子树
	}
}

void PostOrderTraverse(BiTree T)
{
	if(T!=NULL){
		PostOrderTraverse(T->lchild);//后序遍历根的左子树
		PostOrderTraverse(T->rchild);//后序遍历根的右子树
		printf("%c", T->data);       //访问根结点
	}
}

void LayerOrder(BiTree T, int NodeCount)
{
	BiTree *Q;
	int front=0, rear=0;

	Q = (BiTree *)malloc(NodeCount*sizeof(BiTree));
	Q[rear++] = T;
	while(front != rear){
		printf("%c", Q[front]->data);
		if(Q[front]->lchild)
			Q[rear++] = Q[front]->lchild;
		if(Q[front]->rchild)
			Q[rear++] = Q[front]->rchild;
		front++;
	}
	free(Q);
}

void FreeBiTree(BiTree &T)
{
	if(T!=NULL){
		FreeBiTree(T->lchild);
		FreeBiTree(T->rchild);
		printf("%c",T->data);
		free(T);
	}
}

void DisplayBiTree(BiTree T)//A(B(,D(F,G),C(,E(,H)))
{
	if(T!=NULL){
		printf("%c", T->data);
		if(T->lchild || T->rchild){
			printf("(");
			DisplayBiTree(T->lchild);
			if(T->rchild)
				printf(",");
			DisplayBiTree(T->rchild);
			printf(")");
		}
	}
}

void DisplayLeaf(BiTree T, int &LeafCount, int &NodeCount)
{
	if(T!=NULL){
		NodeCount++;
		if(T->lchild==NULL && T->rchild==NULL){
			LeafCount++;
			printf("%c", T->data);
		}
		else{
			DisplayLeaf(T->lchild, LeafCount, NodeCount);
			DisplayLeaf(T->rchild, LeafCount, NodeCount);
		}
	}
}

int BiTreeDepth(BiTree T)
{
	int ldep, rdep;
	if(T==NULL)
		return 0;
	else{
		ldep = BiTreeDepth(T->lchild);
		rdep = BiTreeDepth(T->rchild);
		return (ldep > rdep)? ldep+1: rdep+1;
	}
}

void Sub_BiTreeDepth(BiTree T, char c, bool &b)
{
	if(T!=NULL){
		if(T->data == c){
			printf("值为%c的结点为根的子树高度：%d", T->data, BiTreeDepth(T));
			b = true;
		}
		else{
			Sub_BiTreeDepth(T->lchild, c, b);
			if(!b)
				Sub_BiTreeDepth(T->rchild, c, b);
		}
	}
}

void CreateBiThrTree(BiThrTree &T, char *str)//括号表示法为输入，创建二叉链表
{
	int i, k;
	char *c, ch;
	BiThrTree *S;
	BiThrTree p;
	int top = -1;

	for(i=0,c= str; *c; ++i,++c);
	S = (BiThrTree *)malloc(i*sizeof(BiThrTree));

	T = NULL;
	i = 0;
	ch = str[i];
	while(ch != '\0'){
		switch(ch){
		case '(': 
			top++;
			S[top] = p;
			k = 1;
			break;
		case ')':
			top--;
			break;
		case ',':
			k = 2;
			break;
		default:
			p = (BiThrTree)malloc(sizeof(BiThrNode));
			p->data = ch;
			p->lchild = p->rchild = NULL;
			if(T==NULL)
				T = p;
			else{
				switch(k){
				case 1:
					S[top]->lchild = p;
					break;
				case 2:
					S[top]->rchild = p;
					break;
				}
			}
		}
		ch = str[++i];
	}
	free(S);
}

BiThrTree pre; //pre: 刚刚访问过的结点；

void InThreading(BiThrTree p)
{
	if(p){
		InThreading(p->lchild);
		if(!p->lchild){ //前驱线索
			p->lchild = pre;  
			p->Ltag = 1; 
		}
		else
			p->Ltag = 0;
		if(!pre->rchild){ //后继线索
			pre->rchild = p;  
			pre->Rtag = 1; 
		}
		else
			p->Rtag = 0;
		pre = p;
		InThreading(p->rchild);
	}
}

void InOrderThreading(BiThrTree &Thrt, BiThrTree T) //二叉链表进行中序线索化，带头结点的中序线索二叉链表链表
{
	Thrt = (BiThrTree)malloc(sizeof(BiThrNode));  //创建头结点
	Thrt->Ltag=0;  
	Thrt->Rtag=1;  
	Thrt->rchild=Thrt;
	if (!T)  //空二叉线索链表
		Thrt->lchild=Thrt; 
	else{
		Thrt->lchild = T;
		pre = Thrt; //pre: 刚刚访问过的结点；
		InThreading(T);
		pre->rchild = Thrt;   
		pre->Rtag = 1;
		Thrt->rchild = pre; 
	}
}

void InOrderTraverse_Thr(BiThrTree Thrt) //Thrt:头结点
{
	BiThrTree T;

	T = Thrt->lchild;
	while(T != Thrt){
		while(T->Ltag == 0) 
			T = T->lchild;
		printf("%c", T->data);
		while((T->Rtag==1) && (T->rchild!=Thrt)){
			T = T->rchild;
			printf("%c", T->data);
		}
		T = T->rchild;
	}
}

void FreeBiThrTree(BiThrTree &Thrt)
{
	BiThrTree T, post;

	T = Thrt->lchild;
	while(T->Ltag == 0) //找到中序遍历的第一个结点
		T = T->lchild;
	
	while(T != Thrt){
		post = T->rchild;
		if(T->Rtag == 0) //有右孩子
			while(post->Ltag == 0)
				post = post->lchild;
		printf("%c", T->data);
		free(T);
		T = post;
	}
	free(Thrt);
	Thrt = NULL;
}
void postorder(char *pre, char *in, int n);

void main()
{
	BiTree T;

	printf("输入扩展先序遍历序列：");//AB.DF..G..C.E.H..
	CreateBiTree(T); 

	printf("先序遍历遍历序列：");
	PreOrderTraverse(T);
	printf("\n");

	printf("中序遍历遍历序列：");
	InOrderTraverse(T);
	printf("\n");

	printf("后序遍历遍历序列：");
	PostOrderTraverse(T);
	printf("\n");

	printf("二叉树的括号表示法：");//A(B(,D(F,G)),C(,E(,H)))
	DisplayBiTree(T);
	printf("\n");

	int LeafCount = 0;
	int NodeCount = 0;
	DisplayLeaf(T, LeafCount, NodeCount);
	printf(" 叶子数为：%d, 结点数为：%d\n", LeafCount, NodeCount);

	int TreeDepth;
	TreeDepth = BiTreeDepth(T);
	printf("树的高度为：%d\n", TreeDepth);

	char c = 'C';//M
	bool b = false;
	Sub_BiTreeDepth(T, c, b);
	if(!b)
		printf("二叉树中无值为%c的结点", c);
	printf("\n");


	printf("层序遍历序列为：");
	LayerOrder(T, NodeCount);
	printf("\n");

	printf("删除结点序列为：");
	FreeBiTree(T);
	printf("\n");

	char str[256];
	BiThrTree Thrt, T1;

	printf("括号表示法创建中序线索二叉链表，输入二叉树的括号表示：");
	getchar();
	gets(str); //A(B(,D(F,G)),C(,E(,H)))
	CreateBiThrTree(T1, str);
	InOrderThreading(Thrt, T1);

	printf("线索二叉链表的中序遍历序列：");
	InOrderTraverse_Thr(Thrt);
	printf("\n");

	printf("删除中序线索二叉链表中的结点序列为：");
	FreeBiThrTree(Thrt);
	printf("\n");

	char pre[30], in[30];
	int n;

	gets(pre);//先序序列：ABDGCEFH
	gets(in); //中序序列：DGBAECHF
	n = strlen(pre);
     
    postorder(pre, in, n);//后序序列：GDBEHFCA
	printf("\n");
}

void postorder(char *pre, char *in, int n) 
{
	char *p; 
	int k;
    
    for(p=in; p<in+n; p++)  /*在中序中找子树的位置*/
		if(*p == *pre)   
			break;
    k = p - in;
    if(k)  
		postorder(pre+1, in, k);//左子树
	if(n-k-1) 
		postorder(pre+k+1, p+1, n-k-1);//右子树
	printf("%c", *p);
}