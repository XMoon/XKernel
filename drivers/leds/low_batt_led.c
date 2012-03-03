#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("fentensoft");
MODULE_DESCRIPTION("module to control led when battery is low");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");

int threshold = 15;

int get_charge_counter(void)
{
	struct file *fl;
	int fsize = 5, ret;
	char *buf;
	mm_segment_t old_fs;

	buf = (char *)kmalloc(fsize+1,GFP_ATOMIC);

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	fl = filp_open("/sys/devices/platform/cpcap_battery/power_supply/battery/charge_counter", O_RDONLY, 0);
	fl->f_op->read(fl, buf, fsize, &(fl->f_pos));
	buf[fsize] = 0;
	
	sscanf(buf, "%d", &ret);
	
	filp_close(fl, NULL); 
	set_fs(old_fs);
	return ret;
}

bool low_batt(void)
{
	if (get_charge_counter() < threshold)
		return true;
	return false;
}

int proc_threshold_write(struct file *filp, const char __user *buffer,
		unsigned long len, void *data)
{
	int tmp;
	sscanf(buffer, "%d", &tmp);
	if (tmp <= 0)
		threshold = 0;
	else if (tmp >= 100)
		threshold = 100;
	else
		threshold = tmp;
	return len;
}

int proc_threshold_read(char *buffer, char **buffer_location,
		off_t offset, int count, int *eof, void *data)
{
	int ret;
	ret = scnprintf(buffer, count, "%d", threshold);
	return ret;
}

int __init low_init(void)
{
	struct proc_dir_entry *proc_entry;
	
	proc_mkdir("low_batt", NULL);
	proc_entry = create_proc_read_entry("low_batt/threshold", 0644, NULL, proc_threshold_read, NULL);
	proc_entry->write_proc = proc_threshold_write;

	return 0;
}

void __exit low_exit(void)
{
	remove_proc_entry("low_batt/threshold", NULL);
	remove_proc_entry("low_batt", NULL);
}

module_init(low_init);
module_exit(low_exit);
