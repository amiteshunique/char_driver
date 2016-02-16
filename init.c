#include "header.h"

static int __init init_function(void) {
	printk("Hello Kernel.. Here I am.. \n");
	return 0;
}

module_init(init_function);
