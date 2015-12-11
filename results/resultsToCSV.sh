#!/bin/bash

DIR_BASENAME="./logs."
TARGET_DIR="CSV"

if [ ! -e $TARGET_DIR ]; then
    mkdir $TARGET_DIR
fi

for nbClusters in 1 4 16 64
do
    if [ ! -e $DIR_BASENAME$nbClusters ]; then
	continue;
    fi;
    
    BASENAME="$DIR_BASENAME$nbClusters/tty1.$nbClusters."

    LINE0=""
    LINE1=""
    LINE2=""
    LINE3=""
    LINE4=""

    for nbWords in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432
    do
        LINE0="$LINE0$nbWords;"
        TMP=`cat $BASENAME$nbWords | grep total | cut -d "=" -f2 | cut -d"." -f1 | head -n1`
        LINE1="$LINE1$TMP;"
        TMP=`cat $BASENAME$nbWords | grep total | cut -d "=" -f2 | cut -d"." -f1 | tail -n1`
        LINE2="$LINE2$TMP;"
        TMP=`cat $BASENAME$nbWords | grep mono_time | cut -d "=" -f2 | cut -d"." -f1 | tail -n1`
        LINE3="$LINE3$TMP;"
        TMP=`cat $BASENAME$nbWords | grep multi_time | cut -d "=" -f2 | cut -d"." -f1 | tail -n1`
        LINE4="$LINE4$TMP;"
    done

    echo $LINE0 >  $TARGET_DIR/results$nbClusters.csv
    echo $LINE1 >> $TARGET_DIR/results$nbClusters.csv
    echo $LINE2 >> $TARGET_DIR/results$nbClusters.csv
    echo $LINE3 >> $TARGET_DIR/results$nbClusters.csv
    echo $LINE4 >> $TARGET_DIR/results$nbClusters.csv
done

# speedup.csv

echo "" > $TARGET_DIR/Speedup.CSV

for nbClusters in 1 4 16
do
    BASENAME="$DIR_BASENAME$nbClusters/tty1.$nbClusters."
    
    LINE0=""
    
    for nbWords in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432
    do
        TMP=`cat $BASENAME$nbWords | grep Speedup | cut -d ">" -f2`
        LINE0="$LINE0$TMP;"
    done

    echo $LINE0 >> $TARGET_DIR/Speedup.CSV
    
done


