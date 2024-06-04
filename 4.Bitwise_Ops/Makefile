obj-m += bitwise_ops.o

KDIR = /lib/modules/$(shell uname -r)/build/

all:
	make -C ${KDIR} M=$(shell pwd) modules
clean:
	make -C ${KDIR} M=$(shell pwd) clean
