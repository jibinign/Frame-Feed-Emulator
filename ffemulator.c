// SPDX-License-Identifier: GPL-2.0

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include <linux/ioctl.h>

#define name KBUILD_MODNAME
#define V240P _IOW('a', 'a', int*)
#define V360P _IOW('a', 'b', int*)
#define V480P _IOW('a', 'c', int*)
#define V720P _IOW('a', 'd', int*)
#define V1080P _IOW('a', 'e', int*)

dev_t dev;
struct class *ffe_class;
struct cdev *ffe_cdev;
int count = 1;				//number of frames
long size = 306720;			//240p with aspect ratio 16:9, frame size = 426x240x3
uint8_t *frame;

static void create_frame(void)
{
	pr_info("%s", __func__);
}

static int ffe_open(struct inode *inode, struct file *filp)
{
	pr_info("%s", __func__);
	return 0;
}

static int ffe_release(struct inode *inode, struct file *filp)
{
	pr_info("%s", __func__);
	return 0;
}

static ssize_t ffe_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	pr_info("%s", __func__);
	return 0;
}

static long ffe_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	pr_info("%s", __func__);
	copy_from_user(&count, (int *)arg, sizeof(count));
	switch (cmd) {
	case V240P:
		size = 306720;		//size = 426x240x3
		break;
	case V360P:
		size = 691200;		//size = 640x360x3
		break;
	case V480P:
		size = 1229760;		//size = 854x480x3
		break;
	case V720P:
		size = 2764800;		//size = 1280x720x3
		break;
	case V1080P:
		size = 6220800;		//size = 1920x1080x3
		break;
	default:
		break;
	}
	pr_info("%s: count = %d, size = %ld", __func__, count, size);
	frame = krealloc(frame, size, GFP_KERNEL);
	pr_info("%s: Memory reallocated for data frame with new size..", __func__);
	create_frame();			//generate data frame
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
	frame = kmalloc(size, GFP_KERNEL);
	if (!frame) {
		pr_err("%s: Memory allocation failed..", __func__);
		return -ENOMEM;
	}
	pr_info("%s: Memory allocated for data frame..", __func__);
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
	kfree(frame);
	pr_info("%s: Kernel module removed..", __func__);
}

module_init(ffe_init);
module_exit(ffe_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Frame Feed Emulator");
