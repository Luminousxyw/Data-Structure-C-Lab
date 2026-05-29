#include <stdio.h>
#include "linklist.h"

int main() {
    LNode* list;
    InitList(&list);

    printf("========== 单链表操作测试 ==========\n\n");

    // ---------- 1. 空链表测试 ----------
    printf("【1】空链表测试\n");
    printf("    是否为空: %s\n", ListEmpty(list) ? "是" : "否");
    printf("    长度: %d\n", ListSize(list));

    // ---------- 2. 尾插法 ----------
    printf("\n【2】尾插法插入 10, 20, 30\n");
    InsertList(&list, 10, 1);
    InsertList(&list, 20, 1);
    InsertList(&list, 30, 1);
    printf("    遍历: "); TraverseList(list);
    printf("    长度: %d\n", ListSize(list));

    // ---------- 3. 头插法 ----------
    printf("\n【3】头插法插入 5\n");
    InsertList(&list, 5, 0);
    printf("    遍历: "); TraverseList(list);

    // ---------- 4. 指定位置插入 ----------
    printf("\n【4】在第3个位置插入 15\n");
    InsertList(&list, 15, 3);
    printf("    遍历: "); TraverseList(list);

    // ---------- 5. GetElem ----------
    printf("\n【5】GetElem 取值\n");
    printf("    第1个: %d\n", GetElem(list, 1));
    printf("    第3个: %d\n", GetElem(list, 3));
    printf("    第5个: %d\n", GetElem(list, 5));

    // ---------- 6. FindList ----------
    printf("\n【6】FindList 查找\n");
    int target = 20;
    if (FindList(list, &target)) {
        printf("    值 %d → 找到\n", target);
    } else {
        printf("    值 %d → 未找到\n", target);
    }
    target = 99;
    if (FindList(list, &target)) {
        printf("    值 %d → 找到\n", target);
    } else {
        printf("    值 %d → 未找到\n", target);
    }

    // ---------- 7. UpdateList ----------
    printf("\n【7】UpdateList 更新第一个结点为 88\n");
    printf("    更新前: "); TraverseList(list);
    if (UpdateList(list, 88)) {
        printf("    更新成功!\n");
    } else {
        printf("    更新失败（链表为空）\n");
    }
    printf("    更新后: "); TraverseList(list);

    // ---------- 8. 按位置删除 ----------
    printf("\n【8】按位置删除 第3个元素\n");
    int deletedVal;
    if (DeleteList(&list, &deletedVal, 3)) {
        printf("    删除成功, 值 = %d\n", deletedVal);
    }
    printf("    遍历: "); TraverseList(list);

    // ---------- 9. 按值删除 ----------
    printf("\n【9】按值删除 20\n");
    deletedVal = 20;
    if (DeleteList(&list, &deletedVal, 0)) {
        printf("    删除成功, 值 = %d\n", deletedVal);
    } else {
        printf("    未找到该值\n");
    }
    printf("    遍历: "); TraverseList(list);

    // ---------- 10. 有序输出 ----------
    printf("\n【10】OrderOutputList 有序输出\n");
    printf("     当前链表: "); TraverseList(list);
    printf("     升序输出: "); OrderOutputList(list, 0);
    printf("     降序输出: "); OrderOutputList(list, 1);
    printf("     （验证原链表未变）: "); TraverseList(list);

    // ---------- 11. 清空 ----------
    printf("\n【11】ClearList 清空\n");
    ClearList(&list);
    printf("     是否为空: %s\n", ListEmpty(list) ? "是" : "否");
    printf("     长度: %d\n", ListSize(list));

    // ---------- 12. 空链表更新 ----------
    printf("\n【12】空链表上测试 UpdateList\n");
    if (UpdateList(list, 100)) {
        printf("     更新成功\n");
    } else {
        printf("     更新失败（链表为空，符合预期）\n");
    }

    printf("\n========== 测试完毕 ==========\n");
    getchar();
    return 0;
}