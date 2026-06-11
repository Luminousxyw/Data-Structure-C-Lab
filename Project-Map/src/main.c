#include "map.h"

// 显示主菜单
static void showMenu(void) {
    printf("  Graph Algorithm Toolkit:\n");
    printf("  1. DFS & BFS\n");
    printf("  2. Topological Sort\n");
    printf("  3. Critical Path\n");
    printf("  4. Dijkstra Shortest Path\n");
    printf("  5. Quit\n");
    printf("  Select: ");
}

int main(void) {
    int choice;
    do {
        showMenu();
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1: {   // 遍历演示
            int directed;
            printf("Directed graph? (1=yes 0=no): ");
            scanf("%d", &directed);

            GraphList g;
            CreateGraph(&g, directed);
            PrintAdjList(g);

            DFS(g);
            BFS(g);

            FreeGraph(&g);
            break;
        }
        case 2: {   // 拓扑排序
            GraphList g;
            CreateGraph(&g, 1);
            PrintAdjList(g);
            TopologicalSort(g);
            FreeGraph(&g);
            break;
        }
        case 3: {   // 关键路径
            CreateGraph(&G_Critical, 1);
            PrintAdjList(G_Critical);
            CriticalPath();
            FreeGraph(&G_Critical);
            break;
        }
        case 4: {   // 最短路径
            int directed, start;
            printf("Directed graph? (1=yes 0=no): ");
            scanf("%d", &directed);

            MTGraph s;
            CreateAdjMatrix(&s, directed);
            PrintAdjMatrix(s);

            printf("Start vertex index (0~%d): ", s.numVertex - 1);
            scanf("%d", &start);
            if (start >= 0 && start < s.numVertex)
                ShortestPath(s, start);
            else
                printf("Invalid start vertex.\n\n");
            break;
        }
        case 5:
            printf("Goodbye.\n");
            break;
        default:
            printf("Invalid choice.\n\n");
        }
    } while (choice != 5);

    getchar();
    return 0;
}