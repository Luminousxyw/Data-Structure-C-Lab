#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdbool.h>

#define MAX_LEN 32

typedef char ElemType[MAX_LEN];

typedef struct LNode {
    ElemType data;
    struct LNode* next;
} LNode;

void InitList(LNode** HL);
void ClearList(LNode** HL);
int ListSize(LNode* HL);
bool ListEmpty(LNode* HL);
const char* GetElem(LNode* HL, int pos);
void TraverseList(LNode* HL);
bool FindList(LNode* HL, const char* item);
bool UpdateList(LNode* HL, int pos, const char* item);
void InsertList(LNode** HL, const char* item, int mark);
bool DeleteList(LNode** HL, char* item_out, int mark);
void OrderOutputList(LNode* HL, int mark);

#endif