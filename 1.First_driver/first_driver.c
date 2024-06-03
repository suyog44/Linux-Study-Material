#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init first_driver_init(void)
{
	printk(KERN_INFO"First driver init !");
	return 0;
}

static void __exit first_driver_exit(void)
{
	printk(KERN_INFO"First driver exit !");
}

module_init(first_driver_init);
module_exit(first_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suyog Buradkar");
MODULE_DESCRIPTION("First driver for RPI");
MODULE_VERSION("1.1.1");
