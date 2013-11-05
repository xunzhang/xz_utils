#include <stdio.h>
#include <stdlib.h>
#include "../memlib/xzrefcnt.h"

typedef struct _mydata {
  int refcnt;
  int field1;
  int field2;
} mydata;

void foo(mydata *data) {
  xz_ref(data);
  xz_unref(data);
}

//global pointer var
mydata *globalvar;

void store(mydata *data) {
  xz_ref(data);
  globalvar = data;
  xz_ref(data);
  xz_unref(data);
}

int main(int argc, char *argv[])
{
  mydata *tmp = (mydata *)xzmalloc(sizeof(mydata));
  tmp->refcnt = 0;
  tmp->field1 = 3;
  tmp->field2 = 7;
  foo(tmp);
  store(tmp);
  printf("%d\n", tmp->refcnt);
  return 0;
}
