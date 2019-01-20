#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
        /* Team name */
        "Bomb Squad",
        /* First member's full name */
        "ldx",
        /* First member's email address */
        "@DX",
        /* Second member's full name (leave blank if none) */
        "",
        /* Second member's email address (leave blank if none) */
        ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


// My additional Macros
#define WSIZE     4          // word and header/footer size (bytes)
#define DSIZE     8          // double word size (bytes)
#define INITCHUNKSIZE 48     //针对最后一个数据做出的优化
#define CHUNKSIZE (1<<12)    //mm_malloc、realloc 每次向堆中申请内存的最低限制 !!

#define LISTLIMIT     20

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p)            (*(unsigned int *)(p))
//#define PUT_NOTAG(p, val)       (*(unsigned int *)(p) = (val) | GET_TAG(p))      //设值 ，并且加上原来的RA TAG
#define PUT_NOTAG(p, val) (*(unsigned int *)(p) = (val))                //将 p 指向的值设为val

// Store predecessor or successor pointer for free blocks
#define SET_PTR(p, ptr) (*(unsigned int *)(p) = (unsigned int)(ptr))    // p 指针指向的[值]为 ptr 指针

// Read the size and allocation bit from address p
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_TAG(p)   (GET(p) & 0x2)
#define SET_RATAG(p)   (GET(p) |= 0x2)
#define REMOVE_RATAG(p) (GET(p) &= ~0x2)

// Address of block's header and footer
#define HDRP(ptr) ((char *)(ptr) - WSIZE)
#define FTRP(ptr) ((char *)(ptr) + GET_SIZE(HDRP(ptr)) - DSIZE)

// Address of (physically) next and previous blocks
#define NEXT_BLKP(ptr) ((char *)(ptr) + GET_SIZE((char *)(ptr) - WSIZE))
#define PREV_BLKP(ptr) ((char *)(ptr) - GET_SIZE((char *)(ptr) - DSIZE))

// Address of free block's predecessor and successor entries
#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + WSIZE)

// Address of free block's predecessor and successor on the segregated list
#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_PTR(ptr)))

// Global var
/* 全局变量 分离空闲链表 */
void *segregated_free_lists[LISTLIMIT];

// Functions
static void *extend_heap(size_t size);
static void *coalesce(void *ptr);
static void *place(void *ptr, size_t asize);
static void insert_node(void *ptr, size_t size);
static void delete_node(void *ptr);
int mm_check();

/* 拓展堆的大小size */
static void *extend_heap(size_t size)
{
    void *ptr;
    size_t asize;                // Adjusted size

    asize = ALIGN(size);

    if ((ptr = mem_sbrk(asize)) == (void *)-1)
        return NULL;

    // 设置 header 和 footer
    PUT_NOTAG(HDRP(ptr), PACK(asize, 0));
    PUT_NOTAG(FTRP(ptr), PACK(asize, 0));
    PUT_NOTAG(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));    //设置 终点block
    insert_node(ptr, asize);

    void* ans = coalesce(ptr);
