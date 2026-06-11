#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdbool.h>

// 回调：打印一个元素
typedef void (*PrintFunc)(const void* data);

// 回调：比较两个元素，返回 <0 / 0 / >0
typedef int  (*CompareFunc)(const void* a, const void* b);

// 回调：释放一个元素
typedef void (*FreeFunc)(void* data);

// 节点
typedef struct LNode {
    void*        data;
    struct LNode* next;
} LNode;

// 链表
typedef struct LinkedList {
    LNode*      head;
    PrintFunc   print;
    CompareFunc compare;
    FreeFunc    free_elem;
} LinkedList;

// 初始化，print 和 compare 不可为 NULL
void  InitList   (LinkedList* list, PrintFunc pf, CompareFunc cf, FreeFunc ff);

// 清空
void  ClearList  (LinkedList* list);

// 长度 & 判空
int   ListSize   (const LinkedList* list);
bool  ListEmpty  (const LinkedList* list);

// 按位置取数据指针，失败返回 NULL
void* GetElem    (const LinkedList* list, int pos);

// 遍历打印
void  TraverseList(const LinkedList* list);

// 按值查找，result 接收找到的数据指针
bool  FindList   (const LinkedList* list, const void* key, void** result);

// 按位置更新，释放旧数据，接管新数据
bool  UpdateList (LinkedList* list, int pos, void* new_data);

// 插入：1=头插  -1=尾插  >1=第 mark 个位置
void  InsertList (LinkedList* list, void* data, int mark);

// 删除：0=按值(*item为键)  >0=按位置(*item接收数据指针)
bool  DeleteList (LinkedList* list, void** item, int mark);

// 有序输出：0=升序  非0=降序
void  OrderOutputList(const LinkedList* list, int mark);

#endif