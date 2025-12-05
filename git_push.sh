#!/bin/bash

read -p "enter commit message: " MSG

git add .
git commit -m "$MSG"
git push
