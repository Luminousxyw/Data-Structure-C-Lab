#include "search_sort.h"

/* ========== 查找算法 ========== */

int Search_Seq(SSTable ST, KeyType kval) {
    int i;
    ST.data[0].key = kval;          // 哨兵
    for (i = ST.length; ST.data[i].key != kval; i--);
    return i;
}

int Search_Bin(SSTable ST, KeyType kval) {
    int low = 1, high = ST.length, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (ST.data[mid].key == kval)
            return mid;
        else if (ST.data[mid].key > kval)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;
}

int Search_Block(RecType ST[], Index ind[], KeyType key, int n, int b) {
    int i, j, s;
    int low = 0, high = b - 1, mid;

    // 折半查找索引表
    while (low <= high) {
        mid = (low + high) / 2;
        if (key <= ind[mid].maxKey)
            high = mid - 1;
        else
            low = mid + 1;
    }
    if (low >= b) return -1;

    i = ind[low].start;
    s = (low == b - 1) ? (n - i) : (ind[low + 1].start - i);

    // 块内顺序查找
    for (j = 0; j < s; j++)
        if (ST[i + j].key == key)
            return i + j;
    return -1;
}

BSTNode* Search_BST(BSTNode *T, KeyType key) {
    if (!T || T->key == key) return T;
    if (key < T->key)
        return Search_BST(T->lchild, key);
    else
        return Search_BST(T->rchild, key);
}

void Insert_BST(BSTNode **T, KeyType key) {
    if (!*T) {
        *T = (BSTNode *)malloc(sizeof(BSTNode));
        (*T)->key = key;
        (*T)->lchild = (*T)->rchild = NULL;
    } else if (key < (*T)->key) {
        Insert_BST(&(*T)->lchild, key);
    } else if (key > (*T)->key) {
        Insert_BST(&(*T)->rchild, key);
    }
    // 相等时忽略
}

int Delete_BST(BSTNode **T, KeyType key) {
    BSTNode *p, *q, *s;
    if (!*T) return 0;

    if (key < (*T)->key)
        return Delete_BST(&(*T)->lchild, key);
    else if (key > (*T)->key)
        return Delete_BST(&(*T)->rchild, key);
    else {
        p = *T;
        if (!p->lchild) {               // 只有右子树
            *T = p->rchild;
            free(p);
        } else if (!p->rchild) {        // 只有左子树
            *T = p->lchild;
            free(p);
        } else {                        // 左右子树均存在，用直接前驱替代
            q = p;
            s = p->lchild;
            while (s->rchild) {
                q = s;
                s = s->rchild;
            }
            p->key = s->key;
            if (q != p) q->rchild = s->lchild;
            else        q->lchild = s->lchild;
            free(s);
        }
        return 1;
    }
}

void SearchHash(HashTable H, KeyType K, int *p, int *c) {
    int addr = K % HASHSIZE;
    int count = 0;

    while (H.elem[addr] != NULLKEY && H.elem[addr] != K && count < HASHSIZE) {
        count++;
        addr = (addr + 1) % HASHSIZE;
    }

    *c = count + 1;
    if (H.elem[addr] == K) *p = addr;
    else *p = -1;
}


/* ========== 排序算法 ========== */

void PrintList(SqList L, const char *title) {
    int i;
    printf("%s: ", title);
    for (i = 1; i <= L.length; i++)
        printf("%d ", L.data[i].key);
    printf("\n");
}

void BinInsSort(SqList *L) {
    int i, j, low, high, mid;
    for (i = 2; i <= L->length; i++) {
        L->data[0] = L->data[i];
        low = 1; high = i - 1;
        while (low <= high) {          // 二分定位
            mid = (low + high) / 2;
            if (L->data[0].key < L->data[mid].key)
                high = mid - 1;
            else
                low = mid + 1;
        }
        for (j = i - 1; j >= low; j--)
            L->data[j + 1] = L->data[j];
        L->data[low] = L->data[0];
    }
    PrintList(*L, "BinInsSort");
}

