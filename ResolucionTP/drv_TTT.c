#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_PIN_1 17 // GPIO17 (Pin 11 en la placa)
#define GPIO_PIN_2 27 // GPIO27 (Pin 13 en la placa)

static dev_t first;        
static struct cdev c_dev;  
static struct class *cl;   

static char selected_gpio = 1; // 1 para GPIO_PIN_1, 2 para GPIO_PIN_2

static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "SdeC_drvTTT: open()\n");
    return 0;
}

static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "SdeC_drvTTT: close()\n");
    return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    int gpio_val;
    char value;
    printk(KERN_INFO "SdeC_drvTTT: read()\n");

    if (*off > 0)
        return 0;

    if (selected_gpio == 1)
        gpio_val = gpio_get_value(GPIO_PIN_1);
    else
        gpio_val = gpio_get_value(GPIO_PIN_2);

    value = gpio_val + '0';

    if (copy_to_user(buf, &value, 1))
        return -EFAULT;

    (*off)++;
    return 1;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    char kbuf;

    printk(KERN_INFO "SdeC_drvTTT: write()\n");

    if (copy_from_user(&kbuf, buf, 1))
        return -EFAULT;

    if (kbuf == '1')
        selected_gpio = 1;
    else if (kbuf == '2')
        selected_gpio = 2;
    else
        return -EINVAL; // Invalid argument

    return len;
}

static struct file_operations pugs_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write
};

static int __init drv_TTT_init(void) /* Constructor */
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "El driver de The Tux Titans se ha registrado exitosamente..!!\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "SdeC_drvTTT")) < 0)
    {
        return ret;
    }

    cl = class_create("SdeC_drive");
    if (IS_ERR(cl))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    dev_ret = device_create(cl, NULL, first, NULL, "SdeC_drvTTT");
    if (IS_ERR(dev_ret))
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &pugs_fops);

    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    if (!gpio_is_valid(GPIO_PIN_1) || !gpio_is_valid(GPIO_PIN_2))
    {
        printk(KERN_INFO "Invalid GPIO pins\n");
        return -ENODEV;
    }

    gpio_request(GPIO_PIN_1, "GPIO_PIN_1");
    gpio_direction_input(GPIO_PIN_1);

    gpio_request(GPIO_PIN_2, "GPIO_PIN_2");
    gpio_direction_input(GPIO_PIN_2);

    return 0;
}

static void __exit drv_TTT_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    gpio_free(GPIO_PIN_1);
    gpio_free(GPIO_PIN_2);

    printk(KERN_INFO "SdeC_drvTTT: dice EL trabajo ha sido finalizado!!\n");
}

module_init(drv_TTT_init);
module_exit(drv_TTT_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra Sistemas de Computación");
MODULE_DESCRIPTION("Trabajo Practico 5: The Tux Titans");
