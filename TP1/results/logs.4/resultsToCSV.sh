BASENAME="./tty1.4."

LINE0=""
LINE1=""
LINE2=""
LINE3=""
LINE4=""

for nbWords in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384
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

echo $LINE0 >  results.csv
echo $LINE1 >> results.csv
echo $LINE2 >> results.csv
echo $LINE3 >> results.csv
echo $LINE4 >> results.csv

