adb shell mount -o remount,rw /dev/mtd/mtdblock4 /system
adb push out/target/product/sapphire/system/framework/cz.havlena.sdl.jar system/framework
adb push out/target/product/sapphire/system/lib/libsdl_jni.so system/lib