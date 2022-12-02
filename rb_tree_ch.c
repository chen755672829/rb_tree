#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rb_tree_ch.h"


#define bro(node) ((node)->fath->left == (node)) ? ((node)->fath->right) : ((node)->fath->left)
#define rb_malloc(type) (malloc(sizeof(type)))
#define rb_free(ff)  free(ff)
#define mod 0x000003ff

/* 在这里 SLB_LB_1_FRONT 和 SLB_LB_1_END 只能有一个打开*/
/* 在失黑修正时，把 LB-1状况 放前面的进行修正*/
#define SLB_LB_1_FRONT  1
/* 在失黑修正时，把 LB-1状况 放后面的进行修正 */
#define SLB_LB_1_END    0

//-2是error  -1是小于 ,  0 是等于 1是大于
static inline int compare_rb_node (rb_tree_ch_t* rbtree, rb_node_ch_t * node, int key_val)
{
    if (node == NULL) return -2;  // 节点为空
    return rbtree->rb_tree_contrast (node->key_val, key_val) ;
}


static inline int compare_int(int a, int b)
{
    if (a == b)
        return 0;
    if (b > a)
        return 1;
    return -1;
}


/*** 
 * @brief:前驱 找到第一个比当前节点小的节点
 * @param1 [in] node ：红黑树当前节点
 * return：第一个比当前节点小的节点; 失败返回NULL
***/
rb_node_ch_t* left_prodromal(rb_node_ch_t * node)
{
    if (NULL == node) return NULL;

    if (node->left) {
        node = node->left;
        while (node->right) {
            node = node->right;
        }
    } else {
        while (node->fath && node->fath->left == node) {
            node = node->fath;
        }
        node = node->fath;
    }

    return node;
}


/*** 
 * @brief:后继 找到第一个比当前节点大的节点
 * @param1 [in] node ：红黑树当前节点
 * return：第一个比当前节点大的节点; 失败返回NULL
***/
rb_node_ch_t* right_Successor(rb_node_ch_t * node)
{
    if (node == NULL) return NULL;

    if (node ->right) {
        node = node->right;
        while (node->left) {
            node = node->left;
        }
    } else {
        while(node->fath && node->fath->right == node) {
            node = node->fath;
        }
        node = node->fath;
    }

    return node;
}


/*** 
 * @brief: 真后继：找到子树中第一个比当前节点大的节点
 * @param1 [in] node ：红黑树当前节点
 * return：子树中第一个比当前节点大的节点; 失败返回NULL
***/
rb_node_ch_t* real_Succ(rb_node_ch_t * node)
{
    rb_node_ch_t * tmp = node->right;

    if (tmp != NULL) {
        while (tmp->left != NULL) {
            tmp = tmp->left;
        }
    }

    return tmp;
}


void dfs_free(rb_node_ch_t * root)
{
    if (root == NULL) {
        return ;
    }

    dfs_free(root->left);
    dfs_free(root->right);
    rb_free(root);
}


/***
 * @brief: 以key:val的方式，进行查找红黑树中的节点。
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式的方式进行查找。
 * return 查找到的红黑树节点地址；失败返回NULL;
***/
rb_node_ch_t * search(rb_tree_ch_t* rbtree, int key_val)
{
    assert(rbtree != NULL);
    rb_node_ch_t * ptn = rbtree->root;
    rbtree->_hol = NULL;
    int return_val;
    while (ptn) {
        rbtree->_hol = ptn;
        return_val = compare_rb_node(rbtree, ptn, key_val);
        switch (return_val) {
            case 0:
                return ptn;
                break;
            case 1:
                ptn = ptn->right;
                break;
            case -1:
                ptn = ptn->left;
                break;
            case -2:
                return NULL;
        }
    }
     return ptn;
}


