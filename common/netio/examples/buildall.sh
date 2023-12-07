#!/usr/bin/env bash

./getdeps.sh
res=$?
if [[ $res != 0 ]]; then
    echo "getdeps error $res"
    exit $res
fi

sdir=$PWD

for i in $(ls); do
    if [[ -d $i && -e $i/CMakeLists.txt ]]; then
        cd $i
        
        files0=($(ls))
        cmake . || { echo "cmake error in \"$i\""; exit 1; }
        make || { echo "make error in \"$i\""; exit 1; }
        files1=($(ls))
        
        files2=(`echo ${files0[@]} ${files1[@]} | tr ' ' '\n' | sort | uniq -u`)
        
        for f in ${files2[@]}; do
            rm -rf $f
        done
        
        cd ..
    fi
done

cd $sdir
