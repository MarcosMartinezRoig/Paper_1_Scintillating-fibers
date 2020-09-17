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

void Tritium()
{
    gROOT->SetStyle("Plain");

    //gStyle->SetOptFit(0001);
    gStyle->SetOptStat(111111111);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(1);

    Int_t channel=0;
    Int_t i=0;
    Double_t noe=0;
    Int_t noc=8192;

    Double_t act=0;
    Double_t bincont=0;

    //Time of the measurement
    Double_t time_s = 51972.205000;
    Double_t time_b = 236296.618000;

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
    sig.open("Cs_137_fuente_electrodepoitada_rota.txt");

    //(The first value of the channel is zero so I have to read the file in this way)

    //First I read the signal histogram

    cout << endl;
    cout << "The data of the signal is: " << endl;



    for(i=0; i<noc; i++){
        sig >> noe;



        cout << endl;
        cout << "Channel: " << channel << endl;
        cout << "Number of events: " << noe << endl;

        //I normalized the histogram
        noe=noe/time_s;
        //cout << "Number of normalized events are: " << noe << endl;

        //cout << endl;

        s->SetBinContent(channel ,noe);

        channel=channel+1;

    }

    sig.close();




    //I read the .txt file
    bac.open("1.txt");


    //Second I read the background histogram

    cout << endl;
    cout << "The data of the background is: " << endl;

    channel=0;

    for(i=0; i<noc; i++){
        bac >> noe;

        cout << endl;
        cout << "Channel: " << channel << endl;
        cout << "Number of events: " << noe << endl;

        //I normalized the histogram
        noe=noe/time_b;
        cout << "Number of normalized events are: " << noe << endl;

        cout << endl;

        b->SetBinContent(channel ,noe);

        channel=channel+1;

    }


    bac.close();

    //s->Rebin(3);
    //s->GetXaxis()->SetRangeUser(200,1000);
    //s->GetYaxis()->SetRangeUser(0,0.4);
    //===============

    //======================


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
    leg->AddEntry(s,"Co-60");
    leg->AddEntry(b,"Background");
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

    //Now I are going to divide this histogram
    div->Add(s,b,1.,-1.);
    //div->Rebin();

    for(i=0; i<Hist_Max_D; i++)
    {
        bincont = div->GetBinContent(i);
        if(bincont>0)
        {
        act=act + bincont;
        }
    }

        cout << endl << endl << endl;

        cout << "La actividad total es: " << act << endl << endl << endl;



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

    div->Draw("Same");

    //I put a legend in TCanvas 2
    TLegend *leg2= new TLegend(.1 ,.7 ,.3 ,.9 ,"Tritium");
    leg2->SetFillColor(0);
    div->SetFillColor(0);
    leg2->AddEntry(div,"Co-60");
    leg2->Draw("Same");





    //return 0;

}

