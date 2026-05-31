#ifndef __MAP_H__
#define __MAP_H__

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 100
#define INFINITY 65535

typedef char VertexType;
typedef int  EdgeType;

/* edge node (adjacency list) */
typedef struct EdgeNode {
    int adjvex;
    EdgeType weight;
    struct EdgeNode *next;
} EdgeNode;

/* vertex node */
typedef struct VertexNode {
    VertexType data;
    int inDegree;             /* for topological sort */
    EdgeNode *firstEdge;
} VertexNode;

/* adjacency list graph */
typedef struct {
    VertexNode adjList[MAX_VERTEX_NUM];
    int numVertex;
    int numEdge;
} GraphList;

typedef GraphList AdjGraph;

/* adjacency matrix graph */
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];
    EdgeType edges[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int numVertex;
    int numEdge;
} MTGraph;

/* global graph for CriticalPath() */
extern GraphList G_Critical;

void CreateGraph(GraphList *g);
void DFS(GraphList G);
void BFS(GraphList G);
int  TopologicalSort(AdjGraph G);
void CriticalPath(void);
void ShortestPath(MTGraph G);

#endif