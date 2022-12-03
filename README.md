# rb_tree 红黑树
## 一、文件介绍：  
  1、rb_tree_ch.c和rb_tree_ch.h文件是为了讲解红黑树的实现的  
  详解文档：https://note.youdao.com/s/JVExYtzD  
  2、rbtree.h 文件是通过c语言的宏定义实现了c++中模板的功能。test_rbtree.c文件是为了测试 rbtree.h实现的红黑树。  
  红黑树具体怎么实现的请看上面的文档链接。若看宏定义红黑树的接口怎么使用，请看文档：https://note.youdao.com/s/5RcvOjKJ  
## 二、通过图片简单介绍一下红黑树
###  1、找节点前驱  
  ![image](https://user-images.githubusercontent.com/35031390/205441617-af447318-6dd1-4c92-b780-3b4a74b0dade.png)
###  2、找节点后继
  ![image](https://user-images.githubusercontent.com/35031390/205441650-33dccbbd-11f9-4203-a812-b127fb62b7a9.png)
###  3、zig右旋
  ![image](https://user-images.githubusercontent.com/35031390/205441690-9739038e-6c27-4fec-bf73-e624ad0c5fb1.png)
### 4、zag左旋
  ![image](https://user-images.githubusercontent.com/35031390/205441717-fae6a81a-9c2e-45c0-95b5-5b1e595a5af0.png)
### 5、双红修正和失黑修正
  文档中都有详细介绍：https://note.youdao.com/s/JVExYtzD  
## 三、宏定义使用红黑树的结构与结构介绍
### 专有名词
  1、使用接口前先弄懂一些专有名词的介绍  
  ![image](https://user-images.githubusercontent.com/35031390/205441863-22e6fc15-6548-4c4d-9101-50390a64ba3b.png)  
  2、红黑树结构与创建红黑树  
  ![image](https://user-images.githubusercontent.com/35031390/205441903-6599f0c6-0ca8-4870-b987-bc0acdc01a13.png)
  3、红黑树节点全部类型解析  
  ![image](https://user-images.githubusercontent.com/35031390/205441969-24f91e24-e4d2-424d-8ac8-be2126b6b6d0.png)
  4、红黑树结构图  
  ![image](https://user-images.githubusercontent.com/35031390/205442008-0980c96c-bca2-46a9-a555-f82a5e71cf79.png)
### 接口使用
####  1、红黑树节点全部类型，记得定义红黑树节点字段  
```
  typedef struct node {
    RE_NODE(node) rbnode;
    int key;
  }Node_t;
```
  RE_NODE进行定义红黑树节点字段
```
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
```
####  2、定义与创建一个红黑树  
```
RB_TREE(rbtree, node ,) ttrbtree, *ptr;
```
  RB_TREE对应的接口  
```
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
```
####  3、声明红黑树的函数  
```
RB_TREE_FUNC_DECLARE(rbtree, node , rbnode, cmp, static inline, int, key_cmp, rbfree);
```
  其中的参数：  
```
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
#define RB_TREE_FUNC_DECLARE(name, type, field, cmp, attr, key_type, elem_key_cmp, funcfree)
```
####  4、定义红黑树函数
  参数于声明一样
  #define 定义红黑树下面，有很多代码，这里就不解释了
```
#define RB_TREE_FUNC_DEFINITION(name, type, field, cmp, attr, key_type, elem_key_cmp, funcfree)
```
  调用定义红黑树函数：  
```
RB_TREE_FUNC_DEFINITION(rbtree, node , rbnode, cmp, static inline, int, key_cmp, rbfree)
```
####  5、红黑树接口函数  
##### （1）初始化红黑树
```
/***
 * brief: 初始化红黑树。
 * @param1[in] rbtree：对应红黑树结构的指针变量。
 
***/
#define RE_TREE_INIT(rbtree)  \
    rbtree->_root = rbtree->_hol = NULL;\
    rbtree->_size = 0
```
##### （2）初始化红黑树节点
```
/***
 * brief: 初始化红黑树节点。
 * @param1[in] elem :红黑树节点全部对应的指针变量名
 * @param2[in] field :红黑树节点全部中红黑树节点字段名。
***/
#define RB_NODE_INIT(elem, field)  \
    (elem)->field.left = (elem)->field.right = (elem)->field.ftr = NULL;
```
##### （3）插入节点
```
/***
 * @brief: 插入节点
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * @param3[in] node      为红黑树节点全部类型对应 指针变量名
 *      插入RB_INSERT时的 红黑树节点node需要创建初始化初始化好。
 * return 0 失败，return 1成功
***/
#define RB_INSERT(name, var_name, node) name##_RB_INSERT(var_name, node)
```
##### （4）删除节点
  删除节点有两个接口：  
  **RB_REMOVE** 通过红黑树节点全部类型指针变量（这个节点确定在红黑树上），进行删除；调用这个接口前，用户需要调用RB_FIND或RB_REMOVE_FROM_KEY 查找在树上是否相同的值，若存在，返回相同的值在红黑树树的节点指针，然后把节点指针传入RB_REMOVE的第三个参数进行删除。  
  **RB_REMOVE_FROM_KEY**：删除节点，传入key值，在树上查找并删除节点。  
```
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
```
##### （5）查找节点
  查找节点有两个接口：  
  **RB_FIND**：在树上查找节点。第三个参数node，不是树上的节点，只用于查找时，进行对比使用，没有其他作用  
  **RB_FIND_FROM_KEY**：通过key值，在红黑树上查找节点。  
```
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
```
##### （6）查找红黑树上最小的节点
```
/***
 * @brief: 查找红黑树上最小的节点。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * return 返回红黑树最小的节点指针。失败，返回NULL;
***/
#define RB_MIN_NODE(name, var_name) name##_RB_NODE_MIN(var_name)
```
##### （7）查找红黑树上最大的节点
```
/***
 * @brief: 查找红黑树上最大的节点。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * return 返回红黑树最大的节点指针。失败，返回NULL;
***/
#define RB_MAX_NODE(name, var_name) name##_RB_NODE_MAX(var_name)
```
##### （8）lower_bound 查找第一个大于等于查找的值的节点指针
  两个接口：  
  **RB_LOWER_BOUND**：通过node值进行查找。这里的node不是红黑树上节点，只用对比，和RB_FIND第三个参数一样，只需填充key即可。  
  **RB_LOWER_BOUND_FROM_KEY**：通过key值，查找红黑树上第一个大于等于查找的值的节点指针。  
```
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
```
##### （9）upper_bound 查找第一个大于查找的值的节点指针
  两个接口：  
  **RB_UPPER_BOUND**：通过node值进行查找。这里的node不是红黑树上节点，只用对比，和RB_FIND第三个参数一样，只需填充key即可。  
  **RB_UPPER_BOUND_FROM_KEY**：通过key值，查找红黑树上第一个大于查找的值的节点指针  
```
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
```
##### （10）销毁红黑树
```
/***
 * @brief: 销毁红黑树。
 * @param1[in] name      红黑树结构体名
 * @param2[in] var_name  红黑树结构体的指针变量名
 * return  void.
***/
#define RB_TREE_EXIT(name, var_name) do {           \
    name##_RB_TREE_FREE_DFS(var_name, RB_TREE_ROOT(var_name)); \
    RB_TREE_ROOT(var_name) = NULL; } while(0)
```
##### （11）遍历红黑树
```
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
```
  
