# Android fast screen capture - image or compressed image stream output

- Android fast screen capture take a screenshot or compressed stream on any other Android device.  
- Not needed root devices.   
- ADB required (default run permissions using).  
- Include all using system library, fake version (for [Android 5.1.1](https://github.com/pfalcon/android-platform-headers/tree/master/android-5.1.1_r1) headers)
- Default build tools: [ndk-build](https://developer.android.com/ndk/downloads?hl=hi#beta-downloads), including Code::Blocks project using [C::B NDK project template](https://github.com/ClnViewer/Code-Blocks-Android-NDK), NDK build auto configuration support.
- For best perfomance using `OpenMP` for Android.

----

### Features

- image output format: **raw bitmap** `24 bpp`, include header.
- stream output format: **raw compressed bitmap** `lz4 stream` - `24 bpp`, include header + packet header.
- resize image by ratio: valid values `1-5`.
- rotate image: `90,180,270`
- mirror image.
- save image to location specified (bmp format).
- default save path: /data/local/tmp/AScreenCap.bmp

Run time statistic, milliseconds:

        5 ms. - load and initialization
      151 ms. - capture screen - android::ScreenshotClient()
       40 ms. - image conert, resize and rotate
      196 ms. - total time


### Stream/Image compressed - header format using

Packet stream include no pack header, size 64bit  `uint32[2]`, after image compressed body:
- octet `0` - unique identifier `BMZ1`.  
- octet `1` - real size to uncompressed image.

### Installing pre-build

Download binary for you device from [dist/](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/) directory: 
[arm64-v8a](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/arm64-v8a), 
[armeabi-v7a](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/armeabi-v7a), 
[x86](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/x86), 
[x86_64](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/x86_64)

Building details:

> Application platform: `android-22`   
> Android system library compatible: `5.1.1`   
> NDK build version: `r20-beta2`   

Using ADB to send binary to device: 

      adb pull dist/<my platform>/ascreencap /data/local/tmp/
      adb shell chmod 0777 /data/local/tmp/ascreencap

### Using

       adb exec-out /data/local/tmp/ascreencap -f /data/local/tmp/my.bmp
       adb exec-out /data/local/tmp/ascreencap -f /data/local/tmp/my.bmz // << save lz4 compressed image
       adb exec-out /data/local/tmp/ascreencap --rotate 90 --ratio 2
       adb exec-out /data/local/tmp/ascreencap --rotate 360 // << mirror image :)
       adb exec-out /data/local/tmp/ascreencap --stream
       
