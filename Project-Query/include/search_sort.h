#ifndef __SEARCH_SORT_H__
#define __SEARCH_SORT_H__

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE      100
#define MAX_INDEX     20
#define HASHSIZE      13
#define NULLKEY       -32768

typedef int KeyType;
typedef int InfoType;

/* 记录类型 */
typedef struct {
    KeyType key;
    InfoType other;
} RecType;

/* 顺序表（data[0]用作哨兵/暂存） */
typedef struct {
    RecType data[MAX_SIZE + 1];
    int length;
} SqList;

typedef SqList SSTable;

/* 分块查找索引 */
typedef struct {
    KeyType maxKey;
    int     start;
} Index;

/* 二叉排序树结点 */
typedef struct BSTNode {
    KeyType key;
    struct BSTNode *lchild, *rchild;
} BSTNode;

/* 哈希表（线性探测法） */
typedef struct {
    KeyType elem[HASHSIZE];
    int     count;
} HashTable;

/* 查找算法 */
int        Search_Seq   (SSTable ST, KeyType kval);
int        Search_Bin   (SSTable ST, KeyType kval);
int        Search_Block (RecType ST[], Index ind[], KeyType key, int n, int b);
BSTNode*   Search_BST   (BSTNode *T, KeyType key);
void       Insert_BST   (BSTNode **T, KeyType key);
int        Delete_BST   (BSTNode **T, KeyType key);
void       SearchHash   (HashTable H, KeyType K, int *p, int *c);

/* 排序算法 */
void BinInsSort            (SqList *L);
void Shell_sort            (SqList *L, int dk[], int t);
void Bubble_Sort           (SqList *L);
void QuickSort             (SqList *L, int low, int high);
void Simple_selection_sort (SqList *L);
void Heap_Sort             (SqList *L);
void Merge_sort            (SqList *L, RecType DR[]);

/* 辅助输出 */
void PrintList(SqList L, const char *title);

#endif