/***
 * @brief: 在红黑树中查找第一个大于自己的值
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式的方式进行查找.
 * return 返回第一个大于自己的值；失败返回NULL。
***/
rb_node_ch_t *rb_upper_bound(rb_tree_ch_t* rbtree, int key_val)
{
    assert(rbtree != NULL);
    rbtree->_hol = NULL;
    if (rbtree->root == NULL) return NULL;
    rb_node_ch_t * ptr = rbtree->root;
    int return_val;
    while (ptr) {
        rbtree->_hol = ptr;
        return_val = compare_rb_node(rbtree, ptr, key_val);

        // 因为rb_upper_bound函数要找的时第一个大于自己的值
        // 所以我们让_hol尽可能为 等于或小于自己的值（有等于自己的值就找等于自己的值，没有等于自己的值就找小于自己的值），最后再找一下_hot的后继即可
        // case 0,1 就是为了 让_hol 等于或 小于自己的值
        // 若走 case -1，那么_hol这时候，就 大于自己的值
        switch (return_val) { 
            case 0:
            case 1:
                ptr = ptr->right;
                break;
            case -1:
                ptr = ptr->left;
                break;
            case -2:
                return NULL;
        }
    }

    return_val = rbtree->rb_tree_contrast(rbtree->_hol->key_val, key_val);
     // -1 说明 插入的key_val小
     // -1 说明已经找到了第一个比自己大的节点了
    if(return_val == -1) 
       return rbtree->_hol;
    else if(return_val >= 0)
        return right_Successor(rbtree->_hol);
    return NULL;
}


/***
 * @brief: 在红黑树中查找第一个大于等于自己的值
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式的方式进行查找.
 * return 返回第一个大于等于自己的值；失败返回NULL。
***/
rb_node_ch_t *rb_lower_bound(rb_tree_ch_t* rbtree, int key_val)
{
    assert(rbtree != NULL);
    rbtree->_hol = NULL;
    if (rbtree->root == NULL) return NULL;
    rb_node_ch_t * ptr = rbtree->root;
    int return_val;
    while (ptr) {
        rbtree->_hol = ptr;
        return_val = compare_rb_node(rbtree, ptr, key_val);

        // 因为 rb_lower_bound 函数要找的时第一个 等于或大于 自己的值
        // 所以我们要找的 _hol, 为第一个小于自己的值，然后等于的时候，就朝着左走，如下：
        switch (return_val) { 
            case 0:
            case -1:
                ptr = ptr->left;
                break;
            case 1:
                ptr = ptr->right;
                break;
            
            case -2:
                return NULL;
        }
    }

    return_val = rbtree->rb_tree_contrast(rbtree->_hol->key_val, key_val);
    if(return_val == -1 || return_val == 0)
       return rbtree->_hol;
    else if(return_val > 0)
        return right_Successor(rbtree->_hol);
    return NULL;
}


// zig右旋
void zig(rb_node_ch_t * ptr)
{
    if (ptr->fath) {
        if (ptr->fath->left == ptr) {
            ptr->fath->left = ptr->left;
        }
        else {
            ptr->fath->right = ptr->left;
        }
    }
    if (ptr->left->right) {
        ptr->left->right->fath = ptr;
    }

    ptr->left->fath = ptr->fath;
    ptr->fath = ptr->left;
    ptr->left = ptr->left->right;

    ptr->fath->right = ptr;     // ptr->fath 就是以前ptr->left, 运用的已经复制的指针
    //return ptr->fath;
}


// zag左旋
void zag(rb_node_ch_t * ptr)
{
    if (ptr->fath) {
        if (ptr->fath->left == ptr) {
            ptr->fath->left = ptr->right;
        }
        else {
            ptr->fath->right = ptr->right;
        }
    }

    if(ptr->right->left) {
        ptr->right->left->fath = ptr;
    }
    ptr->right->fath = ptr->fath;
    ptr->fath = ptr->right;
    ptr->right = ptr->right->left;
    ptr->fath->left =ptr;
    //return ptr->fath;
}


rb_node_ch_t *  new_Node(int v, rb_node_color_t RBc)
{
    rb_node_ch_t * new = malloc(sizeof(rb_node_ch_t));
    new->fath = new->left = new->right = NULL;
    new->color = RBc;
    new->key_val = v;
}


