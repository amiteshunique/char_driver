extern int opendev(struct inode*, struct file*);
extern int releasedev(struct inode*, struct file*);

struct file_operations fops = {
	open : opendev,
	release : releasedev
};

