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
        printf("  输入不能为空, q 取消\n");
    }
}

static int read_int(const char* prompt, int* out) {
    char buf[32];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (buf[0] == 'q' && buf[1] == '\n') return 0;
        if (sscanf(buf, "%d", out) == 1) return 1;
        printf("  无效, q 取消\n");
    }
}

int main() {
    LNode* list;
    InitList(&list);

    int choice, pos;
    char val[MAX_LEN];
    ElemType item;

    while (1) {
        printf("\n1.尾 2.头 3.插 4.值删 5.位删 ");
        printf("6.改 7.查 8.取 9.览 10.升 11.降 ");
        printf("12.长 13.空 14.清 0.退\n");

        if (read_int("> ", &choice) != 1 || choice == 0)
            break;

        switch (choice) {
        case 1:
            if (read_str("尾插串: ", val) == 1) {
                InsertList(&list, val, -1);
                show_list(list);
            }
            break;
        case 2:
            if (read_str("头插串: ", val) == 1) {
                InsertList(&list, val, 1);
                show_list(list);
            }
            break;
        case 3:
            if (read_int("位置: ", &pos) != 1) break;
            if (pos < 1) { printf("位置>=1\n"); break; }
            if (read_str("串: ", val) != 1) break;
            InsertList(&list, val, pos == 1 ? 1 : pos);
            show_list(list);
            break;
        case 4:
            if (read_str("删串: ", val) == 1) {
                strcpy(item, val);
                DeleteList(&list, item, 0)
                    ? printf("已删除 %s\n", val)
                    : printf("未找到\n");
                show_list(list);
            }
            break;
        case 5:
            if (read_int("删位: ", &pos) == 1) {
                if (DeleteList(&list, item, pos))
                    printf("已删[%d]=%s\n", pos, item);
                else
                    printf("越界\n");
                show_list(list);
            }
            break;
        case 6:
            if (read_int("改位: ", &pos) != 1) break;
            if (read_str("新串: ", val) != 1) break;
            UpdateList(list, pos, val)
                ? printf("已更新\n")
                : printf("越界\n");
            show_list(list);
            break;
        case 7:
            if (read_str("查串: ", val) == 1) {
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
                  : printf("越界\n");
                show_list(list);
            }
            break;
        case 9:  show_list(list); break;
        case 10: printf("升: ["); OrderOutputList(list, 0); printf("]\n"); break;
        case 11: printf("降: ["); OrderOutputList(list, 1); printf("]\n"); break;
        case 12: printf("长度=%d\n", ListSize(list)); break;
        case 13: printf("空否=%s\n", ListEmpty(list) ? "是" : "否"); break;
        case 14: ClearList(&list); printf("已清空\n"); show_list(list); break;
        default: printf("?\n"); break;
        }
    }

    ClearList(&list);
    printf("再见\n");
    return 0;
}