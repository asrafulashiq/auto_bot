
#include <iostream>
#include "ShortestPath.h"


#define MAX_ROW 5 // row number
#define MAX_COL 10 // column number



void init_array(int a[MAX_ROW][MAX_COL]){
    
    /*
     *  0  = none
     *  1  = object
     * -1  = obstacle
     */
    
    for (int i=0; i<MAX_ROW; i++) {
        for (int j=0; j<MAX_COL; j++) {
            a[i][j]=0;
        }
    }
    
    a[1][2] = 1;
    a[0][4] = 1;
    
    a[0][2] = a[1][1] = a[2][2] = a[2][3] = -1;
    
}



int main(){
    int a[MAX_ROW][MAX_COL];
    init_array(a);
    
    ShortestPath path(a);
    
    int p[50];
    int q[50];
    
    path.getPath(1,2,3,4,p,q);
    
    for(int i=0;i<50;i++){

        printf("(%d,%d)",p[i],q[i]);
        if(p[i]==3 && q[i]==4)break;
    }
    
    
    

}