//    printf("extend_heap size:%d asize:%d final size : %d\n",size,asize,GET_SIZE(HDRP(ptr)));
    return ans;
}
/* 添加空闲节点 */
static void insert_node(void *ptr, size_t size) {
    int list = 0;
    void *search_ptr = ptr;
    void *insert_ptr = NULL;

    // 选择适合大小的分离空闲链表
    while ((list < LISTLIMIT - 1) && (size > 1)) {
        size >>= 1;
        list++;
    }
    // 按照链表中block size的递增序 查找合适的插入位置
    search_ptr = segregated_free_lists[list];
    while ((search_ptr != NULL) && (size > GET_SIZE(HDRP(search_ptr)))) {
        insert_ptr = search_ptr;
        search_ptr = PRED(search_ptr);
    }
    // 将 ptr 指向的空闲 block 在 search_ptr（前驱）和insert_ptr（后继）之间插入
    if (search_ptr != NULL) {   //前驱不为NULL
        if (insert_ptr != NULL) {   //后继不为NULL
            SET_PTR(PRED_PTR(ptr), search_ptr); //设置前驱  [链表基本操作]
            SET_PTR(SUCC_PTR(search_ptr), ptr); //设置前驱的后继
            SET_PTR(SUCC_PTR(ptr), insert_ptr); //设置后继
            SET_PTR(PRED_PTR(insert_ptr), ptr); //设置后继的前驱
        } else {                //后继为NULl
            SET_PTR(PRED_PTR(ptr), search_ptr);
            SET_PTR(SUCC_PTR(search_ptr), ptr);
            SET_PTR(SUCC_PTR(ptr), NULL);
            segregated_free_lists[list] = ptr;  //此时需要改变 链表的头指针
        }
    } else {    //前驱为NULL
        if (insert_ptr != NULL) {   //后继不为NULL
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), insert_ptr);
            SET_PTR(PRED_PTR(insert_ptr), ptr);
        } else {                    //后继为NULL
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), NULL);
            segregated_free_lists[list] = ptr;  //此时需要改变 链表的头指针
        }
    }
    return;
}

/* 删除空闲节点 */
static void delete_node(void *ptr) {
    int list = 0;
    size_t size = GET_SIZE(HDRP(ptr));

    // 选择合适大小的分离空闲链表
    while ((list < LISTLIMIT - 1) && (size > 1)) {
        size >>= 1;
        list++;
    }
    //链表操作
    if (PRED(ptr) != NULL) {
        if (SUCC(ptr) != NULL) {
            SET_PTR(SUCC_PTR(PRED(ptr)), SUCC(ptr));
            SET_PTR(PRED_PTR(SUCC(ptr)), PRED(ptr));
        } else {
            SET_PTR(SUCC_PTR(PRED(ptr)), NULL);
            segregated_free_lists[list] = PRED(ptr);    //没有后继 需要改变头指针为前驱
        }
    } else {
        if (SUCC(ptr) != NULL) {
            SET_PTR(PRED_PTR(SUCC(ptr)), NULL);
        } else {
            segregated_free_lists[list] = NULL;     //前驱后继都是NULL 改变头指针为NULL
        }
    }

    return;
}

