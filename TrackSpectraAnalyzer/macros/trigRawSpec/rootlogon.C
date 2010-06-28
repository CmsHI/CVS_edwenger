{
  // include paths
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");

  // helper classes
  gROOT->Macro("$CMSSW_BASE/src/edwenger/TrackSpectraAnalyzer/macros/trigRawSpec/CPlot.cc+");

  // styles
  gROOT->Macro("$CMSSW_BASE/src/edwenger/TrackSpectraAnalyzer/macros/trigRawSpec/rootlogon_dNdEtaBase.C");
  //gStyle->SetOptStat(1111);
}
