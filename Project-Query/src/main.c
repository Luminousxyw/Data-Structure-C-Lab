#include "search_sort.h"
#include <string.h>

/* Copy array into sequential list */
static void LoadList(SqList *L, int arr[], int n) {
    L->length = n;
    for (int i = 1; i <= n; i++) {
        L->data[i].key   = arr[i - 1];
        L->data[i].other = 0;
    }
}

/* Inorder traversal of BST */
static void InOrder_BST(BSTNode *T) {
    if (!T) return;
    InOrder_BST(T->lchild);
    printf("%d ", T->key);
    InOrder_BST(T->rchild);
}

/* ---------- Search Menu ---------- */
static void menu_search(SqList L, int arr[], int n) {
    int choice, key, pos, p, c;
    clock_t start, end;
    double cpu_time;

    do {
        printf("\n========== Search Algorithms ==========\n");
        printf("  1. Sequential Search\n");
        printf("  2. Binary Search\n");
        printf("  3. Block Search\n");
        printf("  4. BST (build / search / delete)\n");
        printf("  5. Hash Search (linear probing)\n");
        printf("  0. Back to main menu\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: // Sequential Search
            printf("Enter key to search: ");
            scanf("%d", &key);
            start = clock();
            pos = Search_Seq(L, key);
            end = clock();
            cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            if (pos > 0)
                printf("Found %d at position = %d\n", key, pos);
            else
                printf("%d not found\n", key);
            printf("Time: %.4f ms\n", cpu_time);
            break;

        case 2: // Binary Search (auto-sort first)
            {
                SqList tmpL;
                LoadList(&tmpL, arr, n);
                QuickSort(&tmpL, 1, tmpL.length);
                printf("(Auto-sorted) Sequence: ");
                for (int i = 1; i <= tmpL.length; i++)
                    printf("%d ", tmpL.data[i].key);
                printf("\nEnter key to search: ");
                scanf("%d", &key);
                start = clock();
                pos = Search_Bin(tmpL, key);
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                if (pos > 0)
                    printf("Found %d at position = %d\n", key, pos);
                else
                    printf("%d not found\n", key);
                printf("Time: %.4f ms\n", cpu_time);
            }
            break;

        case 3: // Block Search
            {
                SqList tmpL;
                LoadList(&tmpL, arr, n);
                QuickSort(&tmpL, 1, tmpL.length);
                printf("(Auto-sorted) Sequence: ");
                for (int i = 1; i <= tmpL.length; i++)
                    printf("%d ", tmpL.data[i].key);

                /* Auto-build index table */
                RecType ST[MAX_SIZE];
                for (int i = 0; i < n; i++) {
                    ST[i].key = tmpL.data[i + 1].key;
                    ST[i].other = 0;
                }

                int blockSize = (n + 2) / 3;   // roughly 3 blocks
                if (blockSize < 2) blockSize = 2;
                int b = (n + blockSize - 1) / blockSize;
                if (b > MAX_INDEX) b = MAX_INDEX;
                Index ind[MAX_INDEX];
                for (int i = 0; i < b; i++) {
                    int blockEnd = (i + 1) * blockSize - 1;
                    if (blockEnd >= n) blockEnd = n - 1;
                    ind[i].maxKey = tmpL.data[blockEnd + 1].key;
                    ind[i].start  = i * blockSize;
                }

                printf("\nIndex table: ");
                for (int i = 0; i < b; i++)
                    printf("[max=%d,start=%d] ", ind[i].maxKey, ind[i].start);
                printf("\nEnter key to search: ");
                scanf("%d", &key);
                start = clock();
                pos = Search_Block(ST, ind, key, n, b);
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                if (pos >= 0)
                    printf("Found %d at index = %d\n", key, pos);
                else
                    printf("%d not found\n", key);
                printf("Time: %.4f ms\n", cpu_time);
            }
            break;

        case 4: // Binary Search Tree
            {
                BSTNode *bst = NULL;
                printf("Building BST from your data...\n");
                start = clock();
                for (int i = 0; i < n; i++)
                    Insert_BST(&bst, arr[i]);
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                printf("BST built. Inorder traversal: ");
                InOrder_BST(bst);
                printf("\nBuild time: %.4f ms\n", cpu_time);

                printf("Enter key to search: ");
                scanf("%d", &key);
                start = clock();
                BSTNode *res = Search_BST(bst, key);
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                printf("%s\n", res ? "Found" : "Not found");
                printf("Search time: %.4f ms\n", cpu_time);

                printf("Enter key to delete: ");
                scanf("%d", &key);
                start = clock();
                int del = Delete_BST(&bst, key);
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                if (del) {
                    printf("After deletion, inorder: ");
                    InOrder_BST(bst);
                    printf("\n");
                } else {
                    printf("Key %d does not exist, nothing deleted\n", key);
                }
                printf("Delete time: %.4f ms\n", cpu_time);
                FreeBST(&bst);
            }
            break;

        case 5: // Hash Search
            {
                HashTable H;
                for (int i = 0; i < HASHSIZE; i++)
                    H.elem[i] = NULLKEY;
                H.count = 0;

                printf("Building hash table (H(key)=key%%%d, linear probing)...\n", HASHSIZE);
                start = clock();
                for (int i = 0; i < n; i++) {
                    int pp, cc;
                    SearchHash(H, arr[i], &pp, &cc);
                    if (pp != -1 && H.elem[pp] == NULLKEY) {
                        H.elem[pp] = arr[i];
                        H.count++;
                    }
                }
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;

                printf("Hash table: ");
                for (int i = 0; i < HASHSIZE; i++) {
                    if (H.elem[i] == NULLKEY)
                        printf("[ ] ");
                    else
                        printf("[%d] ", H.elem[i]);
                }
                printf("\nBuild time: %.4f ms\n", cpu_time);

                printf("Enter key to search: ");
                scanf("%d", &key);
                start = clock();
                SearchHash(H, key, &p, &c);
                end = clock();
                cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                if (p != -1)
                    printf("Found %d at address=%d, comparisons=%d\n", key, p, c);
                else
                    printf("%d not found\n", key);
                printf("Search time: %.4f ms\n", cpu_time);
            }
            break;

        case 0:
            break;

        default:
            printf("Invalid choice, please try again.\n");
        }
    } while (choice != 0);
}

