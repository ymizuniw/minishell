#!/bin/bash

./test.sh >res_mish.log;
<test.sh  bash >res_bash.log;
diff res_mish.log res_bash.log > res_diff.log