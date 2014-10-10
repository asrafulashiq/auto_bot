
#include "ShortestPath.h"
#include <QueueList.h>


#include "Arduino.h"



using namespace std;

#define MAX_ROW 5 // row number
#define MAX_COL 10 // column number

#define MAXV 50 // vertices number = row*column



ShortestPath::ShortestPath(int a[MAX_ROW][MAX_COL]){
    g = (graph*) malloc(sizeof(graph));
    read_graph(g,a);
    
    
}

void ShortestPath::getPath(int startX,int startY,int endX,int endY,int *pX,int *pY){
    
    bfs(g, getPoint(startX,startY));
    
    find_path(getPoint(startX,startY), getPoint(endX,endY), parent,pX,pY,0);
    make_right(pX);
    make_right(pY);
    
}

int ShortestPath::getPoint(int x,int y){
    return x*MAX_COL+y;
}

int ShortestPath::getX(int point){
    return point/MAX_COL;
}

int ShortestPath::getY( int point ){
    return point%MAX_COL;
}




void ShortestPath::initialize_graph(graph *g)
{
    
    int i;                          /* counter */
    g -> nvertices = 0;
    g -> nedges = 0;
    
    for (i=1; i<=MAXV; i++) g->edges[i] = NULL;
    
}

void ShortestPath::insert_edge(graph *g,int x,int y,bool directed){
    
    edgenode *p;
    
    p = (edgenode *)malloc(sizeof(edgenode));
    p->y = y;
    p->next = g->edges[x];
    
    g->edges[x] = p;
    
}



void ShortestPath::read_graph(graph *g,int a[MAX_ROW][MAX_COL])
{
    initialize_graph(g);
    
    
    g->nvertices = MAX_ROW*MAX_COL;
    
    
    for (int i=0; i<MAX_ROW; i++) {
        for (int j=0; j<MAX_COL; j++) {
            
            if(a[i][j]==-1)continue;
            
            if (i==0 && j==0  ) {
                if(a[i+1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i+1, j)  , false);
                if(a[i][j+1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j+1)  , false);
            }
            else if (i==MAX_ROW-1 && j==0){
                if(a[i-1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i-1, j)  , false);
                if(a[i][j+1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j+1)  , false);
            }
            else if (i==0 && j==MAX_COL-1){
                if(a[i][j-1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j-1)  , false);
                if(a[i+1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i+1, j)  , false);
            }
            else if (i==MAX_ROW-1 && j==MAX_COL-1){
                if(a[i][j-1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j-1)  , false);
                if(a[i-1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i-1, j)  , false);
            }
            
            else if (i==0){
                if(a[i+1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i+1, j)  , false);
                if(a[i][j+1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j+1)  , false);
                if(a[i][j-1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j-1)  , false);
            }
            else if (i==MAX_ROW-1){
                if(a[i][j-1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j-1)  , false);
                if(a[i][j+1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j+1)  , false);
                if(a[i-1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i-1, j)  , false);
            }
            else if (j==0){
                if(a[i+1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i+1, j)  , false);
                if(a[i-1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i-1, j)  , false);
                if(a[i][j+1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j+1)  , false);
            }
            else if (j==MAX_COL-1){
                if(a[i+1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i+1, j)  , false);
                if(a[i-1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i-1, j)  , false);
                if(a[i][j-1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j-1)  , false);
            }
            else{
                if(a[i+1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i+1, j)  , false);
                if(a[i-1][j]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i-1, j)  , false);
                if(a[i][j+1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j+1)  , false);
                if(a[i][j-1]!=-1)
                    insert_edge(g, getPoint(i, j) , getPoint(i, j-1)  , false);
            }
            
        }
    }
    
    
    
}



void ShortestPath::initialize_serach(graph *g){
    
    for (int i=0; i< g->nvertices ; i++) {
        processed[i] = discovered[i] = false;
        parent[i] = -1;
    }
}





void ShortestPath::bfs(graph *g,int start){
    
    QueueList<int> q;
    
    edgenode *p;
    q.push(start);
    int v,y;
    
    while (!q.isEmpty()) {
        
        v = q.peek();
        q.pop();
        
        
        processed[v]=true;
        p =g->edges[v];
        
        while (p!=NULL) {
            
            y = p->y;
            
            if (processed[y]==false ) {
            }
            if( discovered[y]==false ){
                q.push(y);
                discovered[y] = true;
                parent[y] = v;
            }
            p = p->next;
        }
    }
    
    
}

// return path in reverse order
void ShortestPath::find_path(int start, int end, int parents[],int *a,int *b,int i)
{
    if ((start == end) || (end == -1)){
        //printf("\n(%d,%d)",getX(start),getY(start));
        a[i ] = getX(start);
        b[i ] = getY(start);
        a[++i] = -1;
        b[++i] = -1;
    }
    else {
        int tmp = parent[end];
        find_path(start,tmp,parents,a,b,i+1);
       // printf("(%d,%d)",getX(end),getY(end));
        a[i] = getX(end);
        b[i] = getY(end);
        
    }
}


void ShortestPath::make_right(int *a){
    int k;
    int i;
    
    
    for (i=0 ; ; i++) {
        if(a[i]==-1)break;
        
    }
    k = i;
    
    int ta[k+1]; //= (int *)malloc(sizeof(int)*k);
    
    i=0;
    for(int j=k-1;j >=0;j--,i++){
        ta[i]=a[j];
    }
    for(i=0;i<k;i++){
        a[i]= ta[i];
    }
    a[i]=-1;
}







