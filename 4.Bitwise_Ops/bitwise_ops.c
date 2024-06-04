#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>


#define MINORBITS 20
#define MINORMASK ((1UL<<MINORBITS)-1)
#define MAJOR(dev) ((unsigned int)((dev)>>MINORBITS))
#define MINOR(dev) ((unsigned int)((dev)&MINORMASK))

int value_a, value_b;

module_param(value_a, int, S_IRUSR|S_IWUSR);
module_param(value_b, int, S_IRUSR|S_IWUSR);

static int __init test_init(void){
	pr_info("Module loaded");
	pr_info("value of a: %u\n",value_a);
	pr_info("value of b: %u\n",value_b);
	pr_info("MAJOR(%u):%u MINOR(%u):%u\n", MAJOR(value_a), MINOR(value_b));
	return 0;
}

static void __exit test_exit(void){
	pr_info("Kernel unloaded successfully !");
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suyog B");
MODULE_DESCRIPTION("Test driver for bitwise ops check");
MODULE_VERSION("1.0.0");
