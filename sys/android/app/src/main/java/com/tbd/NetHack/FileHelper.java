package com.tbd.NetHack;

import android.content.Context;
import android.os.Build;
import android.os.Environment;
import java.io.File;

/**
 * FileHelper provides app-specific directory paths for NetHack.
 * This is necessary for Android 10+ Scoped Storage compliance.
 */
public class FileHelper {
    
    /**
     * Get the app-specific external files directory for saves.
     * This directory doesn't require WRITE_EXTERNAL_STORAGE permission on Android 10+.
     */
    public static String getSaveDir(Context context) {
        File dir = context.getExternalFilesDir(null);
        if (dir != null) {
            return dir.getAbsolutePath();
        }
        
        // Fallback to internal storage if external is unavailable
        return context.getFilesDir().getAbsolutePath();
    }
    
    /**
     * Get the legacy external storage path for backward compatibility.
     * Only works on Android 9 and below, or with MANAGE_EXTERNAL_STORAGE permission.
     */
    public static String getLegacySaveDir() {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.P) {
            File dir = Environment.getExternalStorageDirectory();
            if (dir != null) {
                return new File(dir, "NetHack").getAbsolutePath();
            }
        }
        return null;
    }
    
    /**
     * Check if we're running on Android 10+ where scoped storage is enforced.
     */
    public static boolean isScopedStorageEnforced() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q;
    }
    
    /**
     * Migrate old save files from legacy location to app-specific directory.
     * Call this on app startup.
     */
    public static void migrateOldSaves(Context context) {
        if (isScopedStorageEnforced()) {
            String legacyPath = getLegacySaveDir();
            String newPath = getSaveDir(context);
            
            if (legacyPath != null) {
                File legacyDir = new File(legacyPath);
                File newDir = new File(newPath);
                
                if (legacyDir.exists() && legacyDir.isDirectory()) {
                    migrateDirectory(legacyDir, newDir);
                }
            }
        }
    }
    
    private static void migrateDirectory(File source, File target) {
        if (!target.exists()) {
            target.mkdirs();
        }
        
        File[] files = source.listFiles();
        if (files != null) {
            for (File file : files) {
                File dest = new File(target, file.getName());
                if (file.isDirectory()) {
                    migrateDirectory(file, dest);
                } else {
                    file.renameTo(dest);
                }
            }
        }
    }
}