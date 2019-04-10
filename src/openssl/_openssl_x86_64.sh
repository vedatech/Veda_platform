#!/bin/bash
make distclean
export ANDROID_NDK=/home/user/Programming/android-ndk-r17b
PATH=$ANDROID_NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin:$PATH
./Configure android-x86_64 -D__ANDROID_API__=28
#make
#cp libcrypto.a ../../build/x86_64/libcrypto.a
#cp libcrypto.so.1.1 ../../build/x86_64/libcrypto.so.1.1
#cp libssl.a ../../build/x86_64/libssl.a
#cp libssl.so.1.1 ../../build/x86_64/libssl.so.1.1
