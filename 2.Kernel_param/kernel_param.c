#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#define SIZE 1<<2

int value,valuearr[SIZE];
char *name;
int cb_value = 0;

module_param(value, int, S_IWUSR|S_IRUSR);
module_param(name, charp, S_IWUSR|S_IRUSR);
module_param_array(valuearr, int, NULL, S_IRUSR|S_IWUSR);

static int __init kernel_param_init(void)
{
	printk(KERN_INFO"Kernel module param loaded!");
	printk(KERN_INFO"Value: %d",value);
	printk(KERN_INFO"Char: %s",name);
	for(int i=0; i<sizeof(valuearr)/sizeof(valuearr[0]); i++){
		printk(KERN_INFO"Arr[%d] = %d \n", i, valuearr[i]);
	}
	return 0;
}

static void __exit kernel_param_exit(void)
{
	printk(KERN_INFO"Kernel param module removed!");
}

module_init(kernel_param_init);
module_exit(kernel_param_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("B SUYOG");
MODULE_DESCRIPTION("Kernel parameter passing");
MODULE_VERSION("1.0.0");
