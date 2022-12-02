
#ifndef __RB_TREE_CH__
#define __RB_TREE_CH__

typedef int int32;
typedef unsigned int uint32;

typedef enum Bool
{
    False = 0,
    Ture = 1,
} bool;

// 这里对比函数，红黑树节点键值先固定为int，这里比较也是使用的int类型进行比较的
// 这里其实可以使用 void* 表示红黑树节点中的键值类型，这样键值类型就可以改变。
// 但是这样也有一个问题，那就是红黑树节点中的键值只能是堆内存了，维护起来麻烦。
// 其实c++中的模块，不是使用void*指针进行实现的，我觉着应该是使用宏定义实现的
// 使用的宏定义实现红黑树，请看我的rbtree.h文件代码。
typedef int (*register_contrast_callback)(int , int );

typedef enum rb_node_color
{
    RED = 0,
    BLACK = 1,
} rb_node_color_t;


// 在这里key值就固定为int类型
typedef struct rb_node_ch {
    rb_node_color_t color;     //颜色
    int key_val;               //key值
    struct rb_node_ch *fath;   //父亲节点
    struct rb_node_ch *left;   //左儿子
    struct rb_node_ch *right;  //右儿子
} rb_node_ch_t;


typedef struct rb_tree_ch {
    rb_node_ch_t * root;   // 红黑树的根节点
    //_hol字段变量，在查找的几个函数都有用来记录当前节点的父亲节点。如search,lower_bound,upper_bound
    rb_node_ch_t * _hol;
    int _size;          //红黑树中的节点数

    // -1 是 error ,  0 是小于 1是 大于等于
    /* 当前回调函数，比较红黑树节点中的key值 */
    register_contrast_callback rb_tree_contrast ;
} rb_tree_ch_t;


/***
 * @brief： 创建并初始化一颗红黑树
 * @param1 [out] rb_tree_ch_t** tree  双重指针，为获取创建的红黑树地址。
 * @param2 [in]  callback   红黑树节点比较函数
***/
void rb_tree_init (rb_tree_ch_t** tree, register_contrast_callback callback);


/***
 * @brief: 销毁一个红黑树
 * @param1 [in] tree 红黑树指针
***/
void rb_tree_exit(rb_tree_ch_t* tree);


/***
 * @brief: 以key:val的方式，向红黑树中插入一个节点
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式进行插入，只是这里的key_val是一个数值，不是结构体。
 * return false 失败， ture 成功
***/
bool insert(rb_tree_ch_t* rbtree, int key_val);


/***
 * @brief: 以key:val的方式，进行查找红黑树中的节点。
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式的方式进行查找。
 * return 查找到的红黑树节点地址；失败返回NULL;
***/
rb_node_ch_t * search(rb_tree_ch_t* rbtree, int key_val);


/***
 * @brief: 在红黑树中查找第一个大于自己的值
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式的方式进行查找.
 * return 返回第一个大于自己的值；失败返回NULL。
***/
rb_node_ch_t *rb_upper_bound(rb_tree_ch_t* rbtree, int key_val) ;


/***
 * @brief: 在红黑树中查找第一个大于等于自己的值
 * @param1 [in] rbtree 红黑树指针
 * @param2 [in] key_val  以key:val的方式的方式进行查找.
 * return 返回第一个大于等于自己的值；失败返回NULL。
***/
rb_node_ch_t *rb_lower_bound(rb_tree_ch_t* rbtree, int key_val);


#endif



