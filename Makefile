#obj-m += init.o
#obj-m += clean.o
obj-m += lkm.o
lkm-objs := init.o clean.o releasedev.o opendev.o sculltrim.o writedev.o readdev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -rf modules
	@mkdir modules
	@mv -f *.o *.ko *.mod.c modules/.
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@rm -rf modules
	@rm -rf write
	@rm -rf a.out
	
