#include <unistd.h> //sbrk system call

int init_flag = 0;
void *buf_bnd;
void *buf_srt;

typedef struct xz_mem_blk {
  short avaliable;
  long sz;
} mem_blk;

void init_xzmalloc() {
  buf_bnd = sbrk(0);
  buf_srt = buf_bnd;
  init_flag = 1;
}

void xzfree(void *p) {
  mem_blk *head;
  head = (mem_blk *)(p - sizeof(mem_blk));
  head->avaliable = 1;
  return;
}

void *xzmalloc(long nbytes) {
  void *location;
  mem_blk *local_offset;
  nbytes = nbytes + sizeof(mem_blk);
  
  // first malloc
  if(!init_flag) init_xzmalloc(); 

  // make sure after init 
  location = 0;
  local_offset = (mem_blk *)buf_srt;

  while(local_offset != (mem_blk *)buf_bnd) {
    if(local_offset->avaliable) {
      if(local_offset->sz >= nbytes) {
        location = (void *)local_offset;
        local_offset->avaliable = 0;
	//local_offset->sz = nbytes;
        break; 
      }
    }
    local_offset += local_offset->sz;
  } 
  if(!location) {
    sbrk(nbytes);
    location = buf_bnd; 
    buf_bnd += nbytes;
    local_offset = location;
    local_offset->avaliable = 0;
    local_offset->sz = nbytes;
  } 

  location += sizeof(mem_blk);
  return location;
}

int main(void)
{
  int *a;
  a = (int *)xzmalloc(10 * sizeof(int));
  xzfree(a);
  char *b;
  b = (char *)xzmalloc(20 * sizeof(char));
  xzfree(b);
  return 0;
}
