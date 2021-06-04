#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct app{  
    int size;
    char name[20];
};

struct app set_app_data()
{       
        struct app newapp;

        printf("ENTER THE NAME OF THE APP: ");
        //scanf("%[^\n]%*c",&newapp.name);
        scanf("%s",&newapp.name);
        printf("ENTER THE SIZE OF THE APP: ");
        scanf("%d",&newapp.size);

        return newapp;
}

struct node{
    struct app data;
    struct node *next;
    struct node *prev;
};

struct node* set_node_data()
{
      struct app newapp = set_app_data();    

      struct node *curnode = (struct node *)malloc(sizeof(struct node));
      curnode->data = newapp;
      curnode->next = NULL;
      curnode->prev = NULL;

      return curnode;
}

struct hash_table{ // functions : insert, search, delete 
    
    struct node **buckets; // array of list 
    int size; // always 7 or prime
};

struct node* insert_node(struct node **b, struct node *newnode,int index)
{
    if(b[index] == NULL)
    { 
      b[index] = newnode;
    }
    else
    {
       struct node *aux =  b[index];
      
       while(aux->next!=NULL)
       {
           aux = aux->next;
       }
       
       aux->next = newnode;
    }
    return b[index];
}

int generate_key(char name[])
{
    int string_size = strlen(name);
    
    int constant = 3;

    int key = 0;

    for(int i=0;i<string_size;i++)
    {
        int smol_constant = pow(constant,i);
        key += (smol_constant*name[i]);
    }

    return key%7; // size of bucket 
}
void print(struct hash_table table)
{
    printf("THE DIRECT ADDRESS TABLE HAS VALUES : \n");
     for(int i=0;i<7;i++)
     {
         if(table.buckets[i])
         {
           struct node *aux = table.buckets[i];
           //int count = 0;
           while(aux)
           {
               //printf("KEY : ",i+count);
               printf("[APP NAME : %s, SIZE : %d] ",aux->data.name,aux->data.size);
               aux = aux->next;
           }
           printf("\n");
         }
     }
     printf("\n");
}
int check(char st1[],char st2[])
{
    int i=0,j=0;
    while(i < strlen(st1) && j < strlen(st2))
    {
        if(st1[i] != st2[j])
        {
            return 0;
        }
        i++;
        j++;
    }
    if(i == strlen(st1) && j == strlen(st2)){return 1;}
    
    return 0;
}
struct node* search(struct node *curbucket,char str[])
{
     struct node *aux = curbucket;
     while(aux && !check(aux->data.name,str))
     {
        aux = aux->next;
     }

     return aux;
}
struct node* delete_app(struct node *curbucket,char str[])
{
    if(curbucket == NULL)
    {
       return NULL;
    }
    
    if(curbucket!=NULL && check(curbucket->data.name,str)) 
    {
       return curbucket->next;
    }
    
    if(curbucket->next == NULL)
    {
        return curbucket;
    }
    
    // first node can never be our answer and atleast 2 nodes 
    
    struct node *aux = curbucket;
    
    while(aux->next!=NULL && !check(aux->next->data.name,str))
    {
        aux = aux->next; 
    }
 
    if(aux->next!=NULL && check(aux->next->data.name,str))
    {
        aux->next = aux->next->next;
    }
    
    return curbucket;
    
}
struct queue{
    struct node *head;
    struct node *tail;
    int cache_size;
    int cache_cur_size;
};

struct queue* set_queue(struct queue *curq)
{
    curq = (struct queue*) malloc(sizeof(struct queue));
    
    curq->head = NULL;
    curq->tail = NULL;

    printf("ENTER THE SIZE OF THE CACHE(>1) :");
    scanf("%d",&curq->cache_size);
    if(curq->cache_size <= 1)
    {
        printf("CACHE SIZE HAS BEEN UPDATED TO 3! \n");
        curq->cache_size = 3;
    }
     
    curq->cache_cur_size = 0;
    return curq;
}

