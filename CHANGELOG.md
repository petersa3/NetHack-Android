# Changelog

## 3.6.7-2 (2026-04-21)

### Modernization for Android 14
- Updated to Android SDK 34 (Android 14)
- Updated to Gradle 8.4
- Updated to NDK r26c
- Added 64-bit ARM (arm64-v8a) support
- Removed deprecated 32-bit ABI support

### Fixes
- Fixed JNI method signatures (addMenu, selectMenu)
- Fixed asset extraction for Android 10+ scoped storage
- Fixed permission handling for modern Android
- Fixed tileset image display in settings
- Fixed build system for modern Java versions

### Build System
- Added proper release signing configuration
- Added Makefile.src support for native library
- Added comprehensive build documentation
- Added .gitignore for build artifacts

### Known Issues
- Custom tileset preview only shows for user-uploaded tilesets
- App exits after game ends (expected behavior)

## Original (3.6.7)

Initial Android port by gurrhack.
