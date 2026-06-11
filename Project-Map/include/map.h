#ifndef __MAP_H__
#define __MAP_H__

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 100
#define INFINITY 65535        // 无穷大，表示无直接边

typedef char VertexType;
typedef int  EdgeType;

// 边表节点
typedef struct EdgeNode {
    int adjvex;               // 邻接顶点下标
    EdgeType weight;          // 边权重
    struct EdgeNode *next;
} EdgeNode;

// 顶点表节点
typedef struct VertexNode {
    VertexType data;          // 顶点名称
    int inDegree;             // 入度，用于拓扑排序和关键路径
    EdgeNode *firstEdge;      // 第一条出边
} VertexNode;

// 邻接表图
typedef struct {
    VertexNode adjList[MAX_VERTEX_NUM];
    int numVertex;
    int numEdge;
} GraphList;

typedef GraphList AdjGraph;

// 邻接矩阵图
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];
    EdgeType edges[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int numVertex;
    int numEdge;
} MTGraph;

extern GraphList G_Critical;  // 关键路径全局图
extern GraphList G_Topo;

void CreateGraph(GraphList *g, int directed);
void CreateAdjMatrix(MTGraph *g, int directed);
void PrintAdjList(GraphList g);
void PrintAdjMatrix(MTGraph g);
void FreeGraph(GraphList *g);
void DFS(GraphList G);
void BFS(GraphList G);
int  TopologicalSort(AdjGraph G);
void CriticalPath(void);
void ShortestPath(MTGraph G, int start);

#endif