/*
list_tip.h
linux-4.9.37内核版本 内核链表接口介绍
源代码路径 linux-4.9.37\include\linux\list.h

*/

/*
static inline
内联函数, 节省函数调用所需的栈空间, 执行效率高
*/

/*
双链表结构, 是一个双向循环链表, 或者说是双向环形链表
*/
struct list_head {
    struct list_head *next, *prev;
};

/*
初始化已经定义过的变量
*/
#define LIST_HEAD_INIT(name) { &(name), &(name) }

/*
定义struct list_head类型头指针, 并且初始化
*/
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

/*
初始化链表头指针head, 将prev和next元素都指向list本身
*/
static inline void INIT_LIST_HEAD(struct list_head *list);

/*
添加new节点到head之后, head是链表头
*/
static inline void list_add(struct list_head *new, struct list_head *head);

/*
添加new节点到head之前, head是链表头
*/
static inline void list_add_tail(struct list_head *new, struct list_head *head);

/*
删除链表节点entry
*/
static inline void list_del(struct list_head *entry);

/*
判断链表是否为空, 也可以判断链表中任意一个节点是否为空
*/
static inline int list_empty(const struct list_head *head);

/*
获取链表中某个节点的地址
*/
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/*
获取链表中第一个节点的地址
*/
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

/*
*/
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/*
*/
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/*
遍历链表
*/
#define list_for_each_entry(pos, head, member)                \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                    \
         pos = list_next_entry(pos, member))

/*
list_for_each_entry的安全版, 当遍历过程中涉及到删除节点的时候, 使用该宏函数
*/
#define list_for_each_entry_safe(pos, n, head, member)            \
    for (pos = list_first_entry(head, typeof(*pos), member),    \
        n = list_next_entry(pos, member);            \
         &pos->member != (head);                     \
         pos = n, n = list_next_entry(n, member))

/*
反向遍历链表
*/
#define list_for_each_entry_reverse(pos, head, member)            \
    for (pos = list_last_entry(head, typeof(*pos), member);        \
         &pos->member != (head);                     \
         pos = list_prev_entry(pos, member))
