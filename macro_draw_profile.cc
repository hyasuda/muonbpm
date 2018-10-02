int range = 10;
TFile* fF;
TTree* fT;
void macro_draw_profile(string input, int evt, int a, int b){
  fF = new TFile(input.c_str(), "read");
  fT = (TTree*)(fF->Get("tree"));
  TH2S* fH2;
  fT -> SetBranchAddress("t1",&fH2);
  fT -> GetEntry(evt);
  //int a,b,c;
  //fH2->GetMaximumBin(a,b,c);
  //int max = fH2->GetBinContent(a,b);
  
  //cout << "# Xbin : " << fH2 -> GetNbinsX() << endl;
  //cout << "# Ybin : " << fH2 -> GetNbinsY() << endl;
  //fH2 -> GetXaxis() -> SetRangeUser(a-range,a+range);
  //fH2 -> GetYaxis() -> SetRangeUser(b-range,b+range);
  //fH2 -> GetXaxis() -> SetRangeUser(0,600);
  //fH2 -> GetYaxis() -> SetRangeUser(0,550);
  //cout << "maximum = " << max << "\t@ " << a << "\t" << b << endl;
  //fH2 -> GetZaxis() -> SetRangeUser(100,1.0*max);
  //fH2 -> GetZaxis() -> SetRangeUser(100,1.0*1351);
  //fH2->Draw("lego2Z");
  fH2->Draw("colz");
}
