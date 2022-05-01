#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

struct node;
struct br;
struct br{
    struct node * e;
    struct br * next;
};
struct node {
    int e;
    struct br * branch;
};

typedef struct node Tree;
typedef struct br Branch;

Tree* empty(int line){
    int n;
    Tree* k = (Tree*)malloc(sizeof(Tree));
    k->branch = (Branch*)malloc(sizeof(Branch));
    printf("Enter Element : ");
    scanf("%d",&(k->e));
    
    printf("Enter no of branches: ");
    scanf("%d",&n);
    Branch * m = k->branch;
    for(int i =0;i<n;i++){
        printf("%d %d\n",line+1,i+1);
        m->next = (Branch*)malloc(sizeof(Branch));
        m->next->e = empty(line+1);
        m = m->next;
    }
    m->next = NULL;
    return k;
}

void displayD(Tree* T,int line ){
    for(int i =0;i<line ;i++){
        printf("\t");
    }
    printf("%d\n",T->e);  
    Branch * k = T->branch;
    while(k->next != NULL){
        k = k->next;
        displayD(k->e,line+1);
    }
}

void findD(Tree * T ,Tree** R, int e){
    if(T->e == e){
        *R = T;
    }
    Branch* k = T->branch;
    while( k->next!= NULL ){
        k = k->next;
        findD(k->e,R,e);
    }
}

void FindPrevD(Tree* T,Tree** R,Tree* l,int e){
    if(T->e == e){
        *R = l;
    }
    Branch* k = T->branch;
    while( k->next!= NULL ){
        k = k->next;
        FindPrevD(k->e,R,T,e);
    }
}

void insert(Tree * T,int e){
    Tree * b;
    findD(T,&b,e);
    Branch* k = b->branch;
    Tree* n = empty(0);
    Branch* s = (Branch*)malloc(sizeof(Branch));
    s->e = n;
    s->next = k->next;
    k->next = s;
}

void makeEmpty(Tree * T){
    Branch* k = T->branch,*c;
    while(k!=NULL){
        c = k->next;
        if(T->branch!=k){
            makeEmpty(k->e);
        }
        free(k);
        k = c;
    }
    free(T);
}

void deleteBranch(Tree * T,int e){
    Tree* R,* G;
    findD(T,&G,e);
    FindPrevD(T,&R,NULL,e);
    Branch *k = R->branch,*c;
    while(k->next!=NULL){
        if(k->next->e->e==e){
            c = k->next;
            k->next = k->next->next;
            free(c);
            break;
        }
        k = k->next;
    }
    makeEmpty(G);
}

Tree* find(Tree* T,int e){
    Tree* k;
    findD(T,&k,e);
    return k;
}

Tree* findPrev(Tree* T,int e){
    Tree* k;
    FindPrevD(T,&k,NULL,e);
    return k;
}

void display(Tree* T){
    displayD(T,0);
}

Tree* initializeTree(){
    return empty(0);
}

int main(){
    
}