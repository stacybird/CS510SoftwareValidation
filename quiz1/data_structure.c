/***************************************************************
 * C Program to Implement a Stack, Queue, and Doubly-Linked List
 * The code is downloaded form http://www.sanfoundry.com
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>

struct node
{
  int info;
  struct node *ptr;
} *top,*top1,*temp_stack,       // for stack
  *front,*rear,*temp_q,*front1; // for queue

struct dll_node
{
  struct dll_node *prev;
  int n;
  struct dll_node *next;
}*h,*dll_temp,*dll_temp1,*dll_temp2,*dll_temp4;


/// STACK ////////////////////////
int  stack_topelement();
void stack_push(int data);
void stack_pop();
void stack_empty();
void stack_display();
void stack_destroy();
void stack_count_elem();
void stack_create();

int stack_count = 0;


/// QUEUE ////////////////////////
int  q_frontelement();
void q_enq(int data);
void q_deq();
void q_empty();
void q_display();
void q_create();
void queuesize();

int q_count = 0;

/// DOUBLE-LINKED LIST ///////////
void insert1();
void insert2();
void insert3();
void traversebeg();
void traverseend(int);
void sort();
void search();
void update();
void delete();
void dll_create();

int dll_count = 0;


////////////////////////////////////
int get_input() {
  int  input;
  char buf[1024];
  while (NULL != fgets(buf, sizeof buf, stdin)) {
    if (1 != sscanf(buf,"%d",&input)) {
      printf("Input was not an integer, try again.\n");
      continue;
    } else {
      return input;
    }
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/* Create empty stack */
void stack_create()
{
  top = NULL;
}

/* Count stack elements */
void stack_count_elem()
{
  printf("\n No. of elements in stack : %d", stack_count);
}

/* Push data into stack */
void stack_push(int data)
{
  if (top == NULL)
    {
      top =(struct node *)malloc(1*sizeof(struct node));
      top->ptr = NULL;
      top->info = data;
    }
  else
    {
      temp_stack =(struct node *)malloc(1*sizeof(struct node));
      temp_stack->ptr = top;
      temp_stack->info = data;
      top = temp_stack;
    }
  stack_count++;
}

/* Display stack elements */
void stack_display()
{
  top1 = top;

  if (top1 == NULL)
    {
      printf("Stack is empty");
      return;
    }

  while (top1 != NULL)
    {
      printf("%d ", top1->info);
      top1 = top1->ptr;
    }
}

/* Pop Operation on stack */
void stack_pop()
{
  top1 = top;

  if (top1 == NULL)
    {
      printf("\n Error : Trying to pop from empty stack");
      return;
    }
  else
    top1 = top1->ptr;
  printf("\n Popped value : %d", top->info);
  free(top);
  top = top1;
  stack_count--;
}

/* Return top element */
int stack_topelement()
{
  return(top->info);
}

/* Check if stack is empty or not */
void stack_empty()
{
  if (top == NULL)
    printf("\n Stack is empty");
  else
    printf("\n Stack is not empty with %d elements", stack_count);
}

/* Destroy entire stack */
void stack_destroy()
{
  top1 = top;

  while (top1 != NULL)
    {
      top1 = top->ptr;
      free(top);
      top = top1;
      top1 = top1->ptr;
    }
  free(top1);
  top = NULL;

  printf("\n All stack elements destroyed");
  stack_count = 0;
}

