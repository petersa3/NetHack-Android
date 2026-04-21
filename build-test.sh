#!/bin/bash
# Build test script for NetHack Android modernization
# Run this from the sys/android directory

echo "=== NetHack Android Build Test ==="
echo "Date: $(date)"
echo ""

# Check prerequisites
echo "1. Checking prerequisites..."
if command -v java &> /dev/null; then
    java_version=$(java -version 2>&1 | head -1 | cut -d'"' -f2)
    echo "   Java: $java_version (found)"
else
    echo "   Java: NOT FOUND (required)"
fi

if [ -n "$ANDROID_SDK_ROOT" ]; then
    echo "   ANDROID_SDK_ROOT: $ANDROID_SDK_ROOT (set)"
else
    echo "   ANDROID_SDK_ROOT: NOT SET (required)"
fi

if command -v adb &> /dev/null; then
    echo "   ADB: $(adb version | head -1) (found)"
else
    echo "   ADB: NOT FOUND (optional for testing)"
fi

echo ""
echo "2. Checking build configuration..."
cd sys/android

echo "   Gradle wrapper:"
grep "distributionUrl" gradle/wrapper/gradle-wrapper.properties

echo ""
echo "   Build configuration:"
echo "   - AGP version: $(grep "classpath.*gradle" build.gradle | cut -d"'" -f2)"
echo "   - compileSdk: $(grep "compileSdk" app/build.gradle | tr -d ' ' | cut -d: -f2)"
echo "   - targetSdk: $(grep "targetSdk" app/build.gradle | tr -d ' ' | cut -d: -f2)"
echo "   - minSdk: $(grep "minSdk" app/build.gradle | tr -d ' ' | cut -d: -f2)"
echo "   - Java version: $(grep "sourceCompatibility" app/build.gradle | tr -d ' ' | cut -d: -f2)"

echo ""
echo "3. Checking NDK configuration..."
echo "   Default ABI: $(grep "^ABI = " Makefile.src | cut -d' ' -f3)"
echo "   NDK version in comments: $(grep "NDK = " Makefile.src | head -1 | cut -d'/' -f5)"

echo ""
echo "4. Checking scoped storage support..."
if grep -q "maxSdkVersion.*28" app/AndroidManifest.xml; then
    echo "   WRITE_EXTERNAL_STORAGE has maxSdkVersion=\"28\" (good)"
else
    echo "   WRITE_EXTERNAL_STORAGE missing maxSdkVersion (needs update)"
fi

if [ -f "app/src/main/java/com/tbd/NetHack/FileHelper.java" ]; then
    echo "   FileHelper.java exists (good)"
else
    echo "   FileHelper.java not found (needs creation)"
fi

echo ""
echo "5. Testing build..."
echo "   Note: Actual build requires Android SDK and NDK"
echo "   To build: ./gradlew build"
echo "   To build debug APK: ./gradlew assembleDebug"

echo ""
echo "=== Test Complete ==="
echo ""
echo "Next steps:"
echo "1. Install Android SDK with NDK r26c+"
echo "2. Set ANDROID_SDK_ROOT environment variable"
echo "3. Run: ./gradlew build"
echo "4. Test APK on Android device/emulator"
echo ""
echo "For Android 10+ compatibility:"
echo "The ForkFront library needs to be updated to use FileHelper.getSaveDir()"
echo "instead of external storage. This requires changes to the ForkFront-Android repository."