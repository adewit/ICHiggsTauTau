#ifndef ICHiggsTauTau_HiggsNuNu_MVAApply_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_MVAApply_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>

namespace ic {

  class MVAApply : public LTModule{
    CLASS_MEMBER(MVAApply,std::vector<std::string>,sets)
    CLASS_MEMBER(MVAApply,std::vector<std::string>,variables)
    CLASS_MEMBER(MVAApply,std::vector<std::string>,methodNames)
    CLASS_MEMBER(MVAApply,std::vector<std::string>,weightFiles)
    CLASS_MEMBER(MVAApply,std::string,weightDir)
    CLASS_MEMBER(MVAApply,std::string,friendDir)
    CLASS_MEMBER(MVAApply,bool,setorfile)
  public:
    MVAApply(std::string);
    virtual ~MVAApply();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