struct queue* insertq(struct node **curnode, struct queue *curqueue,char appName[])
{  
    if(curqueue->head == NULL && curqueue->tail == NULL) // empty queue
    {
        // search in the database

           int index = generate_key(appName);
           struct node *newnode = search(curnode[index],appName);
           
           if(newnode == NULL) // not in the database
           {
               printf("THE APP NAME THAT YOU'VE ENTERED IS NOT PRESENT IN THE DATABASE! \n");
               return curqueue;
           }

           struct node *permN = (struct node*) malloc(sizeof(struct node));
           permN->data = newnode->data; // copy data
           permN->next = NULL;
           permN->prev = NULL;

           curqueue->head = permN;
           curqueue->tail = permN;
           curqueue->cache_cur_size += 1;

           return curqueue;
    } 
   // else if(1){return curqueue;}
    else 
    {
       // already present 
       struct node *aux = curqueue->head;
      
       while(aux!=NULL)
       {
           if(check(aux->data.name,appName) == 1) // yes
           {
                // at beg
                if(aux == curqueue->head)
                {
                    // only one node
                    if(curqueue->head->next == NULL)
                    {
                        return curqueue;
                    }
                    // >1 node
                    else{
                        curqueue->head->next->prev = NULL;
                        curqueue->head = curqueue->head->next; // update head;
                        
                        aux->next = NULL;
                        curqueue->tail->next = aux;
                        aux->prev = curqueue->tail;
                        curqueue->tail = aux; // update tail

                        return curqueue;
                    }
                }

                // mid
                else if(aux->next!=NULL)
                {
                    struct node *aux2 = aux;
                    aux->prev->next = aux->next;
                    aux->next->prev = aux->prev;

                    aux->next = NULL;
                    aux->prev = NULL;

                    curqueue->tail->next = aux;
                    aux->prev = curqueue->tail;
                    curqueue->tail = aux;
                    return curqueue;
                }

                // at end
                else{
                    return curqueue;
                }
           }
           aux = aux->next;
       }
       
       // check for size and insert at the end + 

        int index = generate_key(appName);
        struct node *newnode = search(curnode[index],appName);
        if(newnode == NULL) // not in the database
        {
            printf("THE APP NAME THAT YOU'VE ENTERED IS NOT PRESENT IN THE DATABASE! \n");
            return curqueue;
        }
        
        struct node *permN = (struct node*) malloc(sizeof(struct node));
        permN->data = newnode->data; // copy data
        permN->next = NULL;
        permN->prev = NULL;

       
       if(curqueue->cache_cur_size == curqueue->cache_size) // remove first node
       {
           struct node *aux = curqueue->head;
           curqueue->head = curqueue->head->next;
           if(curqueue->head!=NULL){
           curqueue->head->prev = NULL;
           }           
           free(aux);
           curqueue->cache_cur_size -= 1;
       }

       curqueue->tail->next = permN;
       permN->prev = curqueue->tail;
       curqueue->tail = permN;
       curqueue->cache_cur_size += 1;
       return curqueue;
    }
}
void printLRU(struct queue *curqueue)
{
    printf("ENTER 1 TO SEE THE QUEUE FROM THE HIGHEST PRIORITY TO THE LOWEST PRIORITY \nENTER 2 TO SEE THE QUEUE FROM THE LOWEST PRIORITY TO THE HIGHEST PRIORITY \n");
    printf("HOW DO YOU WANT THE APPS IN THE QUEUE TO APPEAR ? :");
    int choice;
    scanf("%d",&choice);

     
    if(curqueue!=NULL)
    {
        if(choice == 1){
        if(curqueue->tail == NULL)
        {
            printf("THE QUEUE IS EMPTY! \n");
            return;
        }
        
          struct node *aux = curqueue->tail;
          while(aux!=NULL)
          {
            printf("[APP NAME : %s, size : %d] ",aux->data.name,aux->data.size);
            aux = aux->prev;
          }
        }
        else if(choice ==2){
           if(curqueue->head == NULL)
           {
            printf("THE QUEUE IS EMPTY! \n");
            return;
           }
        
           struct node *aux = curqueue->head;
           while(aux!=NULL)
           {
            printf("[APP NAME : %s, SIZE : %d] ",aux->data.name,aux->data.size);
            aux = aux->next;
           }
        }
        else
        {
            printf("WRONG CHOICE, WOULD YOU LIKE TO GO AGAIN(ENTER 1 IF YES)? :");
            scanf("%d",&choice);
            if(choice == 1)
            {
                printLRU(curqueue);
            }
        }
        printf("\n");
    }
    else
    {
        printf("THE QUEUE HASN'T BEEN INITIALIZED YET \n");
    }
}

