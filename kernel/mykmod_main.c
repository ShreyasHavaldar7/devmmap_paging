/* CS18BTECH11042, CS18BTECH11050 */

#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>

#include <mydev.h>

MODULE_DESCRIPTION("My kernel module - mykmod");
MODULE_AUTHOR("maruthisi.inukonda [at] gmail.com");
MODULE_LICENSE("GPL");

// Dynamically allocate major no
#define MYKMOD_MAX_DEVS 256
#define MYKMOD_DEV_MAJOR 0
#define PAGE_SIZE 4096

static int mykmod_init_module(void);
static void mykmod_cleanup_module(void);

static int mykmod_open(struct inode *inode, struct file *filp);
static int mykmod_close(struct inode *inode, struct file *filp);
static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma);

module_init(mykmod_init_module);
module_exit(mykmod_cleanup_module);

static struct file_operations mykmod_fops = {
	.owner  = THIS_MODULE,	/* owner (struct module *) */
	.open   = mykmod_open,	/* open */
	.release  = mykmod_close,     /* release */
	.mmap = mykmod_mmap,	/* mmap */
};

static void mykmod_vm_open(struct vm_area_struct *vma);
static void mykmod_vm_close(struct vm_area_struct *vma);
//static int mykmod_vm_fault(struct vm_fault *vmf);
static int mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf);

// TODO Data-structure to keep per device info 

struct mykmod_dev_info {
	char *data;
	size_t size;
	// int reference;
};

// TODO Device table data-structure to keep all devices

struct mykmod_dev_info **dev_table;

// Data-structure to keep per VMA info 1 pointer to device info, page faults

struct mykmod_vma_info {
	struct mykmod_dev_info *devinfo;
	long unsigned int npagefaults;
};

// struct vm_fault {
// 	unsigned int flags;
// 	pgoff_t pgoff;
// 	void __user *virtual_address;
// 	struct page *page;
// };	

static const struct vm_operations_struct mykmod_vm_ops = {
	.open  = mykmod_vm_open,
	.close = mykmod_vm_close,
	.fault = mykmod_vm_fault
};

int mykmod_major;

static int mykmod_init_module(void)
{
	printk("mykmod loaded\n");
	printk("mykmod initialized at=%p\n", init_module);

	if ((mykmod_major = register_chrdev(MYKMOD_DEV_MAJOR,"mykmod",&mykmod_fops))<0) {
		printk(KERN_WARNING "Failed to register character device\n");
		return 1;
	}
	else {
		printk("register character device %d\n", mykmod_major);
	}
	// TODO initialize device table
	dev_table = kmalloc(MYKMOD_MAX_DEVS*sizeof(struct mykmod_dev_info), GFP_KERNEL);

	return 0;
}

static void mykmod_cleanup_module(void)
{
	printk("mykmod unloaded\n");
	unregister_chrdev(mykmod_major,"mykmod");
	// TODO free device info structures from device table
	int i = 0;
	while (i<MYKMOD_MAX_DEVS && dev_table[i]!= NULL)
	{
		kfree(dev_table[i]);
		i++;
	}	
	
	kfree(dev_table);
	return;
}

static int
mykmod_open(struct inode *inodep, struct file *filep)
{
	printk("mykmod_open: filep=%p f->private_data=%p "
		"inodep=%p i_private=%p i_rdev=%x maj:%d min:%d\n",
		filep, filep->private_data,
		inodep, inodep->i_private, inodep->i_rdev, MAJOR(inodep->i_rdev), MINOR(inodep->i_rdev));
	
	struct mykmod_dev_info *info;

	// TODO: Allocate memory for devinfo and store in device table and i_private.
	if (inodep->i_private == NULL) {
		info = kmalloc(sizeof(struct mykmod_dev_info), GFP_KERNEL);
		info -> data = (char*)kmalloc(MYDEV_LEN, GFP_KERNEL);
		memcpy(info -> data, "Opening File",12);
		inodep -> i_private = info;
		
		int i = 0;
		while(i<MYKMOD_MAX_DEVS && dev_table[i] != NULL) {
			i++;
		}
		if(i<MYKMOD_MAX_DEVS) {
			dev_table[i] = info;		// TODO Device table

		}
	}

	// Store device info in file's private_data aswell
	filep->private_data = inodep->i_private;

	return 0;
}

