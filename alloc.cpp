#include "alloc.h"

#include <iostream>

using namespace std;

lst list;
alloc_lst alt;
char* TOP;

int init()
{
	char * region = (char*) mmap(
    NULL,   // Map from the start of the 2^20th page
    PAGESIZE,                         // for one page length
    PROT_READ|PROT_WRITE|PROT_EXEC,
    MAP_ANON|MAP_PRIVATE,             // to a private block of hardware memory
    0,
    0
  );
    
    if (region == MAP_FAILED) {
        perror("Could not mmap");
        return 1;
    }
    TOP = region;
    list.chk[0].pos = region;
    return 0;

}


int cleanup()
{

    int z = munmap(TOP,PAGESIZE);
    
    TOP = NULL;
    list.nodes=1;
    list.full = false;
    for(int i=0;i<LEN;i++)
    {
        list.chk[i].size = 0;
        list.chk[i].pos = NULL;
    }

    alt.nodes = 0;
    for(int i=1;i<LEN;i++)
    {
        alt.chk[i].size = 0;
        alt.chk[i].pos = NULL;
    }

    return z;
}


char *alloc(int bufSize)
{
    
    if(list.full){ return NULL;}    // memory full
	if(bufSize%8 != 0){ return NULL;}   // not multiple of 8bytes 
    if(bufSize > PAGESIZE){ return NULL;}   // too big
    if(list.chk[0].pos == NULL){ return NULL;}  // init() not called

    int ct = 0;
    char* temp = NULL;
    chunk* arr = list.chk;
    
    while((ct < list.nodes)||(ct < LEN))   //arr[ct].pos != NULL
    {
        
        if(arr[ct].size > bufSize)
        {
            arr[ct].size = arr[ct].size - bufSize;
            temp = arr[ct].pos; 
            arr[ct].pos = arr[ct].pos + bufSize;
            

            alt.chk[alt.nodes].size = bufSize;
            alt.chk[alt.nodes].pos = temp;
            alt.nodes++;

            return temp;
        }

        else if(arr[ct].size == bufSize)   // need to remove this chunk
        {
            temp = arr[ct].pos;

            list.nodes--;
            if(list.nodes == 0){list.full = true;}

            for(int j=ct;j<LEN-1;j++)
            {
                arr[ct].pos = arr[ct+1].pos;
                arr[ct].size = arr[ct+1].size;
            }
            arr[LEN-1].pos = NULL;
            arr[LEN-1].size = 0;
            
            alt.chk[alt.nodes].size = bufSize;
            alt.chk[alt.nodes].pos = temp;
            alt.nodes++;

            return temp;
        }

        else    // Need to find a different chunk
        {
            ct++;
        }
    }

    return NULL;    // not enough space anywhere

}


void dealloc(char *memAddr)
{
	if(alt.nodes==0){ return;}    // nothing allocated at all

    for(int i=0;i<alt.nodes;i++)
    {
        if(alt.chk[i].pos == memAddr)
        {
            if(list.nodes==0){list.full=false;}
            list.chk[list.nodes].pos = memAddr;
            list.chk[list.nodes].size = alt.chk[i].size;
            list.nodes++;

            for(int j=i;j<LEN-1;j++)
            {
                alt.chk[j].pos = alt.chk[j+1].pos;
                alt.chk[j].size = alt.chk[j+1].size;
            }

            alt.chk[LEN-1].pos = NULL;
            alt.chk[LEN-1].size = 0;
            
            alt.nodes--;
            return;
        }
    }

    return; // No such address found

}
