#include "header.h"
#include "scull.h"
#include "declarations.h"

int devno;		/* Device number passed as parameter */
int nod;		/* Number of devices */
int majorno, minorno;	/* Major and minor numbers*/
dev_t dev;		/* dev -> 12bits for majorno , 20bits for minorno */

extern struct scull_dev *scull_devices; 	/* Decalered in scull.h */
int quantum=QUANTUM, qset=QSET;
unsigned long size=SIZE;

static int __init init_function(void) {
	int ret,i;

	printk("Hello Kernel.. Here I am.. \n");

	/* If number of devices is un-declared then, take default as 1 device*/
	if(nod <= 0)
		nod = 1; 

	/* Registering a char device - starts  */
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

	/* Registering a char device - ends  */
	scull_devices = kmalloc(nod * sizeof(struct scull_dev), GFP_KERNEL);
	memset(scull_devices, 0, nod * sizeof(struct scull_dev));

	for(i=0; i < nod; i++) {
		scull_devices[i].data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		scull_devices[i].quantum = quantum;
		scull_devices[i].qset = qset;
		scull_devices[i].size = size;
	//	scull_devices[i].cdev = ;

	}

	/* Scull device creation - starts */
	
	






	/* Scull device creation - ends */




	return 0;
}

module_init(init_function);
