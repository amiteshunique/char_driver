#include "header.h"
#include "declarations.h"


static void __exit exit_function(void) {
	printk(KERN_INFO "clean.c : Unregistering char dev now for majorno(%d) for nod(%d) devices\n", majorno, nod);
	unregister_chrdev_region(dev, nod);
	printk("Bye Kernel.. See you soon.. \n");
}

module_exit(exit_function);
