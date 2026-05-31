#include "map.h"

int main(void) {
    int i, j;

    /* ===== DFS & BFS (6 vertices, directed) ===== */
    GraphList g;
    int e1[][3] = {
        {0,1,1}, {0,2,1}, {1,3,1}, {1,4,1}, {2,4,1}, {3,5,1}, {4,5,1}
    };
    g.numVertex = 6; g.numEdge = 7;
    for (i = 0; i < g.numVertex; i++) {
        g.adjList[i].data = 'A' + i;
        g.adjList[i].firstEdge = NULL;
        g.adjList[i].inDegree = 0;
    }
    for (i = 0; i < 7; i++) {
        EdgeNode *e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = e1[i][1];
        e->weight = e1[i][2];
        e->next = g.adjList[e1[i][0]].firstEdge;
        g.adjList[e1[i][0]].firstEdge = e;
        g.adjList[e1[i][1]].inDegree++;
    }
    DFS(g);
    BFS(g);

    /* ===== TopologicalSort (6 vertices, DAG) ===== */
    AdjGraph t;
    int e2[][2] = {
        {0,1}, {0,2}, {1,3}, {2,3}, {2,4}, {3,5}, {4,5}
    };
    t.numVertex = 6; t.numEdge = 7;
    for (i = 0; i < t.numVertex; i++) {
        t.adjList[i].data = '0' + i;
        t.adjList[i].firstEdge = NULL;
        t.adjList[i].inDegree = 0;
    }
    for (i = 0; i < 7; i++) {
        EdgeNode *e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = e2[i][1]; e->weight = 1;
        e->next = t.adjList[e2[i][0]].firstEdge;
        t.adjList[e2[i][0]].firstEdge = e;
        t.adjList[e2[i][1]].inDegree++;
    }
    TopologicalSort(t);

    /* ===== CriticalPath (9 vertices, AOE) ===== */
    int e3[][3] = {
        {0,1,6}, {0,2,4}, {0,3,5}, {1,4,1}, {2,4,1},
        {3,5,2}, {4,6,9}, {4,7,7}, {5,7,4}, {6,8,2}, {7,8,4}
    };
    G_Critical.numVertex = 9; G_Critical.numEdge = 11;
    for (i = 0; i < G_Critical.numVertex; i++) {
        G_Critical.adjList[i].data = '0' + i;
        G_Critical.adjList[i].firstEdge = NULL;
        G_Critical.adjList[i].inDegree = 0;
    }
    for (i = 0; i < 11; i++) {
        EdgeNode *e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = e3[i][1]; e->weight = e3[i][2];
        e->next = G_Critical.adjList[e3[i][0]].firstEdge;
        G_Critical.adjList[e3[i][0]].firstEdge = e;
        G_Critical.adjList[e3[i][1]].inDegree++;
    }
    CriticalPath();

    /* ===== ShortestPath (5 vertices, undirected) ===== */
    MTGraph s;
    int mat[5][5] = {
        {     0,      3, INFINITY,      7, INFINITY},
        {     3,      0,       4,      2, INFINITY},
        {INFINITY,     4,       0,      5,       6},
        {     7,      2,       5,      0,       4},
        {INFINITY, INFINITY,    6,      4,       0}
    };
    s.numVertex = 5; s.numEdge = 7;
    for (i = 0; i < 5; i++) {
        s.vexs[i] = 'A' + i;
        for (j = 0; j < 5; j++) s.edges[i][j] = mat[i][j];
    }
    ShortestPath(s);
    
    getchar();
    getchar();
    return 0;
}