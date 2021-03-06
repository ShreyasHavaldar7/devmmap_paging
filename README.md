# devmmap_paging #

## OS-II Kernel Programming Assignment ##

------------------------------------------------------------------

- CS18BTECH11042

- CS18BTECH11050

------------------------------------------------------------------
### LIST OF FILES SUBMITTED ###

- *mykmod_main.c*
- *memutil.cpp*
- *README.md*
- *Report.pdf*
------------------------------------------------------------------

### COMPILATION INSTRUCTIONS ###

#### BUILDING AND LOADING THE DRIVER ####

```
[root@cs3523 ~]# cd devmmap_paging
[root@cs3523 devmmap_paging]# make
make[1]: Entering directory `/root/devmmap_paging/kernel'
make -C /lib/modules/3.10.0-1062.9.1.el7.x86_64/build M=/root/devmmap_paging/kernel modules
make[2]: Entering directory `/usr/src/kernels/3.10.0-1062.9.1.el7.x86_64'
  CC [M]  /root/devmmap_paging/kernel/mykmod_main.o
/root/devmmap_paging/kernel/mykmod_main.c:19:0: warning: "PAGE_SIZE" redefined [enabled by default]
 #define PAGE_SIZE 4096
 ^
In file included from ./arch/x86/include/asm/ptrace.h:5:0,
                 from ./arch/x86/include/asm/alternative.h:10,
                 from ./arch/x86/include/asm/bitops.h:16,
                 from include/linux/bitops.h:37,
                 from include/linux/kernel.h:10,
                 from include/linux/sched.h:15,
                 from include/linux/uaccess.h:5,
                 from /root/devmmap_paging/kernel/mykmod_main.c:4:
./arch/x86/include/asm/page_types.h:10:0: note: this is the location of the previous definition
 #define PAGE_SIZE (_AC(1,UL) << PAGE_SHIFT)
 ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_cleanup_module’:
/root/devmmap_paging/kernel/mykmod_main.c:91:2: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  int i = 0;
  ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_open’:
/root/devmmap_paging/kernel/mykmod_main.c:106:2: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  struct mykmod_dev_info *info; // Creating a struct of type mykmod_dev_info to be able to store the info of the devices
  ^
/root/devmmap_paging/kernel/mykmod_main.c:115:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   int i = 0;
   ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_mmap’:
/root/devmmap_paging/kernel/mykmod_main.c:151:2: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  struct mykmod_vma_info *info; // Creating an struct of mykmod_vma_info type to allow for the mapping of data
  ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_vm_fault’:
/root/devmmap_paging/kernel/mykmod_main.c:188:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   struct page *page; // Creating a struct of type page
   ^
  LD [M]  /root/devmmap_paging/kernel/mykmod.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /root/devmmap_paging/kernel/mykmod.mod.o
  LD [M]  /root/devmmap_paging/kernel/mykmod.ko
make[2]: Leaving directory `/usr/src/kernels/3.10.0-1062.9.1.el7.x86_64'
make[1]: Leaving directory `/root/devmmap_paging/kernel'
make[1]: Entering directory `/root/devmmap_paging/util'
g++ -std=c++11 -I ../include -I ../lib -L ../lib -lrt -o memutil  memutil.cpp -g -lpthread
make[1]: Leaving directory `/root/devmmap_paging/util'

[root@cs3523 devmmap_paging]# insmod kernel/mykmod.ko
[root@cs3523 devmmap_paging]# grep mykmod /proc/devices
243 mykmod
```

------------------------------------------------------------------

### INSTRUCTIONS TO RUN (NOT USING THE TEST SCRIPT) ###

#### SYNTAX ####

```
Usage: ./util/memutil [options] <devname>
Options:
--operation <optype>  : Where optype can be mapread, mapwrite
--message <message>   : Message to be written/read-compare to/from the device memory
--paging <ptype>      : Where ptype can be prefetch or demand
--help                : Show this help
```

