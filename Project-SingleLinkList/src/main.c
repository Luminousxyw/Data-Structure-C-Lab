#include <stdio.h>
#include <stdlib.h>
#include "linklist.h"

static void print_int(const void* data) {
    printf("%d", *(const int*)data);
}

static int compare_int(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

static void free_int(void* data) {
    free(data);
}

static int* make_int(int val) {
    int* p = (int*)malloc(sizeof(int));
    if (p == NULL) {
        fprintf(stderr, "malloc failed!\n");
        exit(1);
    }
    *p = val;
    return p;
}

static void show_list(const LinkedList* list) {
    printf("List: ");
    TraverseList(list);
    printf("Length: %d  Empty: %s\n\n",
           ListSize(list), ListEmpty(list) ? "yes" : "no");
}

static int read_int(const char* prompt) {
    int val;
    printf("%s", prompt);
    while (scanf("%d", &val) != 1) {
        printf("Invalid input, try again: ");
        while (getchar() != '\n');
    }
    return val;
}

int main() {
    LinkedList list;
    InitList(&list, print_int, compare_int, free_int);


    int choice;
    do {
        printf("Menu:\n");
        printf(" 1.Tail insert  2.Head insert  3.Position insert\n");
        printf(" 4.Delete by val  5.Delete by pos  6.Update by pos\n");
        printf(" 7.Find by val  8.Get by pos  9.Traverse\n");
        printf("10.Asc output  11.Desc output\n");
        printf("12.Length  13.Empty?  14.Clear\n");
        printf(" 0.Exit\n");

        choice = read_int("Select: ");
        while (getchar() != '\n');
        printf("\n");

        switch (choice) {
        case 1: {
            int val = read_int("Value to tail-insert: ");
            InsertList(&list, make_int(val), -1);
            printf("Tail insert done.\n");
            show_list(&list);
            break;
        }
        case 2: {
            int val = read_int("Value to head-insert: ");
            InsertList(&list, make_int(val), 1);
            printf("Head insert done.\n");
            show_list(&list);
            break;
        }
        case 3: {
            int pos = read_int("Insert at position: ");
            int val = read_int("Value: ");
            if (pos == 1) {
                InsertList(&list, make_int(val), 1);
            } else if (pos > 1) {
                InsertList(&list, make_int(val), pos);
            } else {
                printf("Position must >= 1\n");
            }
            show_list(&list);
            break;
        }
        case 4: {
            int val = read_int("Value to delete: ");
            void* key = &val;
            if (DeleteList(&list, &key, 0)) {
                printf("Deleted (val = %d)\n", val);
            } else {
                printf("Value not found.\n");
            }
            show_list(&list);
            break;
        }
        case 5: {
            int pos = read_int("Position to delete: ");
            void* deleted = NULL;
            if (DeleteList(&list, &deleted, pos)) {
                printf("Deleted, value = %d\n", *(int*)deleted);
                free(deleted);
            } else {
                printf("Delete failed (out of bounds).\n");
            }
            show_list(&list);
            break;
        }
        case 6: {
            int pos = read_int("Position to update: ");
            int val = read_int("New value: ");
            if (UpdateList(&list, pos, make_int(val))) {
                printf("Update done.\n");
            } else {
                printf("Update failed (out of bounds).\n");
            }
            show_list(&list);
            break;
        }
        case 7: {
            int val = read_int("Value to find: ");
            void* found = NULL;
            if (FindList(&list, &val, &found)) {
                printf("Found, value = %d\n", *(int*)found);
            } else {
                printf("Not found.\n");
            }
            show_list(&list);
            break;
        }
        case 8: {
            int pos = read_int("Position: ");
            void* data = GetElem(&list, pos);
            if (data) {
                printf("Element[%d] = %d\n", pos, *(int*)data);
            } else {
                printf("Position out of bounds.\n");
            }
            show_list(&list);
            break;
        }
        case 9:
            show_list(&list);
            break;
        case 10:
            printf("Asc: ");
            OrderOutputList(&list, 0);
            printf("\n");
            break;
        case 11:
            printf("Desc: ");
            OrderOutputList(&list, 1);
            printf("\n");
            break;
        case 12:
            printf("Length = %d\n\n", ListSize(&list));
            break;
        case 13:
            printf("Empty: %s\n\n", ListEmpty(&list) ? "yes" : "no");
            break;
        case 14:
            ClearList(&list);
            printf("List cleared.\n");
            show_list(&list);
            break;
        case 0:
            printf("Bye!\n");
            break;
        default:
            printf("Invalid option, try again.\n\n");
            break;
        }
    } while (choice != 0);

    ClearList(&list);
    return 0;
}