
{
  gSystem->Load( "libFWCoreFWLite" );
  gSystem->Load("libDataFormatsFWLite");
  AutoLibraryLoader::enable();

  //new TFile("dummyfile_numEvent1.root");
  //new TFile("../B8042BB7-760E-DF11-A721-00261894398B.root");
  new TFile("rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/Feb9ReReco_v2/0025/F68A5ABB-E515-DF11-8A77-0026189437F0.root"); //open dummy file to for automatic loading of necessary libraries

}




