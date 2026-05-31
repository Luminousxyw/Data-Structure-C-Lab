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

// 返回单链表中指定序号的结点值
ElemType GetElem(LNode* HL, int pos);

// 遍历单链表
void TraverseList(LNode* HL);

// 从单链表中查找元素
bool FindList(LNode* HL, ElemType* item);

// 更新单链表中的结点值为item
// ？不知道更新哪一个节点
bool UpdateList(LNode* HL, ElemType item);

// 向单链表插入元素
void InsertList(LNode** HL, ElemType item, int mark);

// 从单链表中删除元素
bool DeleteList(LNode** HL, ElemType* item, int mark);

// 对单链表进行有序输出
void OrderOutputList(LNode* HL, int mark);

#endif