#!/bin/bash

./test.sh >res_mish.log;
<test.sh  bash >res_bash.log;
diff res_mish.log res_bash.log > res_diff.log
if [ $(cat res_diff.log | wc -c) -eq 0 ]; then
    echo "no difference!"
else
    echo "differs!"
    cat res_diff.log
fi