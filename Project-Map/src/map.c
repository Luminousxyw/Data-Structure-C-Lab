#include "map.h"
#include <string.h>

static int visited[MAX_VERTEX_NUM];   // DFS 访问标记数组

// 深度优先递归遍历
static void DFS_Recursive(GraphList G, int v) {
    EdgeNode *p;
    visited[v] = 1;
    printf("%c ", G.adjList[v].data);
    for (p = G.adjList[v].firstEdge; p; p = p->next)
        if (!visited[p->adjvex])
            DFS_Recursive(G, p->adjvex);
}

// 交互式创建邻接表图（directed=1 有向，0 无向）
void CreateGraph(GraphList *g, int directed) {
    int i, j, k, weight;
    EdgeNode *e;

    printf("Enter vertex count and edge count: ");
    scanf("%d %d", &g->numVertex, &g->numEdge);

    for (i = 0; i < g->numVertex; i++) {
        printf("  Vertex %d name (single char): ", i);
        scanf(" %c", &g->adjList[i].data);
        g->adjList[i].firstEdge = NULL;
        g->adjList[i].inDegree  = 0;
    }

    for (k = 0; k < g->numEdge; k++) {
        printf("  Edge %d (src_index dst_index weight): ", k);
        scanf("%d %d %d", &i, &j, &weight);

        // 插入边 i -> j
        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = j;
        e->weight = weight;
        e->next = g->adjList[i].firstEdge;
        g->adjList[i].firstEdge = e;
        g->adjList[j].inDegree++;

        // 无向图还需插入反向边 j -> i
        if (!directed) {
            e = (EdgeNode *)malloc(sizeof(EdgeNode));
            e->adjvex = i;
            e->weight = weight;
            e->next = g->adjList[j].firstEdge;
            g->adjList[j].firstEdge = e;
            g->adjList[i].inDegree++;
        }
    }
    printf("Graph created.\n\n");
}

// 交互式创建邻接矩阵图
void CreateAdjMatrix(MTGraph *g, int directed) {
    int i, j, k, w;

    printf("Enter vertex count and edge count: ");
    scanf("%d %d", &g->numVertex, &g->numEdge);

    for (i = 0; i < g->numVertex; i++) {
        printf("  Vertex %d name (single char): ", i);
        scanf(" %c", &g->vexs[i]);
    }

    // 初始化矩阵：对角元为 0，其余为 INFINITY
    for (i = 0; i < g->numVertex; i++)
        for (j = 0; j < g->numVertex; j++)
            g->edges[i][j] = (i == j) ? 0 : INFINITY;

    for (k = 0; k < g->numEdge; k++) {
        printf("  Edge %d (src_index dst_index weight): ", k);
        scanf("%d %d %d", &i, &j, &w);
        g->edges[i][j] = w;
        if (!directed)
            g->edges[j][i] = w;
    }
    printf("Matrix graph created.\n\n");
}

// 打印邻接表结构
void PrintAdjList(GraphList g) {
    int i;
    EdgeNode *p;
    printf("Adjacency List:\n");
    for (i = 0; i < g.numVertex; i++) {
        printf("  [%d] %c  (in:%d) -> ", i, g.adjList[i].data, g.adjList[i].inDegree);
        for (p = g.adjList[i].firstEdge; p; p = p->next)
            printf("%c(w:%d) ", g.adjList[p->adjvex].data, p->weight);
        printf("\n");
    }
}

// 打印邻接矩阵
void PrintAdjMatrix(MTGraph g) {
    int i, j;
    printf("Adjacency Matrix:\n");
    printf("    ");
    for (i = 0; i < g.numVertex; i++) printf("%3c ", g.vexs[i]);
    printf("\n");
    for (i = 0; i < g.numVertex; i++) {
        printf("  %c ", g.vexs[i]);
        for (j = 0; j < g.numVertex; j++) {
            if (g.edges[i][j] == INFINITY)
                printf("  ∞ ");
            else
                printf("%3d ", g.edges[i][j]);
        }
        printf("\n");
    }
}

// 释放邻接表动态边节点
void FreeGraph(GraphList *g) {
    int i;
    EdgeNode *p, *tmp;
    for (i = 0; i < g->numVertex; i++) {
        p = g->adjList[i].firstEdge;
        while (p) {
            tmp = p;
            p = p->next;
            free(tmp);
        }
        g->adjList[i].firstEdge = NULL;
        g->adjList[i].inDegree = 0;
    }
    g->numVertex = 0;
    g->numEdge = 0;
}

// 深度优先遍历入口
void DFS(GraphList G) {
    int i;
    for (i = 0; i < G.numVertex; i++) visited[i] = 0;
    printf("DFS: ");
    for (i = 0; i < G.numVertex; i++)
        if (!visited[i]) DFS_Recursive(G, i);
    printf("\n\n");
}

// 广度优先遍历（使用队列）
void BFS(GraphList G) {
    int i, v;
    int queue[MAX_VERTEX_NUM], front = 0, rear = 0;
    int vis[MAX_VERTEX_NUM] = {0};
    EdgeNode *p;

    printf("BFS: ");
    for (i = 0; i < G.numVertex; i++) {
        if (vis[i]) continue;
        vis[i] = 1;
        printf("%c ", G.adjList[i].data);
        queue[rear++] = i;

        while (front < rear) {
            v = queue[front++];
            for (p = G.adjList[v].firstEdge; p; p = p->next)
                if (!vis[p->adjvex]) {
                    vis[p->adjvex] = 1;
                    printf("%c ", G.adjList[p->adjvex].data);
                    queue[rear++] = p->adjvex;
                }
        }
    }
    printf("\n\n");
}

