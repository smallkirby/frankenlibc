#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

/*****

ls.c

list file/dirs in root directory non recursively.

******/

int main(int argc, char *argv[]) {
  DIR *dir;
  struct dirent *dent;

  if ((dir = opendir("/")) == NULL) {
    fprintf(stderr, "failed to open root dir\n");
    return 1;
  }

  while ((dent = readdir(dir)) != NULL) {
    if ((dent->d_name[0] == '.' && dent->d_name[1] == 0) ||
        (dent->d_name[0] == '.' && dent->d_name[1] == '.')) {
      continue;
    }
    printf("%s\n", dent->d_name);
  }

  closedir(dir);
  return 0;
}