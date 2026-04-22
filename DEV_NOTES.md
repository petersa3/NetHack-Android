# Development Notes

## Tools Used

### Build Tools
- **Android SDK 34** - Target API level
- **NDK r26c** - Native development kit for C/C++ compilation
- **Gradle 8.4** - Build system
- **Java 17** - Required for Android Gradle Plugin compatibility
- **Clang/LLVM** (via NDK) - C compiler for native library

### Development Environment
- **Linux Mint 22.1** - Development OS
- **Git** - Version control
- **keytool** - Keystore generation for signing

### Android-Specific
- **adb** - Android debugging and deployment
- **aapt** - Android asset packaging tool

## Key Changes Made

### Build System Modernization
1. Updated `compileSdk` and `targetSdk` to 34 (Android 14)
2. Updated `minSdk` to 21 (Android 5.0)
3. Removed deprecated 32-bit ABIs (armeabi, armeabi-v7a, x86)
4. Kept arm64-v8a and x86_64 for 64-bit support
5. Updated Gradle wrapper to 8.4
6. Updated Android Gradle Plugin to 8.3.0

### Native Library Build
1. Fixed `Makefile.src` for NDK cross-compilation
2. Fixed `Makefile.utl` for host makedefs compilation
3. Added proper ABI selection in build scripts
4. Removed incompatible ARM32 flags for ARM64 builds

### JNI Fixes
1. Fixed `addMenu` signature - changed `long id` to `int id`
2. Fixed `selectMenu` signature - changed return type from `long[]` to `int[]`

### Asset Handling
1. Moved assets to `app/src/main/assets/` (Gradle standard)
2. Fixed asset path handling for empty `nativeDataDir`
3. Added directory skipping in `UpdateAssets`
4. Added `ver` file for version tracking
5. Added fonts to assets

### Android 10+ (Scoped Storage) Support
1. Fixed permission check to skip `WRITE_EXTERNAL_STORAGE` on API 29+
2. Updated `ForkFront.java` to handle scoped storage
3. Fixed `UpdateAssets` to use app-specific directories

### UI/Resources
1. Moved tileset images to `app/src/main/res/drawable-nodpi/`
2. Added tiles.xml to `app/src/main/res/values/`
3. Fixed tileset configuration

### Release Build Setup
1. Added release signing configuration to `build.gradle`
2. Added `local.properties` support for signing secrets
3. Added `.gitignore` for keystore and sensitive files
4. Created `RELEASE.md` with build instructions

## File Locations

### Important Paths
- **Native library**: `sys/android/app/libs/arm64-v8a/libnethack.so`
- **Assets**: `sys/android/app/src/main/assets/`
- **Resources**: `sys/android/app/src/main/res/`
- **Keystore**: `sys/android/nethack-release.keystore` (not in git)
- **Signing config**: `sys/android/local.properties` (not in git)

### Build Outputs
- **Debug APK**: `sys/android/app/build/outputs/apk/debug/app-debug.apk`
- **Release APK**: `sys/android/app/build/outputs/apk/release/app-release.apk`

## Common Commands

### Debug Build
```bash
cd sys/android
./gradlew assembleDebug
```

### Release Build
```bash
cd sys/android
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
./gradlew assembleRelease
```

### Native Library Only
```bash
./build-native.sh
# or
cd src && make -f ../sys/android/Makefile.src Sysandroid
```

### Clean Build
```bash
cd sys/android
./gradlew clean
```

## Known Issues / Future Work

1. **Custom tileset preview** - Only shows preview for user-uploaded tilesets, not built-in ones
2. **App exit on game end** - Currently exits after death/descent; could be changed to return to menu
3. **Java 21 compatibility** - Build requires Java 17; Java 21 has jlink issues with current config
4. **x86_64 warning** - Build warns about no .so files for x86_64 (harmless, only arm64-v8a is built)

## Version History

- **3.6.7-2** (2026-04-21) - Modernized for Android 14, first working release
- **3.6.7** (original) - Initial Android port by gurrhack
