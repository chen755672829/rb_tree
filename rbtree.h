/*
    此红黑树 无法插入键值相同的元素，如 c++ stl 中set 和 map一样
*/
#ifndef __RBTREE_H__
#define __RBTREE_H__
#include <assert.h>
#include <stdlib.h>

typedef enum RbNodeColor 
{
    RED = 0, 
    BLACK = 1,
} RbNodeColor_t;

/* 整个红黑树结构中，不管理内存。*/

/***
 * brief: 红黑树结构
 * @param1[in] name: 红黑树结构体的名;
 * @param2[in] type: 红黑树 一个节点全部 对应的类型
 * 使用：直接调用该接口 RB_TREE(name, type, typedef_name) 把该结构体的定义，引入到当前文件中
***/
#define RB_TREE(name, type, typedef_name) \
    struct name {           \
        struct type *_root; \
        struct type *_hol; \
        int  _size;    \
    } typedef_name

/***
 * brief: 初始化红黑树。
 * @param1[in] rbtree：对应红黑树结构的指针变量。
***/
#define RE_TREE_INIT(rbtree)  \
    rbtree->_root = rbtree->_hol = NULL;\
    rbtree->_size = 0

/***
 * brief: 定义红黑树节点字段。
 * @param1[in] type: 红黑树一个节点全部对应的类型
 *
***/
#define RE_NODE(type)           \
struct {                        \
    struct type *left;          \
    struct type *right;         \
    struct type *ftr;           \
    RbNodeColor_t color;        \
}

/***
 * brief: 初始化红黑树节点。
 * @param1[in] elem :红黑树节点全部对应的指针变量名
 * @param2[in] field :红黑树节点全部中红黑树节点字段名。
***/
#define RB_NODE_INIT(elem, field)  \
    (elem)->field.left = (elem)->field.right = (elem)->field.ftr = NULL;

/***
 *  获取当前 节点全部 里面 红黑树节点字段的 左、右、父亲、颜色的 接口
 *  elem: 红黑树节点全部类型的指针变量
 *  field  红黑树节点全部类型中红黑树节点的字段 (元素里面的就只是红黑树节点的字段 RE_NODE)
***/
#define RB_NODE_LEFT(elem, field)   (elem)->field.left
#define RB_NODE_RIGHT(elem, field)  (elem)->field.right
#define RB_NODE_FTR(elem, field)    (elem)->field.ftr
#define RB_NODE_COLOR(elem, field)  (elem)->field.color


#define RB_TREE_ROOT(rbtree)    (rbtree)->_root
#define RB_TREE_HOL(rbtree)     (rbtree)->_hol
#define RB_TREE_SIZE(rbtree)    ((rbtree)->_size)
#define RB_TREE_EMPTY(rbtree)   !((rbtree)->_size)

#define RB_NODE_BRO(elem, field)    \
    (RB_NODE_LEFT(RB_NODE_FTR(elem, field), field) == elem) ?               \
    (RB_NODE_RIGHT(RB_NODE_FTR(elem, field), field)) :  RB_NODE_LEFT(RB_NODE_FTR(elem, field), field)


/* zig 右旋 */
#define zig(elem, field)                                                                \
do {                                                                                    \
    if (RB_NODE_FTR(elem, field)) {                                                     \
        if (RB_NODE_LEFT(RB_NODE_FTR(elem, field),field) == elem) {                     \
            RB_NODE_LEFT(RB_NODE_FTR(elem, field),field) = RB_NODE_LEFT(elem,field);    \
        }                                                                               \
        else {                                                                          \
            RB_NODE_RIGHT(RB_NODE_FTR(elem, field),field) = RB_NODE_LEFT(elem,field);   \
        }                                                                               \
    }                                                                                   \
    if (RB_NODE_RIGHT(RB_NODE_LEFT(elem, field),field)) {                               \
        RB_NODE_FTR(RB_NODE_RIGHT(RB_NODE_LEFT(elem, field), field), field) = elem;     \
    }                                                                                   \
    RB_NODE_FTR(RB_NODE_LEFT(elem, field), field) = RB_NODE_FTR(elem, field);           \
    RB_NODE_FTR(elem, field) = RB_NODE_LEFT(elem, field);                               \
    RB_NODE_LEFT(elem, field) = RB_NODE_RIGHT(RB_NODE_LEFT(elem, field), field);        \
    RB_NODE_RIGHT(RB_NODE_FTR(elem, field), field) = elem;                              \
} while(0)


/* zag 左旋 */
#define zag(elem, field)                                                                \
do {                                                                                    \
    if (RB_NODE_FTR(elem, field)) {                                                     \
        if (RB_NODE_LEFT(RB_NODE_FTR(elem, field),field) == elem) {                     \
            RB_NODE_LEFT(RB_NODE_FTR(elem, field),field) = RB_NODE_RIGHT(elem, field);  \
        }                                                                               \
        else {                                                                          \
            RB_NODE_RIGHT(RB_NODE_FTR(elem, field),field) = RB_NODE_RIGHT(elem, field); \
        }                                                                               \
    }                                                                                   \
    if (RB_NODE_LEFT(RB_NODE_RIGHT(elem, field), field)) {                              \
        RB_NODE_FTR(RB_NODE_LEFT(RB_NODE_RIGHT(elem, field), field), field) = elem;     \
    }                                                                                   \
    RB_NODE_FTR(RB_NODE_RIGHT(elem, field), field) = RB_NODE_FTR(elem, field);          \
    RB_NODE_FTR(elem, field) = RB_NODE_RIGHT(elem, field);                              \
    RB_NODE_RIGHT(elem, field) = RB_NODE_LEFT(RB_NODE_RIGHT(elem, field), field);       \
    RB_NODE_LEFT(RB_NODE_FTR(elem, field), field) = elem;                               \
} while (0)


