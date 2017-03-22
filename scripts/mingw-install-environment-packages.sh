#!/bin/bash

while IFS='' read -r line || [[ -n "$line" ]]; do mingw-get install $line; done < ../doc/mingw-packages-list.txt