/***
 * 重点：
 * ① 旋转过程中需要维护根节点
 * ② 判断叔叔为黑色的有两种情况
 * ③ while循环的判断。
 * @brief: 因为除一个节点外，插入的节点颜色都会红色，根据红黑树性质，当插入节点的父亲也为红色时，这时候就是双红现象，要使用双红修正进行解决。
 * @param1 [in] rbtree 红黑树指针。
 * @param2 [in] ptr 刚插入的节点。
***/
void solve_double_red(rb_tree_ch_t* rbtree, rb_node_ch_t *ptr)
{
    /***
     * 为什么不用判断 祖父不为空呢 ?
     * 因为父亲节点为红色，所以父亲一定不是根节点（根节点颜色为黑色），所以不需要判断祖父为空
    ***/
    while (ptr->fath != NULL && ptr->fath->color == RED) {
        rb_node_ch_t * pftr = ptr->fath;
        rb_node_ch_t * grdftr = ptr->fath->fath;
        rb_node_ch_t * uncle = bro(pftr);

        /**
        * 插入写了错，叔叔为黑色 有两种情况，
        * 第一种：叔叔为空。
        * 第二种：因为有递归的情况，所以存在 叔叔不为空且为黑色的情况（这一种不要遗漏，这一种，让我调试一天）
        **/
        if (uncle == NULL || uncle->color == BLACK) {     // RR-1 叔叔为黑色
            if (grdftr ->left == pftr ) {
                if (pftr->left == ptr ) {
                    if (grdftr == rbtree->root) {
                        rbtree->root = pftr;
                    }
                    zig(grdftr);
                    grdftr->color = RED;
                    pftr->color = BLACK;
               }
               else {
                    if (grdftr == rbtree->root) {
                        rbtree->root = ptr;
                    }
                    zag(pftr); zig(grdftr);
                    ptr->color = BLACK;
                    grdftr->color = RED;
                }
            }
            else {
                if (pftr->right == ptr) {
                    if (grdftr == rbtree->root) {
                        rbtree->root = pftr;
                    }
                    zag(grdftr);
                    grdftr->color = RED;
                    pftr->color = BLACK;
                }
                else {
                    if (grdftr == rbtree->root) {
                        rbtree->root = ptr;
                    }
                    zig(pftr); zag(grdftr);
                    ptr->color = BLACK;
                    grdftr->color = RED;
                }
            }
            return ;
        }

        else {          // RR-2 叔叔为红色。
            pftr->color = BLACK;
            uncle ->color = BLACK;
            grdftr->color = RED;
            ptr = grdftr;
        }
    }

    if (ptr == rbtree->root) {
        if (ptr->color == RED) {
            ptr->color = BLACK;
        }
    }
#if 0
    /*若跳出循环后，ptr->fath不存在 或者 ptr->fath颜色不会红，所以根本就存在下面else if这种情况*/
    else if (ptr->fath == rbtree->root && ptr->fath->color == RED) {
        ptr->fath->color = BLACK;
    }
#endif
    return ;
}


/***
 * @brief: 以key:val的方式，向红黑树中插入一个节点
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式进行插入，只是这里的key_val是一个数值，不是结构体。
 * return false 失败， ture 成功
***/
bool insert(rb_tree_ch_t* rbtree, int key_val)
{
    rb_node_ch_t *ptn = search(rbtree, key_val);
    if (ptn) {
        return False;
    }

    if (rbtree->_hol == NULL) {
        rbtree->root = new_Node(key_val, BLACK);
        rbtree->_size  = 1;
        return Ture;
    }
    ++(rbtree->_size);

    ptn = new_Node(key_val, RED);
    int return_val = compare_rb_node(rbtree, rbtree->_hol, key_val);
    if (return_val > 0) {
        rbtree->_hol->right = ptn;
    }
    else if(return_val == -1) {
        rbtree->_hol->left = ptn;
    }
    else {
        return -1;
    }
    ptn->fath = rbtree->_hol;
    // 插入时，忘记调用，双红修正了
    solve_double_red(rbtree, ptn);
}


