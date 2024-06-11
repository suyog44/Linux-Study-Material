#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>

static ssize_t file_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t file_write(struct file *, const char __user *, size_t, loff_t *);
static int file_open(struct inode *, struct file *);
static int file_release(struct inode *, struct file *);

dev_t dev;
static struct cdev cdev_ops;
static struct class *dev_class;

static struct file_operations fops = {
	read: file_read,
	write: file_write,
	open: file_open,
	release: file_release,
};


static ssize_t file_read(struct file *file, char __user *buf, size_t len, loff_t *loff){
	printk("device file opened for reading");
	return 0;
}

static ssize_t file_write(struct file *file, const char __user *buf, size_t len, loff_t *loff){
	printk("device file opened for writing");
	return len;
}

static int file_open(struct inode *inode, struct file *file){
	printk("device file opened successfully");
	return 0;
}

static int file_release(struct inode *inode, struct file *file){
	printk("device file released");
	return 0;
}

static int __init file_operations_init(void)
{
	if(alloc_chrdev_region(&dev, 0, 1, "char_device")<0){
		pr_err("unable to register device");
		return -1;
	}

	pr_info("Major:%d Minor:%d\n", MAJOR(dev), MINOR(dev));

	cdev_init(&cdev_ops, &fops);

	if(cdev_add(&cdev_ops, dev, 1)<0){
		pr_err("cannot create device class");
		unregister_chrdev_region(dev,1);
		return -1;
	}


	return 0;
}

static void __exit file_operations_exit(void)
{
	cdev_del(&cdev_ops);
	unregister_chrdev_region(dev, 1);
}

module_init(file_operations_init);
module_exit(file_operations_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("File operations test driver");
MODULE_AUTHOR("B Suyog");
MODULE_VERSION("1.0.0");