/***
 * brief: 红黑树函数声明
 * @param1 [in] name 为 红黑树 结构体类型名；
 * @param2 [in] type 为 红黑树节点全部  的类型，这里类型中有 红黑树节点(RB_NODE)的字段
 * @param3 [in] field 为 红黑树节点全部中的红黑树节点字段 的变量名
 * @param4 [in] cmp 为 红黑树 节点全部类型 比较大小的函数
 * @param5 [in] attr 为 是否定义为 static inline
 * @param6 [in] key_type 红黑树节点 键值的类型 (如int 、struct KEY类型，若这个为结构体的话，需要带struct关键字)
 * @param7 [in] elem_key_cmp  比较函数 (参数1：红黑树节点全部类型，参数2：键值类型)
 * @param8 [in] funcfree 释放节点资源。(在销毁红黑树时，需要dfs后序遍历，这时候才进行了资源销毁操作)。
 *  _RB_LEFT_NODE 为找节点的前驱
 *  _RB_RIGHT_NODE 后继
 *  _RB_SUCC   为真后继
 *  _RB_FIND 查找节点（参数为节点全部元素）
 *  _RB_FIND_FROM_KEY 根据键值查找节点
 *  _RB_INSERT 插入节点
 *  _RB_SolveDoubleRed 双红修正
 *  _RB_SolveLostBlack 失黑修正
 *  _RB_REMOVE  删除节点（参数为红黑树内存在的节点指针）
 *  _RB_REMOVE_FROM_KEY 删除节点（先通过key查找到红黑树节点，然后再删除）。
 *  _RB_NODE_MIN 返回红黑树中节点中最小的节点
 *  _RB_NODE_MAX 返回红黑树中节点最大的节点
 *  _RB_LOWER_BOUND_NODE lower_bound 查找第一个大于等于自己的节点
 *  _RB_LOWER_BOUND_NODE_FROM_KEY 通过key查找
 *  _RB_UPPER_BOUND_NODE upper_bound 查找第一个大于自己的节点
 *  _RB_UPPER_BOUND_NODE_FROM_KEY 通过key查找。
***/
#define RB_TREE_FUNC_DECLARE(name, type, field, cmp, attr, key_type, elem_key_cmp, funcfree) \
    attr struct type *name##_RB_LEFT_NODE(struct name *, struct type *);        \
    attr struct type *name##_RB_RIGHT_NODE(struct name *, struct type *);       \
    attr struct type *name##_RB_SUCC(struct type *);                            \
    attr struct type *name##_RB_FIND(struct name *, struct type *);             \
    attr struct type *name##_RB_FIND_FROM_KEY(struct name * , key_type);         \
    attr int name##_RB_INSERT(struct name *, struct type *);                    \
    attr void name##_RB_SolveDoubleRed(struct name *, struct type *);           \
    attr void name##_RB_SolveLostBlack(struct name *, struct type *);           \
    attr struct type *name##_RB_REMOVE(struct name *, struct type *);           \
    attr struct type *name##_RB_REMOVE_FROM_KEY(struct name *, key_type );       \
    attr struct type *name##_RB_NODE_MIN(struct name *);                        \
    attr struct type *name##_RB_NODE_MAX(struct name *);                        \
    attr struct type *name##_RB_LOWER_BOUND_NODE(struct name * , struct type * );       \
    attr struct type *name##_RB_LOWER_BOUND_NODE_FROM_KEY(struct name * , key_type );    \
    attr struct type *name##_RB_UPPER_BOUND_NODE(struct name * , struct type * );        \
    attr struct type *name##_RB_UPPER_BOUND_NODE_FROM_KEY(struct name *, key_type );     \
    attr void name##_RB_TREE_FREE_DFS(struct name * , struct type * )

#define RB_TREE_FUNC_DEFINITION(name, type, field, cmp, attr, key_type, elem_key_cmp, funcfree) \
attr struct type *name##_RB_FIND(struct name * rbtree, struct type * elem)      \
{                                                                               \
    assert(rbtree != NULL && elem != NULL);                                     \
    struct type * ptn =  RB_TREE_ROOT(rbtree);                                  \
    RB_TREE_HOL(rbtree) = NULL;                                                 \
    int return_val;                                                             \
    while (ptn) {                                                               \
        RB_TREE_HOL(rbtree) = ptn;                                              \
        return_val = cmp(ptn, elem);                                            \
        switch (return_val) {                                                   \
            case 0:                                                             \
                return ptn;                                                     \
                break;                                                          \
            case 1:                                                             \
                ptn = RB_NODE_RIGHT(ptn, field);                                \
                break;                                                          \
            case -1:                                                            \
                ptn = RB_NODE_LEFT(ptn, field);                                 \
                break;                                                          \
            }                                                                   \
    }                                                                           \
    return ptn;                                                                 \
}                                                                               \
                                                                                \
