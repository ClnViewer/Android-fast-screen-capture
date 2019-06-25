# Android fast screen capture - image or compressed image stream output

- Android fast screen capture take a screenshot or compressed stream on any other Android device.  
- Not needed root devices.   
- ADB required (default run permissions using).  
- For best perfomance using `OpenMP` for Android.
- Include all using system library, fake version (for [Android 5.1.1](https://github.com/ClnViewer/android-platform-headers/tree/master/android-5.1.1_r1) headers)
- Default build tools: [ndk-build](https://developer.android.com/ndk/downloads?hl=hi#beta-downloads), including Code::Blocks project using [C::B NDK project template](https://github.com/ClnViewer/Code-Blocks-Android-NDK), NDK build auto configuration support.

----

- [Features](#features)
- [Stream/Image compressed - header format using](#streamimage-compressed---header-format-using)
- [Installing pre-build binary](#installing-pre-build)
- [Using from command line](#using)
- [Example using SDL2 output format](#example-using-sdl2-output-format)

----

### Features

- image output format: **raw bitmap** `24 bpp`, include header.
- video stream output format: **raw compressed bitmap** `lz4 stream` - `24 bpp`, include header + packet header.
- video stream is 5 - 20 fps, depends on the CPU device.  
- pack image by ratio: valid values `1-9` (`lz4`).
- resize image by ratio: valid values `1-5`.
- rotate image: `90,180,270`
- mirror image.
- SDL2 [Texture](#example-using-sdl2-output-format) output compatible mode: Landscape screen.
- save image to location specified (bmp format).
- default save path: /data/local/tmp/AScreenCap.bmp

Run time statistic, milliseconds:

            5 ms. - load and initialization
       40-151 ms. - capture screen - android::ScreenshotClient(), depends on the CPU device   
           40 ms. - image convert, resize and rotate
          196 ms. - maximal total time


### Stream/Image compressed - header format using

Packet stream include no pack header, size 160bit  `uint32[5]`, after image compressed body:
- octet `0` - unique identifier `BMZ1` (`0x315a4d42`/`828001602`).  
- octet `1` - size to uncompressed image.
- octet `2` - size to compressed image.
- octet `3` - image width.
- octet `4` - image height.

### Installing pre-build

Download binary for you device from [dist/](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/) directory: 
[arm64-v8a](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/arm64-v8a), 
[armeabi-v7a](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/armeabi-v7a), 
[x86](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/x86), 
[x86_64](https://github.com/ClnViewer/Android-fast-screen-capture/blob/master/dist/x86_64)

Building details:

> Application platform: `android-22`   
> Android system AOSP library version compatible: `5.1.1 - 7.x.x`   
> Android system version >= `9.x.x` __NOT SUPPORT__ (temporary).  
> NDK build version: `r20-beta2`   

Using ADB to send binary to device: 

      adb push dist/<my platform>/ascreencap /data/local/tmp/
      adb shell chmod 0777 /data/local/tmp/ascreencap

### Using

Command line options:

	-f	--file		: output save to file, value is name
	-s	--stream	: output to loop stream (STDOUT)
		--stdout	: output to STDOUT
	-p	--pack		: output pack lz4 algorithm, valid value 1-9
		--sdl		: output SDL2 Texture compatible mode: Landscape screen
		--ratio 	: image resize ratio, valid scale 1-5
		--rotate	: image rotate: 90,180,270, value 360 = mirror
	-h	--help		: this help screen

Example run:

       adb exec-out /data/local/tmp/ascreencap -f /data/local/tmp/my.bmp
       adb exec-out /data/local/tmp/ascreencap -f /data/local/tmp/my.bmz // << save lz4 compressed image
       adb exec-out /data/local/tmp/ascreencap --rotate 90 --ratio 2
       adb exec-out /data/local/tmp/ascreencap --rotate 360 // << mirror image :)
       adb exec-out /data/local/tmp/ascreencap --stream
       

### Example using SDL2 output format

Run:

```sh
adb exec-out /data/local/tmp/ascreencap --stream --sdl --ratio 2 | my-capture-prog.executable
```

Part of code `my-capture-prog.executable`   

```C++
    /// remove first 160bit uint32[5] data header from ascreencap
    std::vector<uint8_t> v('size data from ascreencap');
    v.assign('data from ascreencap'.begin() + 160);
    
    void *pix = nullptr;
    int32_t pitch = 0;
    
    SDL_LockTexture(texture, nullptr, &pix, &pitch);
    if ((!pix) || (!pitch))
        return;

    ::memcpy(pix, &v[0], v.size());

    SDL_UnlockTexture(texture);

```

See full [ADB capture source](https://github.com/ClnViewer/ADB-Android-Viewer/blob/f61a59666fd888ba99c79537f0b4ae4c696eec13/src/ADBDriverDLL/src/DriverSocket/DriverSocketCapture.cpp#L7)

