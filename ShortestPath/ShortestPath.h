#ifndef SHORTEST_H
#define SHORTEST_H

#include "Arduino.h"

#define MAX_ROW 5 // row number
#define MAX_COL 10 // column number

#define MAXV 50 // vertices number = row*column

class ShortestPath{
	
public:
	
	ShortestPath(int a[MAX_ROW][MAX_COL]);
	
	void getPath(int startX,int startY,int endX,int endY,int *pX,int *pY);
	

	
private:
    
    
    typedef struct Edge{
        int y;
        struct Edge *next;
    } edgenode;
    
    typedef struct {
        edgenode *edges[MAXV+1];
        int nvertices;
        int nedges;
    } graph;
    
    graph *g;
    
    int getPoint(int x,int y);
    int getX(int point);
    int getY( int point );
    void initialize_graph(graph *g);
    void insert_edge(graph *g,int x,int y,bool directed);
    
    void read_graph(graph *g,int a[MAX_ROW][MAX_COL]);
    
    bool processed[MAXV];
    bool discovered[MAXV];
    int parent[MAXV];
    
    void initialize_serach(graph *g);
    void bfs(graph *g,int start);
    
    void find_path(int start, int end, int parents[],int *a,int *b,int i);
    void make_right(int *a);
    
    
    



};



#endif