set environment RUMP_VERBOSE=1
set args ./tests/fork-test rootfs:disk.img -- hello
catch exec
run
b start_kernel
c
