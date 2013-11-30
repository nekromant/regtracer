


CROSS_COMPILE?=
CFLAGS=-static
CFILES=util.c

all: regtracer bitdiff


%: %.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -o $(@) $(<) $(CFILES)


push: regtracer
	adb push regtracer /system/bin/regtracer
