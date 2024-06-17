#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

static struct i2c_client *mpr121_client;

static int mpr121_probe(struct i2c_client *, const struct i2c_device_id *);
static void mpr121_remove(struct i2c_client *);
static ssize_t mpr121_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t mpr121_write(struct file *flip,const char __user * user_buffer, size_t count, loff_t *offset);

static struct of_device_id devices[] = {
	{ compatible: "mpr121_touchi","mpr121"},
	{}
};
MODULE_DEVICE_TABLE(of, devices);

static struct i2c_device_id devices_list[] = {
	{"mpr121",0},
	{}
};
MODULE_DEVICE_TABLE(i2c, devices_list);

static struct i2c_driver mpr121_driver = {
	probe: mpr121_probe,
	remove: mpr121_remove,
	id_table: devices_list,
	driver: {
		name: "mpr121_touch_driver",
		of_match_table: devices,
	}
};

static struct proc_dir_entry *proc_file;


static ssize_t mpr121_read(struct file *flip,char __user *buffer, size_t len, loff_t *offset)
{
	u8 val;
	val = i2c_smbus_read_byte(mpr121_client);
	return sprintf(buffer, "MPR121: 0x%x\n",val);
}

static ssize_t mpr121_write(struct file *flip,const char __user * user_buffer, size_t count, loff_t *offset)
{
	long val;

	if(kstrtol(user_buffer, 0, &val)==0){
		i2c_smbus_write_byte(mpr121_client, (u8)val);
	}
	return count;
}

static struct proc_ops fops = {
	proc_read: mpr121_read,
	proc_write: mpr121_write,
};
static int mpr121_probe (struct i2c_client *client, const struct i2c_device_id *id)
{
	pr_info("Probe function called");

	if(client->addr != 0x5a){
		pr_err("Wrong i2c address for mpr121");
		return -1;
	}

	mpr121_client = client;

	proc_file = proc_create("mpr121_touch", 0666, NULL, &fops);
	if(proc_file == NULL){
		pr_err("proc file creation failed for mpr121!\n");
		return -ENOMEM;
	}
	return 0;
}

static void mpr121_remove(struct i2c_client *client)
{
	pr_info("remove mpr121 i2c module");
	proc_remove(proc_file);
	return;
}

module_i2c_driver(mpr121_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MPR121 i2c driver for Sony Spresense Music");
MODULE_AUTHOR("B Suyog <suyogburadkar@gmail.com>");
MODULE_VERSION("1.0.0");
