

const double qcut  = 3e4;
const double pcut  = 2500;
//const double rmscut  = 2.4;
const double rmscut  = 2.6;
//const double qcut2qcut  = 2.5e4;
//const double pcut  = 2000;
const double xmin  = -22.5;
const double xmax  = 22.5.;
const int    xnbin = 50.;
const double ymin  = -22.5;
const double ymax  = 22.5;
const int    ynbin = 50.;
const double qmin  = 0; 
const double qmax  = 8e4; 
const int    qnbin = 80; 
const double pmin  = 0; 
const double pmax  = 6e3; 
const int    pnbin = 60; 
TFile* fF;
TTree* fT;
const int nh = 2;
TH2F*  fH [nh];
const string fHname[nh] = {"fH0","fH1"};
#define XY 0
#define PQ 1

const string draw[nh] = {"yc_p*42./510.-22.5:xc_p*42./510.-22.5", "integ_p:height_p"};
//const string sel[nh]  = { Form("integ_p>%lf&&height_p>%lf", qcut,pcut), ""};
const string sel[nh]  = { Form("integ_p>%lf&&height_p>%lf&&yvar_rot<%lf&&sigma1>4", qcut,pcut,rmscut), ""};

const int nh1 = 2;
TH1F*  fH1 [nh];
const string fH1name[nh] = {"fH1_0","fH1_1"};
#define P 0
#define Q 1
const string draw1[nh] = {"height_p", "integ_p"};
//const string sel1[nh]  = { "height_p>500&&yvar_rot<2.4", "integ_p>15000&&yvar_rot<2.4"};
//const string sel1[nh]  = { "yvar_rot<2.4&&sigma1>4", "yvar_rot<2.4&&sigma1>4"};
const string sel1[nh]  = { Form("yvar_rot<%lf&&sigma1>4",rmscut),  Form("yvar_rot<%lf&&sigma1>4",rmscut) }; 
const int    nbins[nh] = {40, 40};
TCanvas* c1;
int nspill;
void macro_171217_ccddraw(string input){
  fF = new TFile(input.c_str(), "read");
  fT = (TTree*)fF->Get("cluster");

  //time
  nspill = fT->GetEntries();
  double tdaq = 1.0*(nspill/25)/60/60;
  cout << tdaq << " hours" << endl;

  for(int i=0; i<nh; i++){
    if(i == XY){
      fH[i] = new TH2F(fHname[i].c_str(), "", xnbin, xmin, xmax, ynbin, ymin, ymax ); 
      fH[i]->SetXTitle("x [mm]");
      fH[i]->SetYTitle("y [mm]");
    }
    else if(i == PQ){
      fH[i] = new TH2F(fHname[i].c_str(), "", pnbin, pmin, pmax, qnbin, qmin, qmax );
      fH[i] -> SetXTitle("height_p");
      fH[i] -> SetYTitle("integ_p");
    }
    fT->Project(fHname[i].c_str(), draw[i].c_str(), sel[i].c_str());
  }//i
  for(int i=0; i<nh1; i++){
    if(i == P){
      fH1[i] = new TH1F(fH1name[i].c_str(), "", nbins[i], pmin, pmax);
      fH1[i] -> SetXTitle("height_p");
    }
    if(i == Q){
      fH1[i] = new TH1F(fH1name[i].c_str(), "", nbins[i], qmin, qmax);
      fH1[i] -> SetXTitle("integ_p");
    }
    fT->Project(fH1name[i].c_str(), draw1[i].c_str(), sel1[i].c_str());
  }//i

  //gStyle -> SetOptStat(0000);
  c1 = new TCanvas("c1","",10,10,1000,1000);
  c1 -> Divide(2,2);
  c1 -> cd(1);
  TEllipse* el = new TEllipse(0,0,20,20);
  el -> SetLineColor(1);
  el -> SetLineStyle(2);
  el -> SetFillColorAlpha(0,1);

  fH[XY] -> SetTitle(Form("%.2lf hours ( %d trigger)",tdaq,nspill));

  fH[XY] -> Draw("colz");
  //el -> Draw("same");
  fH[XY] -> Draw("colz:same");
  c1 -> cd(2);
  fH1[P]  -> Draw();
  c1 -> cd(3);
  fH1[Q]  -> Draw();
  c1 -> cd(4);
  fH[PQ] -> Draw("colz");

  /*
  TLine* a1 = new TLine(0,    qcut, pcut, qcut); 
  TLine* a2 = new TLine(pcut, 0,    pcut, qcut); 
  */
  TLine* a1 = new TLine(pcut, qcut, pmax, qcut); 
  TLine* a2 = new TLine(pcut, qcut, pcut, qmax); 
  a1 -> SetLineColor(2);
  a2 -> SetLineColor(2);
  a1 -> SetLineWidth(2);
  a2 -> SetLineWidth(2);
  a1 -> SetLineStyle(2);
  a2 -> SetLineStyle(2);
  a1 -> Draw();
  a2 -> Draw();
}
