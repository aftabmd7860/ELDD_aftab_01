#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/kthread.h>
#include<linux/wait.h>
#include<linux/semaphor>

uint32_t read_count =0;
uint32_t write_count =0;
static struct task_struct *wait_thread;
dev_t dev=0;
static struct class *dev_class;
static struct cdev wq_cdev;
wait_queue_head_t wait_queue_wq;
int wait_queue_flag =0;
struct semaphor my_sema;



//function prototype
static int __init wq_driver_init(void);
static void __exit wq_driver_exit(void);
static int      wq_open(struct inode *inode,struct file *file);
static int      wq_release(struct inode *inode,struct file *file);
static ssize_t   wq_read(struct file *filp,char __user *buf,size_t len,loff_t * off);
static ssize_t   wq_write(struct file *filp,const char *buf,size_t len,loff_t * off);

//file operation
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = wq_read,
    .write      = wq_write,
    .open       = wq_open,
    .release    = wq_release,
};
static int wait_function(void *unused)
{
    while(1){
            pr_info("waiting for event...\n");
            wait_event_interruptible(wait_queue_wq, wait_queue_flag !=0);
            if(wait_queue_flag == 2){
                pr_info("event came to exit fun\n");
                return 0;
            }
            if(wait_queue_flag==1){
                pr_info("event came from read function %d\n",++read_count);
            }
            if(wait_queue_flag ==3){
                pr_info("event came from write function -%d\n",++write_count);
                wait_queue_flag =0;
            }
            wait_queue_flag=0;

            }
}



static int wq_open(struct inode *inode,struct file *file)
{
    pr_info("device file opened...!!!\n");
    return 0;
}
static int   wq_release(struct inode *inode,struct file *file)
{
    pr_info("device file opened...!!!\n");
    return 0;
}

// THIS FILE WILL CALLED AFTER READ DEVICE FILE

static ssize_t wq_read(struct file *filp,char __user *buf,size_t len,loff_t * off)
{
    
    pr_info("data read :done\n");
    wait_queue_flag=1;
    wake_up_interruptible(&wait_queue_wq);
    return 0;
}

static ssize_t wq_write(struct file *filp,const char __user *buf,size_t len,loff_t * off)
{

    pr_info("data write :done\n");
    wait_queue_flag=3;
    wake_up_interruptible(&wait_queue_wq);
    return len;
}
static int __init wq_driver_init(void)
{
    if((alloc_chrdev_region(&dev,0,1,"hc_dev"))<0)
    {
        pr_info("cannot allocate major no.\n");
        return -1;
    }
    pr_info("Major = %d minor =%d\n",MAJOR(dev),MINOR(dev));
    cdev_init(&wq_cdev,&fops);

    if((cdev_add(&wq_cdev,dev,1))<0){

    pr_info("cannot add the device\n");
    goto r_class;}

    if((dev_class = class_create(THIS_MODULE,"hc_class"))==NULL)
    {
    pr_info("cannot create the struct class\n");
    goto r_class;
    }

    if((device_create(dev_class,NULL,dev,NULL,"hc_dev"))==NULL)
    {
    pr_info("cannot create the device\n");
    goto r_device;
    }

    init_waitqueue_head(&wait_queue_wq);
    wait_thread = kthread_create(wait_function,NULL,"waitThread");
    if(wait_thread)
    {
        pr_info("thread created ............\n");
        wake_up_process(wait_thread);
    }else{
        pr_info("thread created failed\n");
        pr_info("device driver insert ..done !!!!!!!\n");
        return 0;
    }

    r_device:
        class_destroy(dev_class);
    r_class:
        unregister_chrdev_region(dev,1);
        return -1;
    }
    static void __exit wq_driver_exit(void)
    {
        wait_queue_flag =2;
        wake_up_interruptible(&wait_queue_wq);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&wq_cdev);
        unregister_chrdev_region(dev,1);
        pr_info("device drivere removed\n");
    }
module_init(wq_driver_init);
module_exit(wq_driver_exit);

MODULE_AUTHOR("Aftab Alam");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");
MODULE_INFO(board,"RASPBERRY PI 4");





