attr struct type *name##_RB_FIND_FROM_KEY(struct name * rbtree, key_type key)   \
{                                                                               \
    assert(rbtree != NULL);                                                     \
    struct type * ptn =  RB_TREE_ROOT(rbtree);                                  \
    RB_TREE_HOL(rbtree) = NULL;                                                 \
    int return_val;                                                             \
    while (ptn) {                                                               \
        RB_TREE_HOL(rbtree) = ptn;                                              \
        return_val = elem_key_cmp(ptn, key);                                    \
        switch (return_val) {                                                   \
            case 0:                                                             \
                return ptn;                                                     \
                break;                                                          \
            case 1:                                                             \
                ptn = RB_NODE_RIGHT(ptn, field);                                \
                break;                                                          \
            case -1:                                                            \
                ptn = RB_NODE_LEFT(ptn, field);                                 \
                break;                                                          \
            }                                                                   \
    }                                                                           \
    return ptn;                                                                 \
}                                                                               \
                                                                                \
attr struct type *name##_RB_LOWER_BOUND_NODE(struct name * rbtree , struct type * elem)     \
{                                                                                           \
    struct type *ptn = RB_TREE_ROOT(rbtree);                                                \
    if (ptn == NULL) return NULL;                                                           \
    RB_TREE_HOL(rbtree) = NULL;                                                             \
    while (ptn) {                                                                           \
        RB_TREE_HOL(rbtree) = ptn;                                                          \
        if (cmp(ptn, elem) <= 0) {                                                          \
            ptn = RB_NODE_LEFT(ptn, field);                                                 \
        }                                                                                   \
        else {                                                                              \
            ptn = RB_NODE_RIGHT(ptn, field);                                                \
        }                                                                                   \
    }                                                                                       \
    if (cmp(RB_TREE_HOL(rbtree), elem) <= 0) {                                              \
        return RB_TREE_HOL(rbtree);                                                         \
    }                                                                                       \
    else return name##_RB_RIGHT_NODE(rbtree, RB_TREE_HOL(rbtree));                          \
}                                                                                           \
                                                                                            \
attr struct type *name##_RB_LOWER_BOUND_NODE_FROM_KEY(struct name * rbtree , key_type key) \
{                                                                                           \
    struct type *ptn = RB_TREE_ROOT(rbtree);                                                \
    if (ptn == NULL) return NULL;                                                           \
    RB_TREE_HOL(rbtree) = NULL;                                                             \
    while (ptn) {                                                                           \
        RB_TREE_HOL(rbtree) = ptn;                                                          \
        if (elem_key_cmp(ptn, key) <= 0) {                                                  \
            ptn = RB_NODE_LEFT(ptn, field);                                                 \
        }                                                                                   \
        else {                                                                              \
            ptn = RB_NODE_RIGHT(ptn, field);                                                \
        }                                                                                   \
    }                                                                                       \
    if (elem_key_cmp(RB_TREE_HOL(rbtree), key) <= 0) {                                      \
        return RB_TREE_HOL(rbtree);                                                         \
    }                                                                                       \
    else return name##_RB_RIGHT_NODE(rbtree, RB_TREE_HOL(rbtree));                          \
}                                                                                           \
                                                                                            \
attr struct type *name##_RB_UPPER_BOUND_NODE(struct name * rbtree , struct type * elem)     \
{                                                                                           \
    struct type *ptn = RB_TREE_ROOT(rbtree);                                                \
    if (ptn == NULL) return NULL;                                                           \
    RB_TREE_HOL(rbtree) = NULL;                                                             \
    while (ptn) {                                                                           \
        RB_TREE_HOL(rbtree) = ptn;                                                          \
        if (cmp(ptn, elem) < 0) {                                                           \
            ptn = RB_NODE_LEFT(ptn, field);                                                 \
        }                                                                                   \
        else {                                                                              \
            ptn = RB_NODE_RIGHT(ptn, field);                                                \
        }                                                                                   \
    }                                                                                       \
    if (cmp(RB_TREE_HOL(rbtree), elem) < 0) {                                               \
        return RB_TREE_HOL(rbtree);                                                         \
    }                                                                                       \
    else return name##_RB_RIGHT_NODE(rbtree, RB_TREE_HOL(rbtree));                          \
}                                                                                           \
                                                                                            \
