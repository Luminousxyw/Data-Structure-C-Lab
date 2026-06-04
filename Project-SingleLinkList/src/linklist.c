#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize singly linked list
void InitList(LNode** HL) {
    *HL = NULL;
}

// Clear singly linked list
void ClearList(LNode** HL) {
    LNode* p;
    while (*HL != NULL) {
        p = *HL;
        *HL = (*HL)->next;
        free(p);
    }
}

// Get the length of singly linked list
int ListSize(LNode* HL) {
    int count = 0;
    LNode* p = HL;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// Check if singly linked list is empty
bool ListEmpty(LNode* HL) {
    return (HL == NULL);
}

// Return the value of the node at specified position
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

// Traverse singly linked list
void TraverseList(LNode* HL) {
    LNode* p = HL;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

// Search for an element in singly linked list
bool FindList(LNode* HL, ElemType* item) {
    LNode* p = HL;
    while (p != NULL) {
        if (p->data == *item) {
            *item = p->data;   // return the found value
            return true;
        }
        p = p->next;
    }
    return false;
}

// Update the first node's value to item
bool UpdateList(LNode* HL, ElemType item) {
    if (HL == NULL) {
        return false;
    }
    HL->data = item;
    return true;
}

// Insert an element into singly linked list
void InsertList(LNode** HL, ElemType item, int mark) {
    LNode* newNode = (LNode*)malloc(sizeof(LNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = item;
    newNode->next = NULL;

    if (mark == 0) {
        // Head insertion
        newNode->next = *HL;
        *HL = newNode;
    }
    else if (mark == 1) {
        // Tail insertion
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
    else if (mark > 1) {
        // Insert at specified position
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

// Delete an element from singly linked list
bool DeleteList(LNode** HL, ElemType* item, int mark) {
    if (*HL == NULL) return false;

    if (mark == 0) {
        // Delete by value
        LNode *p = *HL, *prev = NULL;
        while (p != NULL) {
            if (p->data == *item) {
                if (prev == NULL) {
                    *HL = p->next;
                } else {
                    prev->next = p->next;
                }
                *item = p->data;  // return the deleted value
                free(p);
                return true;
            }
            prev = p;
            p = p->next;
        }
        return false;
    }
    else if (mark > 0) {
        // Delete by position
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

// Ordered output of singly linked list
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
    // Bubble sort
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