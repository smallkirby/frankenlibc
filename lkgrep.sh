#!/bin/bash

function usage() {
  echo "Usage: lkgrep.sh <search string>"
  exit
}

[ "$#" -ne 1 ] && usage

grep $1 -R ./linux/arch/lkl
grep $1 -R ./linux/tools/lkl
