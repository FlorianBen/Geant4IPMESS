void camera(){
  auto canv = new TCanvas("canv1", "Camera", 1920, 1280);
  canv->Divide(2,1);
  TFile *f = new TFile("camera.root", "read");
  TTree *t1 = (TTree *)f->Get("Layer0");
  TTree *t2 = (TTree *)f->Get("Layer1");
  canv->cd(1);
  t1->Draw("BC");
  canv->cd(2);
  t2->Draw("BC");
}