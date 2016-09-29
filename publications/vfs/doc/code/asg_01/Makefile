obj-m += filesystems.o
KDIR ?= $(LINUXSTABLE)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean