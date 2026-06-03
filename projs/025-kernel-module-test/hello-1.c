#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void) 
{
    printk(KERN_INFO "hello world 1.\n");

    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Goodbye world 1.\n");
} 

MODULE_LICENSE("GPL");
// MODULE_AUTHOR("camel");
// MODULE_DESCRIPTION("Simple hello world kernel module");
// MODULE_VERSION("1.0");