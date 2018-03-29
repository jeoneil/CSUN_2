/*
Jack O'Neil
19 October, 2016
COMP 322
Lab 3: File Permissions and Directory Information
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>

void make_children(int argc, int *parent, int *cp) {
	pid_t 	mypid;
	int i = 1;
	*parent = 1;
	*cp = -1;
	
	/* parent will run though this loop creating children */
	/* children will exit out */
	while (*parent == 1 && i < argc) {
		mypid = fork();
		if (mypid < 0) {
			/*error*/
			perror("Fork failed\n");
			exit(1);
		}
		if (mypid == 0) {
			/*child process*/
			*parent = 0;
			*cp = i;
		}
		else i++;
	}
}

int get_perms_group(struct stat stat_buf, char **group_str) {
	/* owner, group, or general permissions? */
	if (stat_buf.st_uid == getuid()) {
		*group_str = "owner";
		return 0;
	}
	else if (stat_buf.st_gid == getgid()) {
		*group_str = "group";
		return 1;
	} else {
		*group_str = "general";
		return 2;
	}
}

/* read, write, execute permissions ?*/
void get_perms(int group, struct stat stat_buf, char **r_str, char **w_str, char **x_str) {
	/* read, write, execute permissions ?*/
	if (group == 0) {
		if (stat_buf.st_mode & S_IRUSR)
			*r_str = "read ";
		if (stat_buf.st_mode & S_IWUSR)
			*w_str = "write ";
		if (stat_buf.st_mode & S_IXUSR)
			*x_str = "execute ";
	} else if (group == 1) {
		if (stat_buf.st_mode & S_IRGRP)
			*r_str = "read ";
		if (stat_buf.st_mode & S_IWGRP)
			*w_str = "write ";
		if (stat_buf.st_mode & S_IXGRP)
			*x_str = "execute ";
	} else {
		if (stat_buf.st_mode & S_IROTH)
			*r_str = "read ";
		if (stat_buf.st_mode & S_IWOTH)
			*w_str = "write ";
		if (stat_buf.st_mode & S_IXOTH)
			*x_str = "execute ";
	}
}

void print_info(int argc, char **argv, int cp) {
	struct passwd 	pwd, *result;
	struct stat 	stat_buf;
    size_t 	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	char* 	r_str = "";
	char* 	w_str = "";
	char* 	x_str = "";
	char*	group_str = "";
	char*	buf = malloc(bufsize);
	int		group = 0;
	
	/* update structs */
	getpwuid_r(getuid(), &pwd, buf, bufsize, &result);
	stat(argv[cp], &stat_buf);
	
	/* update strings with info */
	group = get_perms_group(stat_buf, &group_str);
	get_perms(group, stat_buf, &r_str, &w_str, &x_str);
	
	/* print out info */
	printf("File: %s\nDirectory: %s\nYou have %s permissions: %s%s%s\n\n", 
		argv[cp], pwd.pw_dir, group_str, r_str, w_str, x_str);
}

int main(int argc, char *argv[]) {
	/* initialize local variables */
	int 	parent, cp, status, return_status;
	
	make_children(argc, &parent, &cp);
	
	/* child process */
	if (parent == 0) {
		print_info(argc, argv, cp);
		exit(0);
	}
	/* parent process */
	else {
		/*wait for all children to terminate*/
		while (wait(&status)) > 0);
		printf("Done!\n");
	}
}