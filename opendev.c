#include "header.h"
#include "scull.h"

int opendev(struct inode* inodep, struct file* filep) {
	struct scull_dev *ldev;

	ldev = scull_devices;


	printk(KERN_INFO "%s: In start of function %s \n",__FILE__,  __FUNCTION__);

	if(inodep == NULL) {
		printk(KERN_ERR "%s: Inode is NULL\n", __FILE__);
		goto OUT;
	} 
	
	if(filep == NULL) {
		printk(KERN_ERR "%s: FilePointer is NULL\n", __FILE__);
		goto OUT;
	}
	if(ldev == NULL) {
		printk(KERN_ERR "%s: ldev is NULL\n", __FILE__);
		goto OUT;
	}
	
	ldev = container_of(inodep->i_cdev, struct scull_dev, cdev);
	
	if(ldev == NULL) {
		printk(KERN_ERR "%s: ldev - problem with container_of\n", __FUNCTION__);
		goto OUT;
	}
	filep->private_data = ldev;

	if( (filep->f_flags & O_ACCMODE) == O_WRONLY ) {
		scull_trim(ldev);
	}
	
	printk(KERN_INFO "%s: End of function %s \n",__FILE__,  __FUNCTION__);

	return 0;

OUT:
	return 0;
}


