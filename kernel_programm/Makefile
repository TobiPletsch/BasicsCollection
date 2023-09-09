obj-m += hello.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	mkdir -p modules
	make -C $(KERNELDIR) M=$(PWD) modules
	mv -t modules/ $(filter-out Makefile hello.c, $(wildcard *))

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -rf modules/

# Load module into kernel: "sudo insmod hello.ko" 
# List module: "lsmod | grep hello"
# Remove module: "sudo rmmod hello" 
# Check kernel log: "sudo dmesg | tail -n 10"
