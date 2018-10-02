const double xmin  = -22.5;
const double xmax  = 22.5.;
const double xnbin = 50.;
//const double xnbin = 12.;
const double ymin  = -22.5;
const double ymax  = 22.5;
const double ynbin = 50.;
//const double ynbin = 12.;
TCanvas*c1;
const double phmin = 0;
const double phmax = 3000;
const double phnbin = 50;

const double integmin = 10000;
const double integmax = 60000;
const double integnbin = 50;

const string draw_pf  = "(yc_p*42./510.-22.5):(xc_p*42./510.-22.5)";
const string select_pf ="integ_p>15000";
//const string select_pf ="integ_p>13000";//180926 by Nakazawa
// const string select_pf ="integ_p>20000&&height_p>700";
const string fH_Name_pf = "profile";

const string draw_ph  = "height_p";
const string select_ph = "integ_p>15000";
const string fH_Name_ph = "PH";

const string draw_integ  = "integ_p.";
const string select_integ ="integ_p>15000";
const string fH_Name_integ = "integ";

const string draw_ph_integ  = "integ_p:height_p.";
const string select_ph_integ ="integ_p>15000 && height_p>100";
//const string select_ph_integ ="integ_p>1000";//180926 by Nakazawa
const string fH_Name_ph_integ = "height_p:integ";

void quickcheck_rev1(string input_filename){

  TFile* fFin  = new TFile(input_filename.c_str(),"read");
  TTree* fTin = (TTree*) (fFin->Get("cluster"));

  TH2F* h_profile = new TH2F(fH_Name_pf.c_str(), Form("%s;x[mm];y[mm]",input_filename.c_str()),xnbin,xmin,xmax,ynbin,ymin,ymax);
  TH1F* h_ph = new TH1F(fH_Name_ph.c_str(), Form("%s;PH[ADC count];#evt",input_filename.c_str()),phnbin,phmin,phmax);
  TH1F* h_integ = new TH1F(fH_Name_integ.c_str(), Form("%s;integ[ADC count];#evt",input_filename.c_str()),integnbin,integmin,integmax);
  TH2F* h_ph_integ = new TH2F(fH_Name_ph_integ.c_str(), Form("%s;PH[ADC count];integ[ADC count]",input_filename.c_str()),phnbin,phmin,phmax,integnbin,integmin,integmax);

  fTin->Project(fH_Name_pf.c_str(),draw_pf.c_str(),select_pf.c_str());
  fTin->Project(fH_Name_ph.c_str(),draw_ph.c_str(),select_ph.c_str());
  fTin->Project(fH_Name_integ.c_str(),draw_integ.c_str(),select_integ.c_str());
  fTin->Project(fH_Name_ph_integ.c_str(),draw_ph_integ.c_str(),select_ph_integ.c_str());


  const double th_q = 15000;
  const double th_ph = 1000;
  const string draw_pf_org  = "yc_p*42./510.-22.5:xc_p*42./510.-22.5";

  const string  fH_Name_pf_org = "profile org";
  ////////////
  TH2F* h_profile_org = new TH2F(fH_Name_pf_org.c_str(), Form("%s;x[mm];y[mm]",input_filename.c_str()),xnbin,xmin,xmax,ynbin,ymin,ymax);
  TH1F* h_x = new TH1F("h_x","x projection;x[mm];#evt",xnbin,xmin,xmax);
  TH1F* h_x_org = new TH1F("h_x_org","x project_org;x[mm];#evt",xnbin,xmin,xmax);
  TH1F* h_y = new TH1F("h_y","y projection;y[mm];#evt",ynbin,ymin,ymax);
  TH1F* h_y_org = new TH1F("h_y_org","y project_org;y[mm];#evt",ynbin,ymin,ymax);

  string select_pf_org = Form("integ_p>%l&&height_p>%lf",th_q, th_ph);  
  fTin->Project(fH_Name_pf_org.c_str(),draw_pf_org.c_str(),select_pf_org.c_str());
  fTin->Project("h_x_org","xc_p*42./510.-22.5",select_pf_org.c_str());
  fTin->Project("h_y_org","yc_p*42./510.-22.5",select_pf_org.c_str());  

  //x projection
  double evt_cur_bin;
  double evt_prev_bin;
  double avail_nbin[xnbin] = {0};
  double cur_ybinmin;
  double cur_ybinmax;
  int    ybinmin_flag;
  double cur_bin_evtsum;

  for( int ix = 1 ; ix <= h_profile_org->GetXaxis()->GetNbins() ; ix++ ){

      cur_ybinmin = 1;
      cur_ybinmax = 1;
      ybinmin_flag = 0;
      cur_bin_evtsum = 0;
    for( int iy = 1 ; iy <= h_profile_org->GetYaxis()->GetNbins() ; iy++ ){

      evt_cur_bin  = h_profile_org->GetBinContent(h_profile_org->GetBin(ix,iy));
      evt_prev_bin = h_profile_org->GetBinContent(h_profile_org->GetBin(ix,iy-1));
      cur_bin_evtsum += evt_cur_bin;

#ifdef DEBUG
      if(iy==1){
	double  xc = h_profile_org->GetXaxis()->GetBinCenter(ix);
	cout<<"xc = "<<xc<<endl;
      }//if
      cout<<" ix = "<<ix<<" iy = "<<iy<<" evt_cur_bin = "<<evt_cur_bin<<endl;
#endif
      
      //minumum bin
      if(evt_cur_bin>0&&ybinmin_flag==0){
	cur_ybinmin = iy;
	ybinmin_flag = 1;
      }//if

      //maximum bin
      if(evt_cur_bin==0&&evt_prev_bin>0){
	cur_ybinmax = iy-1;
      }else if(iy==ynbin&&evt_cur_bin>0&&evt_prev_bin>0){
	cur_ybinmax = iy;
      }//if

    }//for iy

    if(!(cur_ybinmin==cur_ybinmax)){
      avail_nbin[ix-1] = (cur_ybinmax - cur_ybinmin + 1);
    }else{
      avail_nbin[ix-1] = 1;
    }
#ifdef DEBUG
    cout<<" cur_ybinmin = "<<cur_ybinmin<<" cur_ybinmax = "<<cur_ybinmax<<endl;
    cout<<" avail_nbin[ix-1] "<<avail_nbin[ix-1]<<endl;    
    cout<<" cur_bin_evtsum = "<<cur_bin_evtsum<<endl;
    cout<<" Fill "<<h_profile_org->GetXaxis()->GetBinCenter(ix)<<" "<<cur_bin_evtsum/avail_nbin[ix-1]<<endl;
#endif
    //    h_x->Fill( h_profile_org->GetXaxis()->GetBinCenter(ix), cur_bin_evtsum/avail_nbin[ix-1] );

    h_x->SetBinContent( ix, cur_bin_evtsum/avail_nbin[ix-1] );
    h_x->SetBinError( ix, sqrt(cur_bin_evtsum)/avail_nbin[ix-1] );
    
  }//for ix

  //y projection
  double yevt_cur_bin;
  double yevt_prev_bin;
  double yavail_nbin[ynbin] = {0};
  double cur_xbinmin;
  double cur_xbinmax;
  int    xbinmin_flag;
  double ycur_bin_evtsum;

  for( int iy = 1 ; iy <= h_profile_org->GetYaxis()->GetNbins() ; iy++ ){

      cur_xbinmin = 1;
      cur_xbinmax = 1;
      xbinmin_flag = 0;
      ycur_bin_evtsum = 0;
    for( int ix = 1 ; ix <= h_profile_org->GetXaxis()->GetNbins() ; ix++ ){

      yevt_cur_bin  = h_profile_org->GetBinContent(h_profile_org->GetBin(ix,iy));
      yevt_prev_bin = h_profile_org->GetBinContent(h_profile_org->GetBin(ix-1,iy));
      ycur_bin_evtsum += yevt_cur_bin;

#ifdef DEBUG
      if(ix==1){
	double  yc = h_profile_org->GetYaxis()->GetBinCenter(iy);
	cout<<"yc = "<<yc<<endl;
      }//if
      cout<<" ix = "<<ix<<" iy = "<<iy<<" yevt_cur_bin = "<<yevt_cur_bin<<endl;
#endif
      
      //minumum bin
      if(yevt_cur_bin>0&&xbinmin_flag==0){
	cur_xbinmin = ix;
	xbinmin_flag = 1;
      }//if

      //maximum bin
      if(yevt_cur_bin==0&&yevt_prev_bin>0){
	cur_xbinmax = ix-1;
      }else if(ix==xnbin&&yevt_cur_bin>0&&yevt_prev_bin>0){
	cur_xbinmax = ix;
      }//if

    }//for iy

    if(!(cur_xbinmin==cur_xbinmax)){
      yavail_nbin[iy-1] = (cur_xbinmax - cur_xbinmin + 1);
    }else{
      yavail_nbin[iy-1] = 1;
    }
#ifdef DEBUG
    cout<<" cur_xbinmin = "<<cur_xbinmin<<" cur_xbinmax = "<<cur_xbinmax<<endl;
    cout<<" yavail_nbin[ix-1] "<<yavail_nbin[iy-1]<<endl;    
    cout<<" ycur_bin_evtsum = "<<ycur_bin_evtsum<<endl;
    cout<<" Fill "<<h_profile_org->GetYaxis()->GetBinCenter(iy)<<" "<<ycur_bin_evtsum/yavail_nbin[iy-1]<<endl;
#endif
    //    h_y->Fill( h_profile_org->GetYaxis()->GetBinCenter(iy), ycur_bin_evtsum );
    //    h_y->Fill( h_profile_org->GetYaxis()->GetBinCenter(iy), ycur_bin_evtsum/yavail_nbin[iy-1] );

    h_y->SetBinContent( iy, ycur_bin_evtsum/yavail_nbin[iy-1] );
    h_y->SetBinError( iy, sqrt(ycur_bin_evtsum)/yavail_nbin[iy-1] );

  }//for ix


  ////////////////////////////

  gStyle->SetOptStat(111111);
  if(!c1)
    //c1 = new TCanvas("c1",input_filename.c_str(),10,80,700,900);
    c1 = new TCanvas("c1","");
  c1->Clear();
  c1->Divide(2,3);
  c1->cd(1);
  h_profile->Draw("COLZ");
  c1->cd(2);
  h_ph->Draw("");
  c1->cd(3);
  h_integ->Draw("");
  c1->cd(4);
  h_ph_integ->Draw("COLZ");
  c1->cd(5);
  h_x->Draw("E");
  c1->cd(6);
  h_y->Draw("E");

  cout<<"ENTRY "<<h_profile->GetEntries()<<endl;
 
  double meanx = h_profile->GetMean(1);
  double meany = h_profile->GetMean(2);
  double RMSx = h_profile->GetRMS(1);
  double RMSy = h_profile->GetRMS(2);
 
  int cnt = h_profile -> GetEntries();
  

  ofstream outfile ("output.dat",ios::app);
  outfile << meanx  << "\t" << meany << "\t" << RMSx << "\t" << RMSy << "\t" << endl;


}
