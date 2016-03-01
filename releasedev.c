#include "header.h"

int releasedev(struct inode* inodep, struct file* filep) {
	printk(KERN_INFO "%s: In start of function %s \n", __FILE__, __FUNCTION__);

	if(!inodep) {
		printk(KERN_ERR "%s: Inode is NULL in %s\n", __FILE__, __FUNCTION__);
		goto OUT;
	} 
	
	if(!filep) {
		printk(KERN_ERR "%s: FilePointer is NULL in %s()\n",__FILE__,  __FUNCTION__);
		goto OUT;
	}
	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);
	return 0;

OUT:
	return -1;
}


