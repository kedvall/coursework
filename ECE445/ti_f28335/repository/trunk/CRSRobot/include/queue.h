
#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue {
	struct queue *prev, *next;
	void *data;
} queue_t;

extern inline void queue_put(queue_t *head, queue_t *new)
{
	queue_t *prev;
	
	prev = head->prev;
	
	head->prev = new;
	prev->next = new;
	new->prev = prev;
	new->next = head;
}

extern inline queue_t *queue_get(queue_t *head)
{
	queue_t *get;
	
	get = head->next;
	
	head->next->prev = head;
	head->next = head->next->next;

	return get;
}

#define queue_empty(QP)			((QP)->next == (QP)->prev)
#define queue_data(QP,TYPE)		((TYPE)(QP)->data)
#define init_queue(QP)			do { (QP)->next = (QP)->prev = (QP); (QP)->data = NULL; } while (0)

#endif /* __QUEUE_H__ */