void Shell_sort(SqList *L, int dk[], int t) {
    int i, j, k, d;
    for (k = 0; k < t; k++) {
        d = dk[k];
        for (i = d + 1; i <= L->length; i++) {
            if (L->data[i].key < L->data[i - d].key) {
                L->data[0] = L->data[i];
                for (j = i - d; j > 0 && L->data[0].key < L->data[j].key; j -= d)
                    L->data[j + d] = L->data[j];
                L->data[j + d] = L->data[0];
            }
        }
    }
    PrintList(*L, "Shell_sort");
}

void Bubble_Sort(SqList *L) {
    int i, j, swapped;
    RecType tmp;
    for (i = 1; i < L->length; i++) {
        swapped = 0;
        for (j = 1; j <= L->length - i; j++) {
            if (L->data[j].key > L->data[j + 1].key) {
                tmp = L->data[j];
                L->data[j] = L->data[j + 1];
                L->data[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    PrintList(*L, "Bubble_sort");
}

static int Partition(SqList *L, int low, int high) {
    KeyType pivot = L->data[low].key;
    L->data[0] = L->data[low];
    while (low < high) {
        while (low < high && L->data[high].key >= pivot) high--;
        L->data[low] = L->data[high];
        while (low < high && L->data[low].key <= pivot) low++;
        L->data[high] = L->data[low];
    }
    L->data[low] = L->data[0];
    return low;
}

void QuickSort(SqList *L, int low, int high) {
    if (low < high) {
        int pivot = Partition(L, low, high);
        QuickSort(L, low, pivot - 1);
        QuickSort(L, pivot + 1, high);
    }
}

void Simple_selection_sort(SqList *L) {
    int i, j, min;
    RecType tmp;
    for (i = 1; i < L->length; i++) {
        min = i;
        for (j = i + 1; j <= L->length; j++)
            if (L->data[j].key < L->data[min].key)
                min = j;
        if (min != i) {
            tmp = L->data[i];
            L->data[i] = L->data[min];
            L->data[min] = tmp;
        }
    }
    PrintList(*L, "Selection_sort");
}

static void HeapAdjust(SqList *L, int s, int m) {
    int j;
    RecType rc = L->data[s];
    for (j = 2 * s; j <= m; j *= 2) {
        if (j < m && L->data[j].key < L->data[j + 1].key) j++;
        if (rc.key >= L->data[j].key) break;
        L->data[s] = L->data[j];
        s = j;
    }
    L->data[s] = rc;
}

void Heap_Sort(SqList *L) {
    int i;
    RecType tmp;
    for (i = L->length / 2; i >= 1; i--)   // 建大顶堆
        HeapAdjust(L, i, L->length);
    for (i = L->length; i > 1; i--) {       // 排序
        tmp = L->data[1];
        L->data[1] = L->data[i];
        L->data[i] = tmp;
        HeapAdjust(L, 1, i - 1);
    }
    PrintList(*L, "Heap_sort");
}

static void Merge(RecType SR[], RecType TR[], int i, int m, int n) {
    int j, k;
    for (j = m + 1, k = i; i <= m && j <= n; k++) {
        if (SR[i].key <= SR[j].key)
            TR[k] = SR[i++];
        else
            TR[k] = SR[j++];
    }
    while (i <= m) TR[k++] = SR[i++];
    while (j <= n) TR[k++] = SR[j++];
}

static void MSort(RecType SR[], RecType TR1[], int s, int t) {
    RecType TR2[MAX_SIZE + 1];
    if (s == t) {
        TR1[s] = SR[s];
    } else {
        int m = (s + t) / 2;
        MSort(SR, TR2, s, m);
        MSort(SR, TR2, m + 1, t);
        Merge(TR2, TR1, s, m, t);
    }
}

void Merge_sort(SqList *L, RecType DR[]) {
    MSort(L->data, DR, 1, L->length);
    printf("Merge_sort : ");
    for (int i = 1; i <= L->length; i++)
        printf("%d ", DR[i].key);
    printf("\n");
}