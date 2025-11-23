#!/bin/bash

rm -f *.log >/dev/null
rm -fr test1 test2 >/dev/null
cd ..
make -j$(nproc) fclean >/dev/null
cd test