#include "SimCalorimetry/CastorSim/plugins/CastorDigiAnalyzer.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include<iostream>


CastorDigiAnalyzer::CastorDigiAnalyzer(edm::ParameterSet const& conf) 
: hitReadoutName_("CASTORHits"),
  simParameterMap_(),
  castorHitAnalyzer_("CASTORDigi", 1., &simParameterMap_, &castorFilter_),
  castorDigiStatistics_("CASTORDigi", 3, 10., 6., 0.1, 0.5, castorHitAnalyzer_)
{
}


namespace CastorDigiAnalyzerImpl {
  template<class Collection>
  void analyze(edm::Event const& e, CastorDigiStatistics & statistics) {
    edm::Handle<Collection> digis;
    e.getByType(digis);
    if (!digis.isValid()) {
      edm::LogError("CastorDigiAnalyzer") << "Could not find Castor Digi Container ";
    } else {
      for(unsigned i = 0; i < digis->size(); ++i) {
        statistics.analyze((*digis)[i]);
      }
    }
  }
}


void CastorDigiAnalyzer::analyze(edm::Event const& e, edm::EventSetup const& c) {
  edm::Handle<edm::PCaloHitContainer> hits;
  hitReadoutName_ = "CASTORHITS";
  e.getByLabel("g4SimHits",hitReadoutName_, hits);
  if (hits.isValid()) {
    castorHitAnalyzer_.fillHits(*hits);
    CastorDigiAnalyzerImpl::analyze<CastorDigiCollection>(e, castorDigiStatistics_);
  }
}