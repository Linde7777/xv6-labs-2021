#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

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
    fprintf(2, "ls: cannot open %s\n", filepath);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "ls: cannot stat %s\n", filepath);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_FILE:
    // printf("Meet File: %s ",filepath);
    printf("%s %d %d %l\n", fmtname(filepath), st.type, st.ino, st.size);
    break;

  case T_DIR:
    // printf("Meet Dir: %s ",filepath);
    if (strlen(filepath) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, filepath);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.name[0] == '.') {
        continue;
      }
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0) {
        printf("ls: cannot stat %s\n", buf);
        continue;
      }

      // printf("name:%s, type:%d",de.name,st.type);
      // printf("Enter Dir: %s ",filepath);
      // printf("filepath: %s ",filepath);
      // printf("buf: %s ", buf);
      // printf("de.name:%s ",de.name);
      // printf("\n");

      /*
      if current de.name is a, and a is a folder
      then we need to go into find(./a, filename)
      */
      if (st.type == T_DIR) {
        find(buf, filename);
      } else {
        if (strcmp(de.name, filename) == 0) {
          // printf("FIND IT!!!\n");
          // printf("It's path(buf):%s\n",buf);
          printf("%s\n", buf);
          // printf("%s %d %d %d\n", de.name, st.type, st.ino, st.size);
        }
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage: find filepath filename\n");
    exit(0);
  }

  char *filepath = argv[1];
  char *filename = argv[2];
  find(filepath, filename);

  exit(0);
}