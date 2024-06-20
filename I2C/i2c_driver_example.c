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

static const struct of_device_id of_match_ids_example[] = {
{.compatible = "factory_name,chip_name", .data = NULL},
{                          },
}

static const struct i2c_device_id example_ids[]={
{"chip_name",(kernel_ulong_t)NULL},
{ },
}

static int i2c_driver_example_prob(struct i2c_client *client)
{
  return 0;
}

static int i2c_driver_example_remove(struct i2c_client *client)
{
  return 0;
}

static struct i2c_driver i2c_example_driver = {
  .driver = {
      .name ="example",
      .of_match_table = of_match_ids_example,
  },
  .probe_new = i2c_driver_example_prob ,
  .remove = i2c_driver_example_remove,
  .id_table = example_ids,
};

static int __int i2c_driver_example_init(void)
{
  return i2c_add_driver(&i2c_example_driver);
}

static void __exit i2c_driver_example_exit(void)
{
  return i2c_del_driver(&i2c_example_driver);
}

module_init(i2c_driver_example_init);
module_exit(i2c_driver_example_exit);

MODULE_AUTHOR("ygq");
MODULE_LICENSE("GPL");