attr struct type *name##_RB_UPPER_BOUND_NODE_FROM_KEY(struct name * rbtree , key_type key)  \
{                                                                                           \
    struct type *ptn = RB_TREE_ROOT(rbtree);                                                \
    if (ptn == NULL) return NULL;                                                           \
    RB_TREE_HOL(rbtree) = NULL;                                                             \
    while (ptn) {                                                                           \
        RB_TREE_HOL(rbtree) = ptn;                                                          \
        if (elem_key_cmp(ptn, key) < 0) {                                                   \
            ptn = RB_NODE_LEFT(ptn, field);                                                 \
        }                                                                                   \
        else {                                                                              \
            ptn = RB_NODE_RIGHT(ptn, field);                                                \
        }                                                                                   \
    }                                                                                       \
    if (elem_key_cmp(RB_TREE_HOL(rbtree), key) < 0) {                                       \
        return RB_TREE_HOL(rbtree);                                                         \
    }                                                                                       \
    else return name##_RB_RIGHT_NODE(rbtree, RB_TREE_HOL(rbtree));                          \
}                                                                                           \
                                                                                \
attr struct type *name##_RB_SUCC(struct type *elem)                             \
{                                                                               \
    if (elem == NULL) {                                                         \
        return NULL;                                                            \
    }                                                                           \
    struct type * ptn = RB_NODE_RIGHT(elem, field);                             \
    if (ptn != NULL) {                                                          \
        while (RB_NODE_LEFT(ptn, field) != NULL) {                              \
            ptn = RB_NODE_LEFT(ptn, field);                                     \
        }                                                                       \
    }                                                                           \
    return ptn;                                                                 \
}                                                                               \
                                                                                \
attr struct type *name##_RB_RIGHT_NODE(struct name* rbtree, struct type* elem)  \
{                                                                               \
    if (elem == NULL) return NULL;                                              \
    if (RB_NODE_RIGHT(elem, field)) {                                           \
        elem = RB_NODE_RIGHT(elem, field);                                      \
        while (RB_NODE_LEFT(elem, field)) {                                     \
            elem = RB_NODE_LEFT(elem, field);                                   \
        }                                                                       \
    }                                                                           \
    else {                                                                      \
        while (RB_NODE_FTR(elem, field) &&                                      \
            RB_NODE_RIGHT(RB_NODE_FTR(elem, field), field) == elem) {           \
               elem = RB_NODE_FTR(elem, field);                                 \
        }                                                                       \
        elem = RB_NODE_FTR(elem, field);                                        \
    }                                                                           \
    return elem;                                                                \
}                                                                               \
                                                                                \
attr struct type *name##_RB_LEFT_NODE(struct name* rbtree, struct type* elem)   \
{                                                                               \
    if (elem == NULL) return NULL;                                              \
    if (RB_NODE_LEFT(elem, field)) {                                            \
        elem = RB_NODE_LEFT(elem, field);                                       \
        while (RB_NODE_RIGHT(elem, field)) {                                    \
            elem = RB_NODE_RIGHT(elem, field);                                  \
        }                                                                       \
    }                                                                           \
    else {                                                                      \
        while (RB_NODE_FTR(elem, field) &&                                      \
          RB_NODE_LEFT(RB_NODE_FTR(elem, field), field) == elem) {              \
            elem = RB_NODE_FTR(elem, field);                                    \
        }                                                                       \
        elem = RB_NODE_FTR(elem, field);                                        \
    }                                                                           \
    return elem;                                                                \
}                                                                               \
                                                                                \
attr void name##_RB_SolveDoubleRed(struct name * rbtree, struct type *elem)     \
{                                                                               \
    while (RB_NODE_FTR(elem, field) != NULL &&                                  \
            RB_NODE_COLOR(RB_NODE_FTR(elem, field), field) == RED) {            \
        struct type * pftr = RB_NODE_FTR(elem, field);                          \
        struct type * grdftr = RB_NODE_FTR(pftr, field);                        \
        struct type * uncle = RB_NODE_BRO(pftr, field);                         \
        if (uncle == NULL || RB_NODE_COLOR(uncle, field) == BLACK) {            \
            if (RB_NODE_LEFT(grdftr, field) == pftr) {                          \
                if (RB_NODE_LEFT(pftr, field) == elem) {                        \
                    if (grdftr == RB_TREE_ROOT(rbtree)) {                       \
                        RB_TREE_ROOT(rbtree) = pftr;                            \
                    }                                                           \
                    zig(grdftr, field);                                         \
                    RB_NODE_COLOR(grdftr, field) = RED;                         \
                    RB_NODE_COLOR(pftr, field) = BLACK;                         \
                } else {                                                        \
                    if (grdftr == RB_TREE_ROOT(rbtree)) {                       \
                        RB_TREE_ROOT(rbtree) = elem;                            \
                    }                                                           \
                    zag(pftr, field);                                           \
                    zig(grdftr, field);                                         \
                    RB_NODE_COLOR(elem, field) = BLACK;                         \
                    RB_NODE_COLOR(grdftr, field) = RED;                         \
                }                                                               \
            } else {                                                            \
                if (elem == RB_NODE_RIGHT(pftr, field)) {                       \
                    if (grdftr == RB_TREE_ROOT(rbtree)) {                       \
                        RB_TREE_ROOT(rbtree) = pftr;                            \
                    }                                                           \
                    zag(grdftr, field);                                         \
                    RB_NODE_COLOR(grdftr, field) = RED;                         \
                    RB_NODE_COLOR(pftr, field) = BLACK;                         \
                }                                                               \
                else {                                                          \
                    if (grdftr == RB_TREE_ROOT(rbtree)) {                       \
                        RB_TREE_ROOT(rbtree) = elem;                            \
                    }                                                           \
                    zig(pftr, field);                                           \
                    zag(grdftr, field);                                         \
                    RB_NODE_COLOR(elem, field) = BLACK;                         \
                    RB_NODE_COLOR(grdftr, field) = RED;                         \
                }                                                               \
            }                                                                   \
            return ;                                                            \
        }                                                                       \
        else {                                                                  \
            RB_NODE_COLOR(pftr, field) = BLACK;                                 \
            RB_NODE_COLOR(uncle, field) = BLACK;                                \
            RB_NODE_COLOR(grdftr, field) = RED;                                 \
            elem = grdftr;                                                      \
        }                                                                       \
    }                                                                           \
    if (elem == RB_TREE_ROOT(rbtree)) {                                         \
        RB_NODE_COLOR(elem, field) = BLACK;                                     \
    }                                                                           \
    return ;                                                                    \
}                                                                               \
                                                                                \
