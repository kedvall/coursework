#!/bin/bash

cd /workdir/mp3/syscalls
cp /workdir/mp3/fsdir/basicMap.txt /workdir/mp3/syscalls/to_fsdir
make clean
make 2>&1 >/dev/null | tee smake_log.txt
rm -f *.o

cd /workdir/mp3

./elfconvert syscalls/game
mv syscalls/game syscalls/to_fsdir
rm -f syscalls/game.converted

./createfs -i syscalls/to_fsdir -o student-distrib/filesys_img
