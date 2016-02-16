#include "header.h"
#include "declarations.h"

int devno;		/* Device number passed as parameter */
int nod;		/* Number of devices */
int majorno, minorno;	/* Major and minor numbers*/
dev_t dev;		/* dev -> 12bits for majorno , 20bits for minorno */

static int __init init_function(void) {
	int ret;

	printk("Hello Kernel.. Here I am.. \n");

	/* If number of devices is un-declared then, take default as 1 device*/
	if(nod <= 0)
		nod = 1; 

	if(devno) {
		ret = register_chrdev_region(devno, nod, DEVNAME);
		if (ret) 
			printk("init.c: Problem with register_chrdev_region.\n");
		else {
			dev = MKDEV(devno, minorno);
			majorno = MAJOR(dev);
			minorno = MINOR(dev);
			printk("init.c: Successfully registered the chrdev region. majorno(%d) minorno(%d) \n", majorno, minorno);
		}
	} else {
		ret = alloc_chrdev_region(&dev, minorno, nod, DEVNAME );
		if (ret) 
			printk("init.c: Problem with alloc_chrdev_region.\n");
		else {
			majorno = MAJOR(dev);
			minorno = MINOR(dev);
			printk("init.c: Successfully registered the chrdev region. majorno(%d) minorno(%d) \n", majorno, minorno);
		}


	}


	return 0;
}

module_init(init_function);
