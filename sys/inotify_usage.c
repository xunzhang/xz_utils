#include <sys/inotify.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const char *watching_file[] = {
  "/mfs/user/wuhong/tmp/c.cc",
  "/mfs/user/wuhong/tmp/str/"
};

#define EVENT_SZ 28

const char *events[] = {
  "File was accessed",		/* 0x00000001 */
  "File was modified",		/* 0x00000002 */
  "Metadata changed",		/* 0x00000004 */
  "Writtable file was closed",	/* 0x00000008 */
  "Unwrittable file closed",	/* 0x00000010 */
  "File was opened", 		/* 0x00000020 */
  "File was moved from X",	/* 0x00000040 */
  "File was moved to Y",	/* 0x00000080 */
  "Subfile was created",	/* 0x00000100 */
  "Subfile was deleted",	/* 0x00000200 */
  "Self was deleted",		/* 0x00000400 */
  "Self was moved",		/* 0x00000800 */
  "",				
  "Backing fs was unmounted",	/* 0x00002000 */
  "Event queued overflowed",	/* 0x00004000 */
  "File was ignored",		/* 0x00008000 */
  "",
  "",
  "",
  "",
  "Only watch the path if it is a directory",		/* 0x01000000 */
  "Do not follow a sym link",				/* 0x02000000 */
  "Exclude events on unlinked objects",			/* 0x04000000 */
  "",
  "",
  "Add to the mask of an already existing watch",	/* 0x20000000 */
  "Event occurred against dir",				/* 0x40000000 */
  "Only send event once"				/* 0x80000000 */
};

#define SZ 1024

int main(void)
{
  char buffer[SZ];
  char *offset;
  struct inotify_event *evt;
  int wd_list[2];
  int i, len;

  // init inotify fd
  int fd = inotify_init();
  if(fd < 0) {
    printf("Fail to initialize inotify.\n");
    exit(-1);
  }

  // create watch des
  for(i = 0; i < 2; ++i) {
    wd_list[i] = inotify_add_watch(fd, watching_file[i], IN_ALL_EVENTS);
    if(wd_list[i] < 0) {
      printf("Can not add watch for %s.\n", watching_file[i]);
      exit(-1);
    }
    printf("Watching file/directory %s\n", watching_file[i]);
  }
  
  // listening all the time
  while(len = read(fd, buffer, SZ)) {
    printf("Some event(s) happening with length %d.\n", len);
    offset = buffer;
    evt = (struct inotify_event *)buffer;
    // >1 evts in 1 read
    while(((char *)evt - buffer) < len) {
      
      // object type 
      if((evt->mask & IN_ISDIR)) {
        printf("Object type is Directory.\n");
      } else {
        printf("Object type is File.\n");
      }
      
      // object name
      for(i = 0; i < 2; ++i) {
        if(evt->wd != wd_list[i]) continue;
	printf("Object name is %s\n", watching_file[i]);
	break;
      }

      printf("Event mask is %08X\n", evt->mask);
      for(i = 0; i < EVENT_SZ; ++i) {
	 if(evt->mask & (1 << i)) {
	   printf("Event: %s.\n", events[i]);
	 }
      }
      int tmp_sz = sizeof(struct inotify_event) + evt->len;
      evt = (struct inotify_event *)(offset + tmp_sz);
      offset += tmp_sz; 
    } // evt
  } // listening

  return 0;
}
