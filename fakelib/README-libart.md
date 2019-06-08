
### About use fake stub library `libart`

- include `include/libsigchain.h` to Application! (weak symbol, needed to original libart.so)  
- edit build options from you application and add to LDFLAGS:  
  - `-Wl,--export-dynamic` and  
  - `-Wl,--version-script,fakelib/vs-libsigchain.txt`  
- build you application  
- check app compiled symbol: `readelf -sW youApp >youApp.list; cat youApp.list;`  
- check stub library compiled symbol: `readelf -sW libart.so >libart.list; cat libart.list;`  

>  
>Note: Version-script file vs-libart.txt only to compile fake stub libart.so
>  

