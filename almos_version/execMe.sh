#! /bin/bash
# execSim

source SourceMe
export SOCLIB_TTY=FILES

# Compiler pour $nbClusters*4 coeurs
cd apps/dwc/
make clean
# make NB_WORKERS=$nbCores TARGET=tsar BIN=woco$nbClusters
make TARGET=tsar

# Installer sur HDD
#make install BIN=woco$nbClusters OVERWRITE="-D o"
make install OVERWRITE="-D o"

# On initialise les plages de mots pour chaque config
WORDS_SIZE_1="1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384"
WORDS_SIZE_4="1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288"
WORDS_SIZE_16="1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432"
WORDS_SIZE_64=""

cd ../../

for nbClusters in 1 4 16 64
do

    # on organise les logs par dossiers
    mkdir test/pf1/logs.$nbClusters

    WORDS=$WORDS_SIZE_64;
    if [ $nbClusters -eq 1 ];then
	WORDS=$WORDS_SIZE_1;
    fi;
    if [ $nbClusters -eq 4 ]; then
	WORDS=$WORDS_SIZE_4;
    fi;
    if [ $nbClusters -eq 16 ]; then
	WORDS=$WORDS_SIZE_16;
    fi;
	      
    
    for nbWords in $WORDS 
    do
        
        # créer le script de lancement
        cd test/pf1/
        cat <<EOF > shrc
echo started
echo going to exec /bin/dwc $nbWords
exec /bin/dwc $nbWords
echo dwc ended
EOF

        mcopy -D o -i hdd-img.bin ./shrc ::/etc/.

        # Lancer le simulateur
        echo "Hello tty1" > tty1

        echo "[INFO] Going to make sim$nbClusters"
        make sim$nbClusters &

        # Arrêter le simulateur
        while [ `grep -qE "\[MainDwc\] Finished" tty1; echo $?` -eq 1 ]
        do
            sleep 5
        done
        # killall tsar-sim$nbClusters soclib-fb
        killall make
        
        # Copie des logs
        for j in {0..3}
        do
            cp tty$j logs.$nbClusters/tty$j.$nbClusters.$nbWords
        done
        cd ../../
    done
done

exit 0
