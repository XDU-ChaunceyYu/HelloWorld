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

void CreateBiTree(BiTree &T) //��չ�����������Ϊ���룬������������ĵݹ��㷨
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
		printf("%c", T->data);      //���ʸ����
		PreOrderTraverse(T->lchild);//�����������������
		PreOrderTraverse(T->rchild);//�����������������
	}
}

void InOrderTraverse(BiTree T)
{
	if(T!=NULL){
		InOrderTraverse(T->lchild);//�����������������
		printf("%c", T->data);     //���ʸ����
		InOrderTraverse(T->rchild);//�����������������
	}
}

void PostOrderTraverse(BiTree T)
{
	if(T!=NULL){
		PostOrderTraverse(T->lchild);//�����������������
		PostOrderTraverse(T->rchild);//�����������������
		printf("%c", T->data);       //���ʸ����
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
			printf("ֵΪ%c�Ľ��Ϊ���������߶ȣ�%d", T->data, BiTreeDepth(T));
			b = true;
		}
		else{
			Sub_BiTreeDepth(T->lchild, c, b);
			if(!b)
				Sub_BiTreeDepth(T->rchild, c, b);
		}
	}
}

void CreateBiThrTree(BiThrTree &T, char *str)//���ű�ʾ��Ϊ���룬������������
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

BiThrTree pre; //pre: �ոշ��ʹ��Ľ�㣻

void InThreading(BiThrTree p)
{
	if(p){
		InThreading(p->lchild);
		if(!p->lchild){ //ǰ������
			p->lchild = pre;  
			p->Ltag = 1; 
		}
		else
			p->Ltag = 0;
		if(!pre->rchild){ //�������
			pre->rchild = p;  
			pre->Rtag = 1; 
		}
		else
			p->Rtag = 0;
		pre = p;
		InThreading(p->rchild);
	}
}

void InOrderThreading(BiThrTree &Thrt, BiThrTree T) //�������������������������ͷ������������������������
{
	Thrt = (BiThrTree)malloc(sizeof(BiThrNode));  //����ͷ���
	Thrt->Ltag=0;  
	Thrt->Rtag=1;  
	Thrt->rchild=Thrt;
	if (!T)  //�ն�����������
		Thrt->lchild=Thrt; 
	else{
		Thrt->lchild = T;
		pre = Thrt; //pre: �ոշ��ʹ��Ľ�㣻
		InThreading(T);
		pre->rchild = Thrt;   
		pre->Rtag = 1;
		Thrt->rchild = pre; 
	}
}

void InOrderTraverse_Thr(BiThrTree Thrt) //Thrt:ͷ���
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
	while(T->Ltag == 0) //�ҵ���������ĵ�һ�����
		T = T->lchild;
	
	while(T != Thrt){
		post = T->rchild;
		if(T->Rtag == 0) //���Һ���
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

	printf("������չ����������У�");//AB.DF..G..C.E.H..
	CreateBiTree(T); 

	printf("��������������У�");
	PreOrderTraverse(T);
	printf("\n");

	printf("��������������У�");
	InOrderTraverse(T);
	printf("\n");

	printf("��������������У�");
	PostOrderTraverse(T);
	printf("\n");

	printf("�����������ű�ʾ����");//A(B(,D(F,G)),C(,E(,H)))
	DisplayBiTree(T);
	printf("\n");

	int LeafCount = 0;
	int NodeCount = 0;
	DisplayLeaf(T, LeafCount, NodeCount);
	printf(" Ҷ����Ϊ��%d, �����Ϊ��%d\n", LeafCount, NodeCount);

	int TreeDepth;
	TreeDepth = BiTreeDepth(T);
	printf("���ĸ߶�Ϊ��%d\n", TreeDepth);

	char c = 'C';//M
	bool b = false;
	Sub_BiTreeDepth(T, c, b);
	if(!b)
		printf("����������ֵΪ%c�Ľ��", c);
	printf("\n");


	printf("�����������Ϊ��");
	LayerOrder(T, NodeCount);
	printf("\n");

	printf("ɾ���������Ϊ��");
	FreeBiTree(T);
	printf("\n");

	char str[256];
	BiThrTree Thrt, T1;

	printf("���ű�ʾ������������������������������������ű�ʾ��");
	getchar();
	gets(str); //A(B(,D(F,G)),C(,E(,H)))
	CreateBiThrTree(T1, str);
	InOrderThreading(Thrt, T1);

	printf("�����������������������У�");
	InOrderTraverse_Thr(Thrt);
	printf("\n");

	printf("ɾ�������������������еĽ������Ϊ��");
	FreeBiThrTree(Thrt);
	printf("\n");

	char pre[30], in[30];
	int n;

	gets(pre);//�������У�ABDGCEFH
	gets(in); //�������У�DGBAECHF
	n = strlen(pre);
     
    postorder(pre, in, n);//�������У�GDBEHFCA
	printf("\n");
}

void postorder(char *pre, char *in, int n) 
{
	char *p; 
	int k;
    
    for(p=in; p<in+n; p++)  /*����������������λ��*/
		if(*p == *pre)   
			break;
    k = p - in;
    if(k)  
		postorder(pre+1, in, k);//������
	if(n-k-1) 
		postorder(pre+k+1, p+1, n-k-1);//������
	printf("%c", *p);
}