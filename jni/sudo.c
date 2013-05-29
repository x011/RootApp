#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

static FILE *shell;

jboolean
Java_koneu_rootapp_MainActivity_sudo(JNIEnv *env, jobject obj, jstring xmd) {
	char *cmd;
	jboolean ret = JNI_TRUE;
	if(shell) {
		fprintf(shell, "%s\n", ((*env)->GetStringUTFChars(env, xmd, 0)));
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
	if(!shell) {
		shell = popen("su", "w");
	}
}

void
Java_koneu_rootapp_MainActivity_closeshell(JNIEnv *env, jobject obj) {
	pclose(shell);
	shell = NULL;
}
