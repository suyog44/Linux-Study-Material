obj-m += i2c_mpr121.o

KDIR = /lib/modules/$(shell uname -r)/build/

all:
	make -C ${KDIR} M=$(shell pwd) modules
clean:
	make -C ${KDIR} M=$(shell pwd) clean
