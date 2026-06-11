#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void InitList(LNode** HL) {
    *HL = NULL;
}

void ClearList(LNode** HL) {
    LNode* p;
    while (*HL != NULL) {
        p = *HL;
        *HL = (*HL)->next;
        free(p);
    }
}

int ListSize(LNode* HL) {
    int count = 0;
    LNode* p = HL;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

bool ListEmpty(LNode* HL) {
    return HL == NULL;
}

const char* GetElem(LNode* HL, int pos) {
    if (pos < 1) return NULL;
    LNode* p = HL;
    int i = 1;
    while (p != NULL && i < pos) {
        p = p->next;
        i++;
    }
    if (p == NULL) return NULL;
    return p->data;
}

void TraverseList(LNode* HL) {
    LNode* p = HL;
    while (p != NULL) {
        printf("%s ", p->data);
        p = p->next;
    }
}

bool FindList(LNode* HL, const char* item) {
    LNode* p = HL;
    while (p != NULL) {
        if (strcmp(p->data, item) == 0) return true;
        p = p->next;
    }
    return false;
}

bool UpdateList(LNode* HL, int pos, const char* item) {
    if (pos < 1) return false;
    LNode* p = HL;
    int i = 1;
    while (p != NULL && i < pos) {
        p = p->next;
        i++;
    }
    if (p == NULL) return false;
    strcpy(p->data, item);
    return true;
}

void InsertList(LNode** HL, const char* item, int mark) {
    LNode* newNode = (LNode*)malloc(sizeof(LNode));
    strcpy(newNode->data, item);
    newNode->next = NULL;

    if (mark == 1) {
        newNode->next = *HL;
        *HL = newNode;
    } else if (mark == -1) {
        if (*HL == NULL) {
            *HL = newNode;
        } else {
            LNode* p = *HL;
            while (p->next != NULL) p = p->next;
            p->next = newNode;
        }
    } else if (mark > 1) {
        if (*HL == NULL) { free(newNode); return; }
        LNode* p = *HL;
        int i = 1;
        while (p != NULL && i < mark - 1) {
            p = p->next;
            i++;
        }
        if (p == NULL) { free(newNode); return; }
        newNode->next = p->next;
        p->next = newNode;
    } else {
        free(newNode);
    }
}

bool DeleteList(LNode** HL, char* item_out, int mark) {
    if (*HL == NULL) return false;

    if (mark == 0) {
        LNode *p = *HL, *prev = NULL;
        while (p != NULL) {
            if (strcmp(p->data, item_out) == 0) {
                if (prev == NULL) *HL = p->next;
                else              prev->next = p->next;
                free(p);
                return true;
            }
            prev = p;
            p = p->next;
        }
        return false;
    } else if (mark > 0) {
        if (mark == 1) {
            LNode* p = *HL;
            strcpy(item_out, p->data);
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
            strcpy(item_out, q->data);
            p->next = q->next;
            free(q);
            return true;
        }
    }
    return false;
}

// 判断字符串是否纯数字（非负整数），可扩展负号
static int is_number(const char* s) {
    if (*s == '\0') return 0;
    if (*s == '-' && *(s + 1) != '\0') s++;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

// 比较两个元素，返回 <0 表示 a<b, >0 表示 a>b, ==0 相等
// 数字在前且按数值，字符串在后按字典序
static int elem_compare(const char* a, const char* b) {
    int na = is_number(a);
    int nb = is_number(b);

    if (na && nb) {
        // 都是数字，按数值比
        long long va = atoll(a);
        long long vb = atoll(b);
        return (va > vb) - (va < vb);
    }
    if (!na && !nb) {
        // 都是字符串，按字典序
        return strcmp(a, b);
    }
    // 一个是数字一个是字符串，数字在前
    return na ? -1 : 1;
}

void OrderOutputList(LNode* HL, int mark) {
    if (HL == NULL) return;
    int n = ListSize(HL);
    const char** arr = (const char**)malloc(n * sizeof(const char*));
    LNode* p = HL;
    for (int i = 0; i < n; i++) {
        arr[i] = p->data;
        p = p->next;
    }

    // 冒泡排序
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            int cmp = elem_compare(arr[j], arr[j + 1]);
            if ((mark == 0 && cmp > 0) || (mark != 0 && cmp < 0)) {
                const char* tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }

    for (int i = 0; i < n; i++)
        printf("%s ", arr[i]);
    free(arr);
}