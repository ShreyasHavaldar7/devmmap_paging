# devmmap_paging
OS-II Kernel Programming Assignment
------------------------------------------------------------------
CS18BTECH11042
CS18BTECH11050
------------------------------------------------------------------
# COMPILATION INSTRUCTIONS
BUILDING AND LOADING THE DRIVER

[root@cs3523 ~]# cd devmmap_paging
[root@cs3523 devmmap_paging]# make

make[1]: Entering directory `/root/devmmap_paging/kernel'
make -C /lib/modules/3.10.0-1062.9.1.el7.x86_64/build M=/root/devmmap_paging/kernel modules
make[2]: Entering directory `/usr/src/kernels/3.10.0-1062.9.1.el7.x86_64'
  Building modules, stage 2.
  MODPOST 1 modules
make[2]: Leaving directory `/usr/src/kernels/3.10.0-1062.9.1.el7.x86_64'
make[1]: Leaving directory `/root/devmmap_paging/kernel'
make[1]: Entering directory `/root/devmmap_paging/util'
make[1]: `memutil' is up to date.
make[1]: Leaving directory `/root/devmmap_paging/util'

[root@cs3523 devmmap_paging]# insmod kernel/mykmod.ko
[root@cs3523 devmmap_paging]# grep mykmod /proc/devices
243 mykmod

------------------------------------------------------------------
# INSTRUCTIONS TO RUN (NOT USING THE TEST SCRIPT)
SYNTAX

Usage: ./util/memutil [options] <devname>
Options:
--operation <optype> : Where optype can be mapread, mapwrite
--message <message>  : Message to be written/read-compare to/from the device memory
--paging <ptype> 	 : Where ptype can be prefetch or demand
--help           	 : Show this help
------------------------------------------------------------------
# SAMPLE OUTPUTS WITHOUT USING TEST SCRIPT


# CASE 1:PREFETCH (READ ONLY)
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_pR6 c 243 10
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_pR6 --pt prefetch --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256
------------------------------------------------------------------------------------------------------

# CASE 2:DEMAND (READ ONLY)
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_JZ1 c 243 11
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_JZ1 --pt demand --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256
[  248.623157] mykmod_vm_open: vma=ffff9a0f78796288 npagefaults:0
[  248.623164] mykmod_vm_close: vma=ffff9a0f78796288 npagefaults:0
------------------------------------------------------------------------------------------------------

# CASE 3a:PREFETCH (READ AND WRITE):MESSAGE LENGTH<=PAGESIZE(4096)

[root@cs3523 devmmap_paging]# mknod /tmp/mydev_fBc c 243 20
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_fBc --pt prefetch --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256
[  248.623157] mykmod_vm_open: vma=ffff9a0f78796288 npagefaults:0
[  248.623164] mykmod_vm_close: vma=ffff9a0f78796288 npagefaults:0
[  368.153225] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.153907] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  368.153915] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.165094] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256

# CASE 3b:PREFETCH (READ AND WRITE):MESSAGE LENGTH>PAGESIZE(4096)

[root@cs3523 devmmap_paging]# mknod /tmp/mydev_fAc c 243 21
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_fAc --pt prefetch --op mapwrite --op mapread --mes ThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisenthereforealwaysholdsiinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewistothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprinciofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleshello

[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256
[  248.623157] mykmod_vm_open: vma=ffff9a0f78796288 npagefaults:0
[  248.623164] mykmod_vm_close: vma=ffff9a0f78796288 npagefaults:0
[  368.153225] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.153907] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  368.153915] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.165094] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  472.266659] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.275885] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
[  472.275910] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.278093] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
------------------------------------------------------------------------------------------------------

# CASE 4a:DEMAND (READ AND WRITE):MESSAGE LENGTH<=PAGESIZE(4096)

[root@cs3523 devmmap_paging]# mknod /tmp/mydev_Ln5 c 243 22
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_Ln5 --pt demand --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256
[  248.623157] mykmod_vm_open: vma=ffff9a0f78796288 npagefaults:0
[  248.623164] mykmod_vm_close: vma=ffff9a0f78796288 npagefaults:0
[  368.153225] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.153907] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  368.153915] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.165094] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  472.266659] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.275885] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
[  472.275910] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.278093] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
[  740.551923] mykmod_vm_open: vma=ffff9a0ff8d9b1b0 npagefaults:0
[  740.551950] mykmod_vm_close: vma=ffff9a0ff8d9b1b0 npagefaults:256
[  740.551956] mykmod_vm_open: vma=ffff9a0ff8d9b1b0 npagefaults:0
[  740.551965] mykmod_vm_close: vma=ffff9a0ff8d9b1b0 npagefaults:256

# CASE 4b:DEMAND (READ AND WRITE):MESSAGE LENGTH>PAGESIZE(4096)

[root@cs3523 devmmap_paging]# mknod /tmp/mydev_Ln6 c 243 23
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_Ln6 --pt demand --op mapwrite --op mapread --mes ThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisenthereforealwaysholdsiinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewistothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprinciofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleoThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleofselectionThewisemanthereforealwaysholdsinthesematterstothisprincipleshello

[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  151.570537] mykmod_vm_open: vma=ffff9a0f787961b0 npagefaults:0
[  151.572697] mykmod_vm_close: vma=ffff9a0f787961b0 npagefaults:256
[  248.623157] mykmod_vm_open: vma=ffff9a0f78796288 npagefaults:0
[  248.623164] mykmod_vm_close: vma=ffff9a0f78796288 npagefaults:0
[  368.153225] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.153907] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  368.153915] mykmod_vm_open: vma=ffff9a0f78796360 npagefaults:0
[  368.165094] mykmod_vm_close: vma=ffff9a0f78796360 npagefaults:256
[  472.266659] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.275885] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
[  472.275910] mykmod_vm_open: vma=ffff9a0f73fed5e8 npagefaults:0
[  472.278093] mykmod_vm_close: vma=ffff9a0f73fed5e8 npagefaults:256
[  740.551923] mykmod_vm_open: vma=ffff9a0ff8d9b1b0 npagefaults:0
[  740.551950] mykmod_vm_close: vma=ffff9a0ff8d9b1b0 npagefaults:256
[  740.551956] mykmod_vm_open: vma=ffff9a0ff8d9b1b0 npagefaults:0
[  740.551965] mykmod_vm_close: vma=ffff9a0ff8d9b1b0 npagefaults:256
[  861.559056] mykmod_vm_open: vma=ffff9a0f787ed870 npagefaults:0
[  861.559109] mykmod_vm_close: vma=ffff9a0f787ed870 npagefaults:256
[  861.559115] mykmod_vm_open: vma=ffff9a0f787ed870 npagefaults:0
[  861.559145] mykmod_vm_close: vma=ffff9a0f787ed870 npagefaults:256

------------------------------------------------------------------------------------------------------
# UNLOADING THE DRIVER
INSTRUCTIONS:

[root@cs3523 devmmap_paging]# rm -f /tmp/mydev*
[root@cs3523 devmmap_paging]# rmmod mykmod
------------------------------------------------------------------------------------------------------

# SAMPLE OUTPUT USING THE TEST SCRIPT PROVIDED

BUILDING AND LOAD THE DRIVER AS PER INSTRUCTIONS ABOVE.
# INSTRUCTIONS:-
[root@cs3523 devmmap_paging]# chmod +x runtest.sh 
[root@cs3523 devmmap_paging]# ./runtest.sh 

# OUTPUT:
PASS - Test 0 : Module loaded with majorno: 243
PASS - Test 1 : Single process reading using mapping
PASS - Test 2 : Single process writing using mapping
PASS - Test 3 : Multiple process reading using mapping
PASS - Test 4 : Multiple process writing using mapping
PASS - Test 5 : One process writing using mapping and other process reading using mapping
PASS - Test 6 : One process writing to one dev and other process reading from another dev
------------------------------------------------------------------------------------------------------







