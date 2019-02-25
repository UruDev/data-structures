#! /bin/bash

for i in {1..9..1} 
  do
    ./principal < "test/0$i.in" > "test/0$i.sal"
    echo "test 0$i"
    diff "test/0$i.out" "test/0$i.sal"
  done
