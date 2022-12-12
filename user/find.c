#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include <string.h>

// code are copied from ls.c and be adapted

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void find(char *filepath, char *filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(filepath, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", filepath);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", filepath);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_FILE:
    char *curr_filename = fmtname(filepath);
    if(strcmp(*curr_filename,filename)==0){
      printf("%s %d %d %l\n", *curr_filename, st.type, st.ino, st.size);
    }
    break;

  case T_DIR:
    if (strlen(filepath) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, filepath);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0) {
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  int i;

  if (argc < 3) {
    fprintf(2, "Usage: find filepath filename\n");
    exit(0);
  }

  char *filepath = argv[1];
  char *filename = argv[2];
  find(filepath, filename);

  exit(0);
}