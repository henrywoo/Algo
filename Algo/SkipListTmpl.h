#ifndef _SKIP_LIST_TMPL_H
#define _SKIP_LIST_TMPL_H
///http://blog.csdn.net/dickyjyang/article/details/8835338

/* 跳表实现
 * 跳表能够提供log(n)的查找效率，与平衡二叉树效率相当，但是比平衡二叉树简单，在多线程中使用效率比平衡
 * 二叉树高，考虑多线程中的加锁，由于平衡二叉树可能涉及到重新平衡二叉树，加锁比跳表消耗大
 * 跳表是利用随机化的方法建立快速查找的路径
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

namespace exotic_datastructure{
	/* 跳表节点数据结构
 *
 */
template <typename TData>
struct SkipListNode
{
    TData value; // 节点值
    SkipListNode<TData>** forward;  // 节点前向指针数组
};

/* 跳表操作的返回码
 */
enum
{
    SKIP_LIST_TMPL_INSERT_NODE_SUCCESS = 0,                        // 插入节点成功
    SKIP_LIST_TMPL_GET_FREE_SKIP_LIST_NODE_FAILED = 1,     // 获取空闲节点失败
    SKIP_LIST_TMPL_NODE_EXIST = 2,                         // 节点已经存在
    SKIP_LIST_TMPL_NODE_NOT_EXIST = 3,                     // 节点不存在于跳表中
    SKIP_LIST_TMPL_DELETE_NODE_SUCCESS = 4,                   // 删除节点成功
};

/* 跳表
 *
 */
template <typename TData>
class SkipListTmpl
{
public:
    /* 初始化跳表
     */
    bool InitSkipListTmpl();

    /* 获取一个空闲的跳表节点
     * @param level  跳表节点的层数
     * @return 返回NULL表示创建失败，否则返回创建的跳表节点指针
     */
    SkipListNode<TData>* GetFreeSkipListNode(int level);

    /* 增加一个跳表节点
     * @param value 节点值
     * @return 返回操作码，详情看上面跳表操作返回码的意义
     */
    int InsertSkipListNode(const TData &value);

    /* 删除一个跳表节点
     * @param value 节点值
     * @return 返回操作码，详情看上面跳表操作返回码的意义
     */
    int DeleteSkipListNode(const TData &value);

    /* 查找跳表节点
     * @param value 节点值
     * @return 返回NULL表示查找失败，否则返回找到了跳表节点指针
     */
    SkipListNode<TData>* SearchSkipListNode(const TData &value);

    /* 遍历处理跳表中所有节点
     * @param proc 回调的处理函数
     * @return 返回处理的节点数目
     */
    int ProcessEverySkipListNode(void (*proc)(const TData &value));

private:
    int level; // 当前跳表的最大层数
    SkipListNode<TData>* header;  // 跳表头节点

private:
    const static int MAX_SKIP_LIST_TMPL_LEVEL = 32;  // 跳表的最大层数
    const static double SKIP_LIST_TMPL_LEVEL_PROBABILITY;  // i-1层节点属于i层的概率

private:
    /* 获取一个节点的层数，层数范围
     * @return 节点的层数
     */
    int GetRandomSkipListNodeLevel();
};

// i-1层节点属于i层的概率
template <typename TData>
const double SkipListTmpl<TData>::SKIP_LIST_TMPL_LEVEL_PROBABILITY = 0.5;

/* 创建一个空的跳表节点
 * @param level  跳表节点的层数
 * @return 返回NULL表示创建失败，否则返回创建的跳表节点指针
 */
