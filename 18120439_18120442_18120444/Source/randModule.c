//Include library

#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/random.h>

//Global variables
static dev_t first;       // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl;  // Global variable for the device class

static int temp;        // Global variable for temp
static int randNum;     // Global variable for random number


//Register function
static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}

static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) //read random number function
{
    static char string[12]; //  variable for read bufer
    static int i;       // variable for loop in read function
    static int flag;    //  variable flag for negative random number
    i = 0;
    flag = 0;
    
    
     if ((*off) > 0) // Check if offset is end of buffer
    {
        memset(string, 0, sizeof(string));
        return 0;
    }
    
    printk(KERN_INFO "Driver: read()\n");
    get_random_bytes(&randNum, sizeof(randNum)); // Generate random number and store in randNum
	printk("Generating number: %d\n", randNum);         // For debugging: Print a random number
    if (randNum < 0) // if randNum is negative
    {
        flag = 1;      // set flag
        randNum *= -1; // change randNum to positive
    }

    temp = randNum; // Store randNum to temp

    if (flag == 1) // If randNum is negative, set first character of string to '-'
    {
        string[i++] = '-';
    }

    do //Count length of number
    {
        temp = temp / 10;
        i++;
    } while (temp > 0);

    if (flag == 1) // Store randNum to temp again for debugging
        temp = randNum * (-1);
    else
    {
        temp = randNum;
    }

    string[i] = '\0'; // Set last character to '\0'
    i--;

    do // convert number to string
    {
        string[i--] = (randNum % 10) + 48;
        randNum = randNum / 10;
    } while (randNum > 0);

 

    if (copy_to_user(buf, &string, strlen(string)) != 0) //push kernel string to user buf
    {
        return -EFAULT;
    }

    (*off) += strlen(string); // set offset = offset + lentgh of number
    return strlen(string);    // return the number of bytes have been read
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: write()\n");
    return len;
}

static struct file_operations pugs_fops =
    {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
        .read = my_read,
        .write = my_write};

static int __init ofcd_init(void) /* Constructor */
{
    printk(KERN_INFO "Hello: ofcd registered\n");
    if (alloc_chrdev_region(&first, 0, 1, "Random number") < 0)
    {
        return -1;
    }
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cl, NULL, first, NULL, "myrand") == NULL)
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    cdev_init(&c_dev, &pugs_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    return 0;
}

static void __exit ofcd_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Goodbye: ofcd unregistered\n");
}

module_init(ofcd_init);
module_exit(ofcd_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Duc Loc");
MODULE_DESCRIPTION("Random number character device driver");
