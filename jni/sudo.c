#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jni.h>

#include "pimage.h"

static processimage *process;

jboolean
Java_koneu_rootapp_MainActivity_sudo(JNIEnv *env, jobject obj, jstring xmd) {
	char *cmd;
	jboolean ret = JNI_TRUE;
	if(process) {
		/* using asprintf here is a little overkill */
		int count = asprintf(&cmd, "%s\n", ((*env)->GetStringUTFChars(env, xmd, 0)));
		if(write(process->infd, cmd, count) != count) {
			ret = JNI_FALSE;
		}
	} else {
		asprintf(&cmd, "su -c \"%s\"", ((*env)->GetStringUTFChars(env, xmd, 0)));
		if(system(cmd) != EXIT_SUCCESS) {
			ret = JNI_FALSE;
		}
	}
	return ret;
}

void
Java_koneu_rootapp_MainActivity_startshell(JNIEnv *env, jobject obj) {
	if(!process) process = mkprocess("su", 1, 0, 0);
}

void
Java_koneu_rootapp_MainActivity_closeshell(JNIEnv *env, jobject obj) {
	if(process) {
		rmprocess(process);
		process = NULL;
	}
}