------------------------------------------------------------------

### SAMPLE OUTPUTS WITHOUT USING TEST SCRIPT ###

#### CASE 1: PREFETCH (READ ONLY) ####

```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_pR6 c 243 10
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_pR6 --pt prefetch --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_pR6
```
------------------------------------------------------------------------------------------------------

#### CASE 2: DEMAND (READ ONLY) ####

```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_JZ1 c 243 11
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_JZ1 --pt demand --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  248.623157] mykmod_vm_open: vma=ffff9a0f78796288 npagefaults:0
[  248.623164] mykmod_vm_close: vma=ffff9a0f78796288 npagefaults:0

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_JZ1
```
------------------------------------------------------------------------------------------------------

#### CASE 3a: PREFETCH (READ AND WRITE): MESSAGE LENGTH <= PAGESIZE ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_fBc c 243 20
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_fBc --pt prefetch --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  368.153225] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.153907] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  368.153915] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.165094] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_fBc
```

#### CASE 3b: PREFETCH (READ AND WRITE): MESSAGE LENGTH > PAGESIZE ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_fAc c 243 21
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_fAc --pt prefetch --op mapwrite --op mapread --mes ThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisenthereforealwaysholdsiinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewistothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprinciofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleshello
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  472.266659] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.275885] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
[  472.275910] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.278093] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_fAc
```
------------------------------------------------------------------------------------------------------

#### CASE 4a: DEMAND (READ AND WRITE): MESSAGE LENGTH <= PAGESIZE ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_Ln5 c 243 22
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_Ln5 --pt demand --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  740.551923] mykmod_vm_open: vma=ffff9a0ff8d9b1b0 npagefaults:0
[  740.551950] mykmod_vm_close: vma=ffff9a0ff8d9b1b0 npagefaults:256
[  740.551956] mykmod_vm_open: vma=ffff9a0ff8d9b1b0 npagefaults:0
[  740.551965] mykmod_vm_close: vma=ffff9a0ff8d9b1b0 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_Ln5
```

#### CASE 4b: DEMAND (READ AND WRITE): MESSAGE LENGTH > PAGESIZE ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_Ln6 c 243 23
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_Ln6 --pt demand --op mapwrite --op mapread --mes ThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisenthereforealwaysholdsiinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewistothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprinciofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleshello
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  861.559056] mykmod_vm_open: vma=ffff9a0f787ed870 npagefaults:0
[  861.559109] mykmod_vm_close: vma=ffff9a0f787ed870 npagefaults:256
[  861.559115] mykmod_vm_open: vma=ffff9a0f787ed870 npagefaults:0
[  861.559145] mykmod_vm_close: vma=ffff9a0f787ed870 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_Ln6
```
------------------------------------------------------------------------------------------------------

#### CASE 5: MULTIPLE PROCESS READING USING MAPPING ####

#### CASE 5a: PREFETCH ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_krt c 243 30
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_krt --pt prefetch --op mapread
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_krt --pt prefetch --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  270.523497] mykmod_vm_open: vma=ffff89d937ecae58 npagefaults:0
[  270.524166] mykmod_vm_close: vma=ffff89d937ecae58 npagefaults:256
[  274.155160] mykmod_vm_open: vma=ffff89d8b66e4948 npagefaults:0
[  274.155770] mykmod_vm_close: vma=ffff89d8b66e4948 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_krt
```
#### CASE 5b: DEMAND PAGING ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_grt c 243 31
[root@cs3523 devmmap_paging]# ./util/memutil mydev_grt --pt demand --op mapread
[root@cs3523 devmmap_paging]# ./util/memutil mydev_grt --pt demand --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  625.281761] mykmod_vm_open: vma=ffff9cfcf5556e58 npagefaults:0
[  625.281770] mykmod_vm_close: vma=ffff9cfcf5556e58 npagefaults:0
[  628.452384] mykmod_vm_open: vma=ffff9cfcf5556af8 npagefaults:0
[  628.452465] mykmod_vm_close: vma=ffff9cfcf5556af8 npagefaults:0

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_grt
```
------------------------------------------------------------------------------------------------------

