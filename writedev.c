#include "header.h"
#include "scull.h"

ssize_t writedev(struct file *filep, const char __user *buf, size_t len, loff_t *ppos) {
	struct scull_dev *ldev;

	int l;
	long cud_not_copy = 0;
	unsigned long ncsw;	/* no of char successfully written */
	unsigned long nctw;	/* no of char to write */
	unsigned long ncmw;	/* no of char more to write */
	int noq;	/* no of quantums */

	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);

	/* Sanity */	
	if(!filep) {
		printk(KERN_ERR "%s: filep is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}
	
	/* Initialization */
	ldev = filep->private_data;
	ncsw = nctw = 0;
	ncmw = len;
	noq = (ncmw / ldev->quantum) + (ncmw % ldev->quantum ? 1 : 0 );

	/*qset allocation*/
	ldev->data = (struct scull_qset*) kmalloc( sizeof(struct scull_qset), GFP_KERNEL );
	if(!ldev->data)
		printk(KERN_ERR "%s: memory allocation for ldev->data (ie, qset) failed %s()\n", __FILE__, __FUNCTION__);
	memset(ldev->data, 0, sizeof(struct scull_qset) );

	/*qset array allocation*/
	ldev->data->data = (void **) kmalloc( ldev->qset * sizeof(char*), GFP_KERNEL);
	if(!ldev->data->data)
		printk(KERN_ERR "%s: memory allocation for ldev->data->data (ie qset array) failed %s()\n", __FILE__, __FUNCTION__);
	memset(ldev->data->data, 0, ldev->qset * sizeof(char*));

	/* Writing in quantums */
	for(l=0; l < noq; l++) {	

		/*quantum allocation*/
		ldev->data->data[l] = (void **) kmalloc(ldev->quantum * sizeof(char) , GFP_KERNEL);
		if(!ldev->data->data[l])
			printk(KERN_ERR "%s: memory allocation for ldev->data->data[] (ie quantum) failed %s()\n", __FILE__, __FUNCTION__);	
		memset(ldev->data->data[l], 0, ldev->quantum * sizeof(char));

		if(ncmw > ldev->quantum)
			nctw = ldev->quantum;
		else
			nctw = ncmw;

		cud_not_copy = copy_from_user(ldev->data->data[l], buf+ncsw, nctw);

		if(cud_not_copy == 0) 
			printk(KERN_INFO "%s: Wrote %ld bytes successfully in %s() \n", __FILE__, nctw-cud_not_copy, __FUNCTION__);
		else	
			printk(KERN_INFO "%s: Patial Write %ld bytes successfully in %s() \n", __FILE__, nctw-cud_not_copy, __FUNCTION__);
		
		//printk(KERN_INFO "%s: Wrote: %s \n", __FILE__, (char*)ldev->data->data[l]);

		ncsw = ncsw + (nctw - cud_not_copy);
		ncmw = ncmw - (nctw - cud_not_copy);
	}

	/* Updating no of char written successfully in device */	
	ldev->size = ncsw;

	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return ncsw;

OUT:
	return -1;
}


