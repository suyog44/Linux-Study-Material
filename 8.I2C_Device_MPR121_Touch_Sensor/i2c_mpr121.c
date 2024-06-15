#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/i2c.h>
#define DEV_NAME "MPR121"
#define DEV_CLASS "MPR121_class"
#define I2C_BUS 1
#define SLAVE_DEV "mp121_touch"
#define SLAVE_ADDR 0x5a

ssize_t mpr121_read (struct file *, char __user *, size_t, loff_t *);
int mpr121_open (struct inode *, struct file *);
int mpr121_close (struct inode *, struct file *);

static dev_t mpr121_dev;
static struct class *mpr121_class;
static struct cdev mpr121_device;

static struct i2c_adapter * i2c_adapter_mpr121 = NULL;
static struct i2c_client * i2c_client_mpr121 = NULL;

static struct i2c_board_info i2c_board_info = {
	I2C_BOARD_INFO(SLAVE_DEV, SLAVE_ADDR)
};

static struct file_operations fops = {
	owner: THIS_MODULE,
	read: mpr121_read,
	open: mpr121_open,
	release: mpr121_close,
};

ssize_t mpr121_read (struct file *filp, char __user *buf, size_t len, loff_t *offset)
{
	int bytes_read;
	pr_info("Read MPR121 status registers");
	return bytes_read; 
}

int mpr121_open (struct inode *inode, struct file *file)
{
	pr_info("MPR121 device file opened!");
	return 0;
}

int mpr121_close (struct inode *inode, struct file *file)
{
        pr_info("MPR121 device file closed!");
        return 0;
}

static int __init mpr121_init(void)
{
	if(alloc_chrdev_region(&mpr121_dev, 0, 1, DEV_NAME)<0){
		pr_err("Device could not allocated!\n");
		return -1;
	}

	if((mpr121_class=class_create(THIS_MODULE, DEV_CLASS))<0){
		pr_err("Device class creation failed!\n");
		goto class_err;
	}

	if(device_create(mpr121_class, NULL, mpr121_dev, NULL, DEV_NAME)==NULL){
		pr_err("Device file creation failed!\n");
		goto fileerror;
	}

	cdev_init(&mpr121_device, &fops);

	if(cdev_add(&mpr121_device, mpr121_dev, 1)<0){
		pr_err("Registering device to kernel failed!\n");
		goto kernelerror;
	}

	i2c_adapter_mpr121 = i2c_get_adapter(I2C_BUS);

	if(i2c_adapter_mpr121 != NULL){
		i2c_client_mpr121 = i2c_acpi_new_device(i2c_adapter_mpr121, 1, &i2c_board_info);
		if(i2c_client_mpr121!=NULL){
		
		}
		i2c_put_adapter(i2c_adapter_mpr121);
	}
	pr_info("MPR121 device added as character device!\n");

	return 0;

kernelerror:
	device_destroy(mpr121_class, mpr121_dev);
fileerror:
	class_destroy(mpr121_class);
class_err:
	unregister_chrdev(mpr121_dev, DEV_NAME);

	return -1;
}

static void __exit mpr121_exit(void)
{
	pr_info("Unloading module MPR121!\n");
	cdev_del(&mpr121_device);
	device_destroy(mpr121_class, mpr121_dev);
	class_destroy(mpr121_class);
	unregister_chrdev_region(mpr121_dev,1);
	return;
}

module_init(mpr121_init);
module_exit(mpr121_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MPR121 i2c based character driver");
MODULE_AUTHOR("B Suyog");
MODULE_VERSION("1.0.0");
