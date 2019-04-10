#!/bin/bash
make distclean
export ANDROID_NDK=/home/user/Programming/android-ndk-r17b
PATH=$ANDROID_NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin:$PATH
./Configure android-arm64 -D__ANDROID_API__=28
#make
#cp libcrypto.a ../../build/arm64-v8a/libcrypto.a
#cp libcrypto.so.1.1 ../../build/arm64-v8a/libcrypto.so.1.1
#cp libssl.a ../../build/arm64-v8a/libssl.a
#cp libssl.so.1.1 ../../build/arm64-v8a/libssl.so.1.1