/***
 * 删除节点的注意事项：
 * ① 在旋转这个过程中需要维护根节点，
 * ② LB-3 和 LB-2R的染色的过程中，不是要让两个链的黑高度相等，而是删除后节点后，让两个链的黑高度与所有链的黑高度都相等，也就是两条链经旋转调整后，和没有删除节点前的黑高度一样
 * ③ LB-1的染色过程，也要注意，里面有解释
 * ④ 若把LB-1放在下面，一定要注意 LB-2B ptr指针上移一层后，兄弟节点和父亲节点维护，同时ptr等于根节点后（已经修正完毕了），就直接return跳出就可以了，不然直接就进入下面的LB-1里面了
***/
static void solve_lost_black(rb_tree_ch_t* rbtree, rb_node_ch_t *ptr)
{
    while (ptr != rbtree->root) {
        rb_node_ch_t *bro_node = bro(ptr);
        rb_node_ch_t *pftr = ptr->fath;
        // 因为while循环判断ptr不为根节点，ptr本身为黑色节点，根据红黑树性质，兄弟节点一定不为空
        // 所以在这里不需要判断兄弟节点是否为空了。
        
#if SLB_LB_1_FRONT
        // LB-1（父亲为黑色，兄弟为红色） 也就意味着--> 兄弟一定有黑色的侄子，满足不了黑高度相等 和 不能双红现象
        // LB-1 经转化后 LB-3 或 LB-2R的情况，然后再继续修正。
        // 若兄弟为红色，那么父亲就一定为黑色。所以只判断兄弟为红色即可
        if (bro_node->color == RED) {
            // 若想节点 LB-1的情况，不管左旋还是右旋，主干道，黑高度都会-1，因为要把兄弟旋上去，父亲旋下来，兄弟为红色，父亲为黑色，兄弟作为主干道上的节点。
            // 所以需要重染色
            bro_node->color = BLACK;
            pftr->color = RED;
            // 维护根节点
            if (pftr == rbtree->root) {
                rbtree->root = bro_node;
            }
            if (pftr->left == ptr) {
                zag(pftr);
            }
            else {
                zig(pftr);
            }
            // 因为LB-1 会转化为下面的LB-3或 LB-2R的情况继续修正，所以这里需要重新更新兄弟节点和父亲节点
            // 因为 ptr 执行的内存一直没有变化，所以不用判断ptr是否为根节点，兄弟节点也一定不为空。
            bro_node = bro(ptr);
            pftr = ptr->fath;
        }
#endif

        // LB-3 存在红色侄子     ，左红侄子
        if (bro_node->left && bro_node->left->color == RED) {
            rb_node_ch_t *bro_son = bro_node->left;
            if (pftr == NULL) {
                printf("pftr111==NULL\n");
            }
            rb_node_color_t old_color = pftr->color;
            pftr->color = BLACK;
           if (pftr->left == bro_node) {
                if(pftr == rbtree->root) {
                    rbtree->root = bro_node;
                }
                zig(pftr);
                bro_son->color = BLACK;
           }
           else {
                if (pftr == rbtree->root) {
                     rbtree->root = bro_son;
                }
                zig(bro_node);
                zag(pftr);
           }
           pftr->fath->color = old_color;
           return ;

        }
        // LB-3 存在红色侄子     ，右红侄子
        else if (bro_node->right && bro_node->right->color == RED) {
            rb_node_ch_t *bro_son = bro_node->right;

            rb_node_color_t old_color = pftr->color;
            pftr->color = BLACK;
            if (pftr->right == bro_node) {
                if(pftr == rbtree->root) {
                    rbtree->root = bro_node;
                }
                zag(pftr);
                bro_son->color = BLACK;
            }
            else {
                if (pftr == rbtree->root) {
                     rbtree->root = bro_son;
                }
                zag(bro_node);
                zig(pftr);
            }
            // 旋转后的根节点
            pftr->fath->color = old_color;
            return ;
        }
#if 0
        // 写代码时，这里出现了错误
        // 因为有递归的情况，所以使用 兄弟没有儿子 去推断出 没有红色侄子和兄弟为黑色的情况。
        // 在没有递归的情况下，没有红色侄子，兄弟为黑色             <-->   可以推断出 兄弟没有儿子 (因删除的节点为黑色，兄弟又是黑色，那么又侄子，一定为红色，不然就不满足，每条链上黑高度相等这一条)
        if (!bro_node->left && !bro_node->right) {
#endif
            // LB-2R 没有红色侄子，且父亲为红色，兄弟为黑色
            // 若把LB-3放上面，这里就不用判断没有红色侄子的问题了,
            // 若把LB-1 放前面，同时也不用判断 兄弟节点等于为黑色了
#if SLB_LB_1_END
            if (pftr->color == RED && bro_node->color == BLACK) 
#else
            if (pftr->color == RED) 
#endif
            {
                pftr->color = BLACK;
                bro_node->color = RED;
                return ;
            }
            // LB-2B （没有红色侄子，且父亲为黑色，兄弟为黑色）
            // 染红 兄弟节点， 递归父亲，全树黑高度-1
#if SLB_LB_1_END
            else if (pftr->color == BLACK && bro_node->color == BLACK) 
#else
            // LB-1情况 若放前面进行修正
            if (pftr->color == BLACK) 
#endif
            {
                bro_node->color = RED;
                ptr = pftr;
#if SLB_LB_1_END
                /*写代码时这里出现了错误*/
                /*这里若想 重新赋值兄弟和父亲，需要判断，因为上移一层，执行的内存发生了变化，所以需要重新判断ptr是否为空*/
                if(ptr != rbtree->root) {
                    bro_node = bro(ptr);
                    pftr = ptr->fath;
                }
                // 如果是根节点了，就直接跳出就行了，若不跳出，若LB-1放在下面，就直接进入了LB-1里面，从而导致出差
                else return;
#endif
            }

#if SLB_LB_1_END
        // LB-1（父亲为黑色，兄弟为红色） 也就意味着--> 兄弟一定有黑色的侄子，满足不了黑高度相等 和 不能双红现象
        // LB-1 经转化后 LB-3 或 LB-2R的情况，然后再继续修正。
        // 若兄弟为红色，那么父亲就一定为黑色。所以只判断兄弟为红色即可
        if (bro_node->color == RED) {
            // 若想节点 LB-1的情况，不管左旋还是右旋，主干道，黑高度都会-1，因为要把兄弟旋上去，父亲旋下来，兄弟为红色，父亲为黑色，兄弟作为主干道上的节点。
            // 所以需要重染色
            bro_node->color = BLACK;
            pftr->color = RED;
            // 维护根节点
            if (pftr == rbtree->root) {
                rbtree->root = bro_node;
            }
            if (pftr->left == ptr) {
                zag(pftr);
            }
            else {
                zig(pftr);
            }
        }
#endif

#if 0
        }
#endif

    }
}