/* ---------- Sort Menu ---------- */
static void menu_sort(SqList orig, int n) {
    int choice;
    SqList L;
    RecType DR[MAX_SIZE + 1];
    clock_t start, end;
    double cpu_time;
    int dk[] = {5, 3, 1};
    int t = 3;

    do {
        printf("\n========== Sorting Algorithms ==========\n");
        printf("  1. Binary Insertion Sort\n");
        printf("  2. Shell Sort\n");
        printf("  3. Bubble Sort\n");
        printf("  4. Quick Sort\n");
        printf("  5. Simple Selection Sort\n");
        printf("  6. Heap Sort\n");
        printf("  7. Merge Sort\n");
        printf("  8. Compare All (timing only)\n");
        printf("  0. Back to main menu\n");
        printf("Current data: ");
        for (int i = 1; i <= orig.length; i++)
            printf("%d ", orig.data[i].key);
        printf("\nChoice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice >= 1 && choice <= 7) {
            /* Single sort: print before & after */
            L = orig;
            printf("Before: ");
            PrintList(L, "");
            printf("\n");

            switch (choice) {
            case 1:
                start = clock();
                BinInsSort(&L);
                end = clock();
                printf("After:  ");
                PrintList(L, "Binary Insertion Sort");
                break;
            case 2:
                start = clock();
                Shell_sort(&L, dk, t);
                end = clock();
                printf("After:  ");
                PrintList(L, "Shell Sort");
                break;
            case 3:
                start = clock();
                Bubble_Sort(&L);
                end = clock();
                printf("After:  ");
                PrintList(L, "Bubble Sort");
                break;
            case 4:
                start = clock();
                QuickSort(&L, 1, L.length);
                end = clock();
                printf("After:  ");
                PrintList(L, "Quick Sort");
                break;
            case 5:
                start = clock();
                Simple_selection_sort(&L);
                end = clock();
                printf("After:  ");
                PrintList(L, "Simple Selection Sort");
                break;
            case 6:
                start = clock();
                Heap_Sort(&L);
                end = clock();
                printf("After:  ");
                PrintList(L, "Heap Sort");
                break;
            case 7:
                start = clock();
                Merge_sort(&L, DR);
                end = clock();
                printf("After:  ");
                printf("Merge Sort: ");
                for (int i = 1; i <= L.length; i++)
                    printf("%d ", DR[i].key);
                printf("\n");
                break;
            }
            cpu_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            printf("Time: %.4f ms\n", cpu_time);

        } else if (choice == 8) {
            /* Compare all — timing only, no printed output */
            printf("\n========== All Sorting Algorithms Comparison ==========\n");
            printf("Data size: n = %d\n", n);
            printf("%-25s %s\n", "Algorithm", "Time (ms)");
            printf("------------------------------------------\n");

            // 1. Binary Insertion
            L = orig;
            start = clock();
            BinInsSort(&L);
            end = clock();
            printf("%-25s %.4f ms\n", "1. Binary Insertion Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            // 2. Shell
            L = orig;
            start = clock();
            Shell_sort(&L, dk, t);
            end = clock();
            printf("%-25s %.4f ms\n", "2. Shell Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            // 3. Bubble
            L = orig;
            start = clock();
            Bubble_Sort(&L);
            end = clock();
            printf("%-25s %.4f ms\n", "3. Bubble Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            // 4. Quick
            L = orig;
            start = clock();
            QuickSort(&L, 1, L.length);
            end = clock();
            printf("%-25s %.4f ms\n", "4. Quick Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            // 5. Selection
            L = orig;
            start = clock();
            Simple_selection_sort(&L);
            end = clock();
            printf("%-25s %.4f ms\n", "5. Selection Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            // 6. Heap
            L = orig;
            start = clock();
            Heap_Sort(&L);
            end = clock();
            printf("%-25s %.4f ms\n", "6. Heap Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            // 7. Merge
            L = orig;
            start = clock();
            Merge_sort(&L, DR);
            end = clock();
            printf("%-25s %.4f ms\n", "7. Merge Sort",
                   (double)(end - start) / CLOCKS_PER_SEC * 1000);

            printf("\nAll algorithms timed successfully!\n");
            printf("Note: Original data unchanged. Use choices 1-7 to view sorted results.\n");

        } else if (choice != 0) {
            printf("Invalid choice, please try again.\n");
        }
    } while (choice != 0);
}

/* ---------- Main Function ---------- */
int main(void) {
    int arr[MAX_SIZE];
    int n;
    SqList L;
    int mainChoice;

    printf("========================================\n");
    printf("   Search & Sort Algorithm Demo\n");
    printf("========================================\n\n");

    /* Input data */
    printf("How many numbers? ");
    while (scanf("%d", &n) != 1 || n <= 0 || n > MAX_SIZE) {
        printf("Invalid input. Enter an integer between 1 and %d: ", MAX_SIZE);
        while (getchar() != '\n');
    }

    printf("Enter %d integers (space-separated): ", n);
    for (int i = 0; i < n; i++) {
        while (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input. Re-enter number %d: ", i + 1);
            while (getchar() != '\n');
        }
    }

    LoadList(&L, arr, n);

    printf("\nData loaded successfully! %d numbers: ", n);
    for (int i = 1; i <= L.length; i++)
        printf("%d ", L.data[i].key);
    printf("\n");

    do {
        printf("\n============== Main Menu ==============\n");
        printf("  1. Search Algorithms\n");
        printf("  2. Sorting Algorithms\n");
        printf("  3. Re-enter Data\n");
        printf("  0. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &mainChoice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (mainChoice) {
        case 1:
            menu_search(L, arr, n);
            break;
        case 2:
            menu_sort(L, n);
            break;
        case 3:
            printf("How many numbers? ");
            while (scanf("%d", &n) != 1 || n <= 0 || n > MAX_SIZE) {
                printf("Invalid input. Enter an integer between 1 and %d: ", MAX_SIZE);
                while (getchar() != '\n');
            }
            printf("Enter %d integers (space-separated): ", n);
            for (int i = 0; i < n; i++) {
                while (scanf("%d", &arr[i]) != 1) {
                    printf("Invalid input. Re-enter number %d: ", i + 1);
                    while (getchar() != '\n');
                }
            }
            LoadList(&L, arr, n);
            printf("Data updated!\n");
            printf("New data: ");
            for (int i = 1; i <= L.length; i++)
                printf("%d ", L.data[i].key);
            printf("\n");
            break;
        case 0:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice, please try again.\n");
        }
    } while (mainChoice != 0);

    return 0;
}