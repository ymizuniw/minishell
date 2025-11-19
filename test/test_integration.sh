#!/bin/bash

TEST_SCRPT=test.sh

./$TEST_SCRPT >res_mish.log;
<$TEST_SCRPT  bash >res_bash.log;
diff res_mish.log res_bash.log > res_diff.log
if [ $(cat res_diff.log | wc -c) -eq 0 ]; then
    echo "no difference!"
else
    echo "differs!"
    cat res_diff.log
fi