attr int name##_RB_INSERT(struct name * rbtree, struct type * elem)             \
{                                                                               \
    if (elem == NULL) return 0;                                                 \
    struct type * ptn = name##_RB_FIND(rbtree, elem);                           \
    if (ptn) {                                                                  \
        return 0;                                                               \
    }                                                                           \
    if (RB_TREE_HOL(rbtree) == NULL) {                                          \
        RB_NODE_COLOR(elem, field) = BLACK;                                     \
        RB_TREE_ROOT(rbtree) = elem;                                            \
        RB_TREE_SIZE(rbtree) = 1;                                               \
        /*printf("---%s---RB_TREE_SIZE==%d\n",__func__,RB_TREE_SIZE(rbtree));*/ \
        return 1;                                                               \
    }                                                                           \
    RB_NODE_COLOR(elem, field) = RED;                                           \
    ++ RB_TREE_SIZE(rbtree) ;                                                   \
    if (cmp(RB_TREE_HOL(rbtree), elem) < 0) {                                   \
        RB_NODE_LEFT(RB_TREE_HOL(rbtree), field) = elem;                        \
    }                                                                           \
    else {                                                                      \
        RB_NODE_RIGHT(RB_TREE_HOL(rbtree), field) = elem;                       \
    }                                                                           \
    RB_NODE_FTR(elem, field) = RB_TREE_HOL(rbtree);                             \
    name##_RB_SolveDoubleRed(rbtree, elem);                                     \
    return 1;                                                                   \
}                                                                               \
                                                                                \
attr void name##_RB_SolveLostBlack(struct name *rbtree, struct type * elem)     \
{                                                                               \
    while (elem != RB_TREE_ROOT(rbtree)) {                                      \
        struct type *bro_node = RB_NODE_BRO(elem, field);                       \
        struct type *pftr = RB_NODE_FTR(elem, field);                           \
        if (RB_NODE_COLOR(bro_node, field) == RED) {    /*LB-1*/                \
            RB_NODE_COLOR(bro_node, field) = BLACK;                             \
            RB_NODE_COLOR(pftr, field) = RED;                                   \
            if (pftr == RB_TREE_ROOT(rbtree)) {                                 \
                RB_TREE_ROOT(rbtree) = bro_node;                                \
            }                                                                   \
            if (RB_NODE_LEFT(pftr, field) == elem) {                            \
                zag(pftr, field);                                               \
            }                                                                   \
            else {                                                              \
                zig(pftr, field);                                               \
            }                                                                   \
            bro_node = RB_NODE_BRO(elem, field);                                \
            pftr = RB_NODE_FTR(elem, field);                                    \
        }                                                                       \
        /* LB-3 */                                                              \
        if(RB_NODE_LEFT(bro_node, field) &&                                     \
               RB_NODE_COLOR(RB_NODE_LEFT(bro_node, field), field) == RED) {    \
            struct type * bro_son = RB_NODE_LEFT(bro_node, field);              \
            RbNodeColor_t old_color = RB_NODE_COLOR(pftr, field);               \
            RB_NODE_COLOR(pftr, field) = BLACK;                                 \
            if (RB_NODE_LEFT(pftr, field) == bro_node) {                        \
                if (pftr == RB_TREE_ROOT(rbtree)) {                             \
                    RB_TREE_ROOT(rbtree) = bro_node;                            \
                }                                                               \
                zig(pftr, field);                                               \
                RB_NODE_COLOR(bro_son, field) = BLACK;                          \
            }                                                                   \
            else {                                                              \
                if (pftr == RB_TREE_ROOT(rbtree)) {                             \
                    RB_TREE_ROOT(rbtree) = bro_son;                             \
                }                                                               \
                zig(bro_node, field);                                           \
                zag(pftr, field);                                               \
            }                                                                   \
            RB_NODE_COLOR(RB_NODE_FTR(pftr, field), field) = old_color;         \
            return;                                                             \
       }                                                                        \
        else if (RB_NODE_RIGHT(bro_node, field) &&                              \
               RB_NODE_COLOR(RB_NODE_RIGHT(bro_node, field), field) == RED) {   \
            struct type * bro_son = RB_NODE_RIGHT(bro_node, field);             \
            RbNodeColor_t old_color = RB_NODE_COLOR(pftr, field);               \
            RB_NODE_COLOR(pftr, field) = BLACK;                                 \
            if (RB_NODE_RIGHT(pftr, field) == bro_node) {                       \
                if (pftr == RB_TREE_ROOT(rbtree)) {                             \
                    RB_TREE_ROOT(rbtree) = bro_node;                            \
                }                                                               \
                zag(pftr, field);                                               \
                RB_NODE_COLOR(bro_son, field) = BLACK;                          \
            }                                                                   \
            else {                                                              \
                if (pftr == RB_TREE_ROOT(rbtree)) {                             \
                    RB_TREE_ROOT(rbtree) = bro_son;                             \
                }                                                               \
                zag(bro_node, field);                                           \
                zig(pftr, field);                                               \
            }                                                                   \
            RB_NODE_COLOR(RB_NODE_FTR(pftr, field), field) = old_color;         \
            return;                                                             \
        }                                                                       \
        if (RB_NODE_COLOR(pftr, field) == RED) {    /* LB-2R */                 \
            RB_NODE_COLOR(pftr, field) = BLACK;                                 \
            RB_NODE_COLOR(bro_node, field) = RED;                               \
            return;                                                             \
        }                                                                       \
        else {  /*LB-2B */                                                      \
            RB_NODE_COLOR(bro_node, field) = RED;                               \
            elem = pftr;                                                        \
        }                                                                       \
    }                                                                           \
}                                                                               \
                                                                                \
