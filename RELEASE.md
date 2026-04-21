# Building a Release APK

## 1. Create a Signing Keystore

Run this command (keep the keystore file secret!):

```bash
cd sys/android
keytool -genkey -v \
  -keystore nethack-release.keystore \
  -alias nethack \
  -keyalg RSA \
  -keysize 2048 \
  -validity 10000
```

You'll be prompted for:
- Keystore password (create a strong one)
- Key password (can be same as keystore)
- Your name, organization, etc.

## 2. Configure Signing

Create `sys/android/local.properties`:

```properties
RELEASE_STORE_FILE=nethack-release.keystore
RELEASE_STORE_PASSWORD=your_keystore_password
RELEASE_KEY_ALIAS=nethack
RELEASE_KEY_PASSWORD=your_key_password
```

**Never commit this file to git!** It's already in `.gitignore`.

## 3. Build Release APK

```bash
cd sys/android
./gradlew assembleRelease
```

The signed APK will be at:
`app/build/outputs/apk/release/app-release.apk`

## 4. Verify the APK

```bash
# Check it's signed
jarsigner -verify -verbose -certs app/build/outputs/apk/release/app-release.apk

# Get APK info
aapt dump badging app/build/outputs/apk/release/app-release.apk
```

## Version Updates

Before each release, update in `sys/android/app/build.gradle`:

```gradle
defaultConfig {
    versionName '3.6.7-3'  // Increment for each release
    versionCode 3673       // Must increase for Play Store
}
```

## Publishing Checklist

- [ ] Update versionCode and versionName
- [ ] Build release APK
- [ ] Test on a clean device/emulator
- [ ] Create release notes
- [ ] Tag the release: `git tag -a v3.6.7-3 -m "Release 3.6.7-3"`
- [ ] Push tag: `git push origin v3.6.7-3`

## Google Play Store

Additional requirements:
- Privacy policy URL
- App screenshots (phone + tablet)
- Feature graphic (1024x500)
- App icon (512x512)

## F-Droid

For F-Droid inclusion:
- Ensure all dependencies are open source
- Tag releases with version numbers
- Include fastlane metadata structure
