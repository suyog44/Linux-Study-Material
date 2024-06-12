#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define SUCCESS 0
#define FAIL -1

static int __init proc_device_init(void)
{
	return SUCCESS;
}

static void __exit proc_device_exit(void)
{
	return;
}

module_init(proc_device_init);
module_exit(proc_device_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("proc device test 1");
MODULE_AUTHOR("B Suyog");
MODULE_VERSION("1.0.0");