int main()
{
    struct hash_table table;
    
    table.size = 7;
    table.buckets = (struct node **) malloc(table.size*sizeof(struct node*)); // array of nodes 
    
    for(int i=0;i < table.size;i++)
    {
        table.buckets[i] = NULL;
    }
     
    struct queue *newq = set_queue(newq);

    int input_value;
    printf("ENTER 1 TO INPUT VALUES : ");
    scanf("%d",&input_value);
    printf(" ENTER 1 TO INSERT APP IN THE DATABASE \n ENTER 2 TO SEARCH APP IN THE DATABASE \n ENTER 3 TO DELETE APP FROM THE DATABASE \n ENTER 4 TO PRINT THE DATABASE OF APPS \n ENTER 5 TO USE ANY APP \n ENTER 6 TO PRINT THE LRU CACHE \n");
    while(input_value)
    {
        int choice;
        printf("INPUT CHOICE: ");
        scanf("%d",&choice);
        if(choice == 1) 
        {
             struct node *newnode = set_node_data();
             int index = generate_key(newnode->data.name); // using name
             struct node *temp = insert_node(table.buckets, newnode, index);
             table.buckets[index]  = temp;
        }
        else if(choice == 2)
        {
             char app_name[20];
             printf("ENTER THE NAME OF THE APP YOU WANT TO SEARCH : ");
             scanf("%s",&app_name);
             int index = generate_key(app_name);

             struct node *found = search(table.buckets[index],app_name);
             if(found == NULL)
             {
               printf("APP NOT FOUND! \n");
             }
             else
             {
               printf("THE APP IS PRESENT IN THE DATABASE! \n");
             }
        }
        else if(choice == 3)
        {
             char app_name[20];
             printf("ENTER THE NAME OF THE APP THAT YOU WANT TO DELETE : ");
             scanf("%s",&app_name);
             int index = generate_key(app_name);
             table.buckets[index] = delete_app(table.buckets[index],app_name);
             // delete from the queue if present!
             if(newq!=NULL)
             {
                 if(newq->head!=NULL) 
                 {
                     if(newq->head->next==NULL) // only one node
                     {
                         if(check(newq->head->data.name,app_name))
                         {
                             struct node *temp = newq->head;
                             newq->head = NULL;
                             newq->tail = NULL;

                             newq->cache_cur_size = 0;
                             free(temp);
                         }
                     }
                     else // > 1 node 
                     {
                         // at beg
                         if(check(newq->head->data.name,app_name))
                         {
                             struct node *temp = newq->head;
                             newq->head = newq->head->next;
                             newq->head->prev = NULL;

                             newq->cache_cur_size -= 1;
                             free(temp);
                         }
                         else
                         {
                             struct node *trav = newq->head;
                             while(trav!=NULL && !check(trav->data.name,app_name))
                             {
                                 trav = trav->next;
                             }
                             
                         if(trav!=NULL){ // present
                           if(trav->next!=NULL)// at mid
                           {
                               trav->prev->next = trav->next;
                               trav->next->prev = trav->prev;
                               free(trav);
                           }
                           else // at end
                           {
                               newq->tail = trav->prev;
                               newq->tail->next = NULL;
                               trav->prev = NULL;
                               free(trav);
                           }
                         }
                       }
                     }
                     
                 } 
             }
        }
        else if(choice == 4)
        {
             print(table);
        }
        else if(choice == 5)
        {
             char app_name[20];
             printf("ENTER THE NAME OF THE APP THAT YOU WANT TO USE : ");
             scanf("%s",&app_name);
             newq = insertq(table.buckets,newq,app_name);
        }
        else if(choice == 6)
        {
             printLRU(newq);
        }
        else
        {
          printf("WRONG CHOICE! \n");   
        }
        printf("WOULD YOU LIKE TO GO AGAIN?(ENTER 1 IF YES):");
        scanf("%d",&input_value);
    }
}
