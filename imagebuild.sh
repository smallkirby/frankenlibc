#!/bin/bash

INTERPRETER=/lib/ld-musl-x86_64.so.1
TESTDIR=./tests/
DISKNAME=disk.img
MNTPOINT=./mnt
RUMPLIBC=./rump/lib/libc.so

function usage() {
  echo "Build Image: build ext4 image for test."
  echo " $0 <test name>"
}

function _error() {
  local argv=("$@")
  local fmt="[ERROR] $1"
  local args=("${argv[@]:1}")
  >&2 printf "$fmt" "${args[@]}"
  >&2 echo ""
}

function _error_exit() {
  exit_code=$1
  shift
  _error "$@"
  exit "$exit_code"
}

if [ "$#" -ne "1" ]; then
  usage && exit
fi
TESTNAME="$1"

if [ "$NOBUILD" == 1 ]; then
  echo "$NOBUILD"
  echo "[+] compiling test: $TESTNAME"
  gcc -fPIC -pie "$TESTDIR"/"$TESTNAME".c -o "$TESTDIR"/"$TESTNAME" || _error_exit 1 "compile test"
fi

current_preter=$(ldd tests/"$TESTNAME" | grep "ld-*" | xargs;)
current_preter="${current_preter%% *}"
echo "[+] change interpreter for the binary: $current_preter -> $INTERPRETER"
patchelf --set-interpreter /lib/ld-musl-x86_64.so.1 "$TESTDIR"/"$TESTNAME" || _error_exit 1 "change intepreter"

if [ -f "$DISKNAME" ]; then
  echo "[.] skipping creating $DISKNAME"
else
  echo "[+] creating new filesystem"
  dd if=/dev/zero of="$DISKNAME" bs=1 count=1 seek=400M || _error_exit 1 "disk"
  mkfs.ext4 "$DISKNAME"
fi

echo "[+] installing test in $DISKNAME"
if ! [ -d "$MNTPOINT" ]; then
  mkdir "$MNTPOINT"
fi
sudo mount -o loop "$DISKNAME" "$MNTPOINT"
if ! [ -d "$MNTPOINT"/lib ]; then
  sudo mkdir "$MNTPOINT"/lib
fi
sudo cp "$RUMPLIBC" "$MNTPOINT"/lib/
sudo cp "$RUMPLIBC" "$MNTPOINT$INTERPRETER"
sudo cp "$TESTDIR"/"$TESTNAME" "$MNTPOINT"/"$TESTNAME"
#sudo cp "$TESTDIR"/"$TESTNAME" "$MNTPOINT"/"$TESTNAME"


echo "[+] cleaning up mount point"
sudo umount "$MNTPOINT" || _error_exit 1 "umount"
sudo rm -rf "$MNTPOINT"
