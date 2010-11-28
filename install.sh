adb shell mount -o remount,rw /dev/mtd/mtdblock4 /system
echo installing cz.havlena.sdl.jar
adb push ../../out/target/product/sapphire/system/framework/cz.havlena.sdl.jar system/framework
echo installing libsdl_jni.so
adb push ../../out/target/product/sapphire/system/lib/libsdl_jni.so system/lib
echo installing libsdl.so
adb push ../../out/target/product/sapphire/system/lib/libsdl.so system/lib