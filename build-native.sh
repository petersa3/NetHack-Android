#!/bin/bash
# Build script for NetHack Android native library

set -e

# NDK Configuration
export NDK=/home/alex/Android/Sdk/ndk/26.2.11394342
export NDK_TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin

# Target ABI
export ABI=arm64-v8a

# Compiler settings
export CC=$NDK_TOOLCHAIN/aarch64-linux-android24-clang
export CXX=$NDK_TOOLCHAIN/aarch64-linux-android24-clang++
export LINK=$CC

# Flags
export CFLAGS="-DANDROID -O2 -g -Wno-format -fsigned-char -fPIC -I../include"
export LFLAGS="-shared"

# Create output directory
mkdir -p sys/android/app/libs/$ABI

cd src

# Build makedefs first (using host compiler)
cd ../util
make clean || true
make makedefs

# Generate required headers
cd ../src
../util/makedefs -v

# Build the native library
# For now, let's just compile winandroid.c to test
echo "Building native library..."
$CC $CFLAGS -c ../sys/android/winandroid.c -o winandroid.o

# Create the shared library
$LINK $LFLAGS -o ../sys/android/app/libs/$ABI/libnethack.so winandroid.o

echo "Native library built successfully!"