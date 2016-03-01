extern int opendev(struct inode*, struct file*);
extern int releasedev(struct inode*, struct file*);
extern ssize_t writedev(struct file *filep, const char __user *buf, size_t len, loff_t *ppos);
extern ssize_t readdev(struct file *filep, char __user *buf, size_t len, loff_t *ppos);

struct file_operations fops = {
	open : opendev,
	release : releasedev,
	write : writedev,
	read : readdev
};

