/*
& File     : main.c
* Author   : zentut.com
* Purpose  : C Linked List Data Structure
* Copyright: @ zentut.com
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node* next;
} node;

typedef void (*callback)(node* data);
node* head = NULL;
int charCount = 0;

/*
    create a new node
    initialize the data and next field

    return the newly created node
*/
node* create(int data,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;

    return new_node;
}

node* append(node* head, int data)
{
    if(head == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    /* create a new node */
    node* new_node =  create(data,NULL);
    cursor->next = new_node;

    return head;
}

node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;

    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }

    if(back != NULL)
        back->next = NULL;

    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;

    free(cursor);

    return head;
}

void dispose(node *head)
{
    node *cursor, *tmp;

    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}

int secuencia_sent(){

  return 0;
}

int program(){

  return 0;
}

int sent_if(){
  // linked List 1
    //check if
    // ahora checar exp


  return 0;
}


int main(){

  head = append(head, 5);
  char n[ 10 ]; /* n is an array of 10 integers */
  int i,j;

  /* initialize elements of array n to 0 */
  for ( i = 0; i < 10; i++ ) {
     //n[ i ] = i + 100; /* set element at location i to i + 100 */
  }

  /* output each array element's value */
  for (j = 0; j < 10; j++ ) {
     //printf("Element[%d] = %d\n", j, n[j] );
  }

  //dispose();
  return 0;
}
