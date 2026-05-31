#include <stdio.h>
#include "linklist.h"

int main() {
    LNode* list;
    InitList(&list);

    // 空链表测试
    printf("Is Empty: %s\n", ListEmpty(list) ? "true":"false");
    printf("The length of List: %d\n\n", ListSize(list));

    // 尾插法
    InsertList(&list, 10, 1);
    InsertList(&list, 20, 1);
    InsertList(&list, 30, 1);
    printf("List: "); TraverseList(list);
    printf("The length of List: %d\n\n", ListSize(list));

    // 头插法
    InsertList(&list, 35, 0);
    printf("List: "); TraverseList(list);
    printf("The length of List: %d\n\n", ListSize(list));

    // 指定位置插入
    InsertList(&list, 15, 3);
    printf("List: "); TraverseList(list);
    printf("The length of List: %d\n\n", ListSize(list));

    // 按位置删除
    int deletedVal,i;
    printf("Enter the mark: ");
    scanf("%d",&i);
    if (DeleteList(&list, &deletedVal, i)) {
        printf("Delete successful the value is %d\n", deletedVal);
    }
    else{
        printf("Delete failed.\n");
    }
    printf("List: "); TraverseList(list);
    printf("\n");

    // 按值删除
    printf("Enter the deletedVal: ");
    scanf("%d",&deletedVal);
    getchar();
    if (DeleteList(&list, &deletedVal, 0)) {
        printf("Delete successful.\n");
    } 
    else 
    {
        printf("Find failed.\n");
    }
    printf("List: "); TraverseList(list);
    printf("\n");

    // 有序输出
    printf("Ascending List: "); OrderOutputList(list, 0);
    printf("Descending List: "); OrderOutputList(list, 1);
    printf("Origin List:"); TraverseList(list);
    printf("\n");

    // 清空
    ClearList(&list);
    printf("Is Empty: %s\n", ListEmpty(list) ? "true":"false");
    printf("The length of List: %d\n", ListSize(list));

    getchar();
    return 0;
}