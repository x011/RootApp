#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

jboolean
Java_koneu_rootapp_MainActivity_sudo(JNIEnv *env, jobject obj, jstring xmd) {
	char *cmd;
	jboolean ret;
	asprintf(&cmd, "su -c \"%s\"", ((*env)->GetStringUTFChars(env, xmd, 0)));
	if(system(cmd) == EXIT_SUCCESS) {
		ret = JNI_TRUE;
	} else {
		ret = JNI_FALSE;
	}
	return ret;
}
