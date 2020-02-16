// ROOT macro to read the IceCube public 3yr PS data in ROOT format
// and produce a skymap from it.
//-- Nick van Eijndhoven, IIHE-VUB Brussel, December 10, 2019  15:20
{
 gSystem->Load("ncfspack");

 NcAstrolab lab;
 lab.SetExperiment("IceCube");
 lab.Data();

 TChain data("T");
 data.Add("IceCube-PS-event-data*.root");

 Int_t nen=data.GetEntries();
 cout << " Number of entries in the input file : " << nen << endl;
 nen=100000; // Limit the number of events for the skymap
 cout << " Number of entries that will be processed : " << nen << endl;

 Double_t mjd,ra,decl;
 NcTimestamp ts;
 TString name;
 TLeaf* lx=0;
 Double_t mjdfirst=-1;
 Double_t mjdlast=-1;
 NcTimestamp tstart,tend;
 Float_t deltat=0;
 Float_t rate=0;
 Int_t nevt=0;
 for (Int_t i=1; i<=nen; i++)
 {
  data.GetEntry(i-1);

  mjd=-999;
  ra=-999;
  decl=-999;
  
  lx=data.GetLeaf("mjd");
  if (lx) mjd=lx->GetValue();
  lx=data.GetLeaf("ra");
  if (lx) ra=lx->GetValue();
  lx=data.GetLeaf("decl");
  if (lx) decl=lx->GetValue();

  if (mjdfirst<0 || mjd<mjdfirst) mjdfirst=mjd;
  if (mjd>mjdlast) mjdlast=mjd;

  if (mjd>-999 && ra>-999 & decl>=-5)
  {
   ts.SetMJD(mjd);
   name="Event";
   name+=i;
   lab.SetSignal(1,ra,"deg",decl,"deg","equ",&ts,-1,"J",name,1);

   nevt++;
  }
 }

 tstart.SetMJD(mjdfirst);
 tend.SetMJD(mjdlast);

 cout << endl;
 cout << " *** Timestamp of first observed event in the total sample ***" << endl;
 tstart.Date();

 cout << endl;
 cout << " *** Timestamp of last observed event in the total sample ***" << endl;
 tend.Date();

 deltat=tstart.GetDifference(tend,"s");
 cout << endl;
 cout << " *** Elapsed time interval : " << (mjdlast-mjdfirst) << " days --> " << deltat << " seconds" << endl;

 if (deltat) rate=float(nevt)/deltat;
 rate*=1000.; // Event rate in mHz
 cout << endl;
 cout << " *** Number of events for this  selection : " << nevt << " --> Event rate " << rate << " mHz" << endl;

 lab.ListSignals("equ","J",1);
 lab.SetMarkerSize(0.1,1);
 lab.SetMarkerSize(0,2);
 lab.DisplaySignals("equ","J",0);
}
