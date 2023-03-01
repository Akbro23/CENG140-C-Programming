#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct QueryNode{
    char *query;
    struct QueryNode *prev;
    struct QueryNode *next;  
} QueryNode;

typedef struct CountNode{
    int count;
    struct CountNode *prev;
    struct CountNode *next;
    QueryNode** querylist;
} CountNode;

QueryNode* makeQueryNode(char* query);
void appendQueryNode(QueryNode** querylist, QueryNode* queryNode);
int popQueryNode(CountNode **head, QueryNode* queryNode);

void initCountList(CountNode **head, QueryNode *queryTable[MAX_SIZE]);
CountNode* appendOrGetCountNode(CountNode **head, int newVal);
void popCountNode(CountNode **head, CountNode* countNode);
void printCountNode(CountNode *head);

void printQueryTable(QueryNode *queryTable[MAX_SIZE]);

void processQueries(CountNode **head, QueryNode *queryTable[MAX_SIZE]);

int main(){
    CountNode *head;
    QueryNode *queryTable[MAX_SIZE] = {0};
    head = NULL;
    initCountList(&head, queryTable);
    processQueries(&head, queryTable);
    printCountNode(head);
    return 0;
}

QueryNode* makeQueryNode(char* query){
    QueryNode* queryNode;
    queryNode = (QueryNode *)malloc(sizeof(QueryNode));
    queryNode -> query = query;
    queryNode -> next = NULL;
    queryNode -> prev = NULL;
    return queryNode;
}

void appendQueryNode(QueryNode **querylist, QueryNode* queryNode){
    if (!(*querylist))
        *querylist = queryNode;

    else if (strcmp(queryNode -> query, (*querylist) -> query) < 0){
        queryNode -> next = *querylist;
        (*querylist) -> prev = queryNode;
        *querylist = queryNode;
    }
    else{
        QueryNode* curNode;
        curNode = *querylist;
        while (curNode -> next && strcmp(queryNode -> query, curNode -> next -> query) > 0)
            curNode = curNode -> next;
        queryNode -> next = curNode -> next;
        queryNode -> prev = curNode;
        if (curNode -> next)
            curNode -> next -> prev = queryNode;
        curNode -> next = queryNode;
    }
}

int popQueryNode(CountNode **head, QueryNode* queryNode){
    int oldVal, found = 0;
    CountNode *countNode;
    QueryNode **querylist, *curQueryNode;
    countNode = *head;
    while (countNode){
        querylist = countNode -> querylist;            
        curQueryNode = *querylist;
        while (curQueryNode){
            if (strcmp(curQueryNode -> query, queryNode -> query) == 0){
                if (!queryNode -> prev)
                    *querylist = queryNode -> next;
                else
                    queryNode -> prev -> next = queryNode -> next;
                if (queryNode -> next)
                    queryNode -> next -> prev = queryNode -> prev;
                queryNode -> next = NULL;
                queryNode -> prev = NULL;
                found = 1;
                break;
            }
            curQueryNode = curQueryNode -> next;
        }
        if (found)
            break;
        countNode = countNode -> next;
    }
    oldVal = countNode -> count;
    if (!(*querylist))
        popCountNode(head, countNode);

    return oldVal;
}

void initCountList(CountNode **head, QueryNode *queryTable[MAX_SIZE]){
    int n, i = 0;
    int ptr = 0;
    int count;
    char *query, ch;
    CountNode *countNode;
    QueryNode *queryNode;

    scanf("%d\n", &n);
    while (n--){             
        i = 0;
        query = (char *)malloc(sizeof(char));
        while (ch = getchar(), ch != ' ' && ch != EOF){
            query[i++] = ch;
            query = (char *)realloc(query, (i + 1)*sizeof(char));
        } 
        query[i] = '\0';
        scanf("%d\n", &count);        
        countNode = appendOrGetCountNode(head, count);
        
        queryNode = makeQueryNode(query);      
        appendQueryNode(countNode -> querylist, queryNode);
                
        queryTable[i++] = queryNode;
    }
}

