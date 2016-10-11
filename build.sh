#!/bin/sh

message() {
  printf "\033[1;33m ** \033[0;1m$*\033[0m\n"
}

if [ ! -d './build' ]; then
  message "Creating build directory"
  mkdir './build'
fi

cd './build'
message "Running \033[32mcmake ..\033[0;1m in ./build directory"
cmake ..
ret=$?
if [ $ret -ne 0 ]; then
  message "\033[31mcmake failed!"
  message "Please fix the errors and run this script again."

  exit $ret
fi

message "Running \033[32mmake -j2\033[0;1m in ./build directory"
make -j2
ret=$?
if [ $ret -ne 0 ]; then
  message "\033[31mmake failed!"
  message "Please fix the errors and run this script again."

  exit $ret
fi
