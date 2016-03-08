#include "header.h"
#include "scull.h"

loff_t lseekdev(struct file *filep, loff_t off, int whence) {
	struct scull_dev *ldev;
	loff_t newpos;

	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);

	/* Sanity check*/
	if(!filep) {
		printk(KERN_ERR "%s: filep is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}
	
	/*Initialization*/
	newpos = 0;
	ldev = filep->private_data;
	if(!ldev) {
		printk(KERN_ERR "%s: ldev is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}

	printk(KERN_INFO "%s: filep->f_pos=%d %s()\n", __FILE__,(int)filep->f_pos, __FUNCTION__);

	switch (whence) {

		case SEEK_SET:
			newpos = off;
			break;
		case SEEK_CUR:
			newpos = filep->f_pos + off;
			break;
		case SEEK_END:
			newpos = ldev->size + off;
			break;
		default:
			newpos = -1;
			printk(KERN_ERR "%s: lseek has invalid start position in 'whence' in %s()\n", __FILE__, __FUNCTION__);
			goto OUT;
			
	}

	if(newpos < 0 || newpos > ldev->size) 
		goto OUT;
	else
		filep->f_pos = newpos;


	printk(KERN_INFO "%s: filep->f_pos=%d  %s()\n", __FILE__,(int)filep->f_pos, __FUNCTION__);
	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return newpos;

OUT:
	printk(KERN_ERR "%s: Problem with lseek, it crossed the device size boundary(newpos=%d, size=%d) in %s() \n", __FILE__, (int)newpos,(int)ldev->size, __FUNCTION__);
	printk(KERN_INFO "%s: filep->f_pos=%d %s()\n", __FILE__,(int)filep->f_pos, __FUNCTION__);

	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return -EINVAL;
}


