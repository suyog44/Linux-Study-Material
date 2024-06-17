obj-m += mpr121_i2c.o

KDIR = /lib/modules/$(shell uname -r)/build/

all: module dt

module:
	make -C ${KDIR} M=$(shell pwd) modules
dt:
	dtc -@ -I dts -O dtb -o mpr121_overlay.dtbo mpr121_overlay.dts
clean:
	make -C ${KDIR} M=$(shell pwd) clean
