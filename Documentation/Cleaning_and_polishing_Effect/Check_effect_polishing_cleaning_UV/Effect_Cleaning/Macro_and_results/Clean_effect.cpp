// modified by Marcos. 31th July 2017
//This macro open, read and plot a file where there are dates of the histogram, fit this histogram a line plus sum
//of the gaussians function and finally obtein the center of the gaussians ans plot this center vs number of pixels
//for can obtein the gain of the sistem (the gain depend of the temperature and operational voltage)

#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVirtualFitter.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"

#include <iostream>
#include <fstream>

using namespace std; //en C++11/root6 es obligatorio

//I define this function for config. all histograms
void format_h(TH1F* h, int linecolor){
    h->SetLineWidth(2);
    h->SetLineColor(linecolor);
}

void format_line(TAttLine* line, int col, int sty)
{
    line->SetLineWidth(5);
    line->SetLineColor(col);
    line->SetLineStyle(sty);
}

void Clean_effect()
{
    gROOT->SetStyle("Plain");

    //gStyle->SetOptFit(0001);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(1);

    Int_t channel=0;
    Double_t noe=0;
    Int_t noc=8192;

    Int_t i=0;

    Double_t act_s=0;
    Double_t act_b=0;
    Double_t act_neta=0;
    Double_t bincont=0;

    //Time of the measurement for the first test
    //No Cleaning
    //Double_t time_b = 91627.325000; //Time for "Background.txt"
    //Double_t time_s = 12859.890000; //Time for "Cs_137_departamento_medida_corta"
    //Double_t time_s = 80645.316000; //Time for "Cs_137_departamento_medida_larga"
    //Double_t time_s = 12479.989000; //Time for "Sr_90_departamento"

    //Cleaning
    //Double_t time_b = 186741.863000; //Time for "Background.txt"
    //Double_t time_s = 21210.116000; //Time for "Cs_137_departamento"
    //Double_t time_s = 12324.807000; //Time for "Sr_90_departamento"

    //Time of the measurement for the second test
    //No Cleaning
    Double_t time_b = 84009.224000; //Time for "Background.txt"
    //Double_t time_b = 101854.305000; //Time for "Background_Llarga.txt"
    //Double_t time_b = 14344.034000; //Time for "Fuente_Cs137.txt"
    //Double_t time_b = 51675.361000; //Time for "Fuente_Sr90.mcs"

    //Cleaning
    Double_t time_s = 162781.997000; //Time for "Background.txt"
    //Double_t time_s = 71092.414000; //Time for "Cs_medida_corta.txt"
    //Double_t time_s = 171304.649000; //Time for "Cs_medida_larga.mca"
    //Double_t time_s = 67337.009000; //Time for "Sr_medida_corta.txt"
    //Double_t time_s = 112308.832000; //Time for "Sr_medida_larga.txt"



    //Parameter of the signal histogram
    Double_t Hist_Min_S = 0.;
    Double_t Hist_Max_S = 8192;
    Int_t Hist_bin_S = 8192;

    //Parameter of the background histogram
    Double_t Hist_Min_B = 0.;
    Double_t Hist_Max_B = 8192;
    Int_t Hist_bin_B = 8192;

    //Parameter of the rest histogram
    Double_t Hist_Min_D = 0.;
    Double_t Hist_Max_D = 8192;
    Int_t Hist_bin_D = 8192;

    //I define the signal and background histogram
    TH1F *s= new TH1F("S","S",Hist_bin_S,Hist_Min_S,Hist_Max_S);
    TH1F *b= new TH1F("B","B",Hist_bin_B,Hist_Min_B,Hist_Max_B);
    TH1F *div= new TH1F("D","D",Hist_bin_D,Hist_Min_D,Hist_Max_D);



    //I put title for the axis in signal histogram
    s->GetXaxis()->SetTitle("ADC channels");
    s->GetYaxis()->SetTitle("Activity (Bq)");
    s->GetXaxis()->SetLabelSize(0.045);
    s->GetYaxis()->SetLabelSize(0.045);
    s->GetXaxis()->SetTitleSize(0.055);
    s->GetYaxis()->SetTitleSize(0.055);
    s->GetYaxis()->SetTitleOffset(1.15);
    s->GetXaxis()->SetTitleOffset(1);
    //s->GetYaxis()->SetRangeUser(0,0.1);



    //I put title for the axis in background histogram
    b->GetXaxis()->SetTitle("ADC channels");
    b->GetYaxis()->SetTitle("Activity (Bq)");
    b->GetXaxis()->SetLabelSize(0.045);
    b->GetYaxis()->SetLabelSize(0.045);
    b->GetXaxis()->SetTitleSize(0.055);
    b->GetYaxis()->SetTitleSize(0.055);
    b->GetYaxis()->SetTitleOffset(1.15);
    b->GetXaxis()->SetTitleOffset(1);
    //b->GetYaxis()->SetRangeUser(0,0.1);

    //Format of the histogram
    format_h(s,kBlue);
    format_h(b,kRed);
    format_h(div,kGreen);




    //I define a vector for I can read the file.
    ifstream sig, bac;

    //I read the .txt file
    sig.open("Second_test/Cleaning/Background.txt");

    //(The first value of the channel is zero so I have to read the file in this way)

    //First I read the signal histogram

    cout << endl;
    cout << "The data of the signal is: " << endl;



    for(i=0; i<noc; i++){
        sig >> noe;



        //cout << endl;
        //cout << "Channel: " << channel << endl;
        //cout << "Number of events: " << noe << endl;

        //I normalized the histogram
        noe=noe/time_s;
        //cout << "Number of normalized events are: " << noe << endl;

        //cout << endl;
        if(channel > 400){
        act_s += noe;}

        s->SetBinContent(channel ,noe);

        channel=channel+1;

    }

    sig.close();




    //I read the .txt file
    bac.open("Second_test/No_cleaning/Background.txt");


    //Second I read the background histogram

    cout << endl;
    cout << "The data of the background is: " << endl;

    channel=0;

    for(i=0; i<noc; i++){
        bac >> noe;

        //cout << endl;
        //cout << "Channel: " << channel << endl;
        //cout << "Number of events: " << noe << endl;

        //I normalized the histogram
        noe=noe/time_b;
        if(channel > 400){
        act_b += noe;}
        //cout << "Number of normalized events are: " << noe << endl;

        //cout << endl;

        b->SetBinContent(channel ,noe);

        channel=channel+1;

    }


    bac.close();

    //s->Rebin(3);
    //s->GetXaxis()->SetRangeUser(200,1000);
    //s->GetYaxis()->SetRangeUser(0,0.4);
    //===============

    //======================

    //s->Rebin(4);
    //b->Rebin(4);


    TCanvas *c1 = new TCanvas("c1"," Measurements ",20,10,800,600);
    c1->SetFillColor(0);
    c1->SetGrid();
    c1->GetFrame()->SetFillColor(21);
    c1->GetFrame()->SetBorderSize(112);

    //gPad->SetRightMargin(0.05);
    gPad->SetRightMargin(0.08);
    gPad->SetLeftMargin(0.13);

    //gPad->SetBottomMargin(0.05);
    gPad->SetBottomMargin(0.12);
    gPad->SetTopMargin(0.08);

    s->Draw("Same");
    b->Draw("Same");

    //I put a legend in TCanvas 1
    TLegend *leg= new TLegend(.1 ,.7 ,.3 ,.9 ,"Tritium");
    leg->SetFillColor(0);
    s->SetFillColor(0);
    leg->AddEntry(s,"Background Cleaning");
    leg->AddEntry(b,"Background no Cleaning");
    leg->Draw("Same");

    //I put title for the axis in signal tritium
    div->GetXaxis()->SetTitle("ADC channels");
    div->GetYaxis()->SetTitle("Activity (Bq)");
    div->GetXaxis()->SetLabelSize(0.045);
    div->GetYaxis()->SetLabelSize(0.045);
    div->GetXaxis()->SetTitleSize(0.055);
    div->GetYaxis()->SetTitleSize(0.055);
    div->GetYaxis()->SetTitleOffset(1.15);
    div->GetXaxis()->SetTitleOffset(1);

    //div->Rebin(4);

    //Now I are going to divide this histogram
    div->Add(s,b,1.,-1.);


    for(i=400; i<Hist_Max_D; i++)
    {
        bincont = div->GetBinContent(i);
        if(bincont>0)
        {
        act_neta += bincont;
        //cout << bincont << endl;
        }
    }

        cout << endl << endl << endl;
        cout << "El número de cuentas por segundo de la señal es: " << act_s << endl << endl << endl;
        cout << "La número de cuentas por segundo del background es: " << act_b << endl << endl << endl;
        cout << "La número de cuentas por segundo de la fuente es: " << act_neta << endl << endl << endl;


    s->Rebin(4);
    b->Rebin(4);
    div->Rebin(4);

    TF1 *f1 = new TF1("Landau","landau",300,8192);
    //f1->SetParameters(0.2,1.3);
    //f1->Draw();
    format_line(f1,kBlack,2);

    TCanvas *c2 = new TCanvas("c2","Tritium Signal ",20,10,800,600);
    c2->SetFillColor(0);
    c2->SetGrid();
    c2->GetFrame()->SetFillColor(21);
    c2->GetFrame()->SetBorderSize(112);

    //gPad->SetRightMargin(0.05);
    gPad->SetRightMargin(0.08);
    gPad->SetLeftMargin(0.13);

    //gPad->SetBottomMargin(0.05);
    gPad->SetBottomMargin(0.12);
    gPad->SetTopMargin(0.08);

    div->Fit(f1);
    TVirtualFitter *fit = TVirtualFitter::GetFitter();

    div->Draw("Same");
    //f1->Draw("Same");

    //I put a legend in TCanvas 2
    TLegend *leg2= new TLegend(.1 ,.7 ,.3 ,.9 ,"Tritium");
    leg2->SetFillColor(0);
    div->SetFillColor(0);
    leg2->AddEntry(div,"Effect to the cleaning process in the Background");
    leg2->Draw("Same");





    //return 0;

}

