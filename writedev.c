#include "header.h"
#include "scull.h"

ssize_t writedev(struct file *filep, const char __user *buf, size_t len, loff_t *ppos) {
	struct scull_dev *ldev;

	int itemsize;
	long cud_not_copy = 0;

	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);
	
	if(!filep) {
		printk(KERN_ERR "%s: filep is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}
	
	ldev = filep->private_data;

	itemsize = ldev->quantum * ldev->qset;

	ldev->data = (struct scull_qset*) kmalloc( sizeof(struct scull_qset), GFP_KERNEL );

	if(!ldev->data)
		printk(KERN_ERR "%s: memory allocation for ldev->data (ie, qset) failed %s()\n", __FILE__, __FUNCTION__);
		
	memset(ldev->data, 0, sizeof(struct scull_qset) );

	
	ldev->data->data = (void **) kmalloc( ldev->qset * sizeof(char*), GFP_KERNEL);
	if(!ldev->data->data)
		printk(KERN_ERR "%s: memory allocation for ldev->data-> (ie quantum) failed %s()\n", __FILE__, __FUNCTION__);
		
	memset(ldev->data->data, 0, ldev->qset * sizeof(char*));
	
	
	ldev->data->data[0] = (void **) kmalloc(ldev->quantum * sizeof(char) , GFP_KERNEL);
	memset(ldev->data->data[0], 0, ldev->quantum * sizeof(char));

	cud_not_copy = copy_from_user(ldev->data->data[0], buf, len);

	if(cud_not_copy == 0) {
		printk(KERN_INFO "%s: Copied %ld bytes successfully in function %s() \n", __FILE__, len, __FUNCTION__);
		printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
		return len - cud_not_copy;
	}
	
	
	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return 0;

OUT:
	return -1;
}