attr void name##swap_elem(struct type *elem_1, struct type *elem_2) {           \
    struct type * ftr_1 = RB_NODE_FTR(elem_1, field);                           \
    struct type * left_1 = RB_NODE_LEFT(elem_1, field);                         \
    struct type * right_1 = RB_NODE_RIGHT(elem_1, field);                       \
    struct type tmp;                                                            \
    if (ftr_1 && ftr_1 != elem_2) {                                             \
        if (RB_NODE_LEFT(ftr_1, field) == elem_1) {                             \
            RB_NODE_LEFT(ftr_1, field) = elem_2;                                \
        }                                                                       \
        else {                                                                  \
            RB_NODE_RIGHT(ftr_1, field) = elem_2;                               \
        }                                                                       \
    }                                                                           \
    if(left_1 && left_1 != elem_2) RB_NODE_FTR(left_1, field) = elem_2;         \
    if(right_1 && right_1 != elem_2) RB_NODE_FTR(right_1, field) = elem_2;      \
    struct type * ftr_2 = RB_NODE_FTR(elem_2, field);                           \
    struct type * left_2 = RB_NODE_LEFT(elem_2, field);                         \
    struct type * right_2 = RB_NODE_RIGHT(elem_2, field);                       \
    if (ftr_2 && ftr_2 != elem_1) {                                             \
        if (RB_NODE_LEFT(ftr_2, field) == elem_2) {                             \
            RB_NODE_LEFT(ftr_2, field) = elem_1;                                \
        }                                                                       \
        else {                                                                  \
            RB_NODE_RIGHT(ftr_2, field) = elem_1;                               \
        }                                                                       \
    }                                                                           \
    if(left_2 && left_2 != elem_1) RB_NODE_FTR(left_2, field) = elem_1;         \
    if(right_2 && right_2 != elem_1) RB_NODE_FTR(right_2, field) = elem_1;      \
    /*RbNodeColor_t oldcolor_elem_1 = RB_NODE_COLOR(elem_1, field);*/           \
    /*RbNodeColor_t oldcolor_elem_2 = RB_NODE_COLOR(elem_2, field);*/           \
    if (RB_NODE_LEFT(elem_1, field) == elem_2) {                                \
        tmp.field = elem_1->field;                                              \
        elem_1->field = elem_2->field;                                          \
        elem_2->field = tmp.field;                                              \
        RB_NODE_FTR(elem_1, field) = elem_2;                                    \
        RB_NODE_LEFT(elem_2, field) = elem_1;                                   \
    }                                                                           \
    else if(RB_NODE_RIGHT(elem_1, field) == elem_2) {                           \
        tmp.field = elem_1->field;                                              \
        elem_1->field = elem_2->field;                                          \
        elem_2->field = tmp.field;                                              \
        RB_NODE_FTR(elem_1, field) = elem_2;                                    \
        RB_NODE_RIGHT(elem_2, field) = elem_1;                                  \
    }                                                                           \
    else {                                                                      \
        tmp.field = elem_1->field;                                              \
        elem_1->field = elem_2->field;                                          \
        elem_2->field = tmp.field;                                              \
    }                                                                           \
}                                                                               \
attr struct type *name##_RB_REMOVE(struct name *rbtree, struct type * elem)     \
{                                                                               \
    struct type * succ = NULL;                                                  \
    while (RB_NODE_RIGHT(elem, field) || RB_NODE_LEFT(elem, field)) {           \
        if (RB_NODE_LEFT(elem, field) == NULL) {                                \
            succ = RB_NODE_RIGHT(elem, field);                                  \
        }                                                                       \
        else if(RB_NODE_RIGHT(elem, field) == NULL) {                           \
            succ = RB_NODE_LEFT(elem, field);                                   \
        }                                                                       \
        else {                                                                  \
            succ = name##_RB_SUCC(elem);                                        \
        }                                                                       \
        name##swap_elem(elem, succ);                                            \
        if (elem == RB_TREE_ROOT(rbtree)) {                                     \
            RB_TREE_ROOT(rbtree) = succ;                                        \
        }                                                                       \
    }                                                                           \
    --RB_TREE_SIZE(rbtree);                                                     \
    if (RB_NODE_COLOR(elem, field) == BLACK) {                                  \
        name##_RB_SolveLostBlack(rbtree, elem);                                 \
    }                                                                           \
    if (RB_NODE_FTR(elem, field)) {                                             \
        if (RB_NODE_LEFT(RB_NODE_FTR(elem, field), field) == elem) {            \
            RB_NODE_LEFT(RB_NODE_FTR(elem, field), field) = NULL;               \
        }                                                                       \
        else {                                                                  \
            RB_NODE_RIGHT(RB_NODE_FTR(elem, field), field) = NULL;              \
        }                                                                       \
    }                                                                           \
    if (RB_TREE_ROOT(rbtree) == elem) {                                         \
        /*printf("--%s---remove root\n");*/                                     \
        RB_TREE_ROOT(rbtree) = NULL;                                            \
    }                                                                           \
    return elem;                                                                \
}                                                                               \
attr struct type *name##_RB_REMOVE_FROM_KEY(struct name *rbtree, key_type key)  \
{                                                                               \
    struct type *ptn = name##_RB_FIND_FROM_KEY(rbtree, key);                    \
    if (!ptn) {                                                                 \
      return NULL;                                                              \
    }                                                                           \
    return name##_RB_REMOVE(rbtree, ptn);                                       \
}                                                                               \
                                                                                \
