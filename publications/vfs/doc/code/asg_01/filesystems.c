#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>

extern struct file_system_type *file_systems;

static int __init filesystems_init(void)
{
	struct file_system_type *tmp = file_systems;

	if (!file_systems)
		return pr_info("Not found");

	while (tmp) {
		pr_info("%s\n", tmp->name);
		tmp = tmp->next;
	}

	return 0;
}

static void __exit filesystems_exit(void)
{
	pr_info("Removed test module\n");
}

module_init(filesystems_init);
module_exit(filesystems_exit);

MODULE_LICENSE("GPL");
