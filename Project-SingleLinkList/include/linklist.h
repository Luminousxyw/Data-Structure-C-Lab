#ifndef LINKLIST_H
#define LINKLIST_H
#include <stdbool.h>

// 定义ElemType为int
typedef int ElemType;

// 单链表中结点的类型
typedef struct LNode {
    ElemType data;       // 值域
    struct LNode* next;  // 指针域
} LNode;

// 初始化单链表
void InitList(LNode** HL);

// 清空单链表
void ClearList(LNode** HL);

// 求单链表长度
int ListSize(LNode* HL);

// 检查单链表是否为空
bool ListEmpty(LNode* HL);

// 返回单链表中指定序号的结点值（pos从1开始）
ElemType GetElem(LNode* HL, int pos);

// 遍历单链表
void TraverseList(LNode* HL);

// 从单链表中查找元素（找到返回true，并通过item返回该元素值）
bool FindList(LNode* HL, ElemType* item);

// 更新单链表中的第一个结点值为item（若链表非空）
bool UpdateList(LNode* HL, ElemType item);

// 向单链表插入元素
// mark: 0-头插法, 1-尾插法, >1-插入到第mark个位置（从1开始）
void InsertList(LNode** HL, ElemType item, int mark);

// 从单链表中删除元素
// mark: 0-按值删除（item作为要删除的值输入）, >0-按位置删除（item返回被删除的值）
bool DeleteList(LNode** HL, ElemType* item, int mark);

// 对单链表进行有序输出（不修改原链表）
// mark: 0-升序, 1-降序
void OrderOutputList(LNode* HL, int mark);

#endif