template <typename TData>
SkipListNode<TData>* SkipListTmpl<TData>::GetFreeSkipListNode(int level)
{
    // 节点层数范围0
    assert(level >= 0 && level < MAX_SKIP_LIST_TMPL_LEVEL);

    SkipListNode<TData>* newNode = (SkipListNode<TData>*)malloc(sizeof(SkipListNode<TData>));
    if (NULL == newNode)
    {
        return NULL;
    }
    
    // 前向节点初始化为NULL
    newNode->forward = (SkipListNode<TData> **)malloc(sizeof(SkipListNode<TData>*) * (level+1));
    if (NULL == newNode->forward)
    {

        // 申请内存失败那么就归还之前申请的内存
        free(newNode);
        return NULL;
    }
    
    for (int i = 0; i <= level; ++i)
    {
        newNode->forward[i] = NULL;
    }

    // 创建好的跳表节点
    return newNode;
}
	
/* 初始化跳表
 */
template <typename TData>
bool SkipListTmpl<TData>::InitSkipListTmpl()
{
    level = 0; // 当前最大层初始为0
    
    // 头结点的层数为跳表最大层数，这个节点是个冗余节点，增加它是为了方便链表操作
    header = GetFreeSkipListNode(MAX_SKIP_LIST_TMPL_LEVEL-1);

    // 初始化成功
    if (header)
    {
        return true;
    }

    // 初始化失败
    return false;
}

/* 增加一个跳表节点
 * @param value 节点值
 * @return 返回操作码，详情看上面跳表操作返回码的意义
 */
template <typename TData>
int SkipListTmpl<TData>::InsertSkipListNode(const TData &value)
{
    // 保证跳表已经初始化
    assert(NULL != header);

    // 首先查找节点的插入位置
    SkipListNode<TData>* update[MAX_SKIP_LIST_TMPL_LEVEL];  // 插入节点的前驱节点数组

    SkipListNode<TData>* p = header;
    SkipListNode<TData>* q = NULL;

    // 找到跳表中插入节点的前驱节点
    for (int k = level; k >= 0; --k)
    {
        q = p->forward[k];
        while(q && q->value < value)
        {
            p = q;
            q = q->forward[k];
        }
        update[k] = p;
    }

    // 说明节点已经存在
    if (q && value == q->value)
    {
        return SKIP_LIST_TMPL_NODE_EXIST;
    }

    // 随机获取到插入节点的层数
    int nodelevel = GetRandomSkipListNodeLevel();

    // 如果插入节点层数大于当前跳表的最大层数，需要更新插入节点的前驱节点数组
    while(nodelevel > level)
    {
        ++level;
        update[level] = header;
    }

    // 获取一个空闲的跳表节点
    SkipListNode<TData>* freeNode = GetFreeSkipListNode(nodelevel);
    if (NULL == freeNode)
    {
        // 获取空闲节点失败
        return SKIP_LIST_TMPL_GET_FREE_SKIP_LIST_NODE_FAILED;
    }

    // 初始化该节点
    freeNode->value = value;

    // 将节点插入到跳表中，update数组中保存了节点的不同层的前驱节点数组
    for (int k = 0; k <= nodelevel; ++k)
    {
        freeNode->forward[k] = update[k]->forward[k];
        update[k]->forward[k] = freeNode;
    }

    return SKIP_LIST_TMPL_INSERT_NODE_SUCCESS;
}

/* 删除一个跳表节点
 * @param value 节点值
 * @return 返回操作码，详情看上面跳表操作返回码的意义
 */
template <typename TData>
int SkipListTmpl<TData>::DeleteSkipListNode(const TData &value)
{
    // 保证跳表已经初始化
    assert(NULL != header);
    
    // 首先查找节点的前驱节点数组
    SkipListNode<TData>* update[MAX_SKIP_LIST_TMPL_LEVEL];  // 删除节点的前驱节点数组
    
    SkipListNode<TData>* p = header;
    SkipListNode<TData>* q = NULL;

    // 找到跳表中删除节点的前驱节点
    for (int k = level; k >= 0; --k)
    {
        q = p->forward[k];
        while(q && q->value < value)
        {
            p = q;
            q = q->forward[k];
        }
        update[k] = p;
    }

    // 说明删除节点存在
    if (q && q->value == value)
    {
        for (int k = 0; k <= level && update[k]->forward[k] == q; ++k)
        {
            update[k]->forward[k] = q->forward[k];
        }
        free(q->forward);
        free(q);

        // 如果q节点刚好是跳表中的最大层节点，需要更新当前跳表的最大层
        while(NULL == header->forward[level] && level > 0)
        {
            --level;
        }

        // 删除节点成功
        return SKIP_LIST_TMPL_INSERT_NODE_SUCCESS;
    }
    else
    {
        // 说明删除节点不存在
        return SKIP_LIST_TMPL_NODE_NOT_EXIST;
    }
}