#### CASE 6: MULTIPLE PROCESS WRITING USING MAPPING ####

#### CASE 6a: PREFETCH ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_asd c 243 40
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_asd --pt prefetch --op mapwrite --op mapread --mes test2a
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_asd --pt prefetch --op mapwrite --op mapread --mes test2b
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  127.262445] mykmod_vm_open: vma=ffff9c2435ef2438 npagefaults:0
[  127.267992] mykmod_vm_close: vma=ffff9c2435ef2438 npagefaults:256
[  127.268002] mykmod_vm_open: vma=ffff9c2435ef2438 npagefaults:0
[  127.284860] mykmod_vm_close: vma=ffff9c2435ef2438 npagefaults:256
[  130.923799] mykmod_vm_open: vma=ffff9c23b5836798 npagefaults:0
[  130.928635] mykmod_vm_close: vma=ffff9c23b5836798 npagefaults:256
[  130.928642] mykmod_vm_open: vma=ffff9c23b5836798 npagefaults:0
[  130.932848] mykmod_vm_close: vma=ffff9c23b5836798 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_asd
```
#### CASE 6b: DEMAND PAGING ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_qwe c 243 41
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_qwe --pt demand --op mapwrite --op mapread --mes test2c
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_qwe --pt demand --op mapwrite --op mapread --mes test2d
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  380.011792] mykmod_vm_open: vma=ffff9c23b5a9cd80 npagefaults:0
[  380.027935] mykmod_vm_close: vma=ffff9c23b5a9cd80 npagefaults:256
[  380.027964] mykmod_vm_open: vma=ffff9c23b5a9cd80 npagefaults:0
[  380.044239] mykmod_vm_close: vma=ffff9c23b5a9cd80 npagefaults:256
[  383.357414] mykmod_vm_open: vma=ffff9c23b5a9cbd0 npagefaults:0
[  383.366874] mykmod_vm_close: vma=ffff9c23b5a9cbd0 npagefaults:256
[  383.366896] mykmod_vm_open: vma=ffff9c23b5a9cbd0 npagefaults:0
[  383.380902] mykmod_vm_close: vma=ffff9c23b5a9cbd0 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_qwe
```
------------------------------------------------------------------------------------------------------

#### CASE 7: ONE PROCESS WRITING USING MAPPING AND OTHER PROCESS READING USING MAPPING ####

#### CASE 7a: PREFETCH ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_zxc c 243 50
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_zxc --pt prefetch --op mapwrite --mes test7a
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_zxc --pt prefetch --op mapread --mes test7a
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[ 1002.808937] mykmod_vm_open: vma=ffff9c23b8fcfa20 npagefaults:0
[ 1002.813644] mykmod_vm_close: vma=ffff9c23b8fcfa20 npagefaults:256
[ 1025.277486] mykmod_vm_open: vma=ffff9c24379a2e58 npagefaults:0
[ 1025.285407] mykmod_vm_close: vma=ffff9c24379a2e58 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_zxc
```
#### CASE 7b: DEMAND PAGING ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_rty c 243 51
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_rty --pt demand --op mapwrite --mes test7b
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_rty --pt demand --op mapread --mes test7b
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[ 1155.964341] mykmod_vm_open: vma=ffff9c23b8617e58 npagefaults:0
[ 1155.972578] mykmod_vm_close: vma=ffff9c23b8617e58 npagefaults:256
[ 1168.019982] mykmod_vm_open: vma=ffff9c23b8617438 npagefaults:0
[ 1168.028819] mykmod_vm_close: vma=ffff9c23b8617438 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev_rty
```
------------------------------------------------------------------------------------------------------

#### CASE 8: ONE PROCESS WRITING TO ONE DEV AND OTHER PROCESS READING FROM ANOTHER DEV ####

