#include "heap.h"
#include <stdlib.h>

/** @brief An alias to the structure representing the nodes of the binary heap. */
typedef struct heap_node heap_node_t;

/** @brief The structure representing the nodes of the binary heap. */
struct heap_node {
    /** The value in the node */
    void        *value;
    /** A pointer to the left child node */
    heap_node_t *left;
    /** A pointer to the right child node */
    heap_node_t *right;
    /** A pointer to the father node */
    heap_node_t *father;
};

struct heap {
    /** The root of the binary heap. May be `NULL` */
    heap_node_t   *root;
    /** The size of the binary heap */
    size_t         size;
    /** The comparator function used by the elements */
    compare_func_t comparator;
};



// // auxiliary functions
// void print_level(heap_node_t *p_node, int level, int size, int path) {
//     if (path>size) {
//         if (path<=1) printf("nil\n");
//         return;
//     }
//
//     for (int i = level-1; i > 0; i--) {
//         printf(" %c ", ((path>>i&1)==0) ? '|':' ');
//     }
//
//     if (p_node == NULL) {
//         printf("%snil\n", level == 0 ? "" : " +-");
//         return;
//     }
//
//     printf("%s(%d)\n", level == 0 ? "" : " +-", p_node->value);
//     print_level(p_node->left,  level+1, size, (path<<1)+0);
//     print_level(p_node->right, level+1, size, (path<<1)+1);
// }

static heap_node_t **get_child(heap_node_t *father, int index) {
    return ((index & 0x1) == 0x0) ? &father->left : &father->right;
}

// gives a pointer to the heap node representing the
// node number n (starting from 0) in the heap
static heap_node_t *go_to_node(heap_t *p_heap, int n) {
    n++;
    int level=0;
    while (n>>level != 0x1)
        level++;
    heap_node_t *father = p_heap->root;
    while (--level>=0) {
        father = *get_child(father, n>>level);
    }
    return father;
}

// switch this node with his parent
static heap_node_t *ascend_node(heap_node_t *node) {
    void *value = node->value;
    node->value = node->father->value;
    node->father->value = value;
    return node->father;
}

static void ascend_while_possible(heap_node_t *node, compare_func_t comparator) {
    while (node->father != NULL && (*comparator)(node->value, node->father->value) < 0)
        node = ascend_node(node);
}

static void descend_while_possible(heap_node_t *node, compare_func_t comparator) {
    for (;;) {
        void *min = node->value;
        heap_node_t *child = NULL; // itself
        heap_node_t *c;
        for (int i=0; i<2; i++) {
            if ((c=*get_child(node, i))==NULL || c->value==NULL) continue;
            if ((*comparator)(c->value, min) >= 0) continue;
            child = c;
            min = child->value;
        }
        if (child==NULL)
            break;
        ascend_node(child);
        node = child;
    }
}

// functions from the signature
heap_t *heap_new(compare_func_t comparator) {
    heap_t *ans = malloc(sizeof *ans);
    *ans = (heap_t){NULL, 0, comparator};
    return ans;
}

bool heap_is_empty(heap_t const *p_heap) {
    return p_heap->size == 0;
}

void heap_insert(heap_t *p_heap, void *value) {
    if (value==NULL) return;
    heap_node_t **node_cell;
    heap_node_t node = {value, NULL, NULL, NULL};
    if (p_heap->size++ == 0) {
        node_cell = &p_heap->root;
    } else {
        node.father = go_to_node(p_heap, ((p_heap->size)>>1)-1);
        node_cell = get_child(node.father, p_heap->size);
    }
    if (*node_cell == NULL) {
        *node_cell = malloc(sizeof **node_cell);
        **node_cell = node;
    } else // use allocated node again
        (*node_cell)->value = node.value; // keep children
    ascend_while_possible(*node_cell, p_heap->comparator);
}

void *heap_extract_min(heap_t *p_heap) {
    if (p_heap->size == 0) return NULL;
    void *ans = p_heap->root->value;
    heap_node_t *node = go_to_node(p_heap, --p_heap->size);
    p_heap->root->value = node->value;
    node->value = NULL; // no need to deallocate each time an element is extracted
    descend_while_possible(p_heap->root, p_heap->comparator);
    return ans;
}

// void print_heap(heap_t *p_heap) {
//     print_level(p_heap->root, 0, p_heap->size, 1);
// }

static void deallocate_node(heap_node_t *node) {
    if (node==NULL) return;
    for (int i=0; i<2; i++) deallocate_node(*get_child(node, i));
    if (node->value!=NULL) free(node->value);
    free(node);
}

void heap_deallocate(heap_t *p_heap) {
    deallocate_node(p_heap->root);
    free(p_heap);
}
