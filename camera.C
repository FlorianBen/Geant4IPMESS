void camera(){
  auto canv = new TCanvas("canv1", "Camera", 1920, 1280);
  canv->Divide(2,1);
  TFile *f = new TFile("camera_hist.root", "read");
  TTree *t1 = (TTree *)f->Get("Camera0");
  TTree *t2 = (TTree *)f->Get("Camera1");
  canv->cd(1);
  t1->Draw("COLZ");
  canv->cd(2);
  t2->Draw("COLZ");
}