/*
 * linux 芯片寄存器读写工具
 */
/********************************头文件****************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
/********************************宏定义****************************************/
#define REG_R      0
#define REG_W      1
/*******************************全局变量***************************************/
static int major;
static struct class *class;
/*********************************函数*****************************************/
static long reg_rw_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret;
    volatile unsigned int *reg;
	unsigned int val;
	unsigned int addr;

	unsigned int buf[2];

	ret = copy_from_user(buf, (const void __user *)arg, 8);
	addr = buf[0];
	val  = buf[1];

	reg = (volatile unsigned int *)ioremap(addr, 4);

	switch (cmd) {
	case REG_R:
		val = *reg;
		ret = copy_to_user((void __user *)(arg+4), &val, 4);
		break;

	case REG_W:
		*reg = val;
		break;

    default:
        break;
    }

	iounmap(reg);
	return ret;
}

static struct file_operations reg_rw_ops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl = reg_rw_ioctl,
};

static int reg_rw_init(void)
{
	major = register_chrdev(0, "reg_tool", &reg_rw_ops);
	class = class_create(THIS_MODULE, "reg_tool");
	device_create(class, NULL, MKDEV(major, 0), NULL, "reg_tool"); /* /dev/oled */

	return 0;
}

static void reg_rw_exit(void)
{
	device_destroy(class, MKDEV(major, 0));
	class_destroy(class);
	unregister_chrdev(major, "reg_tool");
}

module_init(reg_rw_init);
module_exit(reg_rw_exit);
MODULE_LICENSE("GPL");


