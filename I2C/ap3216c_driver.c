#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/modules.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/mod_devicetable.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/property.h>
#include <linux/acpi.h>
#include <linux/i2c.h>
#include <linux/nvmem-provider.h>
#include <linux/regmap.h>
#include <linux/pm_runtime.h>
#include <linux/gpio/consumer.h>

static int major = 0;
static struct class *ap3216c_class;
static struct i2c_client *ap3216c_client;

static ssize_t ap3216c_read(struct file *file,char __user *buf,size_t size,loff_t *offset)
{
    int err;
    char kenel_buf[6];
    int val;

    if(size != 6)
       return -EINVAL;

    val = i2c_smbus_read_word_data(ap3216c_client,0xA);
    kenel_buf[0] = val & 0xff;
    kenel_buf[1] = (val >> 8) & 0xff;

    val = i2c_smbus_read_word_data(ap3216c_client,0xC);
    kenel_buf[2] = val & 0xff;
    kenel_buf[3] = (val >> 8) & 0xff;

    val = i2c_smbus_read_word_data(ap3216c_client,0xE);
    kenel_buf[4] = val & 0xff;
    kenel_buf[5] = (val >> 8) & 0xff;

    err = copy_to_user(buf,kernel_buf,MIN(1024,size));

    return size;
}

static int ap3216c_open(struct inode *node,struct file *file)
{
    i2c_smbus_write_byte_data(ap3216c_client,0,0x4);/*i2c_smbus_write_byte_data函数有两个版本，一个是i2ctool提供给应用程序用的，一个是给内核提供给驱动程序用的 */
    mdelay(20);
    i2c_smbus_write_byte_data(ap3216c_client,0,0x3);
    return 0;
}

static struct file_operations ap3216c_ops = {
    .owner = THIS_MODULE;
    .open = ap3216c_open;
    .read = ap3216c_read;
    .write = ap3216c_write;
}

static const struct of_device_id of_match_ids_ap3216c[] = {
  {.compatible = "factory_name,chip_name", .data = NULL},
  {                          },
}

static const struct i2c_device_id ap3216c_ids[]={
  {"chip_name",(kernel_ulong_t)NULL},
  { },
}


static int ap3216c_prob(struct i2c_client *client)
{
  ap3216c_client = client;
  /*register_chrdev*/
  major = register_chrdev(0,"ap3216c",&ap3216c_ops);
  ap3216c_class = class_create(THIS_MODULE,"ap3216c_class");
  device_create(ap3216c_class,NULL,MKDEV(major,0),NULL,"ap3216c");/*/dev/ap3216c*/
  return 0;
}

static int ap3216c_remove(struct i2c_client *client)
{
  /*unregister_chrdev*/
  device_destroy(ap3216c_class，MKDEV(major,0));
  class_destroy(ap3216c_class);
  unregister_chrdev(major,"ap3216c");
  
  return 0;
}

static struct i2c_driver i2c_ap3216c_driver = {
  .driver = {
      .name ="ap3216c",
      .of_match_table = of_match_ids_ap3216c,
  },
  .probe_new = ap3216c_prob ,
  .remove = ap3216ce_remove,
  .id_table = ap3216c_ids,
};

static int __int i2c_driver_ap3216c_init(void)
{
  printk("%s %d %s\n",__FILE__,__FUNCTION__,__LINE__);
  return i2c_add_driver(&i2c_ap3216c_driver);
}

static void __exit i2c_driver_ap3216c_exit(void)
{
  return i2c_del_driver(&i2c_ap3216c_driver);
}

module_init(i2c_driver_ap3216c_init);
module_exit(i2c_driver_ap3216c_exit);

MODULE_AUTHOR("ygq");
MODULE_LICENSE("GPL");

