struct scull_qset {
   void **data;
   struct scull_qset *next;
};

struct scull_dev {
   struct scull_qset *data;  /* Pointer to first quantum set */
   int quantum;              /* number of quantums(each quantum is capable of storing 1byte) in each entry of qset-array */
   int qset;                 /* total no of qsets in the device */
   unsigned long size;       /* amount of data stored here */
   struct cdev cdev;         /* Char device structure  */
};

extern struct scull_dev *scull_devices;
extern void scull_trim(struct scull_dev*);
