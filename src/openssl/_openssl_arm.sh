#!/bin/bash
make distclean
export ANDROID_NDK=/home/user/Programming/android-ndk-r17b
PATH=$ANDROID_NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin:$PATH
./Configure android-arm -D__ANDROID_API__=28
#make
#cp libcrypto.a ../../build/armeabi-v7a/libcrypto.a
#cp libcrypto.so.1.1 ../../build/armeabi-v7a/libcrypto.so.1.1
#cp libssl.a ../../build/armeabi-v7a/libssl.a
#cp libssl.so.1.1 ../../build/armeabi-v7a/libssl.so.1.1
