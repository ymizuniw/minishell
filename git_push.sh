#!/bin/bash

read -p "enter branch name: " BRANCH

if ! git checkout $BRANCH
then    exit 1
fi
git add .
read -p "enter commit message: " CMSG
git commit -m "$CMSG"
read -p "push?(y/n)" PUSH
if [[ "$PUSH" == "y" ]]
then    git push
else
    exit 1
fi
