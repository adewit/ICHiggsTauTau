JOBWRAPPER=./scripts/generate_job.sh
JOBSUBMIT=true
#JOBSUBMIT="./scripts/submit_ic_batch_job.sh hepmedium.q"


CONFIG=scripts/TauID_2012.cfg
echo $CONFIG

FILELIST=filelists/Apr11_Data_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Apr11/Data_53X/

PATHS=(
'TauPlusX-2012A-13Jul2012-v1'
'TauPlusX-2012A-recover-06Aug2012-v1'
'TauPlusX-2012B-13Jul2012-v1'
'TauPlusX-2012C-24Aug2012-v1'
'TauPlusX-2012C-PromptReco-v2'
'TauPlusX-2012C-EcalRecover-11Dec2012-v1'
'TauPlusX-2012D-PromptReco-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  JOB="$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_name=TauID_$JOB.root \
  --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=Special_2_"$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt  --output_name=TauID_$JOB.root \
   --special_mode=2 --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done




PATHS=(
'Embedded-MTau-2012A-13Jul2012-v1'
'Embedded-MTau-2012A-recover-06Aug2012-v1'
'Embedded-MTau-2012B-13Jul2012-v1'
'Embedded-MTau-2012C-24Aug2012-v1'
'Embedded-MTau-2012C-PromptReco-v2'
'Embedded-MTau-2012D-PromptReco-v1'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/mt_tauid/$i/

  JOB="$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --is_data=1 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_name=TauID_$JOB.root \
  --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done


FILELIST=filelists/Apr11_MC_53X
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/Apr11/MC_53X/

PATHS=(
'WJetsToLNu-v1'
'WJetsToLNu-v2'
'W1JetsToLNu'
'W2JetsToLNu'
'W3JetsToLNu'
'W4JetsToLNu'
'TTJets'
'TT-v1'
'TT-v2'
'T-tW'
'Tbar-tW'
'WWJetsTo2L2Nu'
'WZJetsTo2L2Q'
'WZJetsTo3LNu'
'ZZJetsTo2L2Nu'
'ZZJetsTo2L2Q'
'ZZJetsTo4L'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/mt_tauid/$i

  JOB="$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_name=TauID_$JOB.root \
  --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done

PATHS=(
'DYJetsToLL'
'DY1JetsToLL'
'DY2JetsToLL'
'DY3JetsToLL'
'DY4JetsToLL'
'WJetsToLNu-v1'
'WJetsToLNu-v2'
'W1JetsToLNu'
'W2JetsToLNu'
'W3JetsToLNu'
'W4JetsToLNu'
'TTJets'
'TT-v1'
'TT-v2'
'T-tW'
'Tbar-tW'
'WWJetsTo2L2Nu'
'WZJetsTo2L2Q'
'WZJetsTo3LNu'
'ZZJetsTo2L2Nu'
'ZZJetsTo2L2Q'
'ZZJetsTo4L'
)
for i in "${PATHS[@]}"
do
  echo "$i"

  mkdir -p $SKIMPATH/mt_tauid/$i

  JOB=ZTT_"$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --ztautau_mode=1 --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_name=TauID_$JOB.root \
  --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh

  JOB=ZL_"$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --faked_tau_selector=1 --ztautau_mode=2 --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_name=TauID_$JOB.root \
  --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
  
  JOB=ZJ_"$i"_mt_tauid
  $JOBWRAPPER "./bin/TauIDMeasurement --faked_tau_selector=2 --ztautau_mode=2 --is_data=0 --cfg=$CONFIG --filelist="$FILELIST"_"$i".dat --channel=mt --output_name=TauID_$JOB.root \
  --input_prefix=$PREFIX &> jobs/$JOB.log" jobs/$JOB.sh
  $JOBSUBMIT jobs/$JOB.sh
done
