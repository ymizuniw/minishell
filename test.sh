#!/bin/bash

export VAR1="a b   c       d"
export VAR2="$USER $HOME $VAR1"
export VAR3='VAR3'
<< EOF cat
$VAR1
$VAR2
$VAR3
EOF
