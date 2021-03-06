#include "TGraphAsymmErrors.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TAxis.h"
#include "TLegend.h"
#include <iterator>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"
#include "TSystem.h"
// #include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "HiggsNuNu/interface/HinvVar.h"
#include "HiggsNuNu/interface/TrigEffAnalysis.h"

using namespace ic;
namespace po = boost::program_options;

EffAndError1D Get1DTrigEff(TH1F const* hist, TH1F const* histpasstrigger, std::vector<double> bins){
  EffAndError1D efftoreturn;
  std::vector<double> trigeff;
  std::vector<std::pair<double,double> > error;
  //Get efficiency for all but last bin
  for(unsigned iBin=0;iBin<bins.size();iBin++){
    double nevents[2];
    double err[2];
    int histbmin=hist->GetXaxis()->FindBin(bins[iBin]);
    int histpasstriggerbmin=histpasstrigger->GetXaxis()->FindBin(bins[iBin]);

    int histbmax,histpasstriggerbmax;
    if(iBin!=(bins.size()-1)){
      histbmax=hist->GetXaxis()->FindBin(bins[iBin+1])-1;
      histpasstriggerbmax=histpasstrigger->GetXaxis()->FindBin(bins[iBin+1])-1;
    }
    else{
      histbmax=(hist->GetNbinsX()+1);
      histpasstriggerbmax=(histpasstrigger->GetNbinsX()+1);
    }

    if((histbmin!=histpasstriggerbmin) && (histbmax !=histpasstriggerbmax)) std::cout<<"Warning binning of histograms with and without trigger is different!"<<std::endl;

    nevents[0]=Integral(hist,histbmin,histbmax);
    nevents[1]=Integral(histpasstrigger,histpasstriggerbmin,histpasstriggerbmax);
    err[0]=Error(hist,histbmin,histbmax);
    err[1]=Error(histpasstrigger,histpasstriggerbmin,histpasstriggerbmax);
    if(nevents[0]!=0) trigeff.push_back(nevents[1]/nevents[0]);
    else{
      trigeff.push_back(0.);
      std::cout<<"0 events even without trigger setting efficiency to 0"<<std::endl;
    }
    double errbase=sqrt(pow(err[0]/nevents[0],2)+pow(err[1]/nevents[1],2)) * trigeff[iBin];
    double errorup;
    double errordown;
    if(trigeff[iBin]+errbase>1.) errorup=1-trigeff[iBin];
    else errorup=errbase;
    if(trigeff[iBin]-errbase<0.) errordown=trigeff[iBin];
    else errordown=errbase;
    error.push_back(std::make_pair(errorup,errordown));
  }
  efftoreturn.effs=trigeff;
  efftoreturn.errs=error;
  return efftoreturn;
}

TGraphAsymmErrors Make1DTrigEffGraph(EffAndError1D trigeffs, std::vector<double> bins){
  std::vector<double> errup,errdown;
  for(unsigned iBin=0;iBin<trigeffs.errs.size();iBin++){
    errup.push_back(trigeffs.errs[iBin].first);
    errdown.push_back(trigeffs.errs[iBin].second);
  }
  std::vector<double> bincentres,binerrors;
  for(unsigned iBin=0;iBin<bins.size(); iBin++){
    if(iBin==bins.size()-1){
      bincentres.push_back(bins[iBin]+0.1);
      binerrors.push_back(0.1);
    }
    else{
      bincentres.push_back((bins[iBin]+bins[iBin+1])/2);
      binerrors.push_back(((bins[iBin+1])-bins[iBin])/2);
    }
  }
  TGraphAsymmErrors outgraph(bincentres.size(),&(bincentres.at(0)),&(trigeffs.effs.at(0)),&(binerrors.at(0)),&(binerrors.at(0)),&(errdown.at(0)),&(errup.at(0)));
  return outgraph;
}

TGraphAsymmErrors Make1DTrigEff(TH1F const* hist, TH1F const* histpasstrigger, Rebinned1DVar* vars){
  std::vector<double> bins=vars->bins();
  EffAndError1D trigeffs =Get1DTrigEff(hist, histpasstrigger, bins);
  TGraphAsymmErrors effgraph=Make1DTrigEffGraph(trigeffs,bins);
  return effgraph;
}

