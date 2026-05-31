#include "binary.h"
#include <stdio.h>
#include <stdlib.h>

// 辅助结构（栈与队列）
#define MAXNODES 500

typedef struct {
    BiTree data[MAXNODES];
    int top;
} Stack;

typedef struct {
    BiTree data[MAXNODES];
    int front, rear;
} Queue;

static void InitStack(Stack *s) { s->top = -1; }
static int  StackEmpty(Stack *s) { return s->top == -1; }
static void Push(Stack *s, BiTree node) { s->data[++(s->top)] = node; }
static BiTree Pop(Stack *s) { return s->data[(s->top)--]; }

static void InitQueue(Queue *q) { q->front = q->rear = 0; }
static int  QueueEmpty(Queue *q) { return q->front == q->rear; }
static void EnQueue(Queue *q, BiTree node) {
    q->data[q->rear] = node;
    q->rear = (q->rear + 1) % MAXNODES;
}
static BiTree DeQueue(Queue *q) {
    BiTree node = q->data[q->front];
    q->front = (q->front + 1) % MAXNODES;
    return node;
}

// 创建二叉树（前序，#表示空）
void CreatBiTree(BiTree *T) {
    char ch;
    scanf(" %c", &ch);
    if (ch == '#') {
        *T = NULL;
    } else {
        *T = (BiTree)malloc(sizeof(BiTNode));
        (*T)->data = ch;
        CreatBiTree(&((*T)->lchild));
        CreatBiTree(&((*T)->rchild));
    }
}

// 递归前序遍历
void PreOrder(BiTree T) {
    if (T) {
        printf("%c ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

// 递归中序遍历
void InOrder(BiTree T) {
    if (T) {
        InOrder(T->lchild);
        printf("%c ", T->data);
        InOrder(T->rchild);
    }
}

// 递归后序遍历
void PostOrder(BiTree T) {
    if (T) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%c ", T->data);
    }
}

// 非递归前序遍历
void PreOrder2(BiTree T) {
    Stack s;
    InitStack(&s);
    BiTree p = T;
    while (p || !StackEmpty(&s)) {
        while (p) {
            printf("%c ", p->data);
            Push(&s, p);
            p = p->lchild;
        }
        if (!StackEmpty(&s)) {
            p = Pop(&s);
            p = p->rchild;
        }
    }
}

// 非递归中序遍历
void InOrder2(BiTree T) {
    Stack s;
    InitStack(&s);
    BiTree p = T;
    while (p || !StackEmpty(&s)) {
        while (p) {
            Push(&s, p);
            p = p->lchild;
        }
        if (!StackEmpty(&s)) {
            p = Pop(&s);
            printf("%c ", p->data);
            p = p->rchild;
        }
    }
}

// 非递归后序遍历（双栈法）
void BiTNodePost(BiTree T) {
    if (!T) return;
    Stack s1, s2;
    InitStack(&s1);
    InitStack(&s2);
    Push(&s1, T);
    while (!StackEmpty(&s1)) {
        BiTree node = Pop(&s1);
        Push(&s2, node);
        if (node->lchild) Push(&s1, node->lchild);
        if (node->rchild) Push(&s1, node->rchild);
    }
    while (!StackEmpty(&s2)) {
        printf("%c ", Pop(&s2)->data);
    }
}

// 层序遍历
void LevelOrder(BiTree T) {
    if (!T) return;
    Queue q;
    InitQueue(&q);
    EnQueue(&q, T);
    while (!QueueEmpty(&q)) {
        BiTree node = DeQueue(&q);
        printf("%c ", node->data);
        if (node->lchild) EnQueue(&q, node->lchild);
        if (node->rchild) EnQueue(&q, node->rchild);
    }
}