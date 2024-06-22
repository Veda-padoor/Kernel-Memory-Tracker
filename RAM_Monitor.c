#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>
#include <linux/string.h>
#include <linux/timer.h>

#define THRESHOLD_VALUE 30

static struct timer_list memory_timer;

static void memory_timer_callback(struct timer_list *t)
{
    struct sysinfo info;
    si_meminfo(&info);
    long total_memory = (info.totalram * info.mem_unit) / (1024 * 1024);
    long free_memory = (info.freeram * info.mem_unit) / (1024 * 1024);
    long used_memory = (total_memory - free_memory);
    int memory_usage_percentage = (used_memory * 100) / total_memory;
    if (memory_usage_percentage > THRESHOLD_VALUE) {
        printk(KERN_ALERT "Memory usage exceeded threshold value of %d%%!\n", THRESHOLD_VALUE);
    }
    mod_timer(&memory_timer, jiffies + msecs_to_jiffies(5000));  // set timer to trigger every minute
}

static int __init memory_init(void)
{
    struct sysinfo info;
    si_meminfo(&info);
    long total_memory = (info.totalram * info.mem_unit) / (1024 * 1024);
    long free_memory = (info.freeram * info.mem_unit) / (1024 * 1024);
    long used_memory = (total_memory - free_memory);
    int memory_usage_percentage = (used_memory * 100) / total_memory;
    printk(KERN_INFO "RAM Memory Usage Details:\n");
    printk(KERN_INFO "Total memory: %ld MB\n", total_memory);
    printk(KERN_INFO "Free memory: %ld MB\n", free_memory);
    printk(KERN_INFO "Used memory: %ld MB (%d%%)\n", used_memory, memory_usage_percentage);
    timer_setup(&memory_timer, memory_timer_callback, 0);
    mod_timer(&memory_timer, jiffies + msecs_to_jiffies(5000));  // set timer to trigger every minute
    return 0;
}

static void __exit memory_exit(void)
{
    del_timer(&memory_timer);
    printk(KERN_INFO "Memory module unloaded.\n");
}

module_init(memory_init);
module_exit(memory_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tharun Kumar P -PES2UG21CS576");
MODULE_DESCRIPTION("Kernel module to periodically display ram memory usage and trigger an alert if memory usage exceeds a threshold");