void test_stack() {
  int no, ch, e;

  printf("\n 1 - Push");
  printf("\n 2 - Pop");
  printf("\n 3 - Top");
  printf("\n 4 - Empty");
  printf("\n 5 - Exit");
  printf("\n 6 - Dipslay");
  printf("\n 7 - Stack Count");
  printf("\n 8 - Destroy stack");

  stack_create();

  while (1)
    {
      printf("\n Enter choice : ");
      ch = get_input(); //scanf("%d", &ch);

      switch (ch)
        {
        case 1:
          printf("Enter data : ");
          no = get_input(); // scanf("%d", &no);
          stack_push(no);
          break;
        case 2:
          stack_pop();
          break;
        case 3:
          if (top == NULL)
            printf("No elements in stack");
          else
            {
              e = stack_topelement();
              printf("\n Top element : %d", e);
            }
          break;
        case 4:
          stack_empty();
          break;
        case 5:
          exit(0);
        case 6:
          stack_display();
          break;
        case 7:
          stack_count_elem();
          break;
        case 8:
          stack_destroy();
          break;
        default :
          printf(" Wrong choice, Please enter correct choice  ");
          break;
        }
    }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/* Create an empty queue */
void q_create()
{
  front = rear = NULL;
}

/* Returns queue size */
void queuesize()
{
  printf("\n Queue size : %d", q_count);
}

/* Enqueing the queue */
void q_enq(int data)
{
  if (rear == NULL)
    {
      rear = (struct node *)malloc(1*sizeof(struct node));
      rear->ptr = NULL;
      rear->info = data;
      front = rear;
    }
  else
    {
      temp_q=(struct node *)malloc(1*sizeof(struct node));
      rear->ptr = temp_q;
      temp_q->info = data;
      temp_q->ptr = NULL;

      rear = temp_q;
    }
  q_count++;
}

/* Displaying the queue elements */
void q_display()
{
  front1 = front;

  if ((front1 == NULL) && (rear == NULL))
    {
      printf("Queue is empty");
      return;
    }
  while (front1 != rear)
    {
      printf("%d ", front1->info);
      front1 = front1->ptr;
    }
  if (front1 == rear)
    printf("%d", front1->info);
}

/* Dequeing the queue */
void q_deq()
{
  front1 = front;

  if (front1 == NULL)
    {
      printf("\n Error: Trying to display elements from empty queue");
      return;
    }
  else
    if (front1->ptr != NULL)
      {
        front1 = front1->ptr;
        printf("\n Dequed value : %d", front->info);
        free(front);
        front = front1;
      }
    else
      {
        printf("\n Dequed value : %d", front->info);
        free(front);
        front = NULL;
        rear = NULL;
      }
  q_count--;
}

/* Returns the front element of queue */
int q_frontelement()
{
  if ((front != NULL) && (rear != NULL))
    return(front->info);
  else
    return 0;
}

/* Display if queue is empty or not */
void q_empty()
{
  if ((front == NULL) && (rear == NULL))
    printf("\n Queue empty");
  else
    printf("Queue not empty");
}

void test_queue() {
  int no, ch, e;

  printf("\n 1 - Enque");
  printf("\n 2 - Deque");
  printf("\n 3 - Front element");
  printf("\n 4 - Empty");
  printf("\n 5 - Exit");
  printf("\n 6 - Display");
  printf("\n 7 - Queue size");
  q_create();
  while (1)
    {
      printf("\n Enter choice : ");
      ch = get_input(); // scanf("%d", &ch);
      switch (ch)
        {
        case 1:
          printf("Enter data : ");
          no = get_input(); // scanf("%d", &no);
          q_enq(no);
          break;
        case 2:
          q_deq();
          break;
        case 3:
          e = q_frontelement();
          if (e != 0)
            printf("Front element : %d", e);
          else
            printf("\n No front element in Queue as queue is empty");
          break;
        case 4:
          q_empty();
          break;
        case 5:
          exit(0);
        case 6:
          q_display();
          break;
        case 7:
          queuesize();
          break;
        default:
          printf("Wrong choice, Please enter correct choice  ");
          break;
        }
    }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/* TO create an empty dll_node */
void dll_create()
{
  int data;

  dll_temp =(struct dll_node *)malloc(1*sizeof(struct dll_node));
  dll_temp->prev = NULL;
  dll_temp->next = NULL;
  printf("\n Enter value to node : ");
  data = get_input(); // scanf("%d", &data);
  dll_temp->n = data;
  dll_count++;
}

/*  TO insert at beginning */
void insert1()
{
  if (h == NULL)
    {
      dll_create();
      h = dll_temp;
      dll_temp1 = h;
    }
  else
    {
      dll_create();
      dll_temp->next = h;
      h->prev = dll_temp;
      h = dll_temp;
    }
}

/* To insert at end */
void insert2()
{
  if (h == NULL)
    {
      dll_create();
      h = dll_temp;
      dll_temp1 = h;
    }
  else
    {
      dll_create();
      dll_temp1->next = dll_temp;
      dll_temp->prev = dll_temp1;
      dll_temp1 = dll_temp;
    }
}

/* To insert at any position */
void insert3()
{
  int pos, i = 2;

  printf("\n Enter position to be inserted : ");
  pos = get_input(); // scanf("%d", &pos);
  dll_temp2 = h;

  if ((pos < 1) || (pos >= dll_count + 1))
    {
      printf("\n Position out of range to insert");
      return;
    }
  if ((h == NULL) && (pos != 1))
    {
      printf("\n Empty list cannot insert other than 1st position");
      return;
    }
  if ((h == NULL) && (pos == 1))
    {
      dll_create();
      h = dll_temp;
      dll_temp1 = h;
      return;
    }
  else
    {
      while (i < pos)
        {
          dll_temp2 = dll_temp2->next;
          i++;
        }
      dll_create();
      dll_temp->prev = dll_temp2;
      dll_temp->next = dll_temp2->next;
      dll_temp2->next->prev = dll_temp;
      dll_temp2->next = dll_temp;
    }
}

/* To delete an element */
void delete()
{
  int i = 1, pos;

  printf("\n Enter position to be deleted : ");
  pos = get_input(); // scanf("%d", &pos);
  dll_temp2 = h;

  if ((pos < 1) || (pos >= dll_count + 1))
    {
      printf("\n Error : Position out of range to delete");
      return;
    }
  if (h == NULL)
    {
      printf("\n Error : Empty list no elements to delete");
      return;
    }
  else
    {
      while (i < pos)
        {
          dll_temp2 = dll_temp2->next;
          i++;
        }
      if (i == 1)
        {
          if (dll_temp2->next == NULL)
            {
              printf("Node deleted from list");
              free(dll_temp2);
              dll_temp2 = h = NULL;
              return;
            }
        }
      if (dll_temp2->next == NULL)
        {
          dll_temp2->prev->next = NULL;
          free(dll_temp2);
          printf("Node deleted from list");
          return;
        }
      dll_temp2->next->prev = dll_temp2->prev;
      if (i != 1)
        dll_temp2->prev->next = dll_temp2->next;    /* Might not need this statement if i == 1 check */
      if (i == 1)
        h = dll_temp2->next;
      printf("\n Node deleted");
      free(dll_temp2);
    }
  dll_count--;
}

/* Traverse from beginning */
void traversebeg()
{
  dll_temp2 = h;

  if (dll_temp2 == NULL)
    {
      printf("List empty to display \n");
      return;
    }
  printf("\n Linked list elements from begining : ");

  while (dll_temp2->next != NULL)
    {
      printf(" %d ", dll_temp2->n);
      dll_temp2 = dll_temp2->next;
    }
  printf(" %d ", dll_temp2->n);
}

/* To traverse from end recursively */
void traverseend(int i)
{
  if (dll_temp2 != NULL)
    {
      i = dll_temp2->n;
      dll_temp2 = dll_temp2->next;
      traverseend(i);
      printf(" %d ", i);
    }
}

/* To search for an element in the list */
void search()
{
  int data, count = 0;
  dll_temp2 = h;

  if (dll_temp2 == NULL)
    {
      printf("\n Error : List empty to search for data");
      return;
    }
  printf("\n Enter value to search : ");
  data = get_input(); // scanf("%d", &data);
  while (dll_temp2 != NULL)
    {
      if (dll_temp2->n == data)
        {
          printf("\n Data found in %d position",dll_count + 1);
          return;
        }
      else
        dll_temp2 = dll_temp2->next;
      count++;
    }
  printf("\n Error : %d not found in list", data);
}

/* To update a node value in the list */
void update()
{
  int data, data1;

  printf("\n Enter node data to be updated : ");
  data = get_input(); // scanf("%d", &data);
  printf("\n Enter new data : ");
  data1 = get_input(); // scanf("%d", &data1);
  dll_temp2 = h;
  if (dll_temp2 == NULL)
    {
      printf("\n Error : List empty no node to update");
      return;
    }
  while (dll_temp2 != NULL)
    {
      if (dll_temp2->n == data)
        {

          dll_temp2->n = data1;
          traversebeg();
          return;
        }
      else
        dll_temp2 = dll_temp2->next;
    }

  printf("\n Error : %d not found in list to update", data);
}

/* To sort the linked list */
void sort()
{
  int i, j, x;

  dll_temp2 = h;
  dll_temp4 = h;

  if (dll_temp2 == NULL)
    {
      printf("\n List empty to sort");
      return;
    }

  for (dll_temp2 = h; dll_temp2 != NULL; dll_temp2 = dll_temp2->next)
    {
      for (dll_temp4 = dll_temp2->next; dll_temp4 != NULL; dll_temp4 = dll_temp4->next)
        {
          if (dll_temp2->n > dll_temp4->n)
            {
              x = dll_temp2->n;
              dll_temp2->n = dll_temp4->n;
              dll_temp4->n = x;
            }
        }
    }
  traversebeg();
}

void test_dll()
{
  int ch;

  h = NULL;
  dll_temp = dll_temp1 = NULL;

  printf("\n 1 - Insert at beginning");
  printf("\n 2 - Insert at end");
  printf("\n 3 - Insert at position i");
  printf("\n 4 - Delete at i");
  printf("\n 5 - Display from beginning");
  printf("\n 6 - Display from end");
  printf("\n 7 - Search for element");
  printf("\n 8 - Sort the list");
  printf("\n 9 - Update an element");
  printf("\n 10- Exit");

  while (1)
    {
      printf("\n Enter choice : ");
      ch = get_input(); // scanf("%d", &ch);
      switch (ch)
        {
        case 1:
          insert1();
          break;
        case 2:
          insert2();
          break;
        case 3:
          insert3();
          break;
        case 4:
          delete();
          break;
        case 5:
          traversebeg();
          break;
        case 6:
          dll_temp2 = h;
          if (dll_temp2 == NULL)
            printf("\n Error : List empty to display ");
          else
            {
              printf("\n Reverse order of linked list is : ");
              traverseend(dll_temp2->n);
            }
          break;
        case 7:
          search();
          break;
        case 8:
          sort();
          break;
        case 9:
          update();
          break;
        case 10:
          exit(0);
        default:
          printf("\n Wrong choice menu");
        }
    }
}




int main()
{
  printf(" 1 - Test Stack\n");
  printf(" 2 - Test Queue\n");
  printf(" 3 - Test Double-Linked List\n");
  printf(" 4 - Exit\n");

  while (1)
    {
      printf("Enter choice: ");
      int ch = get_input();

      switch (ch)
        {
        case 1:
          test_stack();
          break;
        case 2:
          test_queue();
          break;
        case 3:
          test_dll();
          break;
        case 4:
          exit(0);
        default:
          printf("Wrong choice menu\n");
          break;
        }
    }
}
