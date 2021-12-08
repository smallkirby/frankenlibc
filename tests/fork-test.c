#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
  fprintf(stderr, "argc: %d\n", argc);
	for(int ix = 0; argv[ix] != 0; ++ix) {
    fprintf(stderr, "argv[%d]: %s\n", ix, argv[ix]);
  }
	if(argv[argc] != 0) fprintf(stderr, "[!] argv[argc] is not NULL.\n");

  puts("************************");

	int pid, ret, status;
	char buf[16] = "/";
	if (!argv[1])
		argv[1] = "hello";
	strcat(buf, argv[1]);
	const char *path = buf;
	const char *e_argv[] = {argv[1], argv[2], argv[3], argv[4], NULL};
	//const char *e_argv[] = {argv[1], argv[2], NULL};
	const char *e_envv[] = {"RUMP_VERBOSE=1", "PATH=/bin", "LKL_BOOT_CMDLINE=child=5 mem=100M virtio-pci.force_legacy=1", NULL};

	printf("parent pid=%d, ppid=%d\n", getpid(), getppid());
	pid = vfork();
	if (pid < 0) {
		perror("cannot vfork");
		return -1;
	}

	if (pid == 0) {
		// child
		printf("child pid=%d, ppid=%d\n", getpid(), getppid());
		ret = execve(path, (char *const *)e_argv,
			     (char *const *)e_envv);
		if (ret < 0)
			perror("execve error");

		/* should not reach */
		return -1;
	} else {
		// parent
		printf("parent ch pid=%d, pa pid=%d\n", pid, 0);
		/* XXX: since ld.so calls exit(main), this won't be called...  */
		if ((ret = waitid(P_PID, pid, NULL, WEXITED)) < 0) {
			perror("wait error");
			return -1;
		}
		printf("The child (pid=%d) existed with status(%d).\n",
		       pid, WEXITSTATUS(status));

	}


	return 0;
}


//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
//#include <sys/wait.h>
//#include <sys/types.h>
//#include <fcntl.h>
//#include <dirent.h>
//#include <sys/stat.h>
//#include <sys/syscall.h>
//#include <errno.h>
//
//struct linux_dirent {
//    unsigned long  d_ino;
//    off_t          d_off;
//    unsigned short d_reclen;
//    char           d_name[];
//};
//
//int ls(char *root) {
//	int fd;
//	if ((fd = open(root, O_RDONLY | O_DIRECTORY)) < 0) {
//		printf("failed to open %s.\n", root);
//		return -1;
//	}
//
//	while (1==1) {
//    char dbuf[0x100] = {0}, pbuf[0x100] = {0};
//    int n;
//    struct linux_dirent *dent = (struct linux_dirent*)dbuf;
//
//    if((n = syscall(SYS_getdents64, fd, dent, sizeof(dbuf))) < 0) {
//      printf("failed getdent64()\n");
//      return -1;
//    } else if (n == 0) 
//      break;
//
//		while ((char*)dent < dbuf + n) {
//			snprintf(pbuf, sizeof(pbuf), "%s", dent->d_name);
//			printf("%s : ", pbuf);
//			for (int ix = 0; (char)pbuf[ix] != (char)0; ++ix)
//			{
//				printf("%02x", pbuf[ix]);
//			}
//			printf("\n");
//			dent = (void *)((char *)dent + dent->d_reclen);
//		}
//	}
//
//	return 0;
//}
//
//int
//main(int argc, char *argv[])
//{
//	int pid, ret, status;
//	char buf[16] = "/hello\x00";
//	//if (!argv[1])
//	//	argv[1] = "hello";
//	const char *path = buf;
//	const char *e_argv[] = {argv[1], argv[2], argv[3], argv[4], NULL};
//	const char *e_envv[] = {"RUMP_VERBOSE=1", "PATH=/bin", "LKL_BOOT_CMDLINE=child=5 mem=100M virtio-pci.force_legacy=1", NULL};
//
//	ls("/");
//	int fd = open("/flag", O_RDONLY);
//	if (fd < 0) {
//		printf("failed to open flag\n");
//		return 1;
//	}
//	char rbuf[0x100];
//	int rn = read(fd, rbuf, 20);
//	fprintf(stderr, "read fin %d bytes\n", rn);
//	fprintf(stderr, "content: %s\n", rbuf);
//	fprintf(stderr, "closing\n");
//	close(fd);
//
//	fprintf(stderr, "parent pid=%d, ppid=%d\n", getpid(), getppid());
//	fprintf(stderr, "trying to exexute: %s.\n", path);
//	 pid = vfork();
//	if (pid < 0) {
//		perror("cannot vfork");
//		return -1;
//	}
//
//	if (pid == 0) {
//		//// child
//		/** child should not call any funcs **/
//		//fprintf(stderr, "child pid=%d, ppid=%d\n", getpid(), getppid());
//		//fprintf(stderr, "trying to execve: %s\n", path);
//		//fflush(stdin);
//
//		// execve seems evil.
//		ret = execve(path, (char *const *)e_argv,
//								 (char *const *)e_envv);
//		//_exit(1);
//		if (ret < 0)
//			perror("execve error");
//
//		/* should not reach */
//		return -1;
//	} else {
//		// parent
//		fprintf(stderr, "parent ch pid=%d, pa pid=%d\n", pid, 0);
//		/* XXX: since ld.so calls exit(main), this won't be called...  */
//		if ((ret = waitid(P_PID, pid, NULL, WEXITED)) < 0) {
//			perror("wait error");
//			return -1;
//		}
//		fprintf(stderr, "The child (pid=%d) existed with status(%d).\n",
//		       pid, WEXITSTATUS(status));
//
//	}
//
//
//	return 0;
//}
//