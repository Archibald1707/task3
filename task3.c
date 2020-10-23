#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int stdStep = 10;

typedef struct list
{
    char *str;
    struct list *next;
}listOfString;

char *stringInput()
{
    int length = sizeof(char) * stdStep;
    size_t stringLength;
    int position = 0;

    char *string = malloc(sizeof(char) * stdStep);
    if (!string)
    {
        printf("Error: wrong memory allocation!");
        exit(-1);
    }

    while(fgets(string + position, stdStep, stdin)&&(strcmp(string, "exit\n") != 0))
    {
        stringLength = strlen(string);
        if(string[stringLength - 1] != '\n')
        {
            position = stringLength;
            length += sizeof(char) * stdStep;
            string = realloc(string, length);
            if(!string)
            {
                printf("Error: wrong memory reallocation!\n");
                exit(-1);
            }
        }
        else
        {
            string[stringLength - 1] = '\0';
            return string;
        }
    }

    free(string);

    return NULL;
}

void newListElement(char *string, listOfString **head)
{
    listOfString *newUnit = malloc(sizeof(listOfString));
    newUnit->str = string;
    newUnit->next = *head;
    *head = newUnit;
}

void substitution(listOfString ***newList, listOfString **list)
{
        (**newList)->next = *list;
        (**newList) = (**newList)->next;
        *list = (*list)->next;
}

void merge(listOfString *list1, listOfString *list2, listOfString **newList)
{
    listOfString temporary;
    *newList = NULL;
    if(list1 == NULL)
    {
        *newList = list2;
        return;
    }
    if(list2 == NULL)
    {
        *newList = list1;
        return;
    }
    if(strcmp(list1->str, list2->str)<0)
    {
        *newList = list1;
        list1 = list1->next;
    }
    else
    {
        *newList = list2;
        list2 = list2->next;
    }
    temporary.next = *newList;
    while(list1 && list2)
    {
        if (strcmp(list1->str, list2->str) < 0)
        {
            (*newList)->next = list1;
            list1 = list1->next;
        }
        else
        {
            (*newList)->next = list2;
            list2 = list2->next;
        }
        (*newList) = (*newList)->next;
    }
    while(list1)
        substitution(&newList, &list1);        
    while(list2)
        substitution(&newList, &list2);
    *newList = temporary.next;
}

void separation(listOfString *source, listOfString **low, listOfString **high)
{
    listOfString *fast = NULL;
    listOfString *slow = NULL;
    if((source == NULL)||(source->next == NULL))
    {
        (*low) = source;
        (*high) = NULL;
        return;
    }
    slow = source;
    fast = source->next;
    while(fast != NULL)
    {
        fast = fast->next;
        if(fast != NULL)
        {
            fast = fast->next;
            slow = slow->next;
        }
    }
    (*low) = source;
    (*high) = slow->next;
    slow->next = NULL;
}

void mergeSort(listOfString **head)
{
    listOfString *low = NULL;
    listOfString *high = NULL;
    if((*head == NULL)||((*head)->next == NULL))
        return;
    separation(*head, &low, &high);
    mergeSort(&low);
    mergeSort(&high);
    merge(low, high, head);
}

void printLinkedList(listOfString *head) 
{
    int i = 0;
    printf("\n");
    while (head)
    {
        while(((head->str)[i])!='\0')
        {
            printf("%c", (head->str)[i]);
            ++i;
        }
        printf("\n");
        head = head->next;
        i = 0;
    }
    printf("\n");
}

void memoryFree(listOfString **head) 
{
    listOfString *previous = NULL;
    while((*head)->next) 
    {
        previous = (*head);
        (*head) = (*head)->next;
        free(previous);
    }
    free(*head);
    return;
}

int main() 
{
    char *temporaryString;
    char programWorking = 'Y';
    while(programWorking == 'Y')
    {
        listOfString *head = NULL;
        while((temporaryString = stringInput()))
        {
            newListElement(temporaryString, &head);		/* type 'exit' to stop input stream */
        }
        mergeSort(&head);
        printLinkedList(head);
        memoryFree(&head);
        printf("Do you want to try one more time? Y/n?\n");
        scanf("%c\n", &programWorking);
    }
    return 0;
}
