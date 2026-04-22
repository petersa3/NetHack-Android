# NetHack Android

NetHack for modern Android devices. This is a modernized fork of the classic NetHack roguelike game, updated to run on Android 14+.

## Download

Get the latest release from the [Releases page](../../releases).

## Features

- Full NetHack 3.6.7 gameplay on Android 14+
- Multiple tileset options (Absurd, Chozo, DawnHack, Default, Geoduck, Nevanda)
- Customizable interface
- Save and restore games
- Hearse bones file sharing support

## Building from Source

### Prerequisites

- Android Studio or Android SDK
- Android SDK 34
- NDK r26c or newer
- Java 17

### Quick Build (Debug)

```bash
cd sys/android
./gradlew assembleDebug
```

The debug APK will be at `app/build/outputs/apk/debug/app-debug.apk`.

### Release Build

See [RELEASE.md](RELEASE.md) for detailed release build instructions including signing.

Quick summary:

```bash
cd sys/android

# Create keystore (one-time)
keytool -genkey -v -keystore nethack-release.keystore \
  -alias nethack -keyalg RSA -keysize 2048 -validity 10000

# Configure signing in local.properties
echo "RELEASE_STORE_FILE=../nethack-release.keystore
RELEASE_STORE_PASSWORD=your_password
RELEASE_KEY_ALIAS=nethack
RELEASE_KEY_PASSWORD=your_password" > local.properties

# Build release
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
./gradlew assembleRelease
```

The signed APK will be at `app/build/outputs/apk/release/app-release.apk`.

## Native Library Build

To build just the native library (libnethack.so):

```bash
./build-native.sh
```

Or manually via Makefile:

```bash
cd src
make -f ../sys/android/Makefile.src Sysandroid
```

## Project Structure

- `sys/android/` - Android-specific build files and Java code
- `src/` - NetHack C source code
- `dat/` - NetHack data files (levels, descriptions, etc.)
- `include/` - Header files
- `util/` - Build utilities (makedefs, etc.)

## Modernization

This fork modernizes the original NetHack-Android project with:

- Android 14 (API 34) support
- 64-bit ARM (arm64-v8a) support
- Scoped storage for Android 10+
- Updated Gradle and build tools
- Fixed JNI signatures for modern Android

See [MODERNIZATION_SUMMARY.md](MODERNIZATION_SUMMARY.md) for full details.

## License

NetHack is licensed under the NetHack General Public License (NGPL). See the original NetHack documentation for details.

## Credits

- Original NetHack by the NetHack DevTeam
- Original Android port by gurrhack
- Modernization by petersa3