/* 查找跳表节点
 * @param value 节点值
 * @return 返回NULL表示查找失败，否则返回找到了跳表节点指针
 */
template <typename TData>
SkipListNode<TData>* SkipListTmpl<TData>::SearchSkipListNode(const TData &value)
{
    // 确保跳表已经初始化
    assert(NULL != header);

    SkipListNode<TData>* p = header;
    SkipListNode<TData>* q = NULL;
    
    for (int k = level; k >= 0; --k)
    {
        q = p->forward[k];
        while(q && q->value < value)
        {
            p = q;
            q = q->forward[k];
        }
    }

    // 说明找到节点了
    if (q && q->value == value)
    {
        return q;
    }
    else
    {
        // 说明没有找到节点
        return NULL;
    }
}

/* 遍历处理跳表中所有节点
 * @param proc 回调的处理函数
 * @return 返回处理的节点数目
 */
template <typename TData>
int SkipListTmpl<TData>::ProcessEverySkipListNode(void (*proc)(const TData &value))
{
    // 确保跳表已经初始化
    assert(NULL != header);
    
    int cnt = 0;
    SkipListNode<TData>* p = header->forward[0];
    while(p)
    {
        proc(p->value);
        cnt++;
        p = p->forward[0];
    }
    return cnt;
}


/* 随机获取一个节点的层数，层数范围0 - (MAX_SKIP_LIST_TMPL_LEVEL-1)
 * @return 节点的层数
 */
template <typename TData>
int SkipListTmpl<TData>::GetRandomSkipListNodeLevel()
{
    // 选择一个种子值
    static bool seedInit = false;
    if (!seedInit){
        srand(static_cast<unsigned int>(time(NULL)));
        seedInit = true;
    }
    
    // 根据概率计算出节点的层数
    int newLevel = 0;
    while(1){
        double curP = (double)(rand() % 100) / 100.0;
        if (curP < SKIP_LIST_TMPL_LEVEL_PROBABILITY){
            newLevel++;
        }else{
            break;
        }

        // 最大层数是MAX_SKIP_LIST_TMPL_LEVEL-1
        if ( (MAX_SKIP_LIST_TMPL_LEVEL-1) <= newLevel){
            break;
        }
    }
    
    // 最大层数是MAX_SKIP_LIST_TMPL_LEVEL-1
    if (newLevel >= MAX_SKIP_LIST_TMPL_LEVEL){
        newLevel = MAX_SKIP_LIST_TMPL_LEVEL - 1;
    }

    return newLevel;
}




void printInt(const int& val)
{
	std::cout << val << std::endl;
}

int test()
{
	SkipListTmpl<int> skiplist;
	skiplist.InitSkipListTmpl();
	skiplist.InsertSkipListNode(6);
	skiplist.InsertSkipListNode(3);
	skiplist.InsertSkipListNode(12);
	skiplist.InsertSkipListNode(8);
	skiplist.InsertSkipListNode(-5);

	skiplist.ProcessEverySkipListNode(printInt);

	std::cout << "after delete operate : " << std::endl;

	skiplist.DeleteSkipListNode(8);
	skiplist.DeleteSkipListNode(3);

	skiplist.ProcessEverySkipListNode(printInt);

	return 0;
}
}





#endif