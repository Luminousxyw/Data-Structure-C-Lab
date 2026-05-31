#include "map.h"

static int visited[MAX_VERTEX_NUM];

static void DFS_Recursive(GraphList G, int v) {
    EdgeNode *p;
    visited[v] = 1;
    printf("%c ", G.adjList[v].data);
    for (p = G.adjList[v].firstEdge; p; p = p->next)
        if (!visited[p->adjvex])
            DFS_Recursive(G, p->adjvex);
}

/* ---------- CreateGraph ---------- */
void CreateGraph(GraphList *g) {
    int i, j, k, weight;
    EdgeNode *e;

    printf("Enter vertex count and edge count: ");
    scanf("%d %d", &g->numVertex, &g->numEdge);

    for (i = 0; i < g->numVertex; i++) {
        printf("Vertex %d name (char): ", i + 1);
        scanf(" %c", &g->adjList[i].data);
        g->adjList[i].firstEdge = NULL;
        g->adjList[i].inDegree  = 0;
    }

    for (k = 0; k < g->numEdge; k++) {
        printf("Edge %d (src dst weight): ", k + 1);
        scanf("%d %d %d", &i, &j, &weight);

        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = j;
        e->weight = weight;
        e->next = g->adjList[i].firstEdge;
        g->adjList[i].firstEdge = e;

        g->adjList[j].inDegree++;
    }
}

/* ---------- DFS ---------- */
void DFS(GraphList G) {
    int i;
    for (i = 0; i < G.numVertex; i++) visited[i] = 0;
    printf("DFS: ");
    for (i = 0; i < G.numVertex; i++)
        if (!visited[i]) DFS_Recursive(G, i);
    printf("\n");
}

/* ---------- BFS ---------- */
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
    printf("\n");
}

/* ---------- TopologicalSort (Kahn) ---------- */
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
        printf("Cycle detected.\n");
        return 0;
    }
    return 1;
}

/* ---------- CriticalPath ---------- */
GraphList G_Critical;
static int ve[MAX_VERTEX_NUM];
static int vl[MAX_VERTEX_NUM];

void CriticalPath(void) {
    int i, k, v;
    EdgeNode *p;
    int topo[MAX_VERTEX_NUM], topoCount = 0;
    int indegree[MAX_VERTEX_NUM], stack[MAX_VERTEX_NUM], top = -1;
    int ee, el;

    /* step 1: topological sort, compute ve[] */
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
        printf("Cycle detected.\n");
        return;
    }

    /* step 2: reverse order, compute vl[] */
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

    /* step 3: output critical activities (ee == el) */
    printf("Critical path:\n");
    for (i = 0; i < G_Critical.numVertex; i++)
        for (p = G_Critical.adjList[i].firstEdge; p; p = p->next) {
            k = p->adjvex;
            ee = ve[i];
            el = vl[k] - p->weight;
            if (ee == el)
                printf("  %c -> %c (w:%d)\n",
                    G_Critical.adjList[i].data,
                    G_Critical.adjList[k].data,
                    p->weight);
        }
}

/* ---------- ShortestPath (Dijkstra) ---------- */
void ShortestPath(MTGraph G) {
    int i, j, k, start, min;
    int dist[MAX_VERTEX_NUM], path[MAX_VERTEX_NUM], final[MAX_VERTEX_NUM];

    printf("Start vertex index (0~%d): ", G.numVertex - 1);
    scanf("%d", &start);

    for (i = 0; i < G.numVertex; i++) {
        dist[i]  = G.edges[start][i];
        final[i] = 0;
        path[i]  = (dist[i] < INFINITY) ? start : -1;
    }
    dist[start] = 0;
    final[start] = 1;
    path[start]  = -1;

    for (i = 0; i < G.numVertex - 1; i++) {
        min = INFINITY; k = -1;
        for (j = 0; j < G.numVertex; j++)
            if (!final[j] && dist[j] < min) {
                min = dist[j]; k = j;
            }
        if (k == -1) break;

        final[k] = 1;
        for (j = 0; j < G.numVertex; j++)
            if (!final[j] && G.edges[k][j] < INFINITY
                && dist[k] + G.edges[k][j] < dist[j]) {
                dist[j] = dist[k] + G.edges[k][j];
                path[j] = k;
            }
    }

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
}