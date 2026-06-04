#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>

// 初始化单链表
void InitList(LNode** HL) {
    *HL = NULL;
}

// 清空单链表
void ClearList(LNode** HL) {
    LNode* p;
    while (*HL != NULL) {
        p = *HL;
        *HL = (*HL)->next;
        free(p);
    }
}

// 求单链表长度
int ListSize(LNode* HL) {
    int count = 0;
    LNode* p = HL;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// 检查单链表是否为空
bool ListEmpty(LNode* HL) {
    return (HL == NULL);
}

// 返回单链表中指定序号的结点值
ElemType GetElem(LNode* HL, int pos) {
    if (pos < 1) {
        printf("Invalid position!\n");
        exit(1);
    }
    LNode* p = HL;
    int i = 1;
    while (p != NULL && i < pos) {
        p = p->next;
        i++;
    }
    if (p == NULL) {
        printf("Position out of bounds!\n");
        exit(1);
    }
    return p->data;
}

// 遍历单链表
void TraverseList(LNode* HL) {
    LNode* p = HL;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

// 从单链表中查找元素
bool FindList(LNode* HL, ElemType* item) {
    LNode* p = HL;
    while (p != NULL) {
        if (p->data == *item) {
            *item = p->data;   // 返回找到的值
            return true;
        }
        p = p->next;
    }
    return false;
}

// 更新单链表中的给定元素
// 将链表第一个结点的值更新为 item
bool UpdateList(LNode* HL, ElemType item) {
    if (HL == NULL) {
        return false;
    }
    HL->data = item;
    return true;
}

// 向单链表插入元素
void InsertList(LNode** HL, ElemType item, int mark) {
    LNode* newNode = (LNode*)malloc(sizeof(LNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = item;
    newNode->next = NULL;

    if (mark == 1) {
        // 头插法
        newNode->next = *HL;
        *HL = newNode;
    }
    else if (mark == -1) {
        // 尾插法
        if (*HL == NULL) {
            *HL = newNode;
        } else {
            LNode* p = *HL;
            while (p->next != NULL) {
                p = p->next;
            }
            p->next = newNode;
        }
    }
    else if (mark > 0) {
        // 指定位置插入
        if (*HL == NULL) {
            printf("List is empty, cannot insert at specified position!\n");
            free(newNode);
            return;
        }
        LNode* p = *HL;
        int i = 1;
        while (p != NULL && i < mark - 1) {
            p = p->next;
            i++;
        }
        if (p == NULL) {
            printf("Insert position out of bounds!\n");
            free(newNode);
            return;
        }
        newNode->next = p->next;
        p->next = newNode;
    }
    else {
        printf("Invalid mark parameter!\n");
        free(newNode);
    }
}

// 从单链表中删除元素
bool DeleteList(LNode** HL, ElemType* item, int mark) {
    if (*HL == NULL) return false;

    if (mark == 0) {
        // 按值删除
        LNode *p = *HL, *prev = NULL;
        while (p != NULL) {
            if (p->data == *item) {
                if (prev == NULL) {
                    *HL = p->next;
                } else {
                    prev->next = p->next;
                }
                *item = p->data;  // 返回被删除的值
                free(p);
                return true;
            }
            prev = p;
            p = p->next;
        }
        return false;
    }
    else if (mark > 0) {
        // 按位置删除
        if (mark == 1) {
            LNode* p = *HL;
            *item = p->data;
            *HL = p->next;
            free(p);
            return true;
        } else {
            LNode* p = *HL;
            int i = 1;
            while (p != NULL && i < mark - 1) {
                p = p->next;
                i++;
            }
            if (p == NULL || p->next == NULL) return false;
            LNode* q = p->next;
            *item = q->data;
            p->next = q->next;
            free(q);
            return true;
        }
    }
    return false;
}

// 对单链表进行有序输出
void OrderOutputList(LNode* HL, int mark) {
    if (HL == NULL) {
        printf("\n");
        return;
    }
    int n = ListSize(HL);
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    LNode* p = HL;
    for (int i = 0; i < n; i++) {
        arr[i] = p->data;
        p = p->next;
    }
    // 冒泡排序
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (mark == 0) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            } else {
                if (arr[j] < arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
}