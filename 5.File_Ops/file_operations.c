#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

static ssize_t file_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t file_write(struct file *, const char __user *, size_t, loff_t *);
static int file_open(struct inode *, struct file *);
static int file_release(struct inode *, struct file *);

dev_t dev;

static struct file_operations fops = {
	read: file_read,
	write: file_write,
	open: file_open,
	release: file_release,
};


static ssize_t file_read(struct file *file, char __user *buf, size_t len, loff_t *loff){
	return 0;
}

static ssize_t file_write(struct file *file, const char __user *buf, size_t len, loff_t *loff){
	return len;
}

static int file_open(struct inode *inode, struct file *file){
	return 0;
}

static int file_release(struct inode *inode, struct file *file){
	return 0;
}

static int __init file_operations_init(void)
{
	if(alloc_chrdev_region(&dev, 0, 1, "char_device")<0){
		pr_err("unable to register device");
		return -1;
	}

	return 0;
}

static void __exit file_operations_exit(void)
{
	unregister_chrdev_region(&dev, 1);
}

module_init(file_operations_init);
module_exit(file_operations_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("File operations test driver");
MODULE_AUTHOR("B Suyog");
MODULE_VERSION("1.0.0");
