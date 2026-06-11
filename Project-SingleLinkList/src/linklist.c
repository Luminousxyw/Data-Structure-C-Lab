#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>

void InitList(LinkedList* list, PrintFunc pf, CompareFunc cf, FreeFunc ff) {
    if (pf == NULL || cf == NULL) {
        fprintf(stderr, "Error: PrintFunc and CompareFunc must not be NULL.\n");
        exit(1);
    }
    list->head      = NULL;
    list->print     = pf;
    list->compare   = cf;
    list->free_elem = ff;
}

void ClearList(LinkedList* list) {
    LNode* p;
    while (list->head != NULL) {
        p = list->head;
        list->head = list->head->next;
        if (list->free_elem && p->data) {
            list->free_elem(p->data);
        }
        free(p);
    }
}

int ListSize(const LinkedList* list) {
    int count = 0;
    LNode* p = list->head;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

bool ListEmpty(const LinkedList* list) {
    return list->head == NULL;
}

void* GetElem(const LinkedList* list, int pos) {
    if (pos < 1) return NULL;
    LNode* p = list->head;
    int i = 1;
    while (p != NULL && i < pos) {
        p = p->next;
        i++;
    }
    if (p == NULL) return NULL;
    return p->data;
}

void TraverseList(const LinkedList* list) {
    LNode* p = list->head;
    while (p != NULL) {
        list->print(p->data);
        printf(" ");
        p = p->next;
    }
    printf("\n");
}

bool FindList(const LinkedList* list, const void* key, void** result) {
    LNode* p = list->head;
    while (p != NULL) {
        if (list->compare(key, p->data) == 0) {
            if (result) *result = p->data;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool UpdateList(LinkedList* list, int pos, void* new_data) {
    if (pos < 1) return false;
    LNode* p = list->head;
    int i = 1;
    while (p != NULL && i < pos) {
        p = p->next;
        i++;
    }
    if (p == NULL) return false;
    if (list->free_elem && p->data) {
        list->free_elem(p->data);
    }
    p->data = new_data;
    return true;
}

void InsertList(LinkedList* list, void* data, int mark) {
    LNode* newNode = (LNode*)malloc(sizeof(LNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (mark == 1) {
        newNode->next = list->head;
        list->head    = newNode;
    } else if (mark == -1) {
        if (list->head == NULL) {
            list->head = newNode;
        } else {
            LNode* p = list->head;
            while (p->next != NULL) p = p->next;
            p->next = newNode;
        }
    } else if (mark > 1) {
        if (list->head == NULL) {
            printf("List is empty, cannot insert at specified position!\n");
            free(newNode);
            return;
        }
        LNode* p = list->head;
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
    } else {
        printf("Invalid mark parameter!\n");
        free(newNode);
    }
}

bool DeleteList(LinkedList* list, void** item, int mark) {
    if (list->head == NULL) return false;

    if (mark == 0) {
        void* key = *item;
        LNode *p = list->head, *prev = NULL;
        while (p != NULL) {
            if (list->compare(key, p->data) == 0) {
                if (prev == NULL) {
                    list->head = p->next;
                } else {
                    prev->next = p->next;
                }
                if (list->free_elem && p->data) {
                    list->free_elem(p->data);
                }
                free(p);
                return true;
            }
            prev = p;
            p = p->next;
        }
        return false;
    } else if (mark > 0) {
        if (mark == 1) {
            LNode* p = list->head;
            *item = p->data;
            list->head = p->next;
            free(p);
            return true;
        } else {
            LNode* p = list->head;
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

void OrderOutputList(const LinkedList* list, int mark) {
    if (list->head == NULL) {
        printf("\n");
        return;
    }
    int n = ListSize(list);
    void** arr = (void**)malloc(n * sizeof(void*));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }
    LNode* p = list->head;
    for (int i = 0; i < n; i++) {
        arr[i] = p->data;
        p = p->next;
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (list->compare(arr[j], arr[j + 1]) > 0) {
                void* tmp = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    if (mark == 0) {
        for (int i = 0; i < n; i++) {
            list->print(arr[i]);
            printf(" ");
        }
    } else {
        for (int i = n - 1; i >= 0; i--) {
            list->print(arr[i]);
            printf(" ");
        }
    }
    printf("\n");
    free(arr);
}