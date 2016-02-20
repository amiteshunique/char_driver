#include "header.h"
#include "scull.h"
#include "declarations.h"
#include "file_ops.h"

int devno;		/* Device number passed as parameter */
int nod;		/* Number of devices */
int majorno, minorno;	/* Major and minor numbers*/
dev_t dev;		/* dev -> 12bits for majorno , 20bits for minorno */

extern struct scull_dev *scull_devices; 	/* Decalered in scull.h */
int quantum=QUANTUM, qset=QSET;
unsigned long size=SIZE;

module_param(nod, int, S_IRUGO);

static int __init init_function(void) {

	int ret,i;
	struct cdev cdev_entry;
	dev_t dev_entry;		/* dev_entry - used for making entry for each device */

	printk("%s: Hello Kernel.. Here I am.. \n", __FILE__);

	/* If number of devices is un-declared then, take default as 1 device*/
	if(nod <= 0)
		nod = 1; 

	/* Registering a char device - starts  */
	if(devno) {
		ret = register_chrdev_region(devno, nod, DEVNAME);
		if (ret) 
			printk("%s: Problem with register_chrdev_region.\n", __FILE__);
		else {
			dev = MKDEV(devno, minorno);
			majorno = MAJOR(dev);
			minorno = MINOR(dev);
			printk("%s: Successfully registered the chrdev region. majorno(%d) minorno(%d) \n", __FILE__, majorno, minorno);
		}
	} else {
		ret = alloc_chrdev_region(&dev, minorno, nod, DEVNAME );
		if (ret) 
			printk("%s: Problem with alloc_chrdev_region.\n", __FILE__);
		else {
			majorno = MAJOR(dev);
			minorno = MINOR(dev);
			printk("%s: Successfully registered the chrdev region. majorno(%d) minorno(%d) \n", __FILE__, majorno, minorno);
		}
	}
	/* Registering a char device - ends  */


	/* Scull device creation - starts */
	scull_devices = kmalloc(nod * sizeof(struct scull_dev), GFP_KERNEL);
	memset(scull_devices, 0, nod * sizeof(struct scull_dev));

	for(i=0; i < nod; i++) {
		scull_devices[i].data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		scull_devices[i].quantum = quantum;
		scull_devices[i].qset = qset;
		scull_devices[i].size = size;
		
		/* cdev operations - start */
		cdev_init(&cdev_entry, &fops);
		cdev_entry.owner = THIS_MODULE;
		cdev_entry.ops = &fops;
		
		dev_entry = MKDEV(majorno, minorno + i);
		ret = cdev_add(&cdev_entry, dev_entry, 1 );
		if(ret) {
			printk("%s: Problem with cdev_add.\n", __FILE__);	
			goto fail;
		} else
			printk("%s: Successfully added the cdev\n", __FILE__);
		scull_devices[i].cdev = cdev_entry;
		/* cdev operations - end */

	}

	/* Scull device creation - ends */

	
	for(i=0; i < nod; i++) {
		printk("%s: Scull device: majorno(%d) minorno(%d)\n", __FILE__, MAJOR(scull_devices[i].cdev.dev) , MINOR(scull_devices[i].cdev.dev)   );
	}


	return 0;
fail:
	return -1;
}

module_init(init_function);