EffAndError3D Make3DTrigEff(TH3F const* hist, TH3F const* histpasstrigger, RebinnedNDVar* var){
  std::vector<std::vector<double> > Bins=var->bins();
  EffAndError3D effanderror;
  std::vector<std::vector<std::vector<double> > > trigeff;
  std::vector<std::vector<std::vector< std::pair<double,double> > > > error;
  
  
  if(Bins.size()!=3){
    std::cout<<"Error function needs to be given 3D bin ranges"<<std::endl;
    return effanderror;
  }

  for(unsigned iBin=0;iBin<Bins[0].size();iBin++){
    std::vector<std::vector<double> > jefftmp;
    std::vector<std::vector<std::pair<double,double> > > jerrtmp;
    for(unsigned jBin=0;jBin<Bins[1].size();jBin++){
      std::vector<double> kefftmp;
      std::vector<std::pair<double, double> >  kerrtmp;
      for(unsigned kBin=0;kBin<Bins[2].size();kBin++){
	double nevents[2];
	double err[2];
	
	//Get lower bin of range
	int histbmin[3], histpasstriggerbmin[3];
	histbmin[0]=hist->GetXaxis()->FindBin(Bins[0][iBin]);
	histpasstriggerbmin[0]=histpasstrigger->GetXaxis()->FindBin(Bins[0][iBin]);
	histbmin[1]=hist->GetYaxis()->FindBin(Bins[1][jBin]);
	histpasstriggerbmin[1]=histpasstrigger->GetYaxis()->FindBin(Bins[1][jBin]);
	histbmin[2]=hist->GetZaxis()->FindBin(Bins[2][kBin]);
	histpasstriggerbmin[2]=histpasstrigger->GetZaxis()->FindBin(Bins[2][kBin]);
	
	//Get Upper bin of range
	int histbmax[3],histpasstriggerbmax[3];
	if(iBin!=(Bins[0].size()-1)){//x
	  histbmax[0]=hist->GetXaxis()->FindBin(Bins[0][iBin+1])-1;
	  histpasstriggerbmax[0]=histpasstrigger->GetXaxis()->FindBin(Bins[0][iBin+1])-1;
	}
	else{
	  histbmax[0]=(hist->GetNbinsX()+1);
	  histpasstriggerbmax[0]=(histpasstrigger->GetNbinsX()+1);
	}

	if(jBin!=(Bins[1].size()-1)){//y
	  histbmax[1]=hist->GetYaxis()->FindBin(Bins[1][jBin+1])-1;
	  histpasstriggerbmax[1]=histpasstrigger->GetYaxis()->FindBin(Bins[1][jBin+1])-1;
	}
	else{
	  histbmax[1]=(hist->GetNbinsY()+1);
	  histpasstriggerbmax[1]=(histpasstrigger->GetNbinsY()+1);
	}

	if(kBin!=(Bins[2].size()-1)){//z
	  histbmax[2]=hist->GetZaxis()->FindBin(Bins[2][kBin+1])-1;
	  histpasstriggerbmax[2]=histpasstrigger->GetZaxis()->FindBin(Bins[2][kBin+1])-1;
	}
	else{
	  histbmax[2]=(hist->GetNbinsZ()+1);
	  histpasstriggerbmax[2]=(histpasstrigger->GetNbinsZ()+1);
	}
	
	std::cout<<Bins[0][iBin]<<" "<<Bins[1][jBin]<<" "<<Bins[2][kBin]<<std::endl;//!!
	
	//Check binning is the same
	for(unsigned iCheck=0;iCheck<3;iCheck++){
	  if((histbmin[iCheck]!=histpasstriggerbmin[iCheck]) && (histbmax[iCheck] !=histpasstriggerbmax[iCheck])) std::cout<<"Warning binning of histograms with and without trigger is different! "<<iCheck<<std::endl;
	}
	
	//Get events before and after trigger with errors
	nevents[0]=Integral(hist,histbmin[0],histbmax[0],histbmin[1],histbmax[1],histbmin[2],histbmax[2]);
	nevents[1]=Integral(histpasstrigger,histpasstriggerbmin[0],histpasstriggerbmax[0],histpasstriggerbmin[1],histpasstriggerbmax[1],histpasstriggerbmin[2],histpasstriggerbmax[2]);
	err[0]=Error(hist,histbmin[0],histbmax[0],histbmin[1],histbmax[1],histbmin[2],histbmax[2]);
	err[1]=Error(histpasstrigger,histpasstriggerbmin[0],histpasstriggerbmax[0],histbmin[1],histbmax[1],histbmin[2],histbmax[2]);

	//Get efficiency
	double efftmp;
	if(nevents[0]!=0) efftmp=nevents[1]/nevents[0];
	else{
	  efftmp=0.;
	  std::cout<<"0 events even without trigger setting efficiency to 0"<<std::endl;
	}
	double errbase=sqrt(pow(err[0]/nevents[0],2)+pow(err[1]/nevents[1],2)) * efftmp;
	double errorup;
	double errordown;
	if(efftmp+errbase>1.) errorup=1-efftmp;
	else errorup=errbase;
	if(efftmp-errbase<0.) errordown=efftmp;
	else errordown=errbase;
	kefftmp.push_back(efftmp);
	kerrtmp.push_back(std::make_pair(errorup,errordown));
      }
      jefftmp.push_back(kefftmp);
      jerrtmp.push_back(kerrtmp);
    }
    trigeff.push_back(jefftmp);
    error.push_back(jerrtmp);
  }
  effanderror.effs=trigeff;
  effanderror.errs=error;
  return effanderror;
}