CountNode* appendOrGetCountNode(CountNode **head, int newVal){
    CountNode *newNode;
    QueryNode **querylist;
    newNode = (CountNode *)malloc(sizeof(CountNode));  
    querylist = (QueryNode **)malloc(sizeof(QueryNode *));
    *querylist = NULL;
    newNode -> count = newVal;
    newNode -> prev = NULL;
    newNode -> next = NULL;
    newNode -> querylist = querylist;  
    if (!*head){                     
        *head = newNode;        
        return newNode;
    }    
    else if ((*head) -> count == newVal){
        free(newNode -> querylist);
        free(newNode);
        return *head;
    }
    else if (newVal < (*head) -> count){      
        newNode -> next = *head;
        (*head) -> prev = newNode;
        *head = newNode;
        return newNode;
    }
    else { 
        CountNode *curNode;
        curNode = *head;       
        while (curNode -> next && newVal > curNode -> next -> count){
            curNode = curNode -> next;
        }
        if (curNode -> next && newVal == curNode -> next -> count){
            free(newNode -> querylist);
            free(newNode);
            return curNode -> next;
        }
        else {
            newNode -> next = curNode -> next;            
            newNode -> prev = curNode;
            if (curNode -> next)
                curNode -> next -> prev = newNode;  
            curNode -> next = newNode;  
            return newNode;
        }
    }
}

void popCountNode(CountNode **head, CountNode* countNode){
    if (!countNode -> prev)
        *head = countNode -> next;
    else
        countNode -> prev -> next = countNode -> next;   
    if (countNode -> next)
        countNode -> next -> prev = countNode -> prev;
    free(countNode);      
}

void printCountNode(CountNode *head){
    CountNode *curCountNode;
    QueryNode* str;
    curCountNode = head;
    while (curCountNode){
        printf("%d", curCountNode -> count);
        str = *(curCountNode -> querylist);    
        while (str){
            printf(" %s", str -> query);
            str = str -> next;
        }
        printf("\n");
        curCountNode = curCountNode -> next;
    }
}

void processQueries(CountNode **head, QueryNode *queryTable[MAX_SIZE]){
    int n, i;
    char *query, ch;
    CountNode *countNode;
    QueryNode *queryNode;

    scanf("%d\n", &n);
    while (n--){
        i = 0;
        query = (char *)malloc(sizeof(char));
        while (ch = getchar(), ch != ' ' && ch != EOF){
            query[i++] = ch;
            query = (char *)realloc(query, (i + 1)*sizeof(char));
        } 
        query[i] = '\0';

        for (i = 0; i < MAX_SIZE && queryTable[i]; i++){
            if (strcmp(query, queryTable[i] -> query) == 0)
                break;
        }
        
        if (i < MAX_SIZE){  
            if (!queryTable[i]){
                countNode = appendOrGetCountNode(head, 1);
                queryNode = makeQueryNode(query);
            }
            else {
                int newVal;       
                newVal = popQueryNode(head, queryTable[i]) + 1;
                countNode = appendOrGetCountNode(head, newVal);
                queryNode = queryTable[i];
                free(query);
            }
        }
        else {
            int blob;
            char *oldQuery;
            QueryNode *firstQueryNode;          
            firstQueryNode = *((*head) -> querylist);
            oldQuery = firstQueryNode -> query;
            blob = popQueryNode(head, firstQueryNode);
            queryNode = makeQueryNode(query);
            countNode = appendOrGetCountNode(head, 1);
            for (i = 0; i < MAX_SIZE; i++){
                if (strcmp(oldQuery, queryTable[i] -> query) == 0)
                    break;
            }
            free(oldQuery);
            free(firstQueryNode);
        } 
        queryTable[i] = queryNode;   
        appendQueryNode(countNode -> querylist, queryNode);        
    }   
}