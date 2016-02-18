#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

extern int majorno, nod;
extern dev_t dev;

extern int quantum, qset;
extern unsigned long size;