/* 合并 block */
static void *coalesce(void *ptr)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HDRP(ptr));

    if (prev_alloc && next_alloc) {                         // Case 1 前后都已分配
        return ptr;
    }
    else if (prev_alloc && !next_alloc) {                   // Case 2 前分配 后未分配 合并后继
        delete_node(ptr);                                   // 在链表中删除原来两个node，将两个空闲块合并
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT_NOTAG(HDRP(ptr), PACK(size, 0));
        PUT_NOTAG(FTRP(ptr), PACK(size, 0));
    } else if (!prev_alloc && next_alloc) {                 // Case 3 前未分配 后已分配 合并前驱
        delete_node(ptr);                                   // 在链表中删除原来两个node，将两个空闲块合并
        delete_node(PREV_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT_NOTAG(FTRP(ptr), PACK(size, 0));
        PUT_NOTAG(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    } else {                                                // Case 4 合并前驱 后继
        delete_node(ptr);                                   // 在链表中删除原来三个node，将三个空闲块合并
        delete_node(PREV_BLKP(ptr));
        delete_node(NEXT_BLKP(ptr));
        size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT_NOTAG(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        PUT_NOTAG(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    }

    insert_node(ptr, size);     //将合并之后的新block 作为node插入 分离空闲链表

    return ptr;
}
/* 在ptr指向的block中 放置asize大小的数据 */
static void *place(void *ptr, size_t asize)
{
    size_t ptr_size = GET_SIZE(HDRP(ptr));
    size_t remainder = ptr_size - asize;

    delete_node(ptr);           //链表删除node


    if (remainder <= DSIZE * 2) {   //如果剩余大小不超过16B，则最多只能放下free block的附加信息，则不切分
        PUT_NOTAG(HDRP(ptr), PACK(ptr_size, 1));
        PUT_NOTAG(FTRP(ptr), PACK(ptr_size, 1));
    }
    else if (asize >= 110) {    //根据需要占用的空间大小
        // 切分 ： 将后部划分给占用 !!
        PUT_NOTAG(HDRP(ptr), PACK(remainder, 0));
        PUT_NOTAG(FTRP(ptr), PACK(remainder, 0));
        PUT_NOTAG(HDRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        PUT_NOTAG(FTRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        insert_node(ptr, remainder);
        return NEXT_BLKP(ptr);
    }
    else {
        // 切分：将前部划分给占用
        PUT_NOTAG(HDRP(ptr), PACK(asize, 1));
        PUT_NOTAG(FTRP(ptr), PACK(asize, 1));
        PUT_NOTAG(HDRP(NEXT_BLKP(ptr)), PACK(remainder, 0));
        PUT_NOTAG(FTRP(NEXT_BLKP(ptr)), PACK(remainder, 0));
        insert_node(NEXT_BLKP(ptr), remainder);
    }
    return ptr;
}

static char *heapStart; // Pointer to beginning of heap

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    char *heap_start;
    int list;
    // 初始化 分离链表
    for (list = 0; list < LISTLIMIT; list++) {
        segregated_free_lists[list] = NULL;
    }

    // 在堆中分配内存
    if ((long)(heap_start = mem_sbrk(4 * WSIZE)) == -1)
        return -1;

    PUT_NOTAG(heap_start, 0);                            /* 对齐*/
    PUT_NOTAG(heap_start + (1 * WSIZE), PACK(DSIZE, 1)); /* 开始区域header*/
    PUT_NOTAG(heap_start + (2 * WSIZE), PACK(DSIZE, 1)); /* 开始区域footer*/
    PUT_NOTAG(heap_start + (3 * WSIZE), PACK(0, 1));     /* 结束区域footer*/
    heapStart = heap_start+DSIZE;
    //拓展堆初始 chunck 大小

    mm_check();

    if (extend_heap(INITCHUNKSIZE) == NULL)
        return -1;
    return 0;
}
/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    void *ptr = NULL;

    if (size == 0)
        return NULL;

    // 对齐为 8B 的倍数
    // 已分配块 只需要保存 header 和 footer 信息 共一个 DSIZE
    if (size <= DSIZE) {
        asize = 2 * DSIZE;
    } else {
        asize = ALIGN(size+DSIZE);
    }

    int list = 0;
    size_t searchsize = asize;
    // 在分离链表中寻找合适的空闲块
    while (list < LISTLIMIT) {
        if ((list == LISTLIMIT - 1) || ((searchsize <= 1) && (segregated_free_lists[list] != NULL))) {
            ptr = segregated_free_lists[list];
            while ((ptr != NULL) && ((asize > GET_SIZE(HDRP(ptr))))) {
                ptr = PRED(ptr);
            }
            if (ptr != NULL)
                break;
        }

        searchsize >>= 1;
        list++;
    }

    // 如果没有找到 则拓展 heap ，拓展大小最小为 CHUNKSIZE
    if (ptr == NULL) {
        extendsize = MAX(asize, CHUNKSIZE);

        if ((ptr = extend_heap(extendsize)) == NULL)
            return NULL;
    }

    // 放置
    ptr = place(ptr, asize);

    mm_check();

    return ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT_NOTAG(HDRP(ptr), PACK(size, 0));
    PUT_NOTAG(FTRP(ptr), PACK(size, 0));

    insert_node(ptr, size); //先加入node
    coalesce(ptr);      //合并空闲块

    mm_check();

    return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *new_ptr = ptr;
    size_t new_size = size;
    int remainder;
    int extendsize;
    int block_buffer;

    if (size == 0)
        return NULL;

    if (new_size <= DSIZE) {
        new_size = 2 * DSIZE;
    } else {
        new_size = ALIGN(size+DSIZE);
    }

    block_buffer = GET_SIZE(HDRP(ptr)) - new_size;

    if (block_buffer < 0) {  //将当前block扩大
        /* 如果next block 是没有分配block 或者 是结束block | 用下一个空闲块来扩展 */
        if (!GET_SIZE(HDRP(NEXT_BLKP(ptr)))) {      //如果后一个是结束block
            remainder = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - new_size;
            extendsize = MAX(-remainder, CHUNKSIZE);    //最小CHUNKSIZE
            if (extend_heap(extendsize) == NULL)
                return NULL;
            remainder += extendsize;

            delete_node(NEXT_BLKP(ptr));    //删除以前的结束block

            // 不切分块
            PUT_NOTAG(HDRP(ptr), PACK(new_size + remainder, 1));
            PUT_NOTAG(FTRP(ptr), PACK(new_size + remainder, 1));
        }else if(!GET_ALLOC(HDRP(NEXT_BLKP(ptr)))) {        //如果后一个是空闲块
            remainder = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - new_size;
            if (remainder < 0) {    //拓展之后依旧不够 则直接申请内存 复制转移
                new_ptr = mm_malloc(new_size - DSIZE);
                memcpy(new_ptr, ptr, MIN(size, new_size));
                mm_free(ptr);
            } else {        //拓展之后足够 则直接将后一个空闲块 拓展进来
                delete_node(NEXT_BLKP(ptr));    //删除后一个空闲节点
                // 不切分块
                PUT_NOTAG(HDRP(ptr), PACK(new_size + remainder, 1));
                PUT_NOTAG(FTRP(ptr), PACK(new_size + remainder, 1));
            }
        }else {    //否则 直接申请内存 复制转移
            new_ptr = mm_malloc(new_size - DSIZE);
            memcpy(new_ptr, ptr, MIN(size, new_size));
            mm_free(ptr);
        }
        block_buffer = GET_SIZE(HDRP(new_ptr)) - new_size;
    }

    mm_check();

    return new_ptr;
}


int mm_check() {
//    puts("-----mm_check start----");
//    void  *searchPtr,*insert_ptr;
//    int blk_cnt=0,blkCnt=0;
//    for(int i=0;i<LISTLIMIT;i++) {
//        searchPtr = segregated_free_lists[i];
//        if(searchPtr==NULL) continue;
//        //只要能够完成循环遍历 则证明 空闲链表中的指针是否均指向有效的空闲块？
//        while (searchPtr != NULL) {  //是否有连续的空闲块没有被合并？
//            insert_ptr = searchPtr;
//            //只要能正常获取值（prev succ） 则证明• 空闲链表中的指针是否均指向有效的空闲块？
//            if(!GET_ALLOC(HDRP(NEXT_BLKP(insert_ptr)))
//             || !GET_ALLOC(HDRP(PREV_BLKP(insert_ptr)))) {
//                puts("WARNING:some ajacent blk blocks are unmerged");
//                return 0;
//            }
//            if(GET_ALLOC(HDRP(insert_ptr))) { //空闲列表中的每个块是否都标识为free（空闲）？
//                puts("WARNING:a blk block is not free but in list");
//                return 0;
//            }
//            blk_cnt+=1;
//            searchPtr = PRED(searchPtr);
//        }
//    }
//    //只要完成遍历 则证明：堆块中的指针是否指向有效的堆地址？
////    printf("pointer:%p",heapStart);
//    searchPtr = heapStart;
//    while(GET_SIZE(HDRP(searchPtr))>0) {
//        if(!GET_ALLOC(HDRP(searchPtr))) {
//            blkCnt++;
//        }
//        searchPtr = NEXT_BLKP(searchPtr);
//    }
//    if(blkCnt!=blk_cnt) {   //是否每个空闲块都在空闲链表中？
//        puts("WARNING:not every blk block is in the list");
//        return 0;
//    }
//
//    return 1;
}