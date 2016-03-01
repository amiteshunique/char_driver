#include "header.h"
#include "scull.h"

ssize_t readdev(struct file *filep, char __user *buf, size_t len, loff_t *ppos) {
	struct scull_dev *ldev;
	unsigned long nctr; 	/* no of char to read*/ 
	unsigned long ncsr; 	/* no of char successfully read*/
	unsigned long ncmr; 	/* no of char more to read  */
	unsigned long cud_not_copy; 
	int noq; 	/* no of quantums */
	int l;

	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);

	/* Sanity check*/
	if(!filep) {
		printk(KERN_ERR "%s: filep is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}
	
	/*Initialization*/
	ldev = filep->private_data;
	if(!ldev) {
		printk(KERN_ERR "%s: ldev is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}
	nctr = ncsr = cud_not_copy = noq= 0;
	if( len > ldev->size)
		ncmr = ldev->size;
	else
		ncmr = len;

	/*Reading from noq: number of quantum*/
	noq = (ncmr / ldev->quantum) + ( ncmr % ldev->quantum ? 1 : 0 );

	for(l=0; l<noq; l++) {
		if(ncmr > ldev->quantum)
			nctr = ldev->quantum;
		else 	
			nctr = ncmr;

		cud_not_copy = copy_to_user(buf+ncsr, ldev->data->data[l], nctr);

		if(cud_not_copy == 0) {
			printk(KERN_INFO "%s: Read %ld bytes successfully in function %s() \n", __FILE__, nctr - cud_not_copy, __FUNCTION__);
		} else
			printk(KERN_INFO "%s: Partial Read %ld bytes(out of %ld) successfully in function %s() \n", __FILE__, nctr-cud_not_copy, nctr, __FUNCTION__);
		
		//printk(KERN_INFO "%s: Quantum has:>%s<\n", __FILE__, (char*)ldev->data->data[l]);
		ncsr = ncsr + (nctr - cud_not_copy);
		ncmr = ncmr - (nctr - cud_not_copy);
	
	}
	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return ncsr;

OUT:
	return -1;
}


