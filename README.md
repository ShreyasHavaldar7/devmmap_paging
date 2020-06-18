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
[root@cs3523 devmmap_paging]# grep mykmod /proc/devices243 mykmod
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


CASE 1:PREFETCH (READ ONLY)
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_pR6 c 243 10
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_pR6 --pt prefetch --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  594.884897] mykmod_vm_open: vma=ffff99b7785a91b0 npagefaults:0
[  594.885472] mykmod_vm_close: vma=ffff99b7785a91b0 npagefaults:256
------------------------------------------------------------------------------------------------------

CASE 2:DEMAND (READ ONLY)
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_JZ1 c 243 11
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_JZ1 --pt demand --op mapread
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  594.884897] mykmod_vm_open: vma=ffff99b7785a91b0 npagefaults:0
[  594.885472] mykmod_vm_close: vma=ffff99b7785a91b0 npagefaults:256
[  706.687035] mykmod_vm_open: vma=ffff99b775230360 npagefaults:0
[  706.687061] mykmod_vm_close: vma=ffff99b775230360 npagefaults:0
------------------------------------------------------------------------------------------------------

CASE 3:PREFETCH (READ AND WRITE)
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_fBc c 243 20
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_fBc --pt prefetch --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  594.884897] mykmod_vm_open: vma=ffff99b7785a91b0 npagefaults:0
[  594.885472] mykmod_vm_close: vma=ffff99b7785a91b0 npagefaults:256
[  706.687035] mykmod_vm_open: vma=ffff99b775230360 npagefaults:0
[  706.687061] mykmod_vm_close: vma=ffff99b775230360 npagefaults:0
[  911.297362] mykmod_vm_open: vma=ffff99b7f8177510 npagefaults:0
[  911.298046] mykmod_vm_close: vma=ffff99b7f8177510 npagefaults:256
[  911.298054] mykmod_vm_open: vma=ffff99b7f8177510 npagefaults:0
[  911.298720] mykmod_vm_close: vma=ffff99b7f8177510 npagefaults:256
------------------------------------------------------------------------------------------------------

CASE 4:DEMAND (READ AND WRITE)
[root@cs3523 devmmap_paging]# mknod /tmp/mydev_Ln5 c 243 21
[root@cs3523 devmmap_paging]# ./util/memutil /tmp/mydev_Ln5 --pt demand --op mapwrite --op mapread --mes test2
[root@cs3523 devmmap_paging]# dmesg | grep -e mykmod_vm_open -e mykmod_vm_close

[  594.884897] mykmod_vm_open: vma=ffff99b7785a91b0 npagefaults:0
[  594.885472] mykmod_vm_close: vma=ffff99b7785a91b0 npagefaults:256
[  706.687035] mykmod_vm_open: vma=ffff99b775230360 npagefaults:0
[  706.687061] mykmod_vm_close: vma=ffff99b775230360 npagefaults:0
[  911.297362] mykmod_vm_open: vma=ffff99b7f8177510 npagefaults:0
[  911.298046] mykmod_vm_close: vma=ffff99b7f8177510 npagefaults:256
[  911.298054] mykmod_vm_open: vma=ffff99b7f8177510 npagefaults:0
[  911.298720] mykmod_vm_close: vma=ffff99b7f8177510 npagefaults:256
[ 1014.387362] mykmod_vm_open: vma=ffff99b775086ca8 npagefaults:0
[ 1014.387392] mykmod_vm_close: vma=ffff99b775086ca8 npagefaults:1
[ 1014.387399] mykmod_vm_open: vma=ffff99b775086ca8 npagefaults:0
[ 1014.387424] mykmod_vm_close: vma=ffff99b775086ca8 npagefaults:1
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







