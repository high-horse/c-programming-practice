obj-m += hello-1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# check mod info:  modinfo hello-1.ko
# load : sudo insmod hello-1.ko
# check : msg dmesg | tail
# unload : sudo rmmod hello-1