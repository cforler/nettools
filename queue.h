#pragma once

#include <stdbool.h>

typedef struct qelement {
  void *data;
  struct qelement *next;
} qelement_t;
              

typedef struct queue {
  struct qelement *head;
  struct qelement *tail;
} queue_t;


queue_t * queue_new();

bool queue_add(queue_t *q, void *data);

void *queue_remove(queue_t *q);


void queue_free(queue_t *q);
