#!/bin/bash
make clean
export ANDROID_NDK=/home/user/Programming/android-ndk-r17b
PATH=$ANDROID_NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin:$PATH
#./Configure android-arm -D__ANDROID_API__=28
make
cp libcrypto.a ../../build/armeabi-v7a/libcrypto.a
cp libcrypto.so ../../build/armeabi-v7a/libcrypto.so
cp libssl.a ../../build/armeabi-v7a/libssl.a
cp libssl.so ../../build/armeabi-v7a/libssl.so