#### CASE 8a: PREFETCH ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_qw1 c 243 60
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_qw2 c 243 70
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_qw1 --pt prefetch --op mapwrite --mes test8a
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_qw2 --pt prefetch --op mapread 2> /dev/null
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[ 1312.644292] mykmod_vm_open: vma=ffff9c23b8617bd0 npagefaults:0
[ 1312.671105] mykmod_vm_close: vma=ffff9c23b8617bd0 npagefaults:256
[ 1355.097147] mykmod_vm_open: vma=ffff9c23b8617798 npagefaults:0
[ 1355.098022] mykmod_vm_close: vma=ffff9c23b8617798 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
```
#### CASE 8b: DEMAND PAGING ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_aw1 c 243 61
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_aw2 c 243 71
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_aw1 --pt demand --op mapwrite --mes test8b
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_aw2 --pt demand --op mapread 2> /dev/null
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[ 1564.113160] mykmod_vm_open: vma=ffff9c24379a2ca8 npagefaults:0
[ 1564.121769] mykmod_vm_close: vma=ffff9c24379a2ca8 npagefaults:256
[ 1583.809507] mykmod_vm_open: vma=ffff9c24379a2e58 npagefaults:0
[ 1583.809533] mykmod_vm_close: vma=ffff9c24379a2e58 npagefaults:0

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
```
------------------------------------------------------------------------------------------------------
#### CASE 9: MEMORY MAPPING WITH A NON ZERO OFFSET (Setting offset_t off = 4096 in memutil.cpp) ####

