#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it
#define LEN (PAGESIZE/MINALLOC) 
// You can declare any data structures required here

struct chunk{
    
    char* pos;
    int size;

    chunk(){
        pos = NULL;
        size = 0;
    }

};

class lst{

    public:
    
    int nodes;
    chunk chk[LEN];
    bool full;
    
    lst(){
        nodes = 1;
        full = false;
        chk[0].size = PAGESIZE;

        for(int i=1;i<LEN;i++)
        {
            chk[i].size = 0;
            chk[i].pos = NULL;
        }
    }

}; 

class alloc_lst{

    public:
    
    int nodes;
    chunk chk[LEN];
    
    alloc_lst(){
        for(int i=0;i<LEN;i++)
        {
            chk[i].size = 0;
            chk[i].pos = NULL;
        }
    }

};



int init();
int cleanup();
char *alloc(int);
void dealloc(char *);
