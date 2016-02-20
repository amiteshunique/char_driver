#obj-m += init.o
#obj-m += clean.o
obj-m += lkm.o
lkm-objs := init.o clean.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -rf modules
	@mkdir modules
	@mv -f *.o *.ko *.mod.c modules/.
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@rm -rf modules
	
