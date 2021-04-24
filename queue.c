#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"


queue_t * queue_new() {
  queue_t *q = malloc(sizeof(queue_t));
  if(!q) return NULL;

  q->head = NULL;
  q->tail = NULL;
  return q;
}


bool queue_add(queue_t *q, void  *data) {
  if(!q) return false;

  qelement_t *e =  malloc(sizeof(qelement_t));
  if(!e) return false;
  
  e->data = data;
  e->next = NULL;

  if(!q->head) q->head = e;
  else         q->tail->next = e;
  q->tail = e;

  return true;
}


void *queue_remove(queue_t *q) {
  if(!q || !q->head) return NULL;
  qelement_t *e = q->head;
  void *d = e->data;
  q->head = q->head->next;

  free(e);
  return d;
}


void queue_free(queue_t *q) {
  while(queue_remove(q));
  free(q);
}
