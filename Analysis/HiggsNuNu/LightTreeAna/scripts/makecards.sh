#!/bin/bash

if [ "$#" -ne "2" ]; then
    echo "Usage: $0 <doSubmit> <do4params>"
    exit 0
fi


DOSUBMIT=$1
DO4PARAMS=$2
#infolder=output_parkedana
#outfolder=cards_parkedana/
#blind=false
infolder=output_run2ana_160224/
outfolder=cards_run2ana_160229/
blind=false
zvvstat=18
mkdir -p $outfolder

extraoptions="--do_ues=false" #--do_ggh=false --do_separate_qcdewk=false"

#for channel in nunu enu munu mumu #topl topb
for channel in taunu qcd
do
    mkdir -p $outfolder/$channel

    #for mindphicut in 2.31 #1.01 1.21 1.41 1.61 1.81 2.01 2.11 2.21 2.31 2.41 2.51 2.61 2.81
    for mindphicut in -1.
    do
	for minjjcut in 1101 #1601 #801 901 1001 1101 1201 1301 1401 1501 1601 1701 1801 1901
	do
	    OUTNAME=$outfolder/$channel/vbfhinv_${channel}_13TeV_${mindphicut}_${minjjcut}.txt
	    if (( "$DO4PARAMS" == "1" )); then
		OUTNAME=$outfolder/$channel/vbfhinv_${channel}_13TeV_${mindphicut}_${minjjcut}_4params.txt
	    fi
	    echo " ********************************"
	    echo " *** Processing channel $channel"
	    echo " ********************************"
	    if (( "$DOSUBMIT" == "0" )); then
		echo "./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarXcut=$minjjcut --minvarYcut=$mindphicut --histoToIntegrate=alljetsmetnomu_mindphi:dijet_M $extraoptions | tee $outfolder/$channel/card_${mindphicut}_${minjjcut}.log"
	    else
		./bin/makeCountingCard -i $infolder --blind=$blind -o $OUTNAME -m 125 --channel $channel --do_latex true --do_datatop false --zvvstat 0 --qcdrate 0 --mcBkgOnly=true --do_run2=true --do_4params=$DO4PARAMS --minvarXcut=$minjjcut --minvarYcut=$mindphicut --histoToIntegrate=alljetsmetnomu_mindphi:dijet_M $extraoptions | tee $outfolder/$channel/card_${mindphicut}_${minjjcut}.log
	    fi
	done
    done
done

