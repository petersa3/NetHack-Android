# GurrHack NetHack Android Modernization - Implementation Log

## Project Overview
**Repository:** https://github.com/gurrhack/NetHack-Android  
**Current Version:** NetHack 3.6.7-1 (versionCode 3671)  
**Target:** Android 14 (API 34) compatibility  
**Status:** In Progress

---

## Current State Analysis

### Build System (Outdated)
| Component | Current | Required for Android 14 |
|-----------|---------|------------------------|
| compileSdk | 30 | 34 |
| targetSdk | 30 | 34 |
| minSdk | 7 | 21+ (recommended) |
| Gradle | 6.9.4 | 8.4+ |
| Android Gradle Plugin | 4.1.1 | 8.2+ |
| Java | 1.7 | 17 |
| NDK | r21e | r26c+ |

### Critical Issues Found

1. **jcenter() deprecated** - Build will fail, needs migration to mavenCentral()
2. **Missing namespace** - Required in build.gradle for AGP 8.x
3. **32-bit ABI default** - `ABI = armeabi` in Makefile.src is obsolete
4. **Scoped storage** - `WRITE_EXTERNAL_STORAGE` permission incompatible with Android 10+
5. **NDK toolchain paths** - Need updating for NDK r26c+

### Dependencies
- **ForkFront library**: Available at https://github.com/gurrhack/ForkFront-Android.git
  - Referenced via Git repository in settings.gradle
  - Provides the UI layer (com.tbd.forkfront.ForkFront activity)
  - This is NOT a blocker - source is available

---

## Modernization Plan

### Phase 1: Build System Updates
- [x] Update `sys/android/build.gradle`
  - Replace jcenter() with mavenCentral()
  - Update AGP to 8.2.0
- [x] Update `sys/android/app/build.gradle`
  - Add namespace
  - Update compileSdk to 34
  - Update targetSdk to 34
  - Update minSdk to 21
  - Update Java compatibility to 17
- [x] Update `gradle-wrapper.properties`
  - Gradle 8.4
- [x] Update `sys/android/Makefile.src`
  - Update NDK path comments
  - Change default ABI to arm64-v8a

### Phase 2: 64-Bit Only Migration
- [x] Remove 32-bit ABI support from Makefile.src
- [x] Update NDK compiler paths for r26c+
- [x] Update app/build.gradle abiFilters

### Phase 3: Scoped Storage Migration
- [x] Analyze file I/O in native code
- [x] Document JNI bridge requirements for app-specific directories
- [x] Update AndroidManifest.xml permissions
- [ ] Test save/restore functionality (requires ForkFront update)

**Analysis:** The native code receives a path via `Java_com_tbd_forkfront_NetHackIO_RunNetHack()` and calls `chdir()` to it. All file operations are relative to this directory. The ForkFront library is responsible for providing this path. The solution requires ForkFront to use `Context.getExternalFilesDir(null)` instead of external storage root.

### Phase 4: Testing
- [ ] Build on Android Studio / command line
- [ ] Test on Android 14 emulator
- [ ] Test on physical device

---

## Files Created/Modified

### Modified Files (in repository)
1. `sys/android/build.gradle` - Build script dependencies
2. `sys/android/app/build.gradle` - App-level build configuration
3. `sys/android/app/AndroidManifest.xml` - App manifest with scoped storage
4. `sys/android/Makefile.src` - Native build configuration
5. `sys/android/gradle/wrapper/gradle-wrapper.properties` - Gradle wrapper

### New Documentation Files
1. `MODERNIZATION_LOG.md` - This file
2. `MODERNIZATION_SUMMARY.md` - Comprehensive summary for developers
3. `QUICK_START.md` - Quick reference guide
4. `nethack-android-modernization.patch` - Patch file (in parent directory)

---

## Implementation Log

### 2026-03-28: Initial Assessment
- Cloned repository successfully
- Identified all build configuration files
- Confirmed ForkFront library is available (not a blocker)
- Documented all required changes

### 2026-03-28: Phase 1 & 2 Complete - Build System Modernization
**Files Modified:**

1. **sys/android/build.gradle**
   - Replaced deprecated `jcenter()` with `mavenCentral()`
   - Updated Android Gradle Plugin: 4.1.1 → 8.2.0

2. **sys/android/app/build.gradle**
   - Added `namespace 'com.tbd.NetHack'` (required for AGP 8.x)
   - Updated compileSdk: 30 → 34
   - Updated targetSdk: 30 → 34
   - Updated minSdk: 7 → 21
   - Updated versionName: '3.6.7-1' → '3.6.7-2'
   - Updated versionCode: 3671 → 3672
   - Added ndk.abiFilters for 'arm64-v8a', 'x86_64' (64-bit only)
   - Updated Java compatibility: 1.7 → 17

3. **sys/android/gradle/wrapper/gradle-wrapper.properties**
   - Updated Gradle: 6.9.4 → 8.4

4. **sys/android/Makefile.src**
   - Updated NDK reference comments to r26c
   - Changed default ABI: armeabi → arm64-v8a
   - Removed deprecated LD_* variables (unified clang in NDK r26c+)
   - Updated compiler paths to use API 21 (minimum for 64-bit)
   - Added clear comments about 32-bit ABI deprecation

5. **sys/android/app/AndroidManifest.xml**
   - Added `android:maxSdkVersion="28"` to WRITE_EXTERNAL_STORAGE permission
   - This allows the permission on older devices while complying with scoped storage on Android 10+

### Scoped Storage Analysis
The native code receives its working directory via JNI from the ForkFront library:
- `Java_com_tbd_forkfront_NetHackIO_RunNetHack()` receives `path` parameter
- Native code calls `chdir(pChars)` to set working directory
- All file operations (saves, locks, config) are relative to this directory

**Solution:** The ForkFront library needs to be updated to pass `Context.getExternalFilesDir(null).getAbsolutePath()` instead of external storage root. This change needs to happen in the ForkFront-Android repository, not this one.

**Note:** The current `maxSdkVersion="28"` workaround allows the app to use external storage on older devices, but for Android 10+ (API 29+), the app MUST use app-specific directories. The ForkFront library change is required for full Android 14 compatibility.

