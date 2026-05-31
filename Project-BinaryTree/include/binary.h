#ifndef BINARY_H_
#define BINARY_H_

#include <stdio.h>
#include <stdlib.h>

// 二叉树结点定义
typedef struct BiTNode {
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

// 创建二叉树（前序，#表示空）
void CreatBiTree(BiTree *T);

// 递归前序遍历
void PreOrder(BiTree T);

// 递归中序遍历
void InOrder(BiTree T);

// 递归后序遍历
void PostOrder(BiTree T);

// 非递归前序遍历
void PreOrder2(BiTree T);

// 非递归中序遍历
void InOrder2(BiTree T);

// 非递归后序遍历（双栈法）
void BiTNodePost(BiTree T);

// 层序遍历
void LevelOrder(BiTree T);

#endif