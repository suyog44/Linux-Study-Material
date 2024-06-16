#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/i2c.h>
#define DEV_NAME "mpr121"
#define DEV_CLASS "mpr121_class"
#define I2C_BUS 1
#define SLAVE_DEV "mpr121_touch"
#define SLAVE_ADDR 0x5a

ssize_t mpr121_read (struct file *, char __user *, size_t, loff_t *);
int mpr121_open (struct inode *, struct file *);
int mpr121_close (struct inode *, struct file *);
static int get_mpr121_status(void);

static dev_t mpr121_dev;
static struct class *mpr121_class;
static struct cdev mpr121_device;

static unsigned short addr = 0x5a;
static unsigned short addr_list[] = {0x51,0x01};

static struct file_operations fops = {
	owner: THIS_MODULE,
	read: mpr121_read,
	open: mpr121_open,
	release: mpr121_close,
};

static int get_mpr121_status(void)
{
	pr_info("Fetch resister status from device id:0x%x\n", addr);
	return 0xffff;
}

ssize_t mpr121_read (struct file *filp, char __user *buf, size_t len, loff_t *offset)
{
	int bytes_read, to_copy, not_copied;
	char out_string[20];
	pr_info("Read MPR121 status registers");
	int mpr121_status;

	to_copy=min(sizeof(out_string), len);
	mpr121_status = get_mpr121_status();
	snprintf(out_string, sizeof(out_string), "Status Register: %d\n", mpr121_status);
	not_copied = copy_to_user(buf, out_string, to_copy);
	bytes_read = to_copy - not_copied;
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

static int mpr121_probe (struct i2c_client *client, const struct i2c_device_id *mpr121_i2c_device)
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

	return 0;

kernelerror:
	device_destroy(mpr121_class, mpr121_dev);
fileerror:
	class_destroy(mpr121_class);
class_err:
	unregister_chrdev(mpr121_dev, DEV_NAME);

	return -1;
}

static void mpr121_remove (struct i2c_client *client)
{
	pr_info("Unloading module MPR121!\n");
	cdev_del(&mpr121_device);
	device_destroy(mpr121_class, mpr121_dev);
	class_destroy(mpr121_class);
	unregister_chrdev_region(mpr121_dev,1);
	return;
}

static const struct i2c_device_id mpr121_device_id[] = {
	{"mpr121_touch", 0},
	{}
};

static struct i2c_driver mpr121_driver = {
	probe: mpr121_probe,
	remove: mpr121_remove,
	driver: {
		name: "mpr121",
		owner: THIS_MODULE
	},
	address_list: addr_list,
	id_table: mpr121_device_id,
	class: I2C_CLASS_HWMON | I2C_CLASS_SPD,
};

static int __init mpr121_init(void)
{
	i2c_add_driver(&mpr121_driver);
	return 0;
}

static void __exit mpr121_exit(void)
{
	i2c_del_driver(&mpr121_driver);
	return;
}

module_init(mpr121_init);
module_exit(mpr121_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MPR121 i2c based character driver");
MODULE_AUTHOR("B Suyog");
MODULE_VERSION("1.0.0");
