#!/bin/sh
PRODUCTION=Apr04
PARAMS=./filelists/$PRODUCTION/Params${PRODUCTION}.dat

for CHANNEL in nunu enu munu
  do
  
  for MET in 130 #0 70
    do
    for SYST in central #JESUP JESDOWN JERBETTER JERWORSE PUUP PUDOWN
      do

	FOLDER=./output/$CHANNEL/MET$MET/
	PLOTDIR=PLOTS/$CHANNEL/MET$MET/
	
	if [ "$SYST" != "central" ] #if not doing central                                                                                                               
            then
            if [[ "$SYST" != PU* ]] #For PU syst info is in central root file if not doing PU get output from syst subdirectory                                         
                then
                FOLDER=$FOLDER"/"$SYST"/"
            #else #If doing PU syst pass correct options
		#CORRECT WITH CORRECT PU OPTIONS
            fi
            PLOTDIR=$PLOTDIR"/"$SYST"/"
	fi	

		
	mkdir -p $PLOTDIR
	
	BLIND=1
	if (( "$CHANNEL" != "nunu" )) || (( "$MET" != "130" ))
	    then
	    let BLIND=0
	fi

###### n_jets
# n_jetsplotname="n_jets"
# if [[ "$SYST" = PUUP ]]
#     then
#     n_jetsplotname="n_jets_puUp"
# elif [[ "$SYST" = PUDOWN ]]
#     then
#     n_jetsplotname="n_jets_puDown"
# fi
#./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
#    --folder=$FOLDER --plot_dir=$PLOTDIR  \
#    --plot_name=$n_jetsplotname  --x_axis_label="Number of jets" \
#    --blind=$BLIND \
#    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
#    --y_axis_min=0.01 --extra_pad=10000 \
#    --rebin=1 \
#    --norm_bins=false --verbose=false \
#    --log_y=true \
#    --paramfile=$PARAMS

###### n_jetsingap
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="n_jetsingap"  --x_axis_label="Number of jets in rapidity gap" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=20 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### n_vtx
n_vtxplotname="n_vtx"
if [[ "$SYST" = PUUP ]]
    then
    n_vtxplotname="n_vtx_puUp"
elif [[ "$SYST" = PUDOWN ]]
    then
    n_vtxplotname="n_vtx_puDown"
fi
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name=$n_vtxplotname  --x_axis_label="Number of vertices" \
    --blind=$BLIND \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=1 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jpt_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jpt_1"  --x_axis_label="Leading Jet p_{T} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jeta_1
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jeta_1"  --x_axis_label="Leading Jet #eta" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jpt_2
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jpt_2"  --x_axis_label="Next-to-Leading Jet p_{T} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=40 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### jeta_2
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="jeta_2"  --x_axis_label="Next-to-Leading Jet #eta" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=5 \
    --norm_bins=false --verbose=false \
    --log_y=true \
    --paramfile=$PARAMS

###### mjj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="mjj"  --x_axis_label="M_{jj} [GeV]" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3000 \
    --y_axis_min=0.1 --extra_pad=10000 \
    --rebin=100 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### detajj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="detajj"  --x_axis_label="#Delta#eta_{jj}" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=5 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=2 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### dphijj
dphijjplotname="dphijj"
if [[ "$SYST" = PUUP ]]
    then
    dphijjplotname="dphijj_puUp"
elif [[ "$SYST" = PUDOWN ]]
    then
    dphijjplotname="dphijj_puDown"
fi
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name=$dphijjplotname  --x_axis_label="#Delta#phi_{jj}" \
    --blind=$BLIND --x_blind_min=0 --x_blind_max=1.  \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=3.15 \
    --y_axis_min=0.01 --extra_pad=100000 \
    --rebin=2 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### drjj
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="drjj"  --x_axis_label="#Delta R_{jj}" \
    --blind=$BLIND --x_blind_min=0 --x_blind_max=1.  \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=10 \
    --y_axis_min=0.01 --extra_pad=100000 \
    --rebin=2 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### met
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="met"  --x_axis_label="PF MET (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=10 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

###### metNoMuons
./bin/ControlPlots --cfg=scripts/controlPlot.cfg  \
    --folder=$FOLDER --plot_dir=$PLOTDIR  \
    --plot_name="met_noMuons"  --x_axis_label="PF MET + #mu (GeV)" \
    --blind=$BLIND \
    --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1000 \
    --y_axis_min=0.01 --extra_pad=10000 \
    --rebin=10 \
    --norm_bins=false \
    --log_y=true \
    --paramfile=$PARAMS

    done
  done
done


#plot_name=mjj
#met,met_phi,n_vtx,n_jets,n_jetsingap
#jpt_1,jpt_2,jeta_1,jeta_2
#detajj,drjj,dphijj,etaprodjj