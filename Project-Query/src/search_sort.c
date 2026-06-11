#include "search_sort.h"

#ifdef _WIN32
  #include <windows.h>
  static LARGE_INTEGER _freq; static int _init = 0;
  double now_us(void) {
    if (!_init) { QueryPerformanceFrequency(&_freq); _init = 1; }
    LARGE_INTEGER t; QueryPerformanceCounter(&t);
    return (double)(t.QuadPart) * 1000000.0 / (double)(_freq.QuadPart);
  }
#else
  #include <sys/time.h>
  double now_us(void) {
    struct timeval tv; gettimeofday(&tv, NULL);
    return (double)(tv.tv_sec) * 1000000.0 + (double)(tv.tv_usec);
  }
#endif

// ========== 查找 ==========

int Search_Seq(SSTable ST, KeyType kval) {
    int i; ST.data[0].key = kval; // 哨兵
    for (i = ST.length; ST.data[i].key != kval; i--);
    return i;
}

int Search_Bin(SSTable ST, KeyType kval) {
    int low = 1, high = ST.length, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (ST.data[mid].key == kval) return mid;
        else if (ST.data[mid].key > kval) high = mid - 1;
        else low = mid + 1;
    }
    return 0;
}

int Search_Block(RecType ST[], Index ind[], KeyType key, int n, int b) {
    int i, j, s, low = 0, high = b - 1, mid;
    while (low <= high) { // 索引表折半
        mid = (low + high) / 2;
        if (key <= ind[mid].maxKey) high = mid - 1;
        else low = mid + 1;
    }
    if (low >= b) return -1;
    i = ind[low].start;
    s = (low == b - 1) ? (n - i) : (ind[low + 1].start - i);
    for (j = 0; j < s; j++) // 块内顺序
        if (ST[i + j].key == key) return i + j;
    return -1;
}

BSTNode* Search_BST(BSTNode *T, KeyType key) {
    if (!T || T->key == key) return T;
    return key < T->key ? Search_BST(T->lchild, key) : Search_BST(T->rchild, key);
}

void Insert_BST(BSTNode **T, KeyType key) {
    if (!*T) {
        *T = (BSTNode*)malloc(sizeof(BSTNode));
        (*T)->key = key; (*T)->lchild = (*T)->rchild = NULL;
    } else if (key < (*T)->key) Insert_BST(&(*T)->lchild, key);
    else if (key > (*T)->key) Insert_BST(&(*T)->rchild, key);
    // 重复忽略
}

int Delete_BST(BSTNode **T, KeyType key) {
    BSTNode *p, *q, *s;
    if (!*T) return 0;
    if (key < (*T)->key) return Delete_BST(&(*T)->lchild, key);
    if (key > (*T)->key) return Delete_BST(&(*T)->rchild, key);
    p = *T;
    if (!p->lchild) { *T = p->rchild; free(p); }
    else if (!p->rchild) { *T = p->lchild; free(p); }
    else { // 用直接前驱替代
        q = p; s = p->lchild;
        while (s->rchild) { q = s; s = s->rchild; }
        p->key = s->key;
        if (q != p) q->rchild = s->lchild; else q->lchild = s->lchild;
        free(s);
    }
    return 1;
}

int SearchHash(HashTable H, KeyType K, int *p, int *c) {
    int addr = K % HASHSIZE, count = 0;
    while (H.elem[addr] != NULLKEY && H.elem[addr] != K && count < HASHSIZE) {
        count++; addr = (addr + 1) % HASHSIZE;
    }
    *c = count + 1;
    if (H.elem[addr] == K) { *p = addr; return 1; }
    *p = (count < HASHSIZE) ? addr : -1;
    return 0;
}

// ========== 排序 ==========

void PrintList(SqList L, const char *title) {
    printf("%s: ", title);
    for (int i = 1; i <= L.length; i++) printf("%d ", L.data[i].key);
    printf("\n");
}

void BinInsSort(SqList *L) {
    int i, j, low, high, mid;
    for (i = 2; i <= L->length; i++) {
        L->data[0] = L->data[i]; low = 1; high = i - 1;
        while (low <= high) { // 二分定位
            mid = (low + high) / 2;
            if (L->data[0].key < L->data[mid].key) high = mid - 1;
            else low = mid + 1;
        }
        for (j = i - 1; j >= low; j--) L->data[j + 1] = L->data[j];
        L->data[low] = L->data[0];
    }
}

void Shell_sort(SqList *L, int dk[], int t) {
    int i, j, k, d;
    for (k = 0; k < t; k++) {
        d = dk[k];
        for (i = d + 1; i <= L->length; i++)
            if (L->data[i].key < L->data[i - d].key) {
                L->data[0] = L->data[i];
                for (j = i - d; j > 0 && L->data[0].key < L->data[j].key; j -= d)
                    L->data[j + d] = L->data[j];
                L->data[j + d] = L->data[0];
            }
    }
}

void Bubble_Sort(SqList *L) {
    int i, j, swapped; RecType tmp;
    for (i = 1; i < L->length; i++) {
        swapped = 0;
        for (j = 1; j <= L->length - i; j++)
            if (L->data[j].key > L->data[j + 1].key) {
                tmp = L->data[j]; L->data[j] = L->data[j+1]; L->data[j+1] = tmp;
                swapped = 1;
            }
        if (!swapped) break; // 已有序
    }
}

static int Partition(SqList *L, int low, int high) {
    L->data[0] = L->data[low];
    KeyType pivot = L->data[low].key;
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
        int p = Partition(L, low, high);
        QuickSort(L, low, p - 1); QuickSort(L, p + 1, high);
    }
}

void Simple_selection_sort(SqList *L) {
    int i, j, min; RecType tmp;
    for (i = 1; i < L->length; i++) {
        min = i;
        for (j = i + 1; j <= L->length; j++)
            if (L->data[j].key < L->data[min].key) min = j;
        if (min != i) { tmp = L->data[i]; L->data[i] = L->data[min]; L->data[min] = tmp; }
    }
}

static void HeapAdjust(SqList *L, int s, int m) {
    int j; RecType rc = L->data[s];
    for (j = 2 * s; j <= m; j *= 2) {
        if (j < m && L->data[j].key < L->data[j + 1].key) j++;
        if (rc.key >= L->data[j].key) break;
        L->data[s] = L->data[j]; s = j;
    }
    L->data[s] = rc;
}

void Heap_Sort(SqList *L) {
    int i; RecType tmp;
    for (i = L->length / 2; i >= 1; i--) HeapAdjust(L, i, L->length); // 建大顶堆
    for (i = L->length; i > 1; i--) {
        tmp = L->data[1]; L->data[1] = L->data[i]; L->data[i] = tmp;
        HeapAdjust(L, 1, i - 1);
    }
}

static void Merge(RecType SR[], RecType TR[], int i, int m, int n) {
    int j = m + 1, k = i;
    for (; i <= m && j <= n; k++)
        TR[k] = (SR[i].key <= SR[j].key) ? SR[i++] : SR[j++];
    while (i <= m) TR[k++] = SR[i++];
    while (j <= n) TR[k++] = SR[j++];
}

static void MSort(RecType SR[], RecType TR1[], int s, int t) {
    RecType TR2[MAX_SIZE + 1];
    if (s == t) { TR1[s] = SR[s]; return; }
    int m = (s + t) / 2;
    MSort(SR, TR2, s, m); MSort(SR, TR2, m + 1, t);
    Merge(TR2, TR1, s, m, t);
}

void Merge_sort(SqList *L, RecType DR[]) { MSort(L->data, DR, 1, L->length); }

// 释放 BST
void FreeBST(BSTNode **T) {
    if (!*T) return;
    FreeBST(&(*T)->lchild); FreeBST(&(*T)->rchild); free(*T); *T = NULL;
}