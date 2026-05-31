#include "search_sort.h"

/* 将数组拷贝到顺序表 */
static void LoadList(SqList *L, int arr[], int n) {
    L->length = n;
    for (int i = 1; i <= n; i++) {
        L->data[i].key   = arr[i - 1];
        L->data[i].other = 0;
    }
}

/* 中序遍历BST */
static void InOrder_BST(BSTNode *T) {
    if (!T) return;
    InOrder_BST(T->lchild);
    printf("%d ", T->key);
    InOrder_BST(T->rchild);
}

int main(void) {
    // ===== 一、查找测试 =====
    SqList L1;
    int arr1[] = {5, 13, 19, 21, 37, 56, 64, 75, 80, 88, 92};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    LoadList(&L1, arr1, n1);

    printf("=== Search_Seq ===\n");
    printf("  Find 37 -> pos = %d\n", Search_Seq(L1, 37));
    printf("  Find 99 -> pos = %d\n\n", Search_Seq(L1, 99));

    printf("=== Search_Bin ===\n");
    printf("  Find 56 -> pos = %d\n", Search_Bin(L1, 56));
    printf("  Find 10 -> pos = %d\n\n", Search_Bin(L1, 10));

    printf("=== Search_Block ===\n");
    RecType ST[] = {
        {5,0},{13,0},{19,0},
        {21,0},{37,0},{56,0},{64,0},
        {75,0},{80,0},{88,0},{92,0}
    };
    Index ind[3] = { {19,0}, {64,3}, {92,7} };
    int pos;
    pos = Search_Block(ST, ind, 37, 11, 3);
    printf("  Find 37 -> pos = %d\n", pos);
    pos = Search_Block(ST, ind, 99, 11, 3);
    printf("  Find 99 -> pos = %d\n\n", pos);

    printf("=== BST ===\n");
    BSTNode *bst = NULL;
    int bstArr[] = {50, 30, 80, 20, 40, 70, 90};
    for (int i = 0; i < 7; i++)
        Insert_BST(&bst, bstArr[i]);
    printf("  InOrder: ");
    InOrder_BST(bst);
    printf("\n");
    printf("  Search 40 -> %s\n", Search_BST(bst, 40) ? "Found" : "Not found");
    printf("  Delete 50 (root)...\n");
    Delete_BST(&bst, 50);
    printf("  InOrder: ");
    InOrder_BST(bst);
    printf("\n");
    printf("  Search 50 -> %s\n\n", Search_BST(bst, 50) ? "Found" : "Not found");

    printf("=== SearchHash ===\n");
    HashTable H;
    int hashKeys[] = {13, 26, 39, 52, 15, 28};
    for (int i = 0; i < HASHSIZE; i++)
        H.elem[i] = NULLKEY;
    H.count = 0;
    for (int i = 0; i < 6; i++) {
        int p, c;
        SearchHash(H, hashKeys[i], &p, &c);
        if (p != -1 && H.elem[p] == NULLKEY) {
            H.elem[p] = hashKeys[i];
            H.count++;
        }
    }
    printf("  Hash table:\n  ");
    for (int i = 0; i < HASHSIZE; i++) {
        if (H.elem[i] == NULLKEY)
            printf("[ ] ");
        else
            printf("[%d] ", H.elem[i]);
    }
    printf("\n");
    int p, c;
    SearchHash(H, 26, &p, &c);
    printf("  Find 26 -> addr=%d, cmp=%d\n", p, c);
    SearchHash(H, 99, &p, &c);
    printf("  Find 99 -> addr=%d (not found)\n\n", p);

    // ===== 二、排序测试 =====
    int raw[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n2   = sizeof(raw) / sizeof(raw[0]);
    SqList L;

    printf("Raw array:  ");
    for (int i = 0; i < n2; i++) printf("%d ", raw[i]);
    printf("\n\n");

    LoadList(&L, raw, n2);  BinInsSort(&L);

    LoadList(&L, raw, n2);
    int dk[] = {5, 3, 1};
    Shell_sort(&L, dk, 3);

    LoadList(&L, raw, n2);  Bubble_Sort(&L);

    LoadList(&L, raw, n2);
    QuickSort(&L, 1, L.length);
    PrintList(L, "Quick_sort");

    LoadList(&L, raw, n2);  Simple_selection_sort(&L);

    LoadList(&L, raw, n2);  Heap_Sort(&L);

    RecType DR[MAX_SIZE + 1];
    LoadList(&L, raw, n2);
    Merge_sort(&L, DR);

    getchar();
    return 0;
}