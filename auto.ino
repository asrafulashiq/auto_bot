#include <ShortestPath.h>
#include <EEPROM.h>
#include <QueueList.h>

#define MAX_ROW 5 // row number
#define MAX_COL 10 // column number

#define MAXV 50 // vertices number = row*column

boolean dry_run ;
boolean main_run;

int p[5][10];

int forward_status; //  status for forward obstacle

boolean touch_block_node;

int ir1,ir2,ir3,ir4,ir5,ir6; // pins for ir sensors

enum Dir{NORTH,SOUTH,EAST,WEST};

Dir direction;

/*
*  1 = hole
* -1 = block_node
*  0 = node
*  2 = block position
*/

int current_pos_x;
int current_pos_y;

int dry_run_switch ; // while clicked , auto will start dry run
int main_run_switch ; // while clicked , auto will start main run




void setup(){
  
  Serial.begin(9600);
  
  pinMode(dry_run_switch,INPUT_PULLUP);
  pinMode(main_run_switch,INPUT_PULLUP);
  
  
  
}

void loop(){
  
  
  if(digitalRead(dry_run_switch)==LOW){
    if(dry_run==false)start_dry_run();
    if(dry_run==true){
 
  if(current_pos_x==4 && current_pos_y==8){
    // end dry run
    end_dry_run();
    return;
  }
  
  forward_status = go_forward();
  
  if( current_pos_y== -1 ){
    // start dry run .. go forward.. 
    if(forward_status==0){
      set_current_pos(0,0);
      turn_right();
      direction = NORTH;
      return;
    }
    
  }
  
  
  if(direction==NORTH){
    set_current_pos(current_pos_x+1,current_pos_y);
  }
  else if(direction==SOUTH){
     set_current_pos(current_pos_x-1,current_pos_y );   
  }
  
  else if(direction==WEST){
     set_current_pos(current_pos_x,current_pos_y+1);
  }
  
  p[current_pos_x][current_pos_y] = forward_status;

  
  if(current_pos_x==3 && direction==NORTH){
    

    turn_left();
    direction = WEST;
    
  }
  else if(current_pos_x==4 && direction==WEST){

    turn_left();  
    direction = SOUTH;
  }
  
  
  else if( current_pos_x==1 && direction == SOUTH ){
    
    turn_right();
    direction = WEST;
  }
  
  else if(current_pos_x==0 && direction==WEST){

    turn_right();
    direction = NORTH;    
  }
  
  
  }
  }
  
  else if( digitalRead(main_run_switch)==LOW ){
    
    main_run_fun();
    
  }
  
}

/*
* function for main run
*/

void main_run_fun(){

}


/*
* function for forward motion of the auto and continously checking node with IR.
* return node status
*/
int go_forward(){
  
  // code for the auto to go forward
 
  while(true){
   
      if( get_ir(ir1)==0 && 
          get_ir(ir4)==0 &&
          get_ir(ir2)==1 &&
          get_ir(ir3)==1){
          
            // on the line
            continue;
            
          }
       else if(
          get_ir(ir1)==1 && 
          get_ir(ir4)==1 &&
          get_ir(ir2)==1 &&
          get_ir(ir3)==1
       ){
         // on the node
         return 0;
       }
       else if(
          get_ir(ir1)==1 && 
          get_ir(ir4)==1 &&
          get_ir(ir2)==0 &&
          get_ir(ir3)==0
       ){
         if(
           get_ir(ir5)==1 &&
           get_ir(ir6)==1
         ){
           // on the box
            escape_box();
            touch_block_node=true;
            return 2; 
         } 
         // on the block node
         else if(get_ir(ir5)==0 &&
           get_ir(ir6)==0) return -1;
       }
       
        
  }
  
}

void turn_left(){

}

void turn_right(){
  
}

void set_current_pos(int x,int y){
  current_pos_x = x;
  current_pos_y = y;
}

void start_dry_run(){
  dry_run = true;
  direction = WEST;
  current_pos_x = 0;
  current_pos_y = -1;
  
  for(int i=0;i<5;i++){
    for(int j=0;j<10;j++){
     p[i][j] = 0; 
    }
  }
}

void end_dry_run(){
  dry_run = false;
  p[4][3] = p[2][10] = p[3][10] = 1; // hard-code position for hole  
  
  int pos = 0;  // position for eeprom write
  
  
  // EEPROM write for the arduino
  
  for(int i=0;i<MAX_ROW;i++){
    for(int j=0;j<MAX_COL;j++){
      Serial.write(p[i][j]);
      EEPROM.write(p[i][j],pos);
      pos = pos+1;
    } 
  }
    
    
  
  
  
}

int get_ir(int ir_pin){
  
  int val = analogRead(ir_pin);
  
  if(val>500){
    return 0; // black
  }
  else{
     return 1; // white 
  }
  
}

/*int getPoint(int x,int y){
    return x*MAX_COL+y;
}


int getX(int point){
    return point/MAX_COL;
}

int getY( int point ){
    return point%MAX_COL;
}*/




void escape_box(){
  
  while(true){
    
    if(
      get_ir(ir1)==1&&
      get_ir(ir2)==1&&
      get_ir(ir3)==0&&
      get_ir(ir4)==0&&
      get_ir(ir5)==0&&
      get_ir(ir6)==0
    )  return;
  
  }
  
}