// 删除节点找的就是其真后继，若不存在真后继，就找小于自己的第一个元素（在自己下面）， 一直找到元素和 真后继为是叶子节点（无左右子树）
bool remove_node(rb_tree_ch_t* rbtree, rb_node_ch_t *ptr)
{
    rb_node_ch_t *succ_ptr = NULL;  // 真后继
    while (ptr->right || ptr->left) {
        //若要删除的节点，只有一个儿子，根据红黑树的性质6(黑高度相同)和性质5(不存在双红现象)，那么这个儿子是一个红色节点，且只是单个的节点
        if (ptr->left == NULL) {
            succ_ptr = ptr->right;
        }
        else if (ptr->right == NULL) {
            succ_ptr = ptr->left;
        }
        else {
            succ_ptr = real_Succ(ptr);
        }
        // 只更新节点值
        ptr->key_val = succ_ptr->key_val;
        ptr = succ_ptr;
    }
    //printf("size==%d\n",rbtree->_size);
    --(rbtree->_size);

    if (ptr->color == BLACK) {
        solve_lost_black(rbtree, ptr);
    }

    if (ptr->fath) {
        if (ptr->fath->left == ptr) {
            ptr->fath->left = NULL;
        }
        else {
            // 这块写错了
            ptr->fath->right = NULL;
        }
    }
    
    if (rbtree->root == ptr) {
        rbtree->root = NULL;
    }
    //rb_free(ptr->key_val);
    rb_free(ptr);
    return Ture;
}


