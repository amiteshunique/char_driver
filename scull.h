struct scull_qset {
   void **data;
   struct scull_qset *next;
};

struct scull_dev {
   struct scull_qset *data;  /* Pointer to first quantum set */
   int quantum;              /* the current quantum size */
   int qset;                 /* the current array size */
   unsigned long size;       /* amount of data stored here */
   struct cdev cdev;         /* Char device structure              */
};