static int
mykmod_close(struct inode *inodep, struct file *filep)
{
	// TODO: Release memory allocated for data-structures.
	// struct mykmod_dev_info *info;
	// info = filep->private_data;
	// bool match = false;
	// int i = 0;

	// while(dev_table[i] != NULL) {
	// 	if(dev_table[i] == info) {
	// 		kfree(dev_table[i]);
	// 		match = true;
	// 		break;
	// 	}
	// 	i++;
	// }

	// if(match && i<255) {
	// 	while(dev_table[i+1] != NULL) {
	// 		dev_table[i] = dev_table[i+1];
	// 		i++; 
	// 	}
	// }
	// kfree(info);
	// inodep->i_private = NULL;

	printk("mykmod_close: inodep=%p filep=%p\n", inodep, filep);
	return 0;
}

static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma)
{
	printk("mykmod_mmap: filp=%p vma=%p flags=%lx\n", filp, vma, vma->vm_flags);

	//TODO setup vma's flags, save private data (devinfo, npagefaults) in vm_private_data
	vma -> vm_ops = &mykmod_vm_ops;
	vma -> vm_flags |= VM_DONTDUMP | VM_DONTEXPAND; // dont include in core dump and cannot expand with mremap()

	struct mykmod_vma_info *info;
	info = kmalloc(sizeof(struct mykmod_vma_info), GFP_KERNEL);
	info -> devinfo = filp -> private_data;
	vma -> vm_private_data = info;

	// vma->vm_private_data = filp->f_inode->i_private;
	
	mykmod_vm_open(vma);

	// return -ENOSYS; // Remove this once mmap is implemented.
	return 0;
}

static void
mykmod_vm_open(struct vm_area_struct *vma)
{
	struct mykmod_vma_info *info1;
	info1=vma->vm_private_data;
	info1 -> npagefaults = 0;
	printk("mykmod_vm_open: vma=%p npagefaults:%lu\n", vma,info1->npagefaults);
}

static void
mykmod_vm_close(struct vm_area_struct *vma)
{   
	struct mykmod_vma_info *info1;
	info1=vma->vm_private_data;
	printk("mykmod_vm_close: vma=%p npagefaults:%lu\n", vma,info1->npagefaults);
}

static int
mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct mykmod_vma_info *info_fault;
	info_fault = vma->vm_private_data;

	// TODO: build virt->phys mappings
	if (info_fault != NULL || info_fault->devinfo->data != NULL)
	{
		
		info_fault->npagefaults++;
		struct page *page;
		// page = virt_to_page(info_fault->devinfo->data);
		page = virt_to_page(info_fault + (vmf->pgoff * PAGE_SIZE));

		get_page(page);
		vmf->page = page;
		// vmf->pgoff = vma->vm_pgoff;

		printk("mykmod_vm_fault: vma=%p vmf=%p pgoff=%lu page=%p\n", vma, vmf, vmf->pgoff, vmf->page);
	}
	return 0;

	// printk("mykmod_vm_fault: vma=%p vmf=%p pgoff=%lu page=%p\n", vma, vmf, vmf->pgoff, vmf->page);
	// // TODO: build virt->phys mappings
	// struct page *page;
	// struct mykmod_dev_info *info;
	// info = (struct mykmod_dev_info*)vma->vm_private_data;
	// if(info->data){
	// 	page = virt_to_page(info->data);
	// 	get_page(page);
	// 	vmf->page =  page;
	// }
    
	// struct mykmod_vma_info *info1;
	// info1=vma->vm_private_data;
	// info1->npagefaults++;
	// vma->vm_private_data=info1;


	// return 0;
}

    // struct page *pageptr; 
    // unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    // unsigned long physaddr = address - vma->vm_start + offset;
    // unsigned long pageframe = physaddr >> PAGE_SHIFT;

    // if (!pfn_valid(pageframe))
    //     return NOPAGE_SIGBUS;
    // pageptr = pfn_to_page(pageframe);
    // get_page(pageptr);
    // if (type)
    //     *type = VM_FAULT_MINOR;
    // return pageptr;

