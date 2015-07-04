#ifndef __LINKDED_LIST__

#define __LINKDED_LIST__
// TODO: you should focus on one style and write on. Make more improvements in the future. Do not expect a perfect start.
#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/_types/_sigaltstack.h>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 1
#endif

#define debug_print(fmt, ...) \
            do { if (DEBUG_TEST) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) \
    *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#define struct_entry(ptr, type, member) \
	container_of(ptr, type, member)

static inline void prefetch(const void *x) {;}

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

struct linked_list_node {
	struct linked_list_node *next;
};

#define LINKED_LIST_HEAD_INIT(name) { &(name) }
#define LINKED_LIST_HEAD(name) \
	struct linked_list_node name = LIST_HEAD_INIT(name)

static inline void INIT_LINKED_LIST_HEAD(struct linked_list_node *list)
{
	list->next = list;
}

static inline void __linked_list_add(struct linked_list_node *prev, struct linked_list_node *new, struct linked_list_node *next);
static inline void linked_list_add_tail (struct linked_list_node *prev, struct linked_list_node *new);

static inline void __linked_list_del(struct linked_list_node *prev, struct linked_list_node *next);
static inline void __linked_list_del_entry_after(struct linked_list_node *entry);
static inline void linked_list_del_after(struct linked_list_node *entry);
static inline void linked_list_del_init_after(struct linked_list_node *entry);
// static inline void __linked_list_del_entry(struct linked_list_node *entry);
// static inline void linked_list_del(struct linked_list_node *entry);
// static inline void linked_list_del_init(struct linked_list_node *entry);

#define linked_list_for_each(pos, head) \
for (pos = (head)->next; prefetch(pos->next), pos != (head); \
		pos = pos->next)

#define __linked_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define linked_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

// Imitate the Linux kernel that works. Do not try to go too far away in the beginning.
struct double_end_linked_list_node {
	struct linked_list_node linked_list_node;
	struct double_end_linked_list_node *prev;
};

#define DOUBLE_END_LINKED_LIST_HEAD_INIT(name) { &(name), &(name) }
#define DOUBLE_END_LINKED_LIST_HEAD(name) \
	struct double_end_linked_list_node name = LIST_HEAD_INIT(name)

static inline void DOUBLE_END_INIT_LINKED_LIST_HEAD(struct double_end_linked_list_node *list)
{
	((struct linked_list_node*)list)->next = (struct linked_list_node*)list;
	list->prev = list;
}

static inline void __double_end_linked_list_add(struct double_end_linked_list_node *prev, struct double_end_linked_list_node *new, struct double_end_linked_list_node *next);
static inline void double_end_linked_list_add_head(struct double_end_linked_list_node *new, struct double_end_linked_list_node *next);
static inline void double_end_linked_list_add_tail(struct double_end_linked_list_node *prev, struct double_end_linked_list_node *new);

static inline void __double_end_linked_list_del(struct double_end_linked_list_node *prev, struct double_end_linked_list_node *next);
static inline void __double_end_linked_list_del_entry(struct double_end_linked_list_node *entry);
static inline void double_end_linked_list_del(struct double_end_linked_list_node *entry);
static inline void double_end_linked_list_del_init(struct double_end_linked_list_node *entry);

#define double_end_linked_list_for_each(pos, head) \
for (pos = ((linked_list_node*)head)->next; prefetch(((linked_list_node*)pos)->next), pos != (head); \
		pos = ((linked_list_node*)pos)->next)

#define __double_end_linked_list_for_each(pos, head) \
	for (pos = ((linked_list_node*)head)->next; pos != (head); pos = ((linked_list_node*)pos)->next)

#define double_end_linked_list_for_each_safe(pos, n, head) \
	for (pos = ((linked_list_node*)head)->next, n = ((linked_list_node*)pos)->next; pos != (head); \
		pos = n, n = ((linked_list_node*)pos)->next)

void __linked_list_add(struct linked_list_node *prev, struct linked_list_node *new, struct linked_list_node *next)
{
	prev->next = new;
	new->next = next;
}

void linked_list_add_tail (struct linked_list_node *prev, struct linked_list_node *new)
{
	__linked_list_add(prev, new, prev->next);
}

static inline void __double_end_linked_list_add(struct double_end_linked_list_node *prev, struct double_end_linked_list_node *new, struct double_end_linked_list_node *next)
{
	__linked_list_add((struct linked_list_node*)prev, (struct linked_list_node*)new, (struct linked_list_node*)next);
	new->prev = prev;
	next->prev = new;
}

void __linked_list_del(struct linked_list_node *prev, struct linked_list_node *next)
{
	prev->next = next;
}

void __linked_list_del_entry_after(struct linked_list_node *entry)
{
    __linked_list_del(entry, entry->next->next);
}

void linked_list_del_after(struct linked_list_node *entry)
{
    struct linked_list_node *next_entry = entry->next;
    __linked_list_del_entry_after(entry);
//    __linked_list_del(entry, entry->next->next);
    next_entry->next = LIST_POISON1; // Beware here, entry->next->next has changed.
}

/*
 *
 *
 */
void linked_list_del_init_after(struct linked_list_node *entry)
{
    struct linked_list_node *next_entry = entry->next;
    __linked_list_del_entry_after(entry);
    INIT_LINKED_LIST_HEAD(next_entry);
}

void __double_end_linked_list_del(struct double_end_linked_list_node *prev, struct double_end_linked_list_node *next)
{
	((struct linked_list_node *)prev)->next = (struct linked_list_node *)next;
	next->prev = prev;
}

void __double_end_linked_list_del_entry(struct double_end_linked_list_node *entry)
{
	__double_end_linked_list_del(entry->prev, ((struct double_end_linked_list_node*) ((struct linked_list_node*) entry)->next));
}

void double_end_linked_list_del(struct double_end_linked_list_node *entry)
{
    __double_end_linked_list_del_entry(entry);
//	__double_end_linked_list_del(entry->prev, ((struct double_end_linked_list_node*) ((struct linked_list_node*) entry)->next));
	( ( (struct linked_list_node*)entry )->next ) = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

void double_end_linked_list_del_init(struct double_end_linked_list_node *entry)
{
	__double_end_linked_list_del_entry(entry);
	DOUBLE_END_INIT_LINKED_LIST_HEAD(entry);
}

struct linked_list {
	// Question: How to wrap the size into a method, instead of a variable, and remain a safe update?
	struct linked_list_node *linked_list_head;
	void (*init) (void);
};

struct double_end_linked_list {
	struct double_end_linked_list_node *double_end_linked_list_head;
	void (*init) (void);
};

static inline void linked_list_init(struct linked_list* linked_list);
static inline void double_end_linked_list_init(struct double_end_linked_list_node* double_end_linked_list);

struct parallel_linkded_list {

};



#endif