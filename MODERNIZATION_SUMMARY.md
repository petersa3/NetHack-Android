# GurrHack NetHack Android Modernization - Summary

## Overview
This document summarizes the modernization work performed on the GurrHack NetHack Android port to bring it to Android 14 (API 34) compatibility.

**Repository:** https://github.com/gurrhack/NetHack-Android  
**Original Version:** NetHack 3.6.7-1 (versionCode 3671)  
**Updated Version:** NetHack 3.6.7-2 (versionCode 3672)  
**Date:** March 28, 2026

---

## Changes Made

### 1. Build System Updates

#### sys/android/build.gradle
| Change | From | To |
|--------|------|-----|
| Repository | jcenter() | mavenCentral() |
| AGP Version | 4.1.1 | 8.2.0 |

**Rationale:** jcenter() was deprecated by JFrog and shut down. AGP 8.x is required for modern Android development.

#### sys/android/app/build.gradle
| Change | From | To |
|--------|------|-----|
| Namespace | (missing) | 'com.tbd.NetHack' |
| compileSdk | 30 | 34 |
| minSdk | 7 | 21 |
| targetSdk | 30 | 34 |
| versionName | '3.6.7-1' | '3.6.7-2' |
| versionCode | 3671 | 3672 |
| Java Version | 1.7 | 17 |
| ABI Filters | (none) | 'arm64-v8a', 'x86_64' |

**Rationale:** 
- Namespace is required for AGP 8.x
- API 34 is required for Android 14
- minSdk 21 is the practical minimum for modern apps
- Java 17 is the current LTS version
- 64-bit ABIs are required for Android 14+

#### sys/android/gradle/wrapper/gradle-wrapper.properties
| Change | From | To |
|--------|------|-----|
| Gradle | 6.9.4 | 8.4 |

**Rationale:** Gradle 8.x is required for AGP 8.x compatibility.

### 2. Native Build Updates

#### sys/android/Makefile.src
- Updated NDK reference from r21e to r26c
- Changed default ABI from `armeabi` (32-bit) to `arm64-v8a` (64-bit)
- Removed deprecated LD_* variables (unified clang in NDK r26c+)
- Updated compiler paths to use API 34
- Added clear documentation about 32-bit ABI deprecation

**Rationale:** Android 14 devices are 64-bit only. The old default (armeabi) would fail on modern devices.

### 3. Scoped Storage Preparation

#### sys/android/app/AndroidManifest.xml
- Added `android:maxSdkVersion="28"` to WRITE_EXTERNAL_STORAGE permission
- Removed deprecated `<uses-sdk />` element

**Rationale:** 
- WRITE_EXTERNAL_STORAGE is heavily restricted on Android 10+ (API 29+)
- The maxSdkVersion allows the permission on older devices for backward compatibility
- For Android 10+, the app must use app-specific directories

---

## Critical Dependency: ForkFront Library

The NetHack Android port depends on the ForkFront library for its UI:
- **Repository:** https://github.com/gurrhack/ForkFront-Android.git
- **Module:** com.tbd.forkfront:lib:1.1

### Scoped Storage Issue
The native code receives its working directory via JNI:
```c
void Java_com_tbd_forkfront_NetHackIO_RunNetHack(JNIEnv* env, jobject thiz, jstring path, jstring username)
{
    pChars = (*jEnv)->GetStringUTFChars(jEnv, path, 0);
    if(chdir(pChars) != 0)
        debuglog("chdir failed %d", errno);
    // ...
}
```

All file operations (save games, config, locks) are relative to this directory.

**For full Android 14 compatibility, the ForkFront library MUST be updated to pass `Context.getExternalFilesDir(null).getAbsolutePath()` instead of external storage root.**

---

## Build Instructions (Updated)

### Prerequisites
- Android SDK Command-line Tools
- JDK 17 (required by modern Android Gradle Plugin)
- bison and flex
- NDK r26c or newer

### Setup
```bash
export ANDROID_SDK_ROOT=/path/to/android-sdk
export JAVA_HOME=/path/to/jdk-17
```

### Install SDK Components
```bash
cd $ANDROID_SDK_ROOT/cmdline-tools/latest/bin
./sdkmanager --update
./sdkmanager --install "platforms;android-34"
./sdkmanager --install "ndk;26.1.10909125"
```

### Build Native Library
```bash
cd /path/to/NetHack-Android/sys/android
# Edit Makefile.src and set NDK to your NDK path
sh ./setup.sh
cd ../..
make install
```

### Build Android App
```bash
cd /path/to/NetHack-Android/sys/android
./gradlew build
```

Output APK: `sys/android/app/build/outputs/apk/debug/app-debug.apk`

---

## Testing Checklist

- [ ] Build completes without errors
- [ ] APK installs on Android 14 emulator
- [ ] APK installs on Android 14 physical device
- [ ] Game launches successfully
- [ ] New game starts
- [ ] Save game works
- [ ] Restore game works
- [ ] Config file is read/written correctly

---

## Known Issues & Limitations

1. **Scoped Storage:** The current implementation requires WRITE_EXTERNAL_STORAGE permission. For Android 10+ (API 29+), this requires:
   - Updating ForkFront library to use app-specific directories
   - OR requesting MANAGE_EXTERNAL_STORAGE (requires Play Store justification)

2. **ForkFront Dependency:** The UI layer is in a separate repository. Any UI-related issues or Android 14-specific UI changes need to be addressed there.

3. **NDK Path:** Users must manually update the NDK path in Makefile.src before building.

---

## Files Modified

```
sys/android/build.gradle
sys/android/app/build.gradle
sys/android/app/AndroidManifest.xml
sys/android/Makefile.src
sys/android/gradle/wrapper/gradle-wrapper.properties
```

---

## Patch File

A complete patch is available at:
`/home/alex/.openclaw/workspace/nethack-android-modernization.patch`

To apply:
```bash
cd /path/to/NetHack-Android
git apply /path/to/nethack-android-modernization.patch
```

---

## Next Steps for Full Android 14 Compatibility

1. **ForkFront Library Update:**
   - Modify ForkFront-Android to use `Context.getExternalFilesDir(null)` for the working directory
   - Test save/restore with new directory structure
   - Consider migration path for existing saves

2. **Additional Testing:**
   - Test on various Android versions (12, 13, 14)
   - Test on different device architectures
   - Verify performance is acceptable

3. **Play Store Preparation:**
   - Create privacy policy (scoped storage changes may require explanation)
   - Update store listing screenshots
   - Test release signing configuration

---

## Resources

- [Android 14 Behavior Changes](https://developer.android.com/about/versions/14/behavior-changes-14)
- [Scoped Storage Documentation](https://developer.android.com/training/data-storage#scoped-storage)
- [NDK Migration Guide](https://developer.android.com/ndk/guides/migration)
- [Gradle Upgrade Guide](https://developer.android.com/studio/releases/gradle-plugin-upgrader)

---

## Contact

For issues with this modernization:
- Original maintainer: https://github.com/gurrhack
- NetHack official: https://www.nethack.org/

---

*Modernization completed by AI assistant for Alex*
