#!/usr/bin/env sh

set -ex

mkdir -p ./coverage
lcov --capture --directory ./src --output-file coverage/lcov.info.all
lcov --remove coverage/lcov.info.all '*/include/*' --output-file coverage/lcov.info --ignore-errors unused
# genhtml -o coverage/html coverage/lcov.info
