all: regtracer
CROSS_COMPILE?=arm-module-linux-gnueabi-


regtracer: regtracer.c
	$(CROSS_COMPILE)gcc -static -o $(@) $(<)

push: regtracer
	adb push regtracer /system/bin/regtracer
	
