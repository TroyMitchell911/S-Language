/**
  ******************************************************************************
  * @file           : binary_search_string_tree.h
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/21
  ******************************************************************************
  */

#ifndef STRUCTURE_BINARY_SEARCH_STRING_TREE_H
#define STRUCTURE_BINARY_SEARCH_STRING_TREE_H

typedef struct bsst {
    char* str;
    void* user_data;
    struct bsst *left;
    struct bsst *right;
}bsst_t;

void bsst_insert(struct bsst **root, const char* str, void* user_data);
void bsst_delete(struct bsst **root, const char* str);
void bsst_inorder(struct bsst *root);
void* bsst_search(struct bsst *root, char* str);

#endif //STRUCTURE_BINARY_SEARCH_STRING_TREE_H