attr struct type *name##_RB_NODE_MIN(struct name * rbtree)                      \
{                                                                               \
    struct type *ptn = RB_TREE_ROOT(rbtree);                                    \
    if (!ptn) return NULL;                                                      \
    while (RB_NODE_LEFT(ptn, field)) {                                          \
        ptn = RB_NODE_LEFT(ptn, field);                                         \
    }                                                                           \
    return ptn;                                                                 \
}                                                                               \
attr struct type *name##_RB_NODE_MAX(struct name * rbtree)                      \
{                                                                               \
    struct type *ptn = RB_TREE_ROOT(rbtree);                                    \
    if (!ptn) return NULL;                                                      \
    while (RB_NODE_RIGHT(ptn, field)) {                                         \
        ptn = RB_NODE_RIGHT(ptn, field);                                        \
    }                                                                           \
    return ptn;                                                                 \
}                                                                               \
                                                                                \
attr void name##_RB_TREE_FREE_DFS(struct name * rbtree, struct type * elem)     \
{                                                                               \
    if(elem==NULL) return;                                                      \
    name##_RB_TREE_FREE_DFS(rbtree, RB_NODE_LEFT(elem, field));                 \
    name##_RB_TREE_FREE_DFS(rbtree, RB_NODE_RIGHT(elem, field));                \
    funcfree(elem);                                                             \
    --RB_TREE_SIZE(rbtree);                                                     \
}



/***
 * @brief: 插入节点
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] node      为红黑树节点全部类型对应 指针变量名
 *      插入RB_INSERT时的 红黑树节点node需要创建初始化初始化好。
 * return 0 失败，return 1成功
***/
#define RB_INSERT(name, var_name, node) name##_RB_INSERT(var_name, node)


/***
 * @brief: 删除节点，调用这个接口前，用户需要调用RB_FIND或RB_REMOVE_FROM_KEY 查找在树上是否相同的值
    若存在，返回相同的值在红黑树树的节点指针，然后把节点指针传入RB_REMOVE的第三个参数进行删除。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] node      为红黑树节点全部类型对应指针变量名 -> 这个节点是已经确定在红黑树上节点的。
 * return 删除的节点指针，用户用于释放节点资源。删除失败 返回 NULL。
***/
#define RB_REMOVE(name, var_name, node) name##_RB_REMOVE(var_name, node)

/***
 * @brief: 删除节点，传入key值，在树上查找并删除节点。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] key      为红黑树节点全部类型 中需要对比的key值变量
 * return 删除的节点指针，用户用于释放节点资源。删除失败 返回 NULL。
***/
#define RB_REMOVE_FROM_KEY(name, var_name, key) name##_RB_REMOVE_FROM_KEY(var_name, key)

/***
 * @brief: 在树上查找节点。第三个参数node，不是树上的节点，只用于查找时，进行对比使用，没有其他作用
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] node      为红黑树节点全部类型对应指针变量名 -> 这个不是树上节点(只需填入key就行了，用于红黑树节点全部类型之间的比较)
 * return 查找到红黑树节点全部类型指针; 查找失败，返回NULL;
***/
#define RB_FIND(name, var_name, node) name##_RB_FIND(var_name, node)

/***
 * @brief: 通过key值，在红黑树上查找节点。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] key       为红黑树节点全部类型 中需要对比的key值变量
 * return 返回查找到红黑树节点全部类型指针; 查找失败，返回NULL;
***/
#define RB_FIND_FROM_KEY(name, var_name, node) name##_RB_FIND_FROM_KEY(var_name, node)

