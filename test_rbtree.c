
#include <stdio.h>
#include <time.h>

#include "rbtree.h"


#define LEN  1024

typedef struct node {
    RE_NODE(node) rbnode;
    int key;
}Node_t;

static inline int cmp(struct node *k1 ,struct node *k2)
{
    if (k1->key < k2->key)
       return 1;
    else if (k1->key == k2->key)
       return 0;
    return -1;
}

static inline int key_cmp(struct node *kk, int key){
    if (kk->key < key)
       return 1;
    else if (kk->key == key)
       return 0;
    return -1;
}

int FreeNum = 0;
int MallocNum = 0;
static inline void rbfree(struct node * kk)
{
    free(kk);
    FreeNum ++;
}

RB_TREE(rbtree, node ,) ttrbtree, *ptr;

RB_TREE_FUNC_DECLARE(rbtree, node , rbnode, cmp, static inline, int, key_cmp, rbfree);

RB_TREE_FUNC_DEFINITION(rbtree, node , rbnode, cmp, static inline, int, key_cmp, rbfree)


// 当时调试RB_SolveLostBlack失黑修正的代码。
#if 0 
void RB_SolveLostBlack(struct rbtree *rbtree, struct node* elem)
{                                                                             
    while (elem != RB_TREE_ROOT(rbtree)) {                                    
        struct node *bro_node = RB_NODE_BRO(elem, rbnode);                     
        struct node *pftr = RB_NODE_FTR(elem, rbnode);                         
        if(bro_node== NULL) printf("bro_node == NULL\n");
        if (RB_NODE_COLOR(bro_node, rbnode) == RED) {    /*LB-1*/              
            RB_NODE_COLOR(bro_node, rbnode) = BLACK;                           
            RB_NODE_COLOR(pftr, rbnode) = RED;                                 
            if (pftr == RB_TREE_ROOT(rbtree)) {                               
                RB_TREE_ROOT(rbtree) = bro_node;                              
            }                                                                 
            if (RB_NODE_LEFT(pftr, rbnode) == elem) {                          
                zag(pftr, rbnode);                                             
            }                                                                 
            else {                                                            
                zig(pftr, rbnode);                                             
            }                                                                 
            bro_node = RB_NODE_BRO(elem, rbnode);                              
            pftr = RB_NODE_FTR(elem, rbnode);                                  
        }                                                                     
        /* LB-3 */                                                            
        if(RB_NODE_LEFT(bro_node, rbnode) &&                                
               RB_NODE_COLOR(RB_NODE_LEFT(bro_node, rbnode), rbnode) == RED) { 
            struct node * bro_son = RB_NODE_LEFT(bro_node, rbnode);            
            RbNodeColor_t old_color = RB_NODE_COLOR(pftr, rbnode);             
            RB_NODE_COLOR(pftr, rbnode) = BLACK;                               
            if (RB_NODE_LEFT(pftr, rbnode) == bro_node) {                      
                if (pftr == RB_TREE_ROOT(rbtree)) {                           
                    RB_TREE_ROOT(rbtree) = bro_node;                          
                }                                                             
                zig(pftr, rbnode);                                             
                RB_NODE_COLOR(bro_son, rbnode) = BLACK;                        
            }                                                                 
            else {                                                            
                if (pftr == RB_TREE_ROOT(rbtree)) {                           
                    RB_TREE_ROOT(rbtree) = bro_son;                           
                }                                                             
                zig(bro_node, rbnode);                                         
                zag(pftr, rbnode);                                             
            }                                                                 
            RB_NODE_COLOR(RB_NODE_FTR(pftr, rbnode), rbnode) = old_color;       
            return;                                                           
       }                                                                      
        else if (RB_NODE_RIGHT(bro_node, rbnode) &&                           
               RB_NODE_COLOR(RB_NODE_RIGHT(bro_node, rbnode), rbnode) == RED) {
            struct node * bro_son = RB_NODE_RIGHT(bro_node, rbnode);           
            RbNodeColor_t old_color = RB_NODE_COLOR(pftr, rbnode);             
            RB_NODE_COLOR(pftr, rbnode) = BLACK;                               
            if (RB_NODE_RIGHT(pftr, rbnode) == bro_node) {                     
                if (pftr == RB_TREE_ROOT(rbtree)) {                           
                    RB_TREE_ROOT(rbtree) = bro_node;                          
                }                                                             
                zag(pftr, rbnode);                                             
                RB_NODE_COLOR(bro_son, rbnode) = BLACK;                        
            }                                                                 
            else {                                                            
                if (pftr == RB_TREE_ROOT(rbtree)) {                           
                    RB_TREE_ROOT(rbtree) = bro_son;                           
                }                                                             
                zag(bro_node, rbnode);                                         
                zig(pftr, rbnode);                                             
            }                                                                 
            RB_NODE_COLOR(RB_NODE_FTR(pftr, rbnode), rbnode) = old_color;       
            return;                                                           
        }                                                                     
        if (RB_NODE_COLOR(pftr, rbnode) == RED) {    /* LB-2R */               
            RB_NODE_COLOR(pftr, rbnode) = BLACK;                               
            RB_NODE_COLOR(bro_node, rbnode) = RED;                             
            return;                                                           
        }                                                                     
        else {  /*LB-2B */                                                    
            RB_NODE_COLOR(bro_node, rbnode) = RED;                             
            elem = pftr;                                                      
        }                                                                     
    }                                                                         
}

