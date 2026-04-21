/*
 * Scoped Storage Patch for NetHack Android
 * 
 * This file contains modifications needed for Android 10+ Scoped Storage compliance.
 * The main changes are:
 * 1. Use app-specific directories instead of external storage
 * 2. Add JNI methods to get the app-specific directory from Java
 * 3. Modify file path construction to use the new directory
 */

#include <jni.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "NetHackStorage"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/* Global variable to store the app-specific directory */
static char app_specific_dir[1024] = {0};

/* 
 * JNI function to set the app-specific directory.
 * Called from Java before starting NetHack.
 */
JNIEXPORT void JNICALL
Java_com_tbd_forkfront_NetHackIO_setAppSpecificDirectory(JNIEnv *env, jobject thiz, jstring dir) {
    const char *dir_str = (*env)->GetStringUTFChars(env, dir, NULL);
    
    if (dir_str != NULL) {
        strncpy(app_specific_dir, dir_str, sizeof(app_specific_dir) - 1);
        app_specific_dir[sizeof(app_specific_dir) - 1] = '\0';
        LOGI("App-specific directory set to: %s", app_specific_dir);
        (*env)->ReleaseStringUTFChars(env, dir, dir_str);
    } else {
        LOGE("Failed to get app-specific directory from Java");
    }
}

/*
 * Helper function to get the full path for a file.
 * If app_specific_dir is set, use it. Otherwise, use the original path.
 */
const char *get_scoped_storage_path(const char *original_path) {
    static char full_path[2048];
    
    if (app_specific_dir[0] == '\0') {
        /* App-specific directory not set, use original path */
        LOGW("App-specific directory not set, using original path: %s", original_path);
        return original_path;
    }
    
    /* Construct full path in app-specific directory */
    snprintf(full_path, sizeof(full_path), "%s/%s", app_specific_dir, original_path);
    LOGI("Converted path: %s -> %s", original_path, full_path);
    return full_path;
}

/*
 * Wrapper for fopen that uses app-specific directory.
 * This is a simple example - in practice, you'd need to wrap all file operations.
 */
FILE *fopen_scoped(const char *filename, const char *mode) {
    const char *path = get_scoped_storage_path(filename);
    return fopen(path, mode);
}

/*
 * Example of how to modify an existing fopen call:
 * Original: fp = fopen(filename, mode);
 * Modified: fp = fopen_scoped(filename, mode);
 */

/*
 * For integration with NetHack's existing code, you would need to:
 * 1. Call setAppSpecificDirectory() from Java before RunNetHack()
 * 2. Modify fopen_datafile() in files.c to use app-specific directory
 * 3. Or modify the prefix initialization to point to app-specific directory
 */

/*
 * Alternative approach: Modify the prefix paths directly.
 * This function would be called from Java to set the data prefix.
 */
JNIEXPORT void JNICALL
Java_com_tbd_forkfront_NetHackIO_setDataPrefix(JNIEnv *env, jobject thiz, jstring prefix) {
    /* This would need to modify fqn_prefix[DATAPREFIX] in the C code */
    /* Implementation depends on NetHack's internal structures */
}

/*
 * Migration helper: Check if old save files exist in external storage
 * and copy them to app-specific storage.
 */
JNIEXPORT jboolean JNICALL
Java_com_tbd_forkfront_NetHackIO_migrateOldSaves(JNIEnv *env, jobject thiz, 
                                                 jstring old_dir, jstring new_dir) {
    const char *old_dir_str = (*env)->GetStringUTFChars(env, old_dir, NULL);
    const char *new_dir_str = (*env)->GetStringUTFChars(env, new_dir, NULL);
    jboolean result = JNI_FALSE;
    
    if (old_dir_str && new_dir_str) {
        /* Implementation would:
         * 1. Check if old_dir exists
         * 2. Create new_dir if it doesn't exist
         * 3. Copy files from old_dir to new_dir
         * 4. Return true if migration was successful or not needed
         */
        LOGI("Would migrate from %s to %s", old_dir_str, new_dir_str);
        result = JNI_TRUE;
    }
    
    if (old_dir_str) (*env)->ReleaseStringUTFChars(env, old_dir, old_dir_str);
    if (new_dir_str) (*env)->ReleaseStringUTFChars(env, new_dir, new_dir_str);
    
    return result;
}