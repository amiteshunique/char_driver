#include "header.h"
#include "declarations.h"

//int majorno, nod;
//dev_t dev;

static void __exit exit_function(void) {
	int i;

	for(i=0; i<nod;i++)
		printk(KERN_INFO "%s: Unregistering char dev now for majorno(%d) for nod(%d) devices\n", __FILE__, majorno, i);

	unregister_chrdev_region(dev, nod);
	printk("%s: Bye Kernel.. See you soon.. \n",__FILE__);
}

module_exit(exit_function);
