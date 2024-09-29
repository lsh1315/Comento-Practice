#!/bin/sh

makefile(){
    list=$(ls -A)
    if [ "$list" = "" ];then
        touch .gitkeep
    else
        for path in $list
        do
            if [ -d "$path" ];then
                cd "$path"
                makefile
                cd ..
            fi
        done
    fi    
}

cd $1
makefile