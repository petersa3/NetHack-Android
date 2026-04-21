# Quick Reference: Building Modernized NetHack Android

## One-Time Setup

```bash
# 1. Set environment variables
export ANDROID_SDK_ROOT=/path/to/android-sdk
export JAVA_HOME=/path/to/jdk-17

# 2. Install SDK components
cd $ANDROID_SDK_ROOT/cmdline-tools/latest/bin
./sdkmanager --update
./sdkmanager --install "platforms;android-34"
./sdkmanager --install "ndk;26.1.10909125"  # or newer
```

## Build Steps

```bash
# 1. Clone and enter repository
git clone https://github.com/gurrhack/NetHack-Android.git
cd NetHack-Android

# 2. Apply modernization patch (if not already applied)
git apply /path/to/nethack-android-modernization.patch

# 3. Configure NDK path
# Edit sys/android/Makefile.src and set:
# NDK = /path/to/android-ndk-r26c

# 4. Build native library
cd sys/android
sh ./setup.sh
cd ../..
make install

# 5. Build Android app
cd sys/android
./gradlew build

# 6. Find your APK
ls app/build/outputs/apk/debug/app-debug.apk
```

## Common Issues

### Issue: "Could not find com.tbd.forkfront:lib:1.1"
**Solution:** The ForkFront library is fetched from Git. Ensure you have internet connectivity and Git is installed.

### Issue: "NDK not found"
**Solution:** Update the NDK path in `sys/android/Makefile.src`:
```makefile
NDK = /path/to/your/android-ndk-r26c
```

### Issue: "Unsupported class file major version"
**Solution:** You're using the wrong Java version. Set JAVA_HOME to JDK 17:
```bash
export JAVA_HOME=/path/to/jdk-17
```

### Issue: "Could not find method compileSdkVersion()"
**Solution:** You're using an old Gradle version. Ensure the wrapper is using Gradle 8.4:
```bash
cd sys/android
./gradlew wrapper --gradle-version 8.4
```

## Testing on Device

```bash
# Install to connected device
adb install -r app/build/outputs/apk/debug/app-debug.apk

# Or copy to device manually
adb push app/build/outputs/apk/debug/app-debug.apk /sdcard/Download/
```

## Version Information

After building, verify the version:
```bash
# Check APK info
aapt dump badging app/build/outputs/apk/debug/app-debug.apk | grep version
```

Expected output:
```
package: name='com.tbd.NetHack' versionCode='3672' versionName='3.6.7-2'
```

## Architecture Support

The modernized build only includes 64-bit libraries:
- `arm64-v8a` - For physical Android devices (phones/tablets)
- `x86_64` - For Android emulators on x86_64 computers

To verify your APK has the correct libraries:
```bash
unzip -l app/build/outputs/apk/debug/app-debug.apk | grep lib.*\.so
```

Expected output should show:
```
lib/arm64-v8a/libnethack.so
lib/x86_64/libnethack.so
```

(No armeabi, armeabi-v7a, or x86 libraries)

## Scoped Storage Note

For Android 10+ (API 29+), the app uses app-specific storage:
- Path: `/Android/data/com.tbd.NetHack/files/`
- Save files, config, and locks go here
- Files are removed when app is uninstalled

For Android 9 and below (API 28-), the app uses external storage with WRITE_EXTERNAL_STORAGE permission.

## Need Help?

- NetHack Android issues: https://github.com/gurrhack/NetHack-Android/issues
- ForkFront library: https://github.com/gurrhack/ForkFront-Android
- Android NDK docs: https://developer.android.com/ndk
