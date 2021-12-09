set environment RUMP_VERBOSE=1
set args ./tests/fork-test rootfs:disk.img -- ls
catch exec
run
b start_kernel
c
