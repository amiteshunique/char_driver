#include "header.h"
#include "scull.h"

ssize_t readdev(struct file *filep, char __user *buf, size_t len, loff_t *ppos) {
	struct scull_dev *ldev;
	struct scull_qset *lqset;
	unsigned long nctr; 	/* no of char to read*/ 
	unsigned long ncsr; 	/* no of char successfully read*/
	unsigned long ncmr; 	/* no of char more to read  */
	unsigned long cud_not_copy; 
	int noq; 	/* no of quantums */
	int l,i;

	char *start;
	int off_quantum; 	/* offset in a given entry of qset array */
	int off_q_arr;		/* offset in qset array */
	int off_qset;		/* offset from start node in qset linked list */

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
	//if( len > ldev->size)
	//	ncmr = ldev->size;
	//else
		ncmr = len;

	/* Debug: Checking position*/
	printk(KERN_INFO "%s: filep->f_pos=%d  & ppos=%d  %s()\n", __FILE__,(int)filep->f_pos,(int) *ppos, __FUNCTION__);
	
	/*Reading from noq: number of quantum*/
	noq = (ncmr / ldev->quantum) + ( ncmr % ldev->quantum ? 1 : 0 );

	lqset = ldev->data;
	i = 0;
	
	/* Offset calculation for lseek */
	off_qset = filep->f_pos / (ldev->quantum * ldev->qset);
	off_q_arr = (filep->f_pos % (ldev->quantum * ldev->qset) ) / ldev->quantum;
	off_quantum = (filep->f_pos % (ldev->quantum * ldev->qset) ) % ldev->quantum;
	
	for(l=0; l<off_qset;l++) {
		if(lqset != NULL) {
			lqset = lqset->next;
		} else {
			printk(KERN_ERR "%s: Problem with parsing qsets while in function: %s()\n", __FILE__, __FUNCTION__);
			goto OUT;
		}
	}

	start = lqset->data[off_q_arr] + off_quantum;
	ncsr = 0;
	i = off_q_arr;
	printk(KERN_INFO "%s: start=%c, off_qset=%d, off_q_arr=%d, off_quantum=%d,  %s() \n", __FILE__, *start, (int)off_qset, (int)off_q_arr, (int)off_quantum, __FUNCTION__);
	
	while(ncmr > 0) {
		if(ncmr > ldev->quantum)
			nctr = ldev->quantum;
		else 	
			nctr = ncmr;

		if(nctr >= ldev->quantum - off_quantum ) {
			nctr = ldev->quantum - off_quantum;
		} else {
			nctr = nctr - off_quantum;
		}
			
		printk(KERN_INFO "%s: ncsr=%d, nctr=%d, ncmr=%d  %s() \n", __FILE__, (int)ncsr, (int)nctr, (int)ncmr, __FUNCTION__);

		cud_not_copy = copy_to_user(buf+ncsr, start, nctr);
		
		printk(KERN_INFO "Start: %.4s \n",start );
		if(cud_not_copy == 0) {
			printk(KERN_INFO "%s: Read %ld bytes successfully <%s>in function %s() \n", __FILE__, nctr -off_quantum -cud_not_copy, buf, __FUNCTION__);
		} else
			printk(KERN_INFO "%s: Partial Read %ld bytes(out of %ld) successfully <%s>in function %s() \n", __FILE__, nctr-cud_not_copy, nctr,buf, __FUNCTION__);
		
		printk(KERN_INFO "%s: ncmr=%d\n", __FILE__, (int)ncmr);
		ncsr = ncsr + (nctr -off_quantum - cud_not_copy);
		ncmr = ncmr - (nctr -off_quantum - cud_not_copy);
		off_quantum = 0;
		i++;

		if( i == ldev->qset && ncmr !=0) {
			if( lqset->next == NULL ) {
				printk(KERN_ERR "%s: ldev->next has a problem while reading \n", __FILE__ );
				goto OUT;
			}
			i = 0;
			lqset = lqset->next;
		}
		printk(KERN_ERR "%s: i(=%d)  \n", __FILE__ , i);
		start = lqset->data[i];
	
	}
	filep->f_pos = filep->f_pos + ncsr;
	*ppos = filep->f_pos;

	/* Debug: Checking position*/
	printk(KERN_INFO "%s: filep->f_pos=%d  & ppos=%d  %s()\n", __FILE__,(int)filep->f_pos,(int) *ppos, __FUNCTION__);

	printk(KERN_INFO "%s: In end of function %s() \n", __FILE__, __FUNCTION__);
	return ncsr;

OUT:
	return -1;
}


