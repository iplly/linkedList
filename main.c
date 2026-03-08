#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct ListItem {
  struct ListItem *next;
  struct ListItem *prev;
  void *value;
};

struct List {
  struct ListItem *front;
  struct ListItem *back;
  size_t listSize;
  void *(*pushBack)(struct List *, void *);
  int (*popBack)(struct List *);
  void (*clear)(struct List *);
  void *(*pushFront)(struct List *, void *);
  int (*popFront)(struct List *);
  char (*isEmpty)(struct List *);
  size_t (*size)(struct List *);
  void (*print)(struct List *, const char *);
};

void ListPrint(struct List *this, const char *f) {
  if (this->isEmpty(this))
    return;
  struct ListItem *cur = this->front;

  while (cur) {
    printf(f, *(int *)cur->value);
    cur = cur->next;
  }
}

char ListIsEmpty(struct List *this) {
  if (this->front == NULL && this->back == NULL)
    return 1;
  return 0;
}

void *ListPushBack(struct List *this, void *p) {
  this->listSize++;

  if (p == NULL)
    return NULL;
  if (this->isEmpty(this)) {
    struct ListItem *item = malloc(sizeof(struct ListItem));
    if (item == NULL)
      return NULL;

    this->back = item;
    this->front = item;

    item->next = NULL;
    item->prev = NULL;
    item->value = p;

    return item->value;
  }
  struct ListItem *item = malloc(sizeof(struct ListItem));
  if (item == NULL)
    return NULL;
  this->back->next = item;
  item->prev = this->back;
  this->back = item;
  item->next = NULL;
  item->value = p;

  return item->value;
}
void *ListPushFront(struct List *this, void *p) {
  if (p == NULL)
    return NULL;
  this->listSize++;
  if (this->isEmpty(this)) {
    struct ListItem *item = malloc(sizeof(struct ListItem));
    if (item == NULL)
      return NULL;
    this->back = item;
    this->front = item;

    item->next = NULL;
    item->prev = NULL;
    item->value = p;
    return item->value;
  }
  struct ListItem *item = malloc(sizeof(struct ListItem));
  if (item == NULL)
    return NULL;
  this->front->prev = item;
  item->next = this->front;
  this->front = item;
  item->prev = NULL;
  item->value = p;

  return item->value;
}

int ListPopBack(struct List *this) {
  if (!this->back)
    return 1;

  this->listSize--;
  free(this->back->value);
  if (this->back->prev == NULL) {
    free(this->back);
    this->back = NULL;
    this->front = NULL;
    return 0;
  }
  this->back = this->back->prev;

  free(this->back->next);
  this->back->next = NULL;

  return 0;
}

int ListPopFront(struct List *this) {
  if (!this->front)
    return 1;

  this->listSize--;
  free(this->front->value);
  if (this->front->next == NULL) {
    free(this->front);
    this->back = NULL;
    this->front = NULL;
    return 0;
  }
  this->front = this->front->next;
  free(this->front->prev);
  this->front->prev = NULL;

  return 0;
}

size_t ListSize(struct List *this) { return this->listSize; }

void ListClear(struct List *this) {
  while (!this->isEmpty(this)) {
    this->popBack(this);
  }
}

void ListDtor(struct List *this) {
  while (!this->isEmpty(this)) {
    this->popBack(this);
  }
}

void ListCtor(struct List *this) {
  this->front = NULL;
  this->back = NULL;
  this->listSize = 0;
  this->pushBack = ListPushBack;
  this->print = ListPrint;
  this->popBack = ListPopBack;
  this->isEmpty = ListIsEmpty;
  this->size = ListSize;
  this->clear = ListClear;
  this->pushFront = ListPushFront;
  this->popFront = ListPopFront;
}

int main(void) {
  struct List list;
  ListCtor(&list);
  printf("pushBack\n");
  *(int *)list.pushBack(&list, malloc(sizeof(int))) = 4;
  *(int *)list.pushBack(&list, malloc(sizeof(int))) = 5;
  *(int *)list.pushBack(&list, malloc(sizeof(int))) = 6;

  list.print(&list, "%d ");
  *(int *)list.pushFront(&list, malloc(sizeof(int))) = 7;
  printf("\n\npushFront\n");
  list.print(&list, "%d ");
  printf("\n\nsize: %zu\n", list.size(&list));
  printf("isEmpty: %hhd\n", list.isEmpty(&list));
  list.clear(&list);
  printf("list clear\nsize: %zu\n", list.size(&list));
  printf("isEmpty: %hhd\n\n", list.isEmpty(&list));

  *(int *)list.pushFront(&list, malloc(sizeof(int))) = 4;
  *(int *)list.pushFront(&list, malloc(sizeof(int))) = 5;
  *(int *)list.pushFront(&list, malloc(sizeof(int))) = 6;
  *(int *)list.pushFront(&list, malloc(sizeof(int))) = 7;
  *(int *)list.pushFront(&list, malloc(sizeof(int))) = 8;
  list.print(&list, "%d ");
  printf("\nsize: %zu\n", list.size(&list));

  printf("\npopBack\n");
  list.popBack(&list);
  list.print(&list, "%d ");
  printf("\n\n");

  printf("popFront\n");
  list.popFront(&list);
  list.print(&list, "%d ");
  printf("\n");

  ListDtor(&list);
  return 0;
}
