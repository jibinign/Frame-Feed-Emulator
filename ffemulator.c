#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init ffe_init(void)
{
	pr_info("%s", __func__);
	return 0;
}

static void __exit ffe_exit(void)
{
	pr_info("%s", __func__);
}

module_init(ffe_init);
module_exit(ffe_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Frame Feed Emulator");
