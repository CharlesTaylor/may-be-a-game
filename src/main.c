#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <unistd.h>
//typedef enum Direction{  } Direction;


typedef struct Item{
	int8_t rootx;
	int8_t rooty;
	char name[32];
	uint8_t dimx;
	uint8_t dimy;
	uint32_t id;
	char description[256];
} Item;

typedef struct Inventory{
#define X 12
#define Y 8
	Item *items[Y][X];
} Inventory;

typedef struct Character{
	int8_t x;
	int8_t y;
	char symbol;
	Item *grabbed;
} Character;

int grab( Character *chr, Inventory *iptr,int x, int y){
	
	Item *item = iptr->items[y][x];
	
	if(item==NULL){
		return 0;
	}
	
	chr->grabbed = item;
	int i,j;
	for(i=0;i<item->dimx;i++){
		for(j=0;j<item->dimy;j++){
			iptr->items[item->rooty+j][item->rootx+i] = NULL;
		}
	}
	item->rootx=-1;
	item->rooty=-1;
	return item->id;
}


void move(Character *c, int8_t x, int8_t y ){
	c->x= c->x+x>=0?c->x+x:0;
	c->y= c->y+y>=0?c->y+y:0;
}

int drop(Character *chr, Inventory *iptr, int x, int y){
	if(iptr==NULL){
		//DROP TO GROUND
		chr->grabbed = NULL;
	}
	if(chr->grabbed==NULL){
		//Nothing to drop
		return -2;
	}
	Item *temp = chr->grabbed;
	int i,j;
	uint32_t tid = -1;
	for(i=0;i<temp->dimy;i++){
		for(j=0;j<temp->dimx;j++){
			if( iptr->items[y+i][x+j] != NULL){
				if (tid==-1){
					tid = iptr->items[y+i][x+j]->id;
				}
				else if( tid!=iptr->items[y+i][x+j]->id){
					return -1;
				}
			}

		}
	}

	//HANDLE REGRAB and fail drop
	if(tid !=-1){
		
		chr->grabbed = iptr->items[y][x];
		//chr->grabbed->rootx=-1;
		//chr->grabbed->rooty=-1;
		
		for(i=0;i<chr->grabbed->dimy;i++){
			for(j=0;j<chr->grabbed->dimx;j++){
				iptr->items[chr->grabbed->rooty+i][chr->grabbed->rootx+j]=NULL;
			}	
		}	
	}
	else{
		chr->grabbed=NULL;
	}

	for(i=0;i<temp->dimy;i++){
		for(j=0;j<temp->dimx;j++){
			iptr->items[y+i][x+j]=temp;
		}
	}


	temp->rootx = x;
	temp->rooty = y;
	return 0;
}

//Function Test main
int main(int argc, char** argv){

	Item *titem = (Item *) malloc(sizeof(Item));	
	titem->id=99;
	sprintf(titem->name,"Loltester");
	sprintf(titem->description,"Description here");	
	titem->dimx = 2;
	titem->dimy = 3;	
	Character *ch = (Character *)malloc(sizeof(Character));
	ch->x=1;
	ch->y=1;
	ch->symbol ='#';
	ch->grabbed = titem;
	Inventory *inv = (Inventory *) malloc(sizeof(Inventory));
	int i,j;
	for(i=0;i<Y;i++){
		for(j=0;j<X;j++){
			inv->items[i][j]=NULL;
		}
	}
	if(drop(ch,inv,0,0)){
		printf("Drop Failed\n");
	}

	titem = (Item *) malloc(sizeof(Item));
	titem->id=88;
	titem->dimx=2;
	titem->dimy=2;
	sprintf(titem->name,"Lolssssstester");
	sprintf(titem->description,"Deasdascription here");	
	ch->grabbed = titem;	
	if(drop(ch,inv,1,1)){
		printf("Drop Failed\n");
	}
	if(drop(ch,inv,4,4)){
		printf("Drop Failed\n");
	}
	grab(ch,inv,4,4);
	drop(ch,inv,1,3);
	titem = (Item *) malloc(sizeof(Item));
	titem->id=77;
	titem->dimx=1;
	titem->dimy=4;
	sprintf(titem->name,"sssLsolssssstester");
	sprintf(titem->description,"Daaeasdascription here");	
	ch->grabbed = titem;	
	if(drop(ch,inv,5,0)){
		printf("Drop Failed\n");
	}
	if(drop(ch,inv,4,4)){
		printf("Drop Failed\n");
	}
	for(i=0;i<Y;i++){
		for(j=0;j<X;j++){
			if(inv->items[i][j]!=NULL){
				printf("%d\t",inv->items[i][j]->id);
			}
			else{
				printf("null\t");
			}
		}
		printf("\n");
	}

	/*
	int8_t xx,yy,desx,desy;
	int8_t xp,yp;
	
	struct winsize w;

	int8_t movingx= 0;
	int8_t movingy= 0;
	while(1){
	
		nanosleep((const struct timespec[]){{0, 50000000L}}, NULL);

		ioctl(STDOUT_FILENO,TIOCGWINSZ, &w);
		printf("/%d-%d", ch->x,ch->y);	
		for(yp=0;yp<ch->y;yp++){
			printf("\n");
		}
		for(xp=0;xp<ch->x;xp++){
			printf(" ");
		}
		printf("%c",ch->symbol);
		for(;yp<w.ws_row-1;yp++){
			printf("\n");
	}
		if(!(movingx||movingy)){
			scanf("%d,%d",&xx,&yy);
			desx = xx+ch->x;
			desy = yy+ch->y;
			movingx= 1;
			movingy= 1;
		}

		if(ch->x== desx)
			movingx = 0;
		else
			move(ch,xx/abs(xx),0);
		if( ch->y == desy )
			movingy = 0;
		else
			move(ch,0,yy/abs(yy));

	}
	*/
	return 0;
}