int RB_REMOVE(struct rbtree *rbtree, struct node * elem)     
{                                                                      
    struct node * succ = NULL;                                         
    struct node tmp;                                                   
    elem = rbtree_RB_FIND(rbtree, elem);                               
    if (!elem) {                                                       
        return 0;                                                      
    }                                                                  
    while (RB_NODE_RIGHT(elem, rbnode) || RB_NODE_LEFT(elem, rbnode)) { 
        if (RB_NODE_LEFT(elem, rbnode) == NULL) {                      
            succ = RB_NODE_RIGHT(elem, rbnode);                         
        }                                                              
        else if(RB_NODE_RIGHT(elem, rbnode) == NULL) {                 
            succ = RB_NODE_LEFT(elem, rbnode);                          
        }                                                              
        else {                                                         
            succ = rbtree_RB_SUCC(elem);                               
        }                                                              
        elem->key = succ->key;                                      
        elem = succ;                                                
    }                                                                  
    --RB_TREE_SIZE(rbtree);                                            
    if (RB_NODE_COLOR(elem, rbnode) == BLACK) {                       
        RB_SolveLostBlack(rbtree, elem);                        
    }                                                                  
    if (RB_NODE_FTR(elem, rbnode)) {                                  
        if (RB_NODE_LEFT(RB_NODE_FTR(elem, rbnode), rbnode) == elem) {  
            RB_NODE_LEFT(RB_NODE_FTR(elem, rbnode), rbnode) = NULL;     
        }                                                              
        else {                                                         
            RB_NODE_RIGHT(RB_NODE_FTR(elem, rbnode), rbnode) = NULL;    
        }                                                              
    }                                                                  
    if (RB_TREE_ROOT(rbtree) == elem) {                              
        RB_TREE_ROOT(rbtree) = NULL;                                  
    }                                                                  
    rbfree(elem);                                                
    return ;                                                           
}                                                                      

#endif



int main() 
{
    int i;
    ptr = &ttrbtree;
    RE_TREE_INIT(ptr);

    Node_t * nn, *next;
    srand((unsigned)time(NULL));
    for (i = 0; i < LEN ; i++) {
        int tmp = rand() & (LEN - 1);

        nn = malloc(sizeof(Node_t));
        RB_NODE_INIT(nn, rbnode);
        nn->key = tmp;
        //printf("tmp==%d\n",tmp);
        if (RB_INSERT(rbtree, ptr, nn) == 0) {
            free(nn);
        }
    }
#if 1
    for_each_safe(nn, next, rbtree, ptr) {
        printf("key==%d\n",nn->key);
    }
#endif 


#if 1
    Node_t tmp, *ptr_tmp;
    for (i = 0; i < 500 ; i++) {
        //printf("tmp.key==%d\n",tmp.key);
        ptr_tmp = RB_REMOVE_FROM_KEY(rbtree, ptr, i);
        free(ptr_tmp);
    }

    for(i = 500; i<=550; i++) {
        tmp.key = i;
        if (RB_FIND_FROM_KEY(rbtree, ptr, i)) {
            printf("i==%d lower==%d upper==%d\n",i, RB_LOWER_BOUND_FROM_KEY(rbtree, ptr, i)->key,
                    RB_UPPER_BOUND_FROM_KEY(rbtree, ptr, i)->key);
        }
    }
#if 0
    printf("*********************************\n");
    for_each (nn, rbtree, ptr) {
        printf("key==%d\n",nn->key);
    }
#endif
#endif

    printf("rbtree->size == %d\n", RB_TREE_SIZE(ptr));
    RB_TREE_EXIT(rbtree, ptr);
    printf("rbtree->size == %d FreeNum == %d\n", RB_TREE_SIZE(ptr), FreeNum);

    return 0;
}
