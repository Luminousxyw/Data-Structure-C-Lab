#ifndef __SEARCH_SORT_H__
#define __SEARCH_SORT_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE      1000
#define MAX_INDEX     20
#define HASHSIZE      13
#define NULLKEY      -32768

typedef int KeyType;
typedef int InfoType;

/* Record type */
typedef struct {
    KeyType key;
    InfoType other;
} RecType;

/* Sequential list (data[0] used as sentinel / temp storage) */
typedef struct {
    RecType data[MAX_SIZE + 1];
    int length;
} SqList;

typedef SqList SSTable;

/* Block search index */
typedef struct {
    KeyType maxKey;
    int     start;
} Index;

/* Binary Search Tree node */
typedef struct BSTNode {
    KeyType key;
    struct BSTNode *lchild, *rchild;
} BSTNode;

/* Hash table (linear probing) */
typedef struct {
    KeyType elem[HASHSIZE];
    int     count;
} HashTable;

/* Search algorithms */
int       Search_Seq    (SSTable ST, KeyType kval);
int       Search_Bin    (SSTable ST, KeyType kval);
int       Search_Block  (RecType ST[], Index ind[], KeyType key, int n, int b);
BSTNode*  Search_BST    (BSTNode *T, KeyType key);
void      Insert_BST    (BSTNode **T, KeyType key);
int       Delete_BST    (BSTNode **T, KeyType key);
void      SearchHash    (HashTable H, KeyType K, int *p, int *c);

/* Sorting algorithms (pure algorithm, no printing inside) */
void BinInsSort            (SqList *L);
void Shell_sort            (SqList *L, int dk[], int t);
void Bubble_Sort           (SqList *L);
void QuickSort             (SqList *L, int low, int high);
void Simple_selection_sort (SqList *L);
void Heap_Sort             (SqList *L);
void Merge_sort            (SqList *L, RecType DR[]);

/* Utility functions */
void PrintList(SqList L, const char *title);
void FreeBST(BSTNode **T);

#endif