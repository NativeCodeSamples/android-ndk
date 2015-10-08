#include <jni.h>
#include <android/log.h>

JNIEXPORT void JNICALL
Java_com_google_sample_audio_1echo_MainActivity_stopEcho(JNIEnv *env, jclass type) {
    __android_log_print(ANDROID_LOG_INFO, "Audio-Echo", "stopEcho() called");
}

JNIEXPORT void JNICALL
Java_com_google_sample_audio_1echo_MainActivity_startEcho(JNIEnv *env, jclass type, jint rate,
                                                          jint framesPerBuf) {
    __android_log_print(ANDROID_LOG_INFO, "Audio-Echo", "startEcho() called");
}