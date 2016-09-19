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

// #define DATA "Hello World People !!!"

struct proc_dir_entry *proc;
int len;
char *msg = NULL;


static ssize_t my_proc_write(struct file *file, const char __user * buffer, size_t count, loff_t *pos)
{
    printk(KERN_INFO "Writing to proc");

    if (msg != NULL) {
        kfree(msg);
    }
    msg = kmalloc((size_t) count, GFP_KERNEL);
    if (copy_from_user(msg, buffer, count)) {
        return -EFAULT;
    }
    printk(KERN_INFO "msg has been set to %s", msg);
    return 10;
}

ssize_t read_proc(struct file *filp,char *buf,size_t count, loff_t *offp ) 
{
    int err;
    char *data = PDE_DATA(file_inode(filp));

    if ((int) (*offp) > len) {
        return 0;
    }
    printk(KERN_INFO "Reading the proc entry");

    if(!(data)) {
        printk(KERN_INFO "NULL DATA");
        return 0;
    }

    if (count == 0) {
        printk(KERN_INFO "Read of size zero, doing nothing.");
        return count;
    } else {
        printk(KERN_INFO "Read of size %d", (int) count);
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

int create_new_proc_entry(void) {
    int i;
    char *DATA = "Hello People";
    len = strlen(DATA);
    msg = kmalloc((size_t) (len + 1), GFP_KERNEL); // +1 for \0
    
    if (msg != NULL) {
        printk(KERN_INFO "Allocated memory for msg");
    } else {
        return -1;
    }

    strncpy(msg, DATA, len+1);
    for (i=0; i < len +1 ; i++) {
        printk(KERN_INFO "%c", msg[i]);
        if (msg[i] == '\0') {
            printk(KERN_INFO "YES");
        }
    }
    proc = proc_create_data(MY_PROC_ENTRY, 0, NULL, &proc_fops, msg);
    if (proc) {
        return 0;
    }
}

int proc_init (void) {
    if (create_new_proc_entry()) {
        return -1;
    }
    return 0;
}

void proc_cleanup(void) {
    remove_proc_entry(MY_PROC_ENTRY, NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);

