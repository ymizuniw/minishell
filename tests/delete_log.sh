#!/bin/bash

rm -f *.log >/dev/null
cd ..
make -j$(nproc) fclean >/dev/null
cd test