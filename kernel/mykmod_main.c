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

// Data-structure to keep per device info 

struct mykmod_dev_info {
	char *data;
	size_t size;
};

// Device table data-structure to keep all devices, using a pointer to a pointer of type "struct mykmod_dev_info"
// A dynamic array was determined to be the appropriate data structure, to dynamically allocate memory to store information about all 256 devices.

struct mykmod_dev_info **dev_table;

// Data-structure to keep per VMA info 1 pointer to device info, page faults

struct mykmod_vma_info {
	struct mykmod_dev_info *devinfo;
	long unsigned int npagefaults;
};
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
	// Initialize device table using kmalloc, of size 256 times the size of mykmod_Dev_info struct
	dev_table = kmalloc(MYKMOD_MAX_DEVS*sizeof(struct mykmod_dev_info), GFP_KERNEL);

	return 0;
}

static void mykmod_cleanup_module(void)
{
	printk("mykmod unloaded\n");
	unregister_chrdev(mykmod_major,"mykmod");
	// Free device info structures from device table
	int i = 0;
	while (i<MYKMOD_MAX_DEVS && dev_table[i]!= NULL) // Freeing all elements of the device table, starting from the 0th element unless the maximum number of devices is reached or a null element, that is, a free device is encountered.
	{
		kfree(dev_table[i]);
		i++;
	}	
	
	kfree(dev_table); // Ultimately freeing the device table from the memory
	return;
}

static int
mykmod_open(struct inode *inodep, struct file *filep)
{
	printk("mykmod_open: filep=%p f->private_data=%p "
		"inodep=%p i_private=%p i_rdev=%x maj:%d min:%d\n",
		filep, filep->private_data,
		inodep, inodep->i_private, inodep->i_rdev, MAJOR(inodep->i_rdev), MINOR(inodep->i_rdev)); // Printing the required information
	
	struct mykmod_dev_info *info; // Creating a struct of type mykmod_dev_info to be able to store the info of the devices

	// Allocate memory for devinfo and store in device table and i_private.
	if (inodep -> i_private == NULL) { // If there is no info stored in the inode
		info = kmalloc(sizeof(struct mykmod_dev_info), GFP_KERNEL); // Allocate memory for the device info struct.
		info -> data = (char*)kmalloc(MYDEV_LEN, GFP_KERNEL); // Allocate memory for the data to be stored in the device info.
		memcpy(info -> data, "Opening File", 12); // Size of message copied is 12 because the length of the string passed is 12 temporarily
		inodep -> i_private = info; // Storing the info in the inode.
		
		int i = 0;
		while(i<MYKMOD_MAX_DEVS && dev_table[i] != NULL) { // Checking for the first non-empty entry in the device table, while ensuring we do not cross its length.
			i++;
		}
		if(i<MYKMOD_MAX_DEVS) {
			dev_table[i] = info;		// If there is a free space in the device table within its limit of 256 entries, we add the info at the empty entry.
		}
	}

	// Store device info in file's private_data as well
	filep->private_data = inodep->i_private;

	return 0;
}

static int
mykmod_close(struct inode *inodep, struct file *filep)
{
	printk("mykmod_close: inodep=%p filep=%p\n", inodep, filep);
	return 0;
}

static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma)
{
	printk("mykmod_mmap: filp=%p vma=%p flags=%lx\n", filp, vma, vma->vm_flags);

	// Setup vma's flags, save private data (devinfo, npagefaults) in vm_private_data
	if((vma->vm_pgoff) > (vma->vm_end - vma->vm_start)) { // Ensuring that the size and offset does not exceed the size of the entire virtual memory address space.
		return -EINVAL;
	}

	vma -> vm_ops = &mykmod_vm_ops;
	vma -> vm_flags |= VM_DONTDUMP | VM_DONTEXPAND; // Do not include in core dump and cannot expand with mremap()

	struct mykmod_vma_info *info; // Creating an struct of mykmod_vma_info type to allow for the mapping of data
	info = kmalloc(sizeof(struct mykmod_vma_info), GFP_KERNEL);
	info -> devinfo = filp -> private_data;
	vma -> vm_private_data = info; // Saving the info in the vm_area_struct
	
	mykmod_vm_open(vma);

	return 0;
}

static void
mykmod_vm_open(struct vm_area_struct *vma)
{
	struct mykmod_vma_info *info1;
	info1 = vma->vm_private_data;
	info1 -> npagefaults = 0; // Number of pagefults are 0 initially
	printk("mykmod_vm_open: vma=%p npagefaults:%lu\n", vma,info1->npagefaults);
}

static void
mykmod_vm_close(struct vm_area_struct *vma)
{   
	struct mykmod_vma_info *info1;
	info1 = vma->vm_private_data;
	printk("mykmod_vm_close: vma=%p npagefaults:%lu\n", vma,info1->npagefaults); // Printing the number of page faults encountered when a VM segment is closed and re initialising it to 0.
	info1 -> npagefaults = 0;

}

static int
mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct mykmod_vma_info *info_fault; // Creating a struct of type mykmod_vma_info
	info_fault = vma->vm_private_data;

	// Build virt->phys mappings
	if (info_fault != NULL || info_fault->devinfo->data != NULL) // Just ensuring that the mapping was successful
	{		
		info_fault->npagefaults++; // As this function was invoked, a page fault has occured, thus increasing the count by one.
		struct page *page; // Creating a struct of type page

		page = virt_to_page(info_fault->devinfo->data + ((vmf->pgoff + vma->vm_pgoff) * PAGE_SIZE)); // Translating the physical addresses which was obtained by the conversion from the virtual address, to the struct
		// Utilizng the function virt_to_page to convert the correct virtual address, with the required offsets into a page struct.

		get_page(page);
		vmf->page = page;
		// Return the page generated to vm->page

		printk("mykmod_vm_fault: vma=%p vmf=%p pgoff=%lu page=%p\n", vma, vmf, vmf->pgoff, vmf->page);
	}
	return 0;
}