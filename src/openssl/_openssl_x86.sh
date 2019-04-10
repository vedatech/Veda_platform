#!/bin/bash
make distclean
export ANDROID_NDK=/home/user/Programming/android-ndk-r17b
PATH=$ANDROID_NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/bin:$PATH
./Configure android-x86 -D__ANDROID_API__=28
#make
#cp libcrypto.a ../../build/x86/libcrypto.a
#cp libcrypto.so.1.1 ../../build/x86/libcrypto.so.1.1
#cp libssl.a ../../build/x86/libssl.a
#cp libssl.so.1.1 ../../build/x86/libssl.so.1.1
