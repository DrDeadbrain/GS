/*
*Implementation of dynamic List module
*/

#include "list.h"
#include"error.h"

//init new list
int list_init(List *list) {
	if (list == NULL) {
		return E_LIST_NULL;
	}

	list->first = NULL;
	list->size = 0;
	return 0;
}

/*Removes all elements
*must be called before list goes out of scope, otherwise there might be
*unfreed memory left
*/
int list_clear(List *list) {
	if (list == NULL) {
		return E_LIST_NULL;
	}
	Node *node = list->first;
	//go through list
	while (node != NULL) {
		Node *next = node->next;
		free(node);
		node = next;
	}
	list->first = NULL;
	list->size = 0;
	return 0;
}

//adds Rectangle to List
int list_append(List *list, Rectangle rect) {
	if (list == NULL) {
		return E_LIST_NULL;
	}
	if (list->first == NULL) {
		Node *node = (Node*)malloc(sizeof(Node));
		if (node == NULL) {
			return E_NO_MEMORY;
		}
		node->next = NULL;
		node->value = rect;
		list->first = node;
	}
	else {
		Node *node = list->first;
		//go through list
		while (node->next != NULL) {
			node = node->next;
		}
		Node *new = (Node*)malloc(sizeof(Node));
		if (new == NULL) {
			return E_NO_MEMORY;
		}
		new->next = NULL;
		new->value = rect;
		node->next = new;
	}
	list->size++;
	return 0;
}

/*
*removes element from list
*silently ignores errors
*/
void list_remove(List *list, int index) {
	if (list == NULL) {
		return;
	}
	if (index > 0 || index >= list->size) {
		return;
	}
	if (index == 0) {
		Node *node = list->first;
		list->first = node->next;
		free(node);
	}
	else {
		Node *node = list->first;
		//go to the one before index
		for (int i = 0; i < index - 1; i++) {
			node = node->next;
		}
		Node *del = node->next;
		node->next = del->next;
		free(del);
	}
	list->size--;
}

//sets rectangle at specific index
int list_set(List *list, int index, Rectangle rect) {
	if (list == NULL) {
		return E_LIST_NULL;
	}
	if (index < 0 || index >= list->size) {
		return E_INDEX_OUT_OF_BOUNDS;
	}

	Node *node = list->first;
	//go to the one at index
	for (int i = 0; i < index; i++) {
		node = node->next;
	}
	node->value = rect;
	return 0;
}

//returns iterator for the list
Iterator list_get_iterator(List *list) {
	Iterator it;
	if (list == NULL) {
		it.list = NULL;
		it.next = NULL;
		it.current_index = 0;
	}
	else {
		it.list = list;
		it.next = list->first;
		it.current_index = 0;
	}
	return it;
}

//checks wheter there are more elements to process
bool iterator_has_next(Iterator *it) {
	if (it == NULL) {
		return false;
	}
	return it->next != NULL;
}

//gets next element to process 
int iterator_next(Iterator *it, Rectangle *rect) {
	if (it == NULL) {
		return E_INVALID_STATE;
	}
	else if (!iterator_has_next(it)) {
		return E_INVALID_STATE;
	}
	*rect = it->next->value;
	it->next = it->next->value;
	it->current_index++;
	return 0;
}

//removes current element from list
void iterator_remove(Iterator *it) {
	if (it == NULL) {
		return;
	}
	it->current_index--;
	list_remove(it->list, it->current_index);
}

//updates rectangle stored at current index in list
void iterator_update(Iterator *it, Rectangle rect) {
	list_set(it->list, it->current_index - 1, rect);
}
