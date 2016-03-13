#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/string.h> 		/*For memset() */
#include <linux/uaccess.h> 		/*For copy_from_user */
#include <linux/sched.h> 		/*For TASK_INTERRUPTABLE def */
#include <linux/wait.h> 		/*For wait_queue_head_t */

extern int majorno, nod;
extern dev_t dev;

extern int quantum, qset;
extern unsigned long size;

