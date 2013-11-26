all: regtracer

regracer: regtracer.c
	$(CROSS_COMPILE)gcc -static -o $(@) $(<)
