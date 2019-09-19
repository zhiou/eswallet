#!/bin/bash
function format() {
  flist=`ls $1`
  cd $1
  for file in $flist
  do
    
    if [ -d $file ]
    then
        format $file
    else
        if [ $file != "json.hpp" ] && [ "${file##*.}" = "hpp" -o "${file##*.}" = "cpp" ]
        then
          # echo $file
          clang-format -i $file
          chmod 777 $file
        fi
    fi
  done
  cd ../
}

format ./src/