// 拓扑排序（Kahn 算法，用栈暂存入度为 0 的顶点）
int TopologicalSort(AdjGraph G) {
    int i, v;
    int indegree[MAX_VERTEX_NUM];
    int stack[MAX_VERTEX_NUM], top = -1;
    int count = 0;
    EdgeNode *p;

    for (i = 0; i < G.numVertex; i++) {
        indegree[i] = G.adjList[i].inDegree;
        if (indegree[i] == 0) stack[++top] = i;
    }

    printf("Topo: ");
    while (top != -1) {
        v = stack[top--];
        printf("%c ", G.adjList[v].data);
        count++;

        for (p = G.adjList[v].firstEdge; p; p = p->next) {
            indegree[p->adjvex]--;
            if (indegree[p->adjvex] == 0)
                stack[++top] = p->adjvex;
        }
    }
    printf("\n");

    if (count < G.numVertex) {
        printf("Cycle detected.\n\n");
        return 0;
    }
    printf("Topological sort succeeded.\n\n");
    return 1;
}

// 关键路径全局图及辅助数组
GraphList G_Critical;
GraphList G_Topo;
static int ve[MAX_VERTEX_NUM];   // 事件最早发生时间
static int vl[MAX_VERTEX_NUM];   // 事件最晚发生时间

// 关键路径算法（AOE 网）
void CriticalPath(void) {
    int i, k, v;
    EdgeNode *p;
    int topo[MAX_VERTEX_NUM], topoCount = 0;
    int indegree[MAX_VERTEX_NUM], stack[MAX_VERTEX_NUM], top = -1;
    int ee, el;

    // 正向拓扑排序，求 ve
    for (i = 0; i < G_Critical.numVertex; i++) {
        indegree[i] = G_Critical.adjList[i].inDegree;
        ve[i] = 0;
        if (indegree[i] == 0) stack[++top] = i;
    }

    while (top != -1) {
        v = stack[top--];
        topo[topoCount++] = v;
        for (p = G_Critical.adjList[v].firstEdge; p; p = p->next) {
            k = p->adjvex;
            indegree[k]--;
            if (indegree[k] == 0) stack[++top] = k;
            if (ve[v] + p->weight > ve[k])
                ve[k] = ve[v] + p->weight;
        }
    }

    if (topoCount < G_Critical.numVertex) {
        printf("Cycle detected.\n\n");
        return;
    }

    // 逆向逆拓扑序，求 vl
    for (i = 0; i < G_Critical.numVertex; i++)
        vl[i] = ve[G_Critical.numVertex - 1];

    while (topoCount > 0) {
        v = topo[--topoCount];
        for (p = G_Critical.adjList[v].firstEdge; p; p = p->next) {
            k = p->adjvex;
            if (vl[k] - p->weight < vl[v])
                vl[v] = vl[k] - p->weight;
        }
    }

    // 输出关键活动 (最早开始时间 == 最晚开始时间)
    printf("Critical path:\n");
    for (i = 0; i < G_Critical.numVertex; i++)
        for (p = G_Critical.adjList[i].firstEdge; p; p = p->next) {
            k = p->adjvex;
            ee = ve[i];
            el = vl[k] - p->weight;
            if (ee == el)
                printf("  %c -> %c  (weight: %d)\n",
                    G_Critical.adjList[i].data,
                    G_Critical.adjList[k].data,
                    p->weight);
        }
    printf("Project minimum duration: %d\n\n", ve[G_Critical.numVertex - 1]);
}

// Dijkstra 最短路径算法
void ShortestPath(MTGraph G, int start) {
    int i, j, k, min;
    int dist[MAX_VERTEX_NUM], path[MAX_VERTEX_NUM], final[MAX_VERTEX_NUM];

    // 初始化
    for (i = 0; i < G.numVertex; i++) {
        dist[i]  = G.edges[start][i];
        final[i] = 0;
        path[i]  = (dist[i] < INFINITY) ? start : -1;
    }
    dist[start] = 0;
    final[start] = 1;
    path[start]  = -1;

    // 主循环：每次找到一个最近未确定顶点
    for (i = 0; i < G.numVertex - 1; i++) {
        min = INFINITY; k = -1;
        for (j = 0; j < G.numVertex; j++)
            if (!final[j] && dist[j] < min) {
                min = dist[j]; k = j;
            }
        if (k == -1) break;

        final[k] = 1;

        // 松弛操作
        for (j = 0; j < G.numVertex; j++)
            if (!final[j] && G.edges[k][j] < INFINITY
                && dist[k] + G.edges[k][j] < dist[j]) {
                dist[j] = dist[k] + G.edges[k][j];
                path[j] = k;
            }
    }

    // 输出结果及路径回溯
    printf("Shortest paths from %c:\n", G.vexs[start]);
    for (i = 0; i < G.numVertex; i++) {
        if (i == start) continue;
        if (dist[i] == INFINITY) {
            printf("  %c: unreachable\n", G.vexs[i]);
            continue;
        }

        printf("  %c: dist=%d, path=", G.vexs[i], dist[i]);
        int stk[MAX_VERTEX_NUM], tp = -1, p = i;
        while (p != -1) { stk[++tp] = p; p = path[p]; }
        while (tp >= 0) {
            printf("%c", G.vexs[stk[tp--]]);
            if (tp >= 0) printf(" -> ");
        }
        printf("\n");
    }
    printf("\n");
}