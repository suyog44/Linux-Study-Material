#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEV_NAME "chardev"
#define BUF_SIZE 90

static struct class *dev_class;
static char mesg[BUF_SIZE];
static int major;

static ssize_t chardev_read (struct file *, char __user *, size_t, loff_t *);
static ssize_t chardev_write (struct file *, const char __user *, size_t, loff_t *);
static int chardev_open (struct inode *, struct file *);
static int chardev_release (struct inode *, struct file *);

static struct file_operations fops = {
	read: chardev_read,
	write: chardev_write,
	open: chardev_open,
	release: chardev_release,
};

static int __init chardev_init(void)
{
	major = register_chrdev(0, DEV_NAME, &fops);
	if(major<0){
		pr_err("Major number allcation for /dev/%s failed", DEV_NAME);
		return -1;
	}
	pr_info("Major Number:%d", major);

	dev_class = class_create(THIS_MODULE, DEV_NAME);
	device_create(dev_class, NULL, MKDEV(major,0), NULL, DEV_NAME);
	pr_info("Device create at path /dev/%s", DEV_NAME);

	return 0;
}

static void __exit chardev_exit(void)
{
	device_destroy(dev_class, MKDEV(major, 0));
	class_destroy(dev_class);
	unregister_chrdev(major, DEV_NAME);
	pr_info("Char dev module unloaded successfully");
}

static ssize_t chardev_read (struct file *flip, char __user *buf, size_t len, loff_t *offset)
{
	int byte_read = 0;
	const char *msg_ptr = mesg;
	if(!*(msg_ptr+*offset)){
		*offset = 0;
		return 0;
	}

	msg_ptr += *offset;

	while(len && *msg_ptr){
		put_user(*(msg_ptr++),buf++);
		len--;
		byte_read++;
	}

	*offset +=byte_read;

	return byte_read;
}

static ssize_t chardev_write (struct file *flip, const char __user *buf, size_t len, loff_t * offset)
{
	pr_info("Write not supported");
	return 0;
}

static int chardev_open (struct inode *inode, struct file *file)
{
	static int count=0;
	pr_info("Device opened");
	sprintf(mesg, "Read count %d\n", count++);
	return 0;
}

static int chardev_release (struct inode *inode, struct file *file)
{
	pr_info("Device released");
	return 0;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Character Device to count number of reads");
MODULE_AUTHOR("B Suyog");
MODULE_VERSION("1.0.0");
