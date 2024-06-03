#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

dev_t dev=MKDEV(200,1);

static int __init major_minor_init(void)
{
	printk(KERN_INFO"major minor module loaded!");
	register_chrdev_region(dev, 1, "char_buffer_device");
	printk(KERN_INFO"Major: %d Minor:%d",MAJOR(dev),MINOR(dev));
	return 0;
}

static void __exit major_minor_exit(void)
{
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO"major minor module unloaded!");
}

module_init(major_minor_init);
module_exit(major_minor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("B SUYOG");
MODULE_DESCRIPTION("Major Minor number for chardev");
MODULE_VERSION("1.0.0");
