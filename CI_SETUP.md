# CI/CD Setup

This project uses GitHub Actions for automated builds.

## Automated Workflows

### Debug Builds
Every push to `master` or `main` triggers a debug build. The APK is uploaded as a build artifact.

### Release Builds
When you create a GitHub Release, a signed release APK is automatically built and attached to the release.

## Setting Up Release Signing

To enable automatic release builds, you need to add two secrets to your GitHub repository:

### 1. Encode Your Keystore

```bash
cd sys/android
base64 -w 0 nethack-release.keystore
```

Copy the output (a long base64 string).

### 2. Add GitHub Secrets

Go to **Settings → Secrets and variables → Actions** in your GitHub repository.

Add two secrets:

- **Name**: `KEYSTORE_BASE64`  
  **Value**: The base64 output from step 1

- **Name**: `KEYSTORE_PASSWORD`  
  **Value**: Your keystore password

### 3. Test It

Create a new GitHub Release. The workflow should automatically:
1. Build the native library
2. Build a signed release APK
3. Upload it to the release

## Manual Trigger

You can also manually trigger builds from the **Actions** tab in GitHub.

## Troubleshooting

### Build Fails on Native Library
Make sure the NDK is properly installed. The workflow installs NDK r26c.

### Signing Fails
- Verify `KEYSTORE_BASE64` is correct (no newlines in the base64 string)
- Verify `KEYSTORE_PASSWORD` matches your keystore password
- Check that the key alias is `nethack`

### Cache Issues
If builds fail mysteriously, try clearing the GitHub Actions cache from the repository settings.
