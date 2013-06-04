#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jni.h>

#include "pimage.h"

static processimage *process;

jboolean
Java_koneu_rootapp_MainActivity_sudo(JNIEnv *env, jobject obj, jstring xmd) {
	const char *cmd = ((*env)->GetStringUTFChars(env, xmd, 0));
	jboolean ret = JNI_TRUE;
	if(process) {
		while(cmd[0]  != 0) {
			if(write(process->infd, cmd, 1) != 1) {
				ret = JNI_FALSE;
				goto END;
			}
			cmd++;
		}
		if(write(process->infd, "\n", 1) != 1)
			ret = JNI_FALSE;
	} else {
		char *buf;
		asprintf(&buf, "su -c \"%s\"", cmd);
		if(system(cmd) != EXIT_SUCCESS)
			ret = JNI_FALSE;
	}
	END:
	(*env)->ReleaseStringUTFChars(env, xmd, cmd);
	return ret;
}

void
Java_koneu_rootapp_MainActivity_startshell(JNIEnv *env, jobject obj) {
	if(!process)
		process = mkprocess("su", 1, 0, 0);
}

void
Java_koneu_rootapp_MainActivity_closeshell(JNIEnv *env, jobject obj) {
	if(process) {
		rmprocess(process);
		process = NULL;
	}
}
