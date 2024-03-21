/**
  ******************************************************************************
  * @file           : binary_search_string_tree.cpp
  * @author         : Troy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/21
  ******************************************************************************
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_search_string_tree.h"

/**
 * @brief 在二叉搜索树中查找拥有最小字符串的节点
 * @param root 二叉搜索树根节点
 * @return 二叉搜索树种最小字符串的节点
 */
struct bsst* bsst_min_node(struct bsst *root) {
    if(root == NULL) {
        return NULL;
    } else if(root->left == NULL) {
        /* 如果该节点已经是叶子节点，则可以直接返回值 */
        return root;
    } else {
        /* 递归一直到左子树为叶子节点 */
        return bsst_min_node(root->left);
    }
}
/**
 * @brief 二叉搜索字符串树插入函数
 * @param root 根节点指针的地址
 * @param str 要插入的字符串
 * @param user_data 要插入的用户数据
 */
void bsst_insert(struct bsst **root, const char* str, void* user_data) {
    struct bsst *root_prt = *root;
    if(root_prt == NULL) {
        /* 如果节点为空 说明找到了可以进行插入的节点 则申请一个新的节点 */
        struct bsst *temp = (struct bsst*)malloc(sizeof(struct bsst));
        if(!temp) {
            return;
        }
        /* 申请字符串所需要的内存 */
        temp->str = (char*)malloc(strlen(str) + 1);
        if(!temp->str) {
            /* 如果字符串内存申请失败，则释放节点的内存，以防内存泄露 */
            free(temp);
            return;
        }
        /* 拷贝字符串 */
        strcpy(temp->str, str);
        temp->user_data = user_data;
        /* 左右节点置为NULL */
        temp->left = temp->right = NULL;
        /* 将当前节点的指针更改为temp */
        *root = temp;
    }
    else if(strcmp(str, root_prt->str) < 0) {
        /* 如果要插入的字符串比当前节点的小就去左树 */
        bsst_insert(&(root_prt->left), str, user_data);
    } else {
        /* 如果要插入的字符串比当前节点的大就去右树 */
        bsst_insert(&(root_prt->right), str, user_data);
    }
}
/**
 * @brief 二叉搜索字符串树删除节点且保持二叉搜索树特性
 * @param root 根节点的地址
 * @param str 要删除的字符串
 */
void bsst_delete(struct bsst **root, const char* str) {
    struct bsst *root_ptr = *root;
    struct bsst *temp;
    int str_cmp_result;
    if(root_ptr == NULL) {
        /* 如果是空的话没法删除 */
        return;
    }
    /* 获取字符串比较结果 */
    str_cmp_result = strcmp(str, root_ptr->str);
    if(str_cmp_result < 0) {
        /* 如果要删除的字符串节点比当前节点的字符串小 就去左树删除 */
        bsst_delete(&root_ptr->left, str);
    } else if(str_cmp_result > 0) {
        /* 如果要删除的字符串节点比当前节点的字符串大 就去右树删除 */
        bsst_delete(&root_ptr->right, str);
    } else {
        /* 找到了要删除的字符串的节点 首先释放字符串的内存 */
        free(root_ptr->str);
        root_ptr->str = NULL;
        if(root_ptr->left == NULL && root_ptr->right == NULL) {
            /* 没有左右节点 直接释放并且置为NULL即可 */
            free(root_ptr);
            *root = NULL;
        } else if(root_ptr->left == NULL) {
            /* 左节点为空，则直接释放且将父节点与右节点连接 */
            temp = root_ptr->right;
            free(root_ptr);
            *root = temp;
        } else if(root_ptr->right == NULL) {
            /* 右节点为空，则直接释放且将父节点与左节点连接 */
            temp = root_ptr->left;
            free(root_ptr);
            *root = temp;
        } else {
            /* 左右两个节点都不为空 这时候我们需要将问题简化为删除一个节点的值
             * 从右子树中查找最小的节点，将指针放入temp中
             * */
            temp = bsst_min_node(root_ptr->right);
            root_ptr->user_data = temp->user_data;
            /* 为字符串申请内存 */
            root_ptr->str = (char*)malloc(strlen(temp->str) + 1);
            /* 字符串拷贝 */
            strcpy(root_ptr->str, temp->str);
            /* 此时该节点已经被右子树的最小节点替换，所以删除掉右子树的最小节点即可
             * 问题被简化
             * */
            bsst_delete(&root_ptr->right, temp->str);
        }
    }
}

/**
 * @brief 二叉搜索字符串树中序遍历 深度优先中的一种
 * @param root 二叉树根节点指针
 * @note 经过该算法遍历的二叉搜索树应该是升序的
 */
void bsst_inorder(struct bsst *root) {
    if(root == NULL) {
        /* 触底回归 递归线终止条件 */
        return;
    }
    /* 中序遍历的顺序为 <left><root->data><right> */
    bsst_inorder(root->left);
    printf("%s\n", root->str);
    bsst_inorder(root->right);
}

void* bsst_search(struct bsst *root, char* str) {
    int str_cmp_result;
    if(root == NULL) {
        return NULL;
    }
    /* 获取字符串比较结果 */
    str_cmp_result = strcmp(str, root->str);
    if(str_cmp_result < 0) {
        /* 如果要查找的字符串比当前节点的字符串小 就去当前字符串的左树查找 */
        return bsst_search(root->left, str);
    } else if(str_cmp_result > 0) {
        /* 如果要查找的字符串比当前节点的字符串大 就去当前字符串的右树查找 */
        return bsst_search(root->right, str);
    } else {
        /* 相等则返回用户数据 */
        return root->user_data;
    }
}