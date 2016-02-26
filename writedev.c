#include "header.h"

ssize_t writedev(struct file *filep, const char __user *buf, size_t len, loff_t *ppos) {
	printk(KERN_INFO "%s: In start of function %s \n", __FILE__, __FUNCTION__);

	/*
	if(!inodep) {
		printk(KERN_ERR "%s: Inode is NULL in %s\n", __FILE__, __FUNCTION__);
		goto OUT;
	} 
	
	if(!filep) {
		printk(KERN_ERR "%s: FilePointer is NULL in %s()\n",__FILE__,  __FUNCTION__);
		goto OUT;
	}
	*/
	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);
	return 0;

OUT:
	return -1;
}


