
{
  gSystem->Load( "libFWCoreFWLite" );
  gSystem->Load("libDataFormatsFWLite");
  AutoLibraryLoader::enable();
 
  //open dummy file to for automatic loading of necessary libraries
  new TFile("./trkAnaSkimAOD.root");

}




