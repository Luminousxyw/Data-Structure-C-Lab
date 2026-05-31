#include "binary.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    BiTree root = NULL;
    
    printf("Enter preorder sequence to build binary tree ('#' for null):\n");
    CreatBiTree(&root);

    printf("\nRecursive Traversal:\n");
    printf("Preorder:"); PreOrder(root); printf("\n");
    printf("Inorder:"); InOrder(root); printf("\n");
    printf("Postorder:"); PostOrder(root); printf("\n");

    printf("\nNon-recursive Traversal:\n");
    printf("Preorder:"); PreOrder2(root); printf("\n");
    printf("Inorder:"); InOrder2(root); printf("\n");
    printf("Postorder:"); BiTNodePost(root); printf("\n");

    printf("\nLevel Order:\n");
    printf("Level:"); LevelOrder(root); printf("\n");

    getchar();
    getchar();
    return 0;
}