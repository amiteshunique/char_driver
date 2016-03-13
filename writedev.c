#include "header.h"
#include "scull.h"

ssize_t writedev(struct file *filep, const char __user *buf, size_t len, loff_t *ppos) {

	struct scull_dev *ldev;
	struct scull_qset *lqset, *firstqset;

	int l, i;
	long cud_not_copy = 0;
	unsigned long ncsw;	/* no of char successfully written */
	unsigned long nctw;	/* no of char to write */
	unsigned long ncmw;	/* no of char more to write */
	int noq;		/* no of quantums */
	int qset_count;		/* no of qsets */

	printk(KERN_INFO "%s: In start of function %s() \n", __FILE__, __FUNCTION__);

	/* Sanity */	
	if(!filep) {
		printk(KERN_ERR "%s: filep is NULL in %s()\n", __FILE__, __FUNCTION__);
		goto OUT;
	}
	
	/* Initialization */
	ldev = filep->private_data;
	ncsw = nctw = 0;
	qset_count = 0;
	ncmw = len;
	noq = (ncmw / ldev->quantum) + (ncmw % ldev->quantum ? 1 : 0 );
	printk(KERN_ERR "%s: noq= %d :  %s()\n", __FILE__,noq, __FUNCTION__);
	qset_count = ( ncmw / (ldev->quantum * ldev->qset) ) + (ncmw % (ldev->quantum * ldev->qset) ? 1 : 0);
	lqset = ldev->data;
	firstqset = NULL;

	printk(KERN_INFO "%s: filep->f_pos=%d  & ppos=%d  %s()\n", __FILE__,(int)filep->f_pos,(int) *ppos, __FUNCTION__);

	/* Allocation of QSETS & QSET ARRAY */
	for(l=0 ; l< qset_count; l++) {
		
		if( l == 0 ) {
			/* Allocation of First Qset of this Device..  */
			/*qset allocation*/
			ldev->data = (struct scull_qset*) kmalloc( sizeof(struct scull_qset), GFP_KERNEL );
			if( ldev->data == NULL)
				printk(KERN_ERR "%s: memory allocation for ldev->data (ie, qset) failed %s()\n", __FILE__, __FUNCTION__);
			memset(ldev->data, '\0', sizeof(struct scull_qset) );
			
			firstqset = ldev->data;
	
			/*qset array allocation*/
			ldev->data->data = (void **) kmalloc( ldev->qset * sizeof(char*), GFP_KERNEL);
			if(!ldev->data->data)
				printk(KERN_ERR "%s: memory allocation for ldev->data->data (ie qset array) failed %s()\n", __FILE__, __FUNCTION__);
			memset(ldev->data->data, '\0', ldev->qset * sizeof(char*)); 
		
			/* Now localqset should point to the firstqset should & is ready to create new links of qset*/
			lqset = ldev->data; 

		} else {
			/* lqset stands at the last qset available, so allocating for next qset now.. */

			/*qset allocation*/
			lqset->next = (struct scull_qset*) kmalloc( sizeof(struct scull_qset), GFP_KERNEL );
			if( lqset->next == NULL )
				printk(KERN_ERR "%s: memory allocation for lqset->next (ie, qset) failed %s()\n", __FILE__, __FUNCTION__);
			else
				printk(KERN_INFO "%s: memory allocation for lqset->next (ie, qset) was success: %s()\n", __FILE__, __FUNCTION__);
			memset(lqset->next,'\0', sizeof(struct scull_qset) );
		
			/*qset array allocation*/
			lqset->next->data = (void **) kmalloc( ldev->qset * sizeof(char*), GFP_KERNEL);
			if(!lqset->next->data)
				printk(KERN_ERR "%s: memory allocation for ldev->data->data (ie qset array) failed %s()\n", __FILE__, __FUNCTION__);
			memset(lqset->next->data, '\0', ldev->qset * sizeof(char*)); 

			/* Now lqset stands at the last available qset */
			lqset = lqset->next;
		}
	}

	/* Reinitializing lqset to first qset for writing */
	lqset = firstqset;
	i = 0 ;
	/* Writing in quantums */
	for(l=0; l < noq; l++) {	

		/*quantum allocation*/
		lqset->data[i] = (void **) kmalloc(ldev->quantum * sizeof(char) , GFP_KERNEL);
		if(!lqset->data[i])
			printk(KERN_ERR "%s: memory allocation for ldev->data->data[] (ie quantum) failed %s()\n", __FILE__, __FUNCTION__);	
		memset(lqset->data[i], '\0', ldev->quantum * sizeof(char));

		if(ncmw > ldev->quantum)
			nctw = ldev->quantum;
		else
			nctw = ncmw;

		cud_not_copy = copy_from_user(lqset->data[i], buf+ncsw, nctw);

		if(cud_not_copy == 0) 
			printk(KERN_INFO "%s: Wrote %ld bytes successfully in %s() \n", __FILE__, nctw-cud_not_copy, __FUNCTION__);
		else	
			printk(KERN_INFO "%s: Patial Write %ld bytes successfully in %s() \n", __FILE__, nctw-cud_not_copy, __FUNCTION__);
		
		//printk(KERN_INFO "%s: Wrote: %s \n", __FILE__, (char*)ldev->data->data[l]);

		ncsw = ncsw + (nctw - cud_not_copy);
		ncmw = ncmw - (nctw - cud_not_copy);

		i++;

		if( i == ldev->qset && ncmw!=0 ) {
			if( !(lqset->next) ) {
				printk(KERN_ERR "%s: Problem with lqset->next for i=%d in %s() \n", __FILE__, i, __FUNCTION__);
				goto OUT;
			}

			lqset = lqset->next;
			i = 0;		
		}
	}

	/* Updating no of char written successfully in device */	
	ldev->size = ncsw;
	
	filep->f_pos = ncsw;
	*ppos = ncsw;
	printk(KERN_INFO "%s: filep->f_pos=%d  & ppos=%d  %s()\n", __FILE__,(int)filep->f_pos,(int) *ppos, __FUNCTION__);

	wake_up_interruptible(&ldev->scull_queue);
	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return ncsw;

OUT:
	return -1;
}


