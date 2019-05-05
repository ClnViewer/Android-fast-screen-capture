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
- default save path: /data/tmp/AScreenCap.bmp

### Stream header format using

Packet stream include no pack header, size 64bit  `uint32[2]`:
- octet `0` - unique identifier `BMZ1`.  
- octetoctet `1` - real size to uncompressed image.

### Using

       adb exec-out /data/tmp/ascreencap -f /data/tmp/my.bmp
       adb exec-out /data/tmp/ascreencap -f /data/tmp/my.bmz // << save lz4 compressed image
       adb exec-out /data/tmp/ascreencap --rotate 90 --ratio 2
       adb exec-out /data/tmp/ascreencap --rotate 360 // << mirror image :)
       adb exec-out /data/tmp/ascreencap --stream
       