int main(int argc, char* argv[]){//main

  std::string cfg;
  std::string infolder;
  std::string outfolder;

  double Alumi;
  double Blumi;
  double Clumi;
  double Dlumi;

  double mjjcut;
  double metcut;
  double j2ptcut;
  double j1ptcut;
  double dijet_detacut;
  double dijet_dphicut;
  double cjvcut;
  double l1metcut;
  double l1metprecut;
  bool do1deffs;
  bool do3deffs;
  int verbosity;

  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->default_value("scripts/DefaultConfigTrigEff.cfg"));
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    ("infolder",            po::value<std::string>(&infolder)-> default_value("output_trigeff"))
    ("outfolder",           po::value<std::string>(&outfolder)-> default_value("trigeffplots"))
    ("Alumi",               po::value<double>(&Alumi)-> default_value(889.362))
    ("Blumi",               po::value<double>(&Blumi)-> default_value(4429.635))
    ("Clumi",               po::value<double>(&Clumi)-> default_value(7152))
    ("Dlumi",               po::value<double>(&Dlumi)-> default_value(7317))
    ("do1deffs",            po::value<bool>(&do1deffs)-> default_value(true))
    ("do3deffs",            po::value<bool>(&do3deffs)-> default_value(true))
    ("verbosity",           po::value<int>(&verbosity)-> default_value(2))
    ("mjjcut",              po::value<double>(&mjjcut)-> default_value(1100))
    ("metcut",              po::value<double>(&metcut)-> default_value(130))
    ("j2ptcut",             po::value<double>(&j2ptcut)-> default_value(50))
    ("j1ptcut",             po::value<double>(&j1ptcut)-> default_value(50))
    ("dijet_detacut",       po::value<double>(&dijet_detacut)-> default_value(4.2))
    ("dijet_dphicut",       po::value<double>(&dijet_dphicut)-> default_value(1))
    ("cjvcut",              po::value<double>(&cjvcut)-> default_value(1))
    ("l1metcut",            po::value<double>(&l1metcut)-> default_value(0))
    ("l1metprecut",         po::value<double>(&l1metprecut)-> default_value(0));
  

  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  //Load necessary root libraries for custom class I/O
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();
  
  //Define object which keeps track of variable names and files
  TrigEffAnalysis* trigeffana=new TrigEffAnalysis();
  
  //Set up files
  trigeffana->addfile("Run A","SingleMu_SingleMu-2012A-22Jan2013-v1");
  trigeffana->addfile("Run B","SingleMu_SingleMu-2012B-22Jan2013-v1");
  trigeffana->addfile("Run C","SingleMu_SingleMu-2012C-22Jan2013-v1");
  trigeffana->addfile("Run D","SingleMu_SingleMu-2012D-22Jan2013-v1");

  //Set up basic selection
  trigeffana->set_baseselection("jet1_pt>"+boost::lexical_cast<std::string>(j1ptcut)+" && n_jets_cjv_30<"+boost::lexical_cast<std::string>(cjvcut)+" && dijet_dphi<"+boost::lexical_cast<std::string>(dijet_dphicut));

  //Set up trigger information
  Rebinned1DVar *notrig=new Rebinned1DVar("No trigger","","","");
  notrig->binsPushBack(1);
  notrig->binsPushBack(1);
  notrig->binsPushBack(1);
  notrig->binsPushBack(1);

  Rebinned1DVar *prompttrig=new Rebinned1DVar("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets","",""," && passtrigger==1");
  prompttrig->binsPushBack(1);
  prompttrig->binsPushBack(1);
  prompttrig->binsPushBack(1);
  prompttrig->binsPushBack(1);

  Rebinned1DVar *parkedtrig1= new Rebinned1DVar("HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF","",""," && passparkedtrigger1==1");
  parkedtrig1->binsPushBack(0);
  parkedtrig1->binsPushBack(1);
  parkedtrig1->binsPushBack(1);
  parkedtrig1->binsPushBack(1);

  Rebinned1DVar *parkedtrig2= new Rebinned1DVar("HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF","",""," && passparkedtrigger2==1");
  parkedtrig2->binsPushBack(0);
  parkedtrig2->binsPushBack(0);
  parkedtrig2->binsPushBack(0);
  parkedtrig2->binsPushBack(1);

  Rebinned1DVar *l1trig=new Rebinned1DVar("L1ETM40","",""," && l1met>"+boost::lexical_cast<std::string>(l1metcut));
  l1trig->binsPushBack(1);
  l1trig->binsPushBack(1);
  l1trig->binsPushBack(1);
  l1trig->binsPushBack(1);

  std::vector<Rebinned1DVar*> triggers;
  triggers.push_back(notrig);//No trigger must be first
  triggers.push_back(prompttrig);
  triggers.push_back(parkedtrig1);
  triggers.push_back(parkedtrig2);
  triggers.push_back(l1trig);

  TFile *f1[triggers.size()-1];
  TFile *f2;

  //Set up 3D variable
  RebinnedNDVar *multidvariable=new RebinnedNDVar("3D Var","","met:dijet_M:jet2_pt(200,0.,1000.,400,0.,2000.,20,0.,100.)"," && l1met>"+boost::lexical_cast<std::string>(l1metcut)+" && dijet_deta>"+boost::lexical_cast<std::string>(dijet_detacut),3);
  for(int iBin=0;iBin<6;iBin++){
    multidvariable->binsPushBack(0,iBin*40+40);
  }
  multidvariable->binsPushBack(0,400);

  for(int jBin=0;400+jBin*400<=2400;jBin++){
    std::cout<<400+jBin*400<<std::endl;
    multidvariable->binsPushBack(1,400+jBin*400);
  }
  
  for(int kBin=0;10+kBin*20<=100;kBin++){
    multidvariable->binsPushBack(2,10+kBin*20);
  }


  //Set up 1D variables

  //METHLT
  Rebinned1DVar *methlt=new Rebinned1DVar("METHLT","met","met(200,0.,1000.)","&& l1met>"+boost::lexical_cast<std::string>(l1metcut)+" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && dijet_deta>"+boost::lexical_cast<std::string>(dijet_detacut));
  for(int iBin=0;iBin<20;iBin++){
    double newbin;
    newbin=iBin*20;
    methlt->binsPushBack(newbin);
  }

  //MJJHLT
  Rebinned1DVar *mjjhlt=new Rebinned1DVar("MjjHLT","m_{jj}","dijet_M(400,0.,2000.)","&& l1met>"+boost::lexical_cast<std::string>(l1metcut)+" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_deta>"+boost::lexical_cast<std::string>(dijet_detacut));
  for(int iBin=0;iBin<16;iBin++){
    double newbin;
    newbin=600+iBin*50;
    mjjhlt->binsPushBack(newbin);
  }

  //JetHLT
  Rebinned1DVar *jethlt=new Rebinned1DVar("JetHLT","jet_{2} p_{T}","jet2_pt(20,0.,100.)","&& l1met>"+boost::lexical_cast<std::string>(l1metcut)+" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && dijet_deta>"+boost::lexical_cast<std::string>(dijet_detacut));
  for(int iBin=0;iBin<20;iBin++){
    double newbin;
    newbin=iBin*5;
    jethlt->binsPushBack(newbin);
  }

  //METL1
  Rebinned1DVar *metforl1=new Rebinned1DVar("METL1","metnomu","metnomuons(200,0.,1000.)"," && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && dijet_deta>"+boost::lexical_cast<std::string>(dijet_detacut));
  for(int iBin=0;iBin<20;iBin++){
    double newbin=iBin*20;
    metforl1->binsPushBack(newbin);
  }

  //deta
  Rebinned1DVar *deta=new Rebinned1DVar("deta","#Delta#eta_{jj}","dijet_deta(100,0.,10.)","&& l1met>"+boost::lexical_cast<std::string>(l1metcut)+" && met>"+boost::lexical_cast<std::string>(metcut)+" && dijet_M>"+boost::lexical_cast<std::string>(mjjcut)+" && jet2_pt>"+boost::lexical_cast<std::string>(j2ptcut));
  for(int iBin=0;iBin<15;iBin++){
    double newbin;
    if(iBin<10)newbin=3.+iBin*0.1;
    else newbin=4.+(iBin-10)*0.2;
    deta->binsPushBack(newbin);
  }

  std::vector<Rebinned1DVar*> vars;
  vars.push_back(methlt);
  vars.push_back(mjjhlt);
  vars.push_back(jethlt);
  vars.push_back(metforl1);
  vars.push_back(deta);


  //Open Files
  trigeffana->OpenFiles(infolder);

  TH3F heff[triggers.size()][trigeffana->nfiles()];
  TH1F hvar[vars.size()][triggers.size()][trigeffana->nfiles()];

  gStyle->SetPaintTextFormat("3.2f");
  gStyle->SetOptStat("");

  for(unsigned iFile=0;iFile<trigeffana->nfiles();iFile++){
    TTree *tree=trigeffana->GetTree(trigeffana->filenames(iFile));     //Get tree

    //Get 3D histo of met,j2pt,mjj
    if(do3deffs){
      if(verbosity>=1)std::cout<<"Getting 3D Histograms:"<<std::endl;
      for(unsigned iTrigger=0;iTrigger<triggers.size();iTrigger++){
	if(verbosity>=2)std::cout<<"  "<<triggers[iTrigger]->name()<<std::endl;
	heff[iTrigger][iFile]=GetShape3D(multidvariable->variable(),trigeffana->baseselection()+multidvariable->selection()+triggers[iTrigger]->selection(),"","",tree);
	heff[iTrigger][iFile].Sumw2();
      }
    }
    
    //Get 1D histos of met,j2pt,mjj,deta
    if(do1deffs){
      if(verbosity>=1)std::cout<<"Getting 1D Histograms:"<<std::endl;
      for(unsigned iVar=0; iVar<vars.size();iVar++){
	if(verbosity>=1)std::cout<<"  "<<vars[iVar]->name()<<std::endl;
	for(unsigned iTrigger=0;iTrigger<triggers.size();iTrigger++){
	  if(verbosity>=2)std::cout<<"    "<<triggers[iTrigger]->name()<<std::endl;
	  hvar[iVar][iTrigger][iFile]=GetShape(vars[iVar]->variable(),trigeffana->baseselection()+vars[iVar]->selection()+triggers[iTrigger]->selection(),"","",tree);
	  hvar[iVar][iTrigger][iFile].Sumw2();
	}
      }
    }
  }

  if(verbosity>=1)std::cout<<"Calculating and drawing efficiencies"<<std::endl;
  TH3F heffallruns[triggers.size()-1][2];
  EffAndError3D trigeffs3d[triggers.size()-1];
  if(do3deffs){
    TCanvas *c2 = new TCanvas("c2","c2");
    TH2F metbinnedtrigeffs[triggers.size()-1][multidvariable->bins()[0].size()];
    TH2F metbinnedtrigeffstext[triggers.size()-1][multidvariable->bins()[0].size()];
    for(unsigned iTrigger=0;iTrigger<triggers.size()-1;iTrigger++){
      bool empty=true;
      for(unsigned iFile=0;iFile<trigeffana->nfiles();iFile++){
	if(triggers[iTrigger+1]->bins()[iFile]==1){
	  if(verbosity>=3)std::cout<<"Filling trigger "<<triggers[iTrigger+1]->name()<<" for run "<<trigeffana->filenames(iFile)<<std::endl;
	  if(empty==true){
	    heffallruns[iTrigger][0]=heff[0][iFile];
	    heffallruns[iTrigger][1]=heff[iTrigger+1][iFile];
	    empty=false;
	  }
	  else{
	    heffallruns[iTrigger][0].Add(&heff[0][iFile]);
	    heffallruns[iTrigger][1].Add(&heff[iTrigger+1][iFile]);
	  }
	}
      }
      trigeffs3d[iTrigger] = Make3DTrigEff(&heffallruns[iTrigger][0],&heffallruns[iTrigger][1],multidvariable);

      //Make TH2s binned in met
      for(unsigned iBin=0; iBin<trigeffs3d[iTrigger].effs.size();iBin++){
	std::vector<std::vector<double> > bins3d=multidvariable->bins();
	std::string name = triggers[iTrigger+1]->name()+"met"+boost::lexical_cast<std::string>(bins3d[0][iBin])+"trigeff";
	metbinnedtrigeffs[iTrigger][iBin] = TH2F(name.c_str(),name.c_str(),(bins3d[1].size()-1),&bins3d[1][0],(bins3d[2].size()-1),&bins3d[2][0]);
	metbinnedtrigeffstext[iTrigger][iBin] = TH2F(("text"+name).c_str(),("text"+name).c_str(),(bins3d[1].size()-1),&bins3d[1][0],(bins3d[2].size()-1),&bins3d[2][0]);
	for(unsigned jBin=0; jBin<trigeffs3d[iTrigger].effs[0].size();jBin++){
	  for(unsigned kBin=0; kBin<trigeffs3d[iTrigger].effs[0][0].size();kBin++){
	    if(verbosity>=5)std::cout<<"Efficiency for met: "<<bins3d[0][iBin]<<" jet 2 pt: "<<bins3d[2][kBin]<<" mjj: "<<bins3d[1][jBin]<<" is: "<<trigeffs3d[iTrigger].effs[iBin][jBin][kBin]<<"+/-"<<trigeffs3d[iTrigger].errs[iBin][jBin][kBin].first<<std::endl;
	    metbinnedtrigeffs[iTrigger][iBin].SetBinContent(jBin+1,kBin+1,trigeffs3d[iTrigger].effs[iBin][jBin][kBin]);
	    metbinnedtrigeffstext[iTrigger][iBin].SetBinContent(jBin+1,kBin+1,std::max(trigeffs3d[iTrigger].errs[iBin][jBin][kBin].first,trigeffs3d[iTrigger].errs[iBin][jBin][kBin].second));
	  }
	}
	metbinnedtrigeffs[iTrigger][iBin].Draw("colz");
	metbinnedtrigeffstext[iTrigger][iBin].Draw("textsame");
	c2->SaveAs((outfolder+name+".pdf").c_str());
	c2->SaveAs((outfolder+name+".root").c_str());
      }
    }
    
   
  }
  
  

  //sum events over all runs the trigger ran in and get efficiencies
  TH1F hvarallruns[vars.size()][triggers.size()-1][2];
  TGraphAsymmErrors vareffgraph[vars.size()][triggers.size()-1];
  if(do1deffs){
    for(unsigned iVar=0;iVar<vars.size();iVar++){
      if(verbosity>=3)std::cout<<"Examining variable: "<<vars[iVar]->name()<<std::endl;
      for(unsigned iTrigger=0;iTrigger<triggers.size()-1;iTrigger++){
	bool empty=true;
	if(verbosity>=3)std::cout<<"Trigger: "<<triggers[iTrigger+1]->name()<<std::endl;
	for(unsigned iFile=0;iFile<trigeffana->nfiles();iFile++){
	  if(verbosity>=3)std::cout<<"examining file: "<<trigeffana->filenames(iFile)<<std::endl;
	  if(triggers[iTrigger+1]->bins()[iFile]==1){
	    if(verbosity>=3)std::cout<<"File does have this trigger"<<std::endl;
	    if(empty==true){
	      hvarallruns[iVar][iTrigger][0]=hvar[iVar][0][iFile];
	      hvarallruns[iVar][iTrigger][1]=hvar[iVar][iTrigger+1][iFile];
	      empty=false;
	      if(verbosity>=3)std::cout<<"First file filled"<<std::endl;
	    }
	    else{
	      hvarallruns[iVar][iTrigger][0].Add(&hvar[iVar][0][iFile]);
	      hvarallruns[iVar][iTrigger][1].Add(&hvar[iVar][iTrigger+1][iFile]);
	      if(verbosity>=3)std::cout<<"Not first file filled"<<std::endl;

	    }
	  }
	}
	vareffgraph[iVar][iTrigger]=Make1DTrigEff(&hvarallruns[iVar][iTrigger][0],&hvarallruns[iVar][iTrigger][1],vars[iVar]);
      }
    }
  
    TH1F *vareffs[vars.size()][triggers.size()-1];
    for(unsigned iTrigger=0;iTrigger<triggers.size()-1;iTrigger++){
      f1[iTrigger]= new TFile((triggers[iTrigger+1]->name()+"DataMCWeight_53X_v2.root").c_str(),"RECREATE");  
      f1[iTrigger]->cd();
      for(unsigned iVar=0;iVar<vars.size();iVar++){
	std::vector<double> binstmp=vars[iVar]->bins();
	TH1F *histnew=(TH1F*)hvarallruns[iVar][iTrigger][1].Rebin((binstmp.size()-1),"histnew",&binstmp[0]);
	TH1F *histnewpasstrigger=(TH1F*)hvarallruns[iVar][iTrigger][0].Rebin((binstmp.size()-1),"histnewpasstrigger",&binstmp[0]);
	vareffs[iVar][iTrigger]=histnew;
	vareffs[iVar][iTrigger]->SetName(vars[iVar]->namecstr());
	vareffs[iVar][iTrigger]->Divide(histnew,histnewpasstrigger);
	vareffs[iVar][iTrigger]->Write();
      }
      f1[iTrigger]->Write();
    }
    f2=new TFile("DataMCWeight_53X_v2.root","RECREATE");
    f2->cd();
    double totlumi=Alumi+Blumi+Clumi+Dlumi;
    for(unsigned iVar=0;iVar<vars.size()-1;iVar++){
      TH1F *weightedvareff;
      if(iVar!=vars.size()-2){
	std::cout<<"Doing weighted eff for "<<vars[iVar]->name()<<std::endl;
	weightedvareff=vareffs[iVar][0];
	weightedvareff->Add(vareffs[iVar][0],vareffs[iVar][1],Alumi/totlumi,(Blumi+Clumi)/totlumi);
	weightedvareff->Add(vareffs[iVar][2],Dlumi/totlumi);
      }
      else{
	std::cout<<"Writing L1 eff for "<<vars[iVar]->name()<<std::endl;
	weightedvareff=vareffs[iVar][triggers.size()-2];//MET for L1, and L1_ETM40
      }
      weightedvareff->Write();
    }
    f2->Write();
    
        
    //Draw graphs
    int colours[4]={4,6,2,8};
    double markersize[4]={1,0.9,0.8,0.7};
    std::string drawopts[4]={"AP","P","P","P"};
    
    TCanvas *c1 = new TCanvas("c1","c1");
    for(unsigned iVar=0;iVar<vars.size();iVar++){
      c1->Clear();
      c1->SetGrid();    
      vareffgraph[iVar][0].SetTitle("Trigger Efficiency");
      vareffgraph[iVar][0].GetXaxis()->SetTitle((vars[iVar]->latex()+"/GeV").c_str());
      vareffgraph[iVar][0].GetYaxis()->SetTitle("Efficiency");
      TLegend* varleg=new TLegend(0.45,0.15,0.9,0.35);
      
      for(unsigned iTrigger=0;iTrigger<triggers.size()-1;iTrigger++){
	vareffgraph[iVar][iTrigger].SetMarkerColor(colours[iTrigger]);                                                                  
	vareffgraph[iVar][iTrigger].SetLineColor(colours[iTrigger]);
	vareffgraph[iVar][iTrigger].SetMarkerStyle(20);
	vareffgraph[iVar][iTrigger].SetMarkerSize(markersize[iTrigger]);
	vareffgraph[iVar][iTrigger].Draw(drawopts[iTrigger].c_str());
	
	varleg->AddEntry(&vareffgraph[iVar][iTrigger],triggers[iTrigger+1]->namecstr(),"p");
      }
      varleg->Draw("same");
      c1->SaveAs((outfolder+vars[iVar]->name()+"efficiency.pdf").c_str());
      c1->SaveAs((outfolder+vars[iVar]->name()+"efficiency.root").c_str());
    }
  }
  return 0;
}
