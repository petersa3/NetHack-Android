package com.tbd.NetHack;

import android.content.Context;
import android.util.Log;

/**
 * NativeBridge provides JNI methods for NetHack to access Android-specific functionality.
 * This is necessary for Scoped Storage compliance on Android 10+.
 */
public class NativeBridge {
    private static final String TAG = "NetHackNativeBridge";
    
    private static Context appContext;
    private static String saveDirectory;
    
    /**
     * Initialize the native bridge with application context.
     * Must be called before any native code runs.
     */
    public static void initialize(Context context) {
        appContext = context.getApplicationContext();
        
        // Get app-specific save directory
        saveDirectory = FileHelper.getSaveDir(appContext);
        Log.d(TAG, "Save directory: " + saveDirectory);
        
        // Migrate old saves if needed
        FileHelper.migrateOldSaves(appContext);
        
        // Initialize native paths
        initNativePaths(saveDirectory);
    }
    
    /**
     * Get the current save directory.
     */
    public static String getSaveDirectory() {
        return saveDirectory;
    }
    
    /**
     * Native method to initialize file paths in the C code.
     */
    private static native void initNativePaths(String saveDir);
    
    /**
     * Native method to get a file path for a specific prefix.
     * Used by the C code to construct full paths.
     */
    public static native String getFilePathForPrefix(int prefix, String filename);
    
    /**
     * Check if a file exists in the app-specific directory.
     */
    public static boolean fileExists(String filename) {
        if (saveDirectory == null || filename == null) {
            return false;
        }
        
        java.io.File file = new java.io.File(saveDirectory, filename);
        return file.exists();
    }
    
    /**
     * List files in the save directory (for debugging).
     */
    public static String[] listSaveFiles() {
        if (saveDirectory == null) {
            return new String[0];
        }
        
        java.io.File dir = new java.io.File(saveDirectory);
        if (dir.exists() && dir.isDirectory()) {
            return dir.list();
        }
        return new String[0];
    }
    
    // Load native library
    static {
        System.loadLibrary("nethack");
    }
}