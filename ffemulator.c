// SPDX-License-Identifier: GPL-2.0

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define name KBUILD_MODNAME

dev_t dev;
struct class *ffe_class;
struct cdev *ffe_cdev;

int ffe_open(struct inode *inode, struct file *filp)
{
	pr_info("%s", __func__);
	return 0;
}

int ffe_release(struct inode *inode, struct file *filp)
{
	pr_info("%s", __func__);
	return 0;
}

ssize_t ffe_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	pr_info("%s", __func__);
	return 0;
}

long ffe_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	pr_info("%s", __func__);
	return 0;
}


static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = ffe_open,
	.release = ffe_release,
	.read = ffe_read,
	.unlocked_ioctl = ffe_ioctl,
};

static int __init ffe_init(void)
{
	pr_info("%s", __func__);
	if (alloc_chrdev_region(&dev, 0, 1, name) < 0) {
		pr_err("%s: device registration failed..", __func__);
		return -1;
	}
	pr_info("%s: Device: %s, Major number: %d", __func__, name, MAJOR(dev));
	ffe_class = class_create(THIS_MODULE, name);
	device_create(ffe_class, NULL, dev, NULL, name);
	ffe_cdev = cdev_alloc();
	cdev_init(ffe_cdev, &fops);
	cdev_add(ffe_cdev, dev, 1);
	pr_info("%s: Kernel module inserted..", __func__);
	return 0;
}

static void __exit ffe_exit(void)
{
	pr_info("%s", __func__);
	cdev_del(ffe_cdev);
	device_destroy(ffe_class, dev);
	class_destroy(ffe_class);
	unregister_chrdev_region(dev, 1);
	pr_info("%s: Kernel module removed..", __func__);
}

module_init(ffe_init);
module_exit(ffe_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Frame Feed Emulator");
