#include "header.h"
#include "scull.h"

void trimdev(struct scull_dev* ldev) {

	int l;
	struct scull_qset *lqset, *slqset; /*lqset = last qset, slqset = second last qset */
	lqset = ldev->data;

	printk(KERN_INFO "%s: Start Function %s",__FILE__, __FUNCTION__);
	if (lqset == NULL) {
		printk(KERN_INFO "%s: Nothing to be trimmed for now\n", __FILE__);
		goto OUT;
	}

	while(lqset != NULL) {
		printk(KERN_INFO "%s: Start Function1 %s",__FILE__, __FUNCTION__);
		while (lqset != NULL) {
			slqset = lqset;
			lqset = lqset->next;
		}

		printk(KERN_INFO "%s: Start Function2 %s",__FILE__, __FUNCTION__);
		for(l=ldev->quantum-1 ; l>=0 ; l-- ) {
			printk(KERN_INFO "%s: Start Function2.5 %s",__FILE__, __FUNCTION__);
			if(lqset->data[l] != NULL) {
				printk(KERN_INFO "%s: Start Function3 %s",__FILE__, __FUNCTION__);
				kfree(lqset->data[l]);	
				printk(KERN_INFO "%s: Start Function4 %s",__FILE__, __FUNCTION__);
				printk(KERN_INFO "%s: kfreed lqset->data[%d] \n", __FILE__, l);
			}
		}
	
		printk(KERN_INFO "%s: Start Function5 %s",__FILE__, __FUNCTION__);
		kfree(lqset);
		printk(KERN_INFO "%s: kfreed lqset \n", __FILE__);
		slqset->next = NULL;
		printk(KERN_INFO "%s: Start Function6 %s",__FILE__, __FUNCTION__);
	
		lqset = ldev->data;
		printk(KERN_INFO "%s: Start Function7 %s",__FILE__, __FUNCTION__);
	}
	
		
	OUT:
	printk(KERN_INFO "%s: End Function %s",__FILE__, __FUNCTION__);

}
