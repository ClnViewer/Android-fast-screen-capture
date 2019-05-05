# Android fast screen capture - image or compressed image strem output

- Android fast screen capture take a screenshot or compressed stream on any other Android device.  
- Not needed root devices.   
- ADB required.  
- Include all using system library, fake version (for [Android 5.1.1](https://github.com/pfalcon/android-platform-headers/tree/master/android-5.1.1_r1) headers)

----

### Features

- image output format: **raw bitmap** `24 bpp`, include header.
- stream output format: **raw compressed bitmap** `lz4 stream` - `24 bpp`, include header + packet header.
- resize image by ratio: valid values `1-5`.
- rotate image: `90,180,270`
- mirror image.
- save image to location specified (bmp format).