bool remove_key(rb_tree_ch_t* rbtree, int k)
{
    rb_node_ch_t* ptr = search(rbtree, k);
    if (!ptr)
        return False;
    remove_node(rbtree, ptr);
}


uint32 get_rb_size(rb_tree_ch_t* tree)
{
    return tree->_size;
}


int32 empty(rb_tree_ch_t* tree)
{
    return !tree->_size;
}


void rb_tree_init(rb_tree_ch_t** tree, register_contrast_callback callback)
{
    *tree =  malloc(sizeof(rb_tree_ch_t));
    (*tree)->root = (*tree)->_hol = NULL;
    (*tree)->_size = 0;
    (*tree)->rb_tree_contrast = callback;
}


void rb_tree_exit(rb_tree_ch_t* tree)
{
    dfs_free(tree->root);
    rb_free(tree);
}


rb_node_ch_t * min_node(rb_tree_ch_t *tree)
{
    rb_node_ch_t* min_node = tree->root;
    if (!min_node) {
        return NULL;
    }
    while (min_node->left) {
        min_node = min_node->left;
    }
    return min_node;
}


int main() 
{
    int i;
    rb_tree_ch_t *tree;
    rb_tree_init(&tree, compare_int);
    srand((unsigned)time(NULL));
    //printf("mod==%d\n", mod);
#if 1
    for (i = 0; i < 1024 ;i++) {
        int tmp = rand() & mod;
        //printf("tmp==%d\n", tmp);
        if (insert(tree, tmp) == False) {
            //printf("tmp_false==%d\n",tmp);
        }
    }
#endif

    rb_node_ch_t *node;
    //printf("insert  -----------begin-------------%d\n", remove_key(tree, 50));
#if 1
    for (node = min_node(tree); node != NULL ; node = right_Successor(node)) {
        printf("%d\n",node->key_val);
    }
#endif

#if 0
    printf("remove_key  -----------begin-------------%d\n", remove_key(tree, 50));
    
    printf("remove_key  -----------begin_1-------------%d \n", remove_key(tree, 51));
    remove_key(tree, 78);
    remove_key(tree, 100);
    remove_key(tree, 1);
    remove_key(tree, 2);
    remove_key(tree, 3);
    remove_key(tree, 4);
    remove_key(tree, 11);
    remove_key(tree, 12);
    remove_key(tree, 13);
    remove_key(tree, 14);
    remove_key(tree, 22);
    remove_key(tree, 33);
    remove_key(tree, 44);
    
    printf("remove_key  -----------end-------------\n");
    for (node = min_node(tree); node != NULL ; node = right_Successor(node)) {
        printf("%d\n",*(node->key_val));
    }
#else
    for (i = 0; i < 500 ;i++) {
        printf("i==%d\n",i);
        remove_key(tree, i);
    }
#endif
    rb_node_ch_t *lower = rb_lower_bound(tree, 100000);
    rb_node_ch_t *upper = rb_upper_bound(tree, 100000);
    if(lower==NULL) {
        printf("lower==NULL\n");
    }
    if(upper == NULL) {
        printf("upper==NULL\n");
    }
    else printf("lower==%d upper==%d\n", lower->key_val, upper->key_val);

    for(i = 500; i < 550; i++) {
        if (search(tree, i)) {
          printf("i==%d lower==%d upper==%d\n", i, rb_lower_bound(tree, i)->key_val, 
                    rb_upper_bound(tree, i)->key_val);
        }
    }

    return 0;
}