#### CASE 9a: PREFETCH ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_pR6 c 243 10
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_pR6 --pt prefetch --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close
[   97.523685] mykmod_vm_open: vma=ffff8aa575d531b0 npagefaults:0
[   97.524498] mykmod_vm_close: vma=ffff8aa575d531b0 npagefaults:256
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_fBc c 243 20
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_fBc --pt prefetch --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close
[   97.523685] mykmod_vm_open: vma=ffff8aa575d531b0 npagefaults:0
[   97.524498] mykmod_vm_close: vma=ffff8aa575d531b0 npagefaults:256
[  118.309585] mykmod_vm_open: vma=ffff8aa5f8dff6c0 npagefaults:0
[  118.339803] mykmod_vm_close: vma=ffff8aa5f8dff6c0 npagefaults:256
[  118.339904] mykmod_vm_open: vma=ffff8aa677388a20 npagefaults:0
[  118.346404] mykmod_vm_close: vma=ffff8aa677388a20 npagefaults:256

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
```
------------------------------------------------------------------------------------------------------

#### CASE 9b: DEMAND PAGING: ####
```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_JZl c 243 11
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_JZl --pt demand --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close
[  124.246823] mykmod_vm_open: vma=ffff9517fab545e8 npagefaults:0
[  124.246849] mykmod_vm_close: vma=ffff9517fab545e8 npagefaults:0
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_Ln5 c 243 21
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_Ln5 --pt demand --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close
[  124.246823] mykmod_vm_open: vma=ffff9517fab545e8 npagefaults:0
[  124.246849] mykmod_vm_close: vma=ffff9517fab545e8 npagefaults:0
[  151.983395] mykmod_vm_open: vma=ffff9517fa8b0af8 npagefaults:0
[  151.991419] mykmod_vm_close: vma=ffff9517fa8b0af8 npagefaults:255
[  151.991431] mykmod_vm_open: vma=ffff9517fa8b0af8 npagefaults:0
[  151.996843] mykmod_vm_close: vma=ffff9517fa8b0af8 npagefaults:255

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
```
------------------------------------------------------------------------------------------------------
#### CASE 10: MEMORY MAPPING SUCH THAT IT EXCEEDS THE DEVICE FILE BOUNDS ####

```
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_JZl c 243 11
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_JZl --pt demand --op mapread
mmap read failed: Invalid argument

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
```

------------------------------------------------------------------------------------------------------

### UNLOADING THE DRIVER ###

#### INSTRUCTIONS: ####
```
[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
[root@cs3523 devmmap_paging]# rmmod mykmod
```
------------------------------------------------------------------------------------------------------

### SAMPLE OUTPUT USING THE TEST SCRIPT PROVIDED ###

#### BUILDING AND LOAD THE DRIVER AS PER INSTRUCTIONS ABOVE THEN EXECUTING THE TEST SCRIPT ####
####  ####
```
[root@cs3523 ~]# cd devmmap_paging/
[root@cs3523 devmmap_paging]# make
make[1]: Entering directory `/root/devmmap_paging/kernel'
make -C /lib/modules/3.10.0-1062.9.1.el7.x86_64/build M=/root/devmmap_paging/kernel modules
make[2]: Entering directory `/usr/src/kernels/3.10.0-1062.9.1.el7.x86_64'
  CC [M]  /root/devmmap_paging/kernel/mykmod_main.o
/root/devmmap_paging/kernel/mykmod_main.c:19:0: warning: "PAGE_SIZE" redefined [enabled by default]
 #define PAGE_SIZE 4096
 ^
In file included from ./arch/x86/include/asm/ptrace.h:5:0,
                 from ./arch/x86/include/asm/alternative.h:10,
                 from ./arch/x86/include/asm/bitops.h:16,
                 from include/linux/bitops.h:37,
                 from include/linux/kernel.h:10,
                 from include/linux/sched.h:15,
                 from include/linux/uaccess.h:5,
                 from /root/devmmap_paging/kernel/mykmod_main.c:4:
./arch/x86/include/asm/page_types.h:10:0: note: this is the location of the previous definition
 #define PAGE_SIZE (_AC(1,UL) << PAGE_SHIFT)
 ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_cleanup_module’:
/root/devmmap_paging/kernel/mykmod_main.c:91:2: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  int i = 0;
  ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_open’:
/root/devmmap_paging/kernel/mykmod_main.c:106:2: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  struct mykmod_dev_info *info; // Creating a struct of type mykmod_dev_info to be able to store the info of the devices
  ^
/root/devmmap_paging/kernel/mykmod_main.c:115:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   int i = 0;
   ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_mmap’:
/root/devmmap_paging/kernel/mykmod_main.c:151:2: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  struct mykmod_vma_info *info; // Creating an struct of mykmod_vma_info type to allow for the mapping of data
  ^
/root/devmmap_paging/kernel/mykmod_main.c: In function ‘mykmod_vm_fault’:
/root/devmmap_paging/kernel/mykmod_main.c:188:3: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
   struct page *page; // Creating a struct of type page
   ^
  LD [M]  /root/devmmap_paging/kernel/mykmod.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /root/devmmap_paging/kernel/mykmod.mod.o
  LD [M]  /root/devmmap_paging/kernel/mykmod.ko
make[2]: Leaving directory `/usr/src/kernels/3.10.0-1062.9.1.el7.x86_64'
make[1]: Leaving directory `/root/devmmap_paging/kernel'
make[1]: Entering directory `/root/devmmap_paging/util'
g++ -std=c++11 -I ../include -I ../lib -L ../lib -lrt -o memutil  memutil.cpp -g -lpthread
make[1]: Leaving directory `/root/devmmap_paging/util'
[root@cs3523 devmmap_paging]# chmod +x runtest.sh 
[root@cs3523 devmmap_paging]# ./runtest.sh 
PASS - Test 0 : Module loaded with majorno: 243
PASS - Test 1 : Single process reading using mapping
PASS - Test 2 : Single process writing using mapping
PASS - Test 3 : Multiple process reading using mapping
PASS - Test 4 : Multiple process writing using mapping
PASS - Test 5 : One process writing using mapping and other process reading using mapping
PASS - Test 6 : One process writing to one dev and other process reading from another dev

```

------------------------------------------------------------------------------------------------------
