#include <jni.h>
#include <android/log.h>

JNIEXPORT void JNICALL
Java_com_google_sample_audio_1echo_MainActivity_hello_1jni(JNIEnv *env, jclass type) {

    // TODO

    __android_log_print(ANDROID_LOG_INFO, "Audio-Echo", "Hello-Jni");

}