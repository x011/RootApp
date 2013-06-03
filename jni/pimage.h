#ifndef _PIMAGE_H_
#define _PIMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
	pid_t pid;
	int infd,
	outfd,
	errfd;
} processimage;

static processimage*
mkprocess(const char *cmd, const short in, const short out, const short err) {
	int infd[2], outfd[2], errfd[2];
	pid_t child;
	processimage *pimage;
	
	if(in) pipe(infd);
	if(out) pipe(outfd);
	if(err) pipe(errfd);
	child = fork();
	
	if(child) {
		pimage->pid = child;
		if(in) {
			close(infd[0]);
			pimage->infd = infd[1];
		}
		if(out) {
			close(outfd[1]);
			pimage->outfd = outfd[0];
		}
		if(err) {
			close(errfd[1]);
			pimage->errfd = errfd[0];
		}
		return pimage;
	} else {
		if(in) {
			close(infd[1]);
			dup2(infd[0], STDIN_FILENO);
		}
		if(out) {
			close(outfd[0]);
			dup2(outfd[1], STDOUT_FILENO);
		}
		if(err) {
			close(errfd[0]);
			dup2(errfd[1], STDERR_FILENO);
		}
		execlp(cmd, cmd, NULL);
	}
	return NULL;
}

static void
rmprocess(const processimage *pimage) {
	fsync(pimage->infd);
	close(pimage->infd);
	close(pimage->outfd);
	close(pimage->errfd);
	waitpid(pimage->pid, NULL, 0);
}

#endif
