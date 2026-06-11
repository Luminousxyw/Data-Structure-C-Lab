#include "search_sort.h"

#define REPEAT_MAX 1000000

// 数组 → 顺序表
static void LoadList(SqList *L, int arr[], int n) {
    L->length = n;
    for (int i = 1; i <= n; i++) { L->data[i].key = arr[i-1]; L->data[i].other = 0; }
}

// 中序遍历 BST
static void InOrder_BST(BSTNode *T) {
    if (!T) return;
    InOrder_BST(T->lchild); printf("%d ", T->key); InOrder_BST(T->rchild);
}

// ========== 自动校准高精度计时（通用） ==========
typedef void (*VoidFn)(void*);

static double auto_time_us(VoidFn run, void *ctx, VoidFn setup, void *src) {
    double t0, t1, single;
    int repeat = 1;
    setup(ctx, src);
    t0 = now_us(); run(ctx); t1 = now_us();
    single = t1 - t0;
    while (single == 0.0 && repeat < REPEAT_MAX / 10) {
        repeat *= 10;
        setup(ctx, src);
        t0 = now_us();
        for (int i = 0; i < repeat; i++) { setup(ctx, src); run(ctx); }
        t1 = now_us();
        single = (t1 - t0) / repeat;
    }
    if (single == 0.0) return 0.0;
    if (single < 300000.0) {
        int need = (int)(300000.0 / single) + 1;
        if (need > REPEAT_MAX) need = REPEAT_MAX;
        if (need > repeat) {
            t0 = now_us();
            for (int i = 0; i < need; i++) { setup(ctx, src); run(ctx); }
            t1 = now_us();
            return (t1 - t0) / need;
        }
    }
    return single;
}

// ---- 查找上下文 ----
typedef struct { SSTable *ST; KeyType key; int *pos; } Sctx;
static void s_setup(void *c, void *s) { *(Sctx*)c = *(Sctx*)s; }
static void seq_run(void *c){ Sctx *x = (Sctx*)c; *x->pos = Search_Seq(*x->ST, x->key); }
static void bin_run(void *c){ Sctx *x = (Sctx*)c; *x->pos = Search_Bin(*x->ST, x->key); }

typedef struct { RecType *ST; Index *ind; KeyType key; int n,b,*pos; } Bctx;
static void b_setup(void *c, void *s){ *(Bctx*)c = *(Bctx*)s; }
static void b_run  (void *c){ Bctx *x = (Bctx*)c; *x->pos = Search_Block(x->ST,x->ind,x->key,x->n,x->b); }

typedef struct { HashTable *H; KeyType key; int *p,*c,*found; } Hctx;
static void h_setup(void *c, void *s){ *(Hctx*)c = *(Hctx*)s; }
static void h_run  (void *c){ Hctx *x = (Hctx*)c; *x->found = SearchHash(*x->H, x->key, x->p, x->c); }

typedef struct { BSTNode **T; KeyType key; BSTNode **res; } Tctx;
static void t_setup(void *c, void *s) { Tctx *a=(Tctx*)c, *b=(Tctx*)s; *a = *b; }
static void t_run(void *c) { Tctx *x=(Tctx*)c; *x->res = Search_BST(*x->T, x->key); }

// ---- 排序计时（统一） ----
static double sort_time_us(SqList *orig, int choice) {
    double t0, t1, single;
    int repeat = 1, dk[]={5,3,1};
    SqList tmp; RecType DR[MAX_SIZE+1];

    // 单次试跑
    tmp = *orig;
    switch (choice) {
        case 1: t0=now_us(); BinInsSort(&tmp); t1=now_us(); break;
        case 2: t0=now_us(); Shell_sort(&tmp,dk,3); t1=now_us(); break;
        case 3: t0=now_us(); Bubble_Sort(&tmp); t1=now_us(); break;
        case 4: t0=now_us(); QuickSort(&tmp,1,tmp.length); t1=now_us(); break;
        case 5: t0=now_us(); Simple_selection_sort(&tmp); t1=now_us(); break;
        case 6: t0=now_us(); Heap_Sort(&tmp); t1=now_us(); break;
        case 7: t0=now_us(); Merge_sort(&tmp,DR); t1=now_us(); break;
        default: return 0;
    }
    single = t1 - t0;
    while (single == 0.0 && repeat < REPEAT_MAX / 10) {
        repeat *= 10;
        t0 = now_us();
        for (int r = 0; r < repeat; r++) {
            tmp = *orig;
            switch (choice) {
                case 1: BinInsSort(&tmp); break;
                case 2: Shell_sort(&tmp,dk,3); break;
                case 3: Bubble_Sort(&tmp); break;
                case 4: QuickSort(&tmp,1,tmp.length); break;
                case 5: Simple_selection_sort(&tmp); break;
                case 6: Heap_Sort(&tmp); break;
                case 7: Merge_sort(&tmp,DR); break;
            }
        }
        t1 = now_us();
        single = (t1 - t0) / repeat;
    }
    if (single == 0.0) return 0.0;
    if (single < 300000.0) {
        int need = (int)(300000.0 / single) + 1;
        if (need > REPEAT_MAX) need = REPEAT_MAX;
        if (need > repeat) {
            t0 = now_us();
            for (int r = 0; r < need; r++) {
                tmp = *orig;
                switch (choice) {
                    case 1: BinInsSort(&tmp); break;
                    case 2: Shell_sort(&tmp,dk,3); break;
                    case 3: Bubble_Sort(&tmp); break;
                    case 4: QuickSort(&tmp,1,tmp.length); break;
                    case 5: Simple_selection_sort(&tmp); break;
                    case 6: Heap_Sort(&tmp); break;
                    case 7: Merge_sort(&tmp,DR); break;
                }
            }
            t1 = now_us();
            return (t1 - t0) / need;
        }
    }
    return single;
}

// 时间打印（自适应单位）
static void print_time(double us) {
    if (us == 0.0) printf("< 0.001 us");
    else if (us < 1000.0) printf("%.3f us", us);
    else printf("%.3f ms", us / 1000.0);
}

// ========== 查找菜单 ==========
static void menu_search(SqList L, int arr[], int n) {
    int choice, key, pos, p, c, found;
    do {
        printf("\n========== Search Algorithms ==========\n");
        printf("  1. Sequential Search\n");
        printf("  2. Binary Search\n");
        printf("  3. Block Search\n");
        printf("  4. BST (build / search / delete)\n");
        printf("  5. Hash Search (linear probing)\n");
        printf("  0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); continue; }

        switch (choice) {
        case 1:
            printf("Enter key: "); scanf("%d", &key);
            { Sctx c={&L,key,&pos}; double us=auto_time_us(seq_run,&c,s_setup,&c);
              if(pos>0) printf("Found %d at pos=%d\n",key,pos);
              else printf("%d not found\n",key);
              printf("Time: "); print_time(us); printf("\n"); }
            break;
        case 2:
            printf("Enter key: "); scanf("%d", &key);
            { SqList sorted; LoadList(&sorted,arr,n); QuickSort(&sorted,1,sorted.length);
              printf("(Auto-sorted) "); PrintList(sorted,"");
              Sctx c={&sorted,key,&pos}; double us=auto_time_us(bin_run,&c,s_setup,&c);
              if(pos>0) printf("Found %d at pos=%d\n",key,pos);
              else printf("%d not found\n",key);
              printf("Time: "); print_time(us); printf("\n"); }
            break;
        case 3:
            printf("Enter key: "); scanf("%d", &key);
            { SqList sorted; LoadList(&sorted,arr,n); QuickSort(&sorted,1,sorted.length);
              printf("(Auto-sorted) "); PrintList(sorted,"");
              RecType ST[MAX_SIZE]; for(int i=0;i<n;i++){ST[i].key=sorted.data[i+1].key; ST[i].other=0;}
              int bs=(n+2)/3; if(bs<2)bs=2; int nb=(n+bs-1)/bs; if(nb>MAX_INDEX)nb=MAX_INDEX;
              Index ind[MAX_INDEX];
              for(int i=0;i<nb;i++){
                  int end=(i+1)*bs-1; if(end>=n)end=n-1;
                  ind[i].maxKey=sorted.data[end+1].key; ind[i].start=i*bs;
              }
              printf("Index: "); for(int i=0;i<nb;i++) printf("[%d,%d] ",ind[i].maxKey,ind[i].start);
              printf("\n");
              Bctx bc={ST,ind,key,n,nb,&pos}, src=bc;
              double us=auto_time_us(b_run,&bc,b_setup,&src);
              if(pos>=0) printf("Found %d at index=%d\n",key,pos);
              else printf("%d not found\n",key);
              printf("Time: "); print_time(us); printf("\n"); }
            break;
        case 4: {
            BSTNode *bst=NULL; double t0,t1;
            t0=now_us(); for(int i=0;i<n;i++) Insert_BST(&bst,arr[i]); t1=now_us();
            printf("BST built. Inorder: "); InOrder_BST(bst);
            printf("\nBuild time: %.3f ms\n", (t1-t0)/1000.0);

            printf("Search key: "); scanf("%d",&key);
            { BSTNode *r=NULL; Tctx tc={&bst,key,&r}; double us=auto_time_us(t_run,&tc,t_setup,&tc);
              printf("%s\n",r?"Found":"Not found");
              printf("Search time: "); print_time(us); printf("\n"); }

            printf("Delete key: "); scanf("%d",&key);
            t0=now_us(); int del=Delete_BST(&bst,key); t1=now_us();
            if(del){ printf("After delete: "); InOrder_BST(bst); printf("\n"); }
            else printf("Key %d not exist\n", key);
            printf("Delete time: %.3f ms\n", (t1-t0)/1000.0);
            FreeBST(&bst); }
            break;
        case 5:
            printf("Search key: "); scanf("%d",&key);
            { HashTable H; for(int i=0;i<HASHSIZE;i++) H.elem[i]=NULLKEY; H.count=0;
              for(int i=0;i<n;i++){ int pp,cc,f=SearchHash(H,arr[i],&pp,&cc);
                  if(!f&&pp!=-1){ H.elem[pp]=arr[i]; H.count++; } }
              printf("Hash table: ");
              for(int i=0;i<HASHSIZE;i++) printf(H.elem[i]==NULLKEY?"[ ] ":"[%d] ",H.elem[i]);
              printf("\n(%d/%d slots used)\n",H.count,HASHSIZE);
              Hctx hc={&H,key,&p,&c,&found}, src=hc;
              double us=auto_time_us(h_run,&hc,h_setup,&src);
              if(found) printf("Found %d addr=%d cmp=%d\n",key,p,c);
              else printf("%d not found\n",key);
              printf("Time: "); print_time(us); printf("\n"); }
            break;
        case 0: break;
        default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

// ========== 排序菜单 ==========
static void menu_sort(SqList orig, int n) {
    int choice; SqList L; RecType DR[MAX_SIZE+1];
    int dk[]={5,3,1}; double us;
    const char *names[]={"","Binary Insertion","Shell","Bubble","Quick","Selection","Heap","Merge"};

    do {
        printf("\n========== Sorting Algorithms ==========\n");
        for(int i=1;i<=7;i++) printf("  %d. %s Sort\n",i,names[i]);
        printf("  8. Compare All (auto-calibrated)\n");
        printf("  0. Back\n");
        printf("Data: "); PrintList(orig,"");
        printf("Choice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }

        if(choice>=1 && choice<=7){
            L = orig;
            printf("Before: "); PrintList(L,"");
            if(choice==1) BinInsSort(&L);
            else if(choice==2) Shell_sort(&L,dk,3);
            else if(choice==3) Bubble_Sort(&L);
            else if(choice==4) QuickSort(&L,1,L.length);
            else if(choice==5) Simple_selection_sort(&L);
            else if(choice==6) Heap_Sort(&L);
            else if(choice==7) Merge_sort(&L,DR);
            printf("After:  ");
            if(choice==7){ printf("Merge Sort: "); for(int i=1;i<=L.length;i++) printf("%d ",DR[i].key); printf("\n"); }
            else PrintList(L, names[choice]);
            us = sort_time_us(&orig, choice);
            printf("Time: "); print_time(us); printf("\n");
        } else if(choice==8){
            printf("\n========== Compare All (n=%d) ==========\n",n);
            printf("%-25s %s\n","Algorithm","Avg Time");
            printf("--------------------------------------\n");
            for(int i=1;i<=7;i++){
                us = sort_time_us(&orig, i);
                printf("%-25s ",names[i]); print_time(us); printf("\n");
            }
            printf("Done. Original data unchanged.\n");
        } else if(choice!=0) printf("Invalid choice.\n");
    } while(choice!=0);
}

// ========== 主函数 ==========
int main(void){
    int arr[MAX_SIZE], n, mc; SqList L;
    printf("========================================\n");
    printf("  Search & Sort Algorithm Demo\n");
    printf("========================================\n\n");
    printf("How many numbers? ");
    while(scanf("%d",&n)!=1||n<=0||n>MAX_SIZE){ while(getchar()!='\n'); printf("1~%d: ",MAX_SIZE); }
    printf("Enter %d integers: ",n);
    for(int i=0;i<n;i++) while(scanf("%d",&arr[i])!=1){ while(getchar()!='\n'); printf("Re-enter #%d: ",i+1); }
    LoadList(&L,arr,n);
    printf("Loaded: "); PrintList(L,"");
    do {
        printf("\n========== Main Menu ==========\n");
        printf("  1. Search\n  2. Sort\n  3. Re-enter data\n  0. Exit\nChoice: ");
        if(scanf("%d",&mc)!=1){ while(getchar()!='\n'); continue; }
        switch(mc){
            case 1: menu_search(L,arr,n); break;
            case 2: menu_sort(L,n); break;
            case 3:
                printf("How many? ");
                while(scanf("%d",&n)!=1||n<=0||n>MAX_SIZE){ while(getchar()!='\n'); printf("1~%d: ",MAX_SIZE); }
                printf("Enter %d integers: ",n);
                for(int i=0;i<n;i++) while(scanf("%d",&arr[i])!=1){ while(getchar()!='\n'); printf("Re-enter #%d: ",i+1); }
                LoadList(&L,arr,n); printf("Updated.\n"); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(mc!=0);
    return 0;
}