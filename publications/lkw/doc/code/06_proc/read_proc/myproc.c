// Module to make a read entry in the proc file system.
// Module to write a command line calculator

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/types.h>

#include "myproc.h"

struct proc_dir_entry *proc;
int len;
char *msg;

int bytes_read;
ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp) 
{
    int err;
    char *data = PDE_DATA(file_inode(filp));

    if ((int) (*offp) > len) {
        return 0;
    }

    if(!(data)) {
        printk(KERN_INFO "NULL DATA");
        return 0;
    }

    if (count == 0) {
        printk(KERN_INFO "Read of size zero, doing nothing.");
        return count;
    }

    count = len + 1; // +1 to read the \0
    err = copy_to_user(buf, data, count+1); // +1 for \0
    *offp = count;

    if (err) {      
        printk(KERN_INFO "Error in copying data.");
    } else {
        printk(KERN_INFO "Successfully copied data.");
    }

    return count;
}

struct file_operations proc_fops = {
    .read = read_proc,
};

void create_new_proc_entry(void) {
    msg = "Hello World";
    proc = proc_create_data(MY_PROC_ENTRY, 0, NULL, &proc_fops, msg);
    len = strlen(msg);
}

int proc_init (void) {
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void) {
    remove_proc_entry(MY_PROC_ENTRY, NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);

