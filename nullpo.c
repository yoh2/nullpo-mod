/*
 * Copyright (C) 2020 yoh2
 *
 * nullpo.ko is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public Licence as published by the
 * Free Software Foundation, version 2 of the License, or (at your option)
 * any later version.
 */
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/signal.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");

#define NULLPO_DEV_NAME      "nullpo"

static loff_t nullpo_llseek(struct file *file, loff_t offset, int whence)
{
	/* dummy implementation like /dev/null */
	file->f_pos = 0;
	return 0;
}

static ssize_t nullpo_read(
	struct file *file, char __user *buf,
	size_t count, loff_t *ppos)
{
	if (count > 0) {
		send_sig(SIGSEGV, current, 0);
		return -EIO;
	} else
		return 0;
}

static ssize_t nullpo_write(
	struct file *file, const char __user *buf,
	size_t count, loff_t *ppos)
{
	if (count > 0) {
		send_sig(SIGSEGV, current, 0);
		return -EIO;
	} else
		return 0;
}

static const struct file_operations nullpo_fops = {
	.owner   = THIS_MODULE,
	.llseek  = nullpo_llseek,
	.read    = nullpo_read,
	.write   = nullpo_write,
};

static struct miscdevice nullpo_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = NULLPO_DEV_NAME,
	.fops  = &nullpo_fops,
};

static int __init nullpo_init(void)
{
	int err = misc_register(&nullpo_dev);
	if (err != 0) {
		printk(KERN_ALERT "nullpo: failed to register an nullpo device. err = %d\n", err);
		return err;
	}

	return 0;
}

static void __exit nullpo_cleanup(void)
{
	misc_deregister(&nullpo_dev);
}

module_init(nullpo_init);
module_exit(nullpo_cleanup);
