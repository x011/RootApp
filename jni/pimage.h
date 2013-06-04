#ifndef _PIMAGE_H_
#define _PIMAGE_H_

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
mkprocess(const char *cmd, const char in, const char out, const char err) {
	int infd[2], outfd[2], errfd[2];
	pid_t child;
	processimage* const pimage = malloc(sizeof(processimage));
	if(!pimage)
		return NULL;

	if(in)
		if(pipe(infd))
			goto END;
	if(out)
		if(pipe(outfd))
			goto END;
	if(err)
		if(pipe(errfd))
			goto END;
	if((child = fork()) == -1)
		goto END;

	if(child) {
		pimage->pid = child;
		if(in) {
			close(infd[0]);
			pimage->infd = infd[1];
		} else {
			pimage->infd = 0;
		}
		if(out) {
			close(outfd[1]);
			pimage->outfd = outfd[0];
		} else {
			pimage->outfd = 0;
		}
		if(err) {
			close(errfd[1]);
			pimage->errfd = errfd[0];
		} else {
			pimage->errfd = 0;
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
	END:
	free(pimage);
	return NULL;
}

static void
rmprocess(processimage *pimage) {
	if(pimage->infd)
		close(pimage->infd);
	if(pimage->outfd)
		close(pimage->outfd);
	if(pimage->errfd)
		close(pimage->errfd);
	waitpid(pimage->pid, NULL, 0);
	free(pimage);
}

#endif