/***
 * @brief: 查找红黑树上最小的节点。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * return 返回红黑树最小的节点指针。失败，返回NULL;
***/
#define RB_MIN_NODE(name, var_name) name##_RB_NODE_MIN(var_name)

/***
 * @brief: 查找红黑树上最大的节点。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * return 返回红黑树最大的节点指针。失败，返回NULL;
***/
#define RB_MAX_NODE(name, var_name) name##_RB_NODE_MAX(var_name)

/***
 * @brief: 查找红黑树上第一个大于等于查找的值的节点指针。这里通过node；
        这里的node不是红黑树上节点，和RB_FIND第三个参数一样，只需填充key即可。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] node      为红黑树节点全部类型对应指针变量名 -> 这个不是树上节点(只需填入key就行了，用于红黑树节点全部类型之间的比较)
 * return 返回第一个大于等于查找的值的节点指针。失败，返回NULL;
***/
#define RB_LOWER_BOUND(name, var_name, node)  name##_RB_LOWER_BOUND_NODE(var_name , node)

/***
 * @brief: 通过key值，查找红黑树上第一个大于等于查找的值的节点指针。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] key       为红黑树节点全部类型 中需要对比的key值变量
 * return  返回第一个大于等于查找的值的节点指针。失败，返回NULL;
***/
#define RB_LOWER_BOUND_FROM_KEY(name, var_name, key)  name##_RB_LOWER_BOUND_NODE_FROM_KEY(var_name , key)

/***
 * @brief: 查找红黑树上第一个大于查找的值的节点指针。这里通过node；
        这里的node不是红黑树上节点，和RB_FIND第三个参数一样，只需填充key即可。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] node      为红黑树节点全部类型对应指针变量名 -> 这个不是树上节点(只需填入key就行了，用于红黑树节点全部类型之间的比较)
 * return 返回第一个大于查找的值的节点指针。失败，返回NULL;
***/
#define RB_UPPER_BOUND(name, var_name, node)  name##_RB_UPPER_BOUND_NODE(var_name , node)

/***
 * @brief: 通过key值，查找红黑树上第一个大于查找的值的节点指针。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] key       为红黑树节点全部类型 中需要对比的key值变量
 * return  返回第一个大于查找的值的节点指针。失败，返回NULL;
***/
#define RB_UPPER_BOUND_FROM_KEY(name, var_name, key)  name##_RB_UPPER_BOUND_NODE_FROM_KEY(var_name , key)

/***
 * @brief: 销毁红黑树。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * return  void.
***/
#define RB_TREE_EXIT(name, var_name) do {           \
    name##_RB_TREE_FREE_DFS(var_name, RB_TREE_ROOT(var_name)); \
    RB_TREE_ROOT(var_name) = NULL; } while(0)


/***
 * brief: 从小到大遍历红黑树（也是中序遍历）
 * @param1[in] node      为红黑树节点全部类型 对应 指针变量名
 * @param2[in] name      为红黑树结构体名
 * @param3[in] var_name  红黑树结构体的指针变量名
 *
***/
#define for_each(node, name, var_name)          \
for (node = RB_MIN_NODE(name, var_name);        \
    node != NULL;                               \
    node = name##_RB_RIGHT_NODE(var_name, node))

/***
 * brief: 从大到小遍历红黑树（也是中序遍历）
 * @param1[in] node      为红黑树节点全部类型 对应 指针变量名
 * @param2[in] name      为红黑树结构体名
 * @param3[in] var_name  红黑树结构体 的 指针变量名
 *
***/
#define for_each_reverse(node, name ,var_name)      \
for (node = RB_MAX_NODE(name, var_name);            \
    node != NULL;                                   \
    node = name##_RB_LEFT_NODE(var_name, node))


/** 
 * 因为_RB_RIGHT_NODE函数，有elem元素为NULL判断 所以可以这样调用next = name##_RB_RIGHT_NODE(var_name, node);
 * 若没有elem元素为NULL判断，则需要想法解决，因为node这时候可能为NULL。
 * brief: 从小到大遍历红黑树（也是中序遍历），能避免在for循环中对当前指针节点的操作，带来的影响。
 * @param1[in] node      为红黑树节点全部类型 对应 指针变量名
 * @param2[in] next      为红黑树节点全部类型 对应 指针变量名
 * @param3[in] name      为红黑树结构体名
 * @param4[in] var_name  红黑树结构体 的 指针变量名
**/
#define for_each_safe(node, next, name, var_name)                                       \
for (node = RB_MIN_NODE(name, var_name), next = name##_RB_RIGHT_NODE(var_name, node);   \
    node != NULL;                                   \
    node = next, next = name##_RB_RIGHT_NODE(var_name, node))


#define for_each_reverse_safe(node, next, name ,var_name)      \
for (node = RB_MAX_NODE(name, var_name), next = name##_RB_LEFT_NODE(var_name, node);    \
    node != NULL;                                   \
    node = next, next= name##_RB_LEFT_NODE(var_name, node))



#endif












