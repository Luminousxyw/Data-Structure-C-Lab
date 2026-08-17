#include <stdio.h>
#include <string.h>
#include "linklist.h"

static void show_list(LNode* list) {
    printf("链表: [");
    LNode* p = list;
    while (p != NULL) {
        printf("%s", p->data);
        p = p->next;
        if (p != NULL) printf(" ");
    }
    printf("]  长度=%d  %s\n",
           ListSize(list),
           ListEmpty(list) ? "空" : "");
}

static int read_str(const char* prompt, char* out) {
    char buf[MAX_LEN + 4];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (buf[0] == 'q' && buf[1] == '\n') return 0;
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (buf[0] != '\0') {
            strcpy(out, buf);
            return 1;
        }
        printf("  输入不能为空, q 退出\n");
    }
}

static int read_int(const char* prompt, int* out) {
    char buf[32];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (buf[0] == 'q' && buf[1] == '\n') return 0;
        if (sscanf(buf, "%d", out) == 1) return 1;
        printf("  输入错误, q 退出\n");
    }
}

int main() {
    LNode* list;
    InitList(&list);
    int choice, pos;
    char val[MAX_LEN];
    ElemType item;

    while (1) {
        printf("\n1.尾插 2.头插 3.按位插入 4.按值删除 5.按位删除 ");
        printf("6.修改 7.查找 8.取值 9.显示 10.正序 11.逆序 ");
        printf("12.长度 13.判空 14.清空 0.退出\n");

        if (read_int("> ", &choice) != 1 || choice == 0)
            break;

        switch (choice) {
        case 1:
            if (read_str("尾插值: ", val) == 1) {
                InsertList(&list, val, -1);
                show_list(list);
            }
            break;
        case 2:
            if (read_str("头插值: ", val) == 1) {
                InsertList(&list, val, 1);
                show_list(list);
            }
            break;
        case 3:
            if (read_int("位置: ", &pos) != 1) break;
            if (pos < 1) { printf("位置需>=1\n"); break; }
            if (read_str("值: ", val) != 1) break;
            InsertList(&list, val, pos == 1 ? 1 : pos);
            show_list(list);
            break;
        case 4:
            if (read_str("删除值: ", val) == 1) {
                strcpy(item, val);
                DeleteList(&list, item, 0)
                    ? printf("删除成功 %s\n", val)
                    : printf("未找到\n");
                show_list(list);
            }
            break;
        case 5:
            if (read_int("位置: ", &pos) == 1) {
                if (DeleteList(&list, item, pos))
                    printf("删除[%d]=%s\n", pos, item);
                else
                    printf("删除失败\n");
                show_list(list);
            }
            break;
        case 6:
            if (read_int("修改位置: ", &pos) != 1) break;
            if (read_str("新值: ", val) != 1) break;
            UpdateList(list, pos, val)
                ? printf("修改成功\n")
                : printf("修改失败\n");
            show_list(list);
            break;
        case 7:
            if (read_str("查找值: ", val) == 1) {
                FindList(list, val)
                    ? printf("找到 %s\n", val)
                    : printf("未找到\n");
                show_list(list);
            }
            break;
        case 8:
            if (read_int("位置: ", &pos) == 1) {
                const char* s = GetElem(list, pos);
                s ? printf("[%d]=%s\n", pos, s)
                  : printf("获取失败\n");
                show_list(list);
            }
            break;
        case 9:  show_list(list); break;
        case 10: printf("正序: ["); OrderOutputList(list, 0); printf("]\n"); break;
        case 11: printf("逆序: ["); OrderOutputList(list, 1); printf("]\n"); break;
        case 12: printf("链表长度=%d\n", ListSize(list)); break;
        case 13: printf("是否为空=%s\n", ListEmpty(list) ? "是" : "否"); break;
        case 14: ClearList(&list); printf("链表已清空\n"); show_list(list); break;
        default: printf("无效选择\n"); break;
        }
    }

    ClearList(&list);
    printf("再见\n");
    return 0;
}