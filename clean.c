#include "header.h"

static void __exit exit_function(void) {
	printk("Bye Kernel.. See you soon.. \n");
}

module_exit(exit_function);
