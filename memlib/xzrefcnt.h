#ifndef XZREFCNT_HPP
#define XZREFCNT_HPP

#include <stdlib.h>
#include "xzmalloc.h"

typedef struct _refcnt {
  int cnt;
} refcnt;

void xz_ref(void *data) {
  refcnt *ref;
  ref = (refcnt *)data;
  ref->cnt++;
}

void xz_unref(void *data) {
  refcnt *ref;
  ref->cnt--;
  if(ref->cnt == 0) {
    xzfree(ref);
  }
}

#endif
