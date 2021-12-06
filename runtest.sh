#!/bin/bash

function usage() {
  echo "Build and run test"
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
echo "[+] compiling test: $TESTNAME"
make -C tests "$TESTNAME"

echo "[+] running test: $TESTNAME"
RUMP_VERBOSE=1 ./rump/bin/rexec ./tests/"$TESTNAME"



