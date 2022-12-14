#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tnode_t {
    struct tnode_t** children;
    struct tnode_t* parent;
    void* data;
};

struct tree_t {
    struct tnode_t* root;
    size_t bytes;
};

static size_t tnode_children_count(const struct tnode_t* node)
{
    size_t i;
    for (i = 0; node->children[i]; ++i);
    return i;
}

static size_t tnode_children_capacity(const size_t children_count)
{
    size_t i;
    for (i = 1; i < children_count; i <<= 1);
    return i;
}

static struct tnode_t* tnode_create(const void* data, const size_t bytes)
{
    struct tnode_t* node = malloc(sizeof(struct tnode_t));
    node->parent = NULL;
    node->children = calloc(sizeof(struct tnode_t*), 1);
    node->data = malloc(bytes);
    memcpy(node->data, data, bytes);
    return node;
}

static void tnode_free(struct tnode_t* node)
{
    size_t i;
    for (i = 0; node->children[i]; ++i) {
        tnode_free(node->children[i]);
    }

    free(node->children);
    free(node->data);
    
    memset(node, 0, sizeof(struct tnode_t));
    free(node);
}

static void tnode_push(struct tnode_t* root, struct tnode_t* leave)
{
    const size_t size = tnode_children_count(root);
    const size_t cap = tnode_children_capacity(size);

    if (size + 1 == cap) {
        root->children = realloc(root->children, cap * 2 * sizeof(struct tnode_t));
    }
    
    leave->parent = root;
    root->children[size] = leave;
    root->children[size + 1] = NULL;
}

static struct tree_t tree_create(const size_t bytes)
{
    struct tree_t tree;
    tree.root = NULL;
    tree.bytes = bytes;
    return tree;
}

static void tree_free(struct tree_t* tree)
{
    tnode_free(tree->root);
    tree->root = NULL;
}

static void tree_push_root(struct tree_t* tree, const void* data)
{
    struct tnode_t* node = tnode_create(data, tree->bytes);
    if (tree->root) {
        tnode_push(node, tree->root);
    }
    tree->root = node;
}

static void tree_push_leave(struct tree_t* tree, const void* data)
{
    struct tnode_t* node = tnode_create(data, tree->bytes);
    if (tree->root) {
        tnode_push(tree->root, node);
    } else {
        tree->root = node;
    }
}

#endif /* TREE_H */
