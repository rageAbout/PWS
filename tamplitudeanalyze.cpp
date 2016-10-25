#include "tamplitudeanalyze.h"
#include <iostream>
#include <fstream>
#include <cmath>
#define PI 3.14159265

using namespace std;

TAmplitudeAnalyzer::TAmplitudeAnalyzer()
{
    amplitudeGenerator_ = new TGenerator;
}

TAmplitudeAnalyzer::~TAmplitudeAnalyzer(){
    delete amplitudeGenerator_;
}
void TAmplitudeAnalyzer::setSignalLine(TSignalLine *aSignalLine) {
    amplitudeAnalyzerSignalLine_ = new TSignalLine(aSignalLine->getSamplingFrequency(),aSignalLine->getObservationTime());
    amplitudeAnalyzerSignalLine_ = aSignalLine;
}

void TAmplitudeAnalyzer::cleanMemory() {
    delete amplitudeAnalyzerSignalLine_;
    delete[] amplitudeArray_;
    delete[] frequencyArray_;
}

TSignalLine* TAmplitudeAnalyzer::getSignalLine() {
    return amplitudeAnalyzerSignalLine_;
}

double TAmplitudeAnalyzer::DFT() {
    double frequency;
    double tempAmplitude;
    double startFrequancy = 1.0/amplitudeAnalyzerSignalLine_->getObservationTime();
    double endFrequancy = ((1.0/amplitudeAnalyzerSignalLine_->getObservationTime()) * (amplitudeAnalyzerSignalLine_->getMaxSize() / 2.0));
    double step = startFrequancy;
    double tempFrequancy;
    int i = 0.0;
    int j = 0.0;

    frequencyArray_ = new double[amplitudeAnalyzerSignalLine_->getMaxSize()];
    amplitudeArray_ = new double[amplitudeAnalyzerSignalLine_->getMaxSize()];
    for(frequency = startFrequancy; frequency<=endFrequancy; frequency = frequency + step) {
        tempFrequancy = getFrequancy(frequency);
        if(tempFrequancy != 0.0) {
            frequencyArray_[i] = tempFrequancy;
            amplitudeArray_[i] = getAmplitude(frequency);
            cout<<frequencyArray_[i]<<"  "<<amplitudeArray_[i]<<endl;
            i++;
         }
     }
    arrayLength_ = i;
    tempFrequancy = 0.0;
    tempAmplitude = 0.0;
    for(j=0;j<arrayLength_;j++) {
        if((amplitudeArray_[j] > tempAmplitude) && (frequencyArray_[j] >= 1.0) && (frequencyArray_[j] < 4.0)) {
            tempAmplitude = amplitudeArray_[j];
            tempFrequancy = frequencyArray_[j];
        }
    }

    tempFrequancy = tempFrequancy*60.0;

    return tempFrequancy;
}

double TAmplitudeAnalyzer::getFrequancy(double aFrequancy) {
    TSignalLine* sin;
    TSignalLine* cos;
    TSignalLine* rezult;
    double middleSin = 0;
    double middleCos = 0;
    double counter = 0.0;
    sin = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    amplitudeGenerator_->generateNewSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime(),0.0,1.0,0.0, aFrequancy);
    sin = amplitudeGenerator_->getSignalLine();
    rezult = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    add(sin, rezult);
    for(int i=0; i<rezult->getCurrentSize(); i++) {
        middleSin = middleSin+rezult->getYContainer(i);
        counter +=1.0;
    }
    middleSin = middleSin/counter;
    delete rezult;
    delete sin;
    counter = 0.0;
    amplitudeGenerator_->cleanMemory();
    amplitudeGenerator_->generateNewSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime(),0.0,1.0,PI/2.0, aFrequancy);
    cos = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    cos = amplitudeGenerator_->getSignalLine();
    rezult = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    add(cos, rezult);
    for(int i=0; i<rezult->getCurrentSize(); i++) {
        middleCos = middleCos+rezult->getYContainer(i);
        counter +=1.0;
    }
    middleCos = middleCos/counter;
    delete rezult;
    delete cos;
    if(((middleCos > 0.0001)) || (middleSin > 0.0001)) {
      aFrequancy = aFrequancy;
    } else {
        aFrequancy = aFrequancy - aFrequancy;
    }
    return aFrequancy;
}

void TAmplitudeAnalyzer::add(TSignalLine *aSignalLine, TSignalLine* aRezultSignalLine) {
    for(int i=0; i<amplitudeAnalyzerSignalLine_->getCurrentSize(); i++) {
        aRezultSignalLine->append(amplitudeAnalyzerSignalLine_->getXContainer(i), amplitudeAnalyzerSignalLine_->getYContainer(i)*aSignalLine->getYContainer(i));
    }
}
double TAmplitudeAnalyzer::getAmplitude(double aFrequancy) {
    TSignalLine* sin;
    TSignalLine* cos;
    TSignalLine* rezult;
    double middleSin = 0;
    double middleCos = 0;
    double counter = 0.0;
    double amplitude;
    sin = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    amplitudeGenerator_->generateNewSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime(),0.0,1.0,0.0, aFrequancy);
    sin = amplitudeGenerator_->getSignalLine();
    rezult = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    add(sin, rezult);
    for(int i=0; i<rezult->getCurrentSize(); i++) {
        middleSin = middleSin+rezult->getYContainer(i);
        counter +=1.0;
    }
    middleSin = middleSin/counter;
    delete rezult;
    delete sin;
    counter = 0.0;
    amplitudeGenerator_->cleanMemory();
    amplitudeGenerator_->generateNewSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime(),0.0,1.0,PI/2.0, aFrequancy);
    cos = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    cos = amplitudeGenerator_->getSignalLine();
    rezult = new TSignalLine(amplitudeAnalyzerSignalLine_->getSamplingFrequency(), amplitudeAnalyzerSignalLine_->getObservationTime());
    add(cos, rezult);
    for(int i=0; i<rezult->getCurrentSize(); i++) {
        middleCos = middleCos+rezult->getYContainer(i);
        counter +=1.0;
    }
    middleCos = middleCos/counter;
    delete rezult;
    delete cos;
    middleSin = pow(middleSin, 2.0);
    middleCos = pow(middleCos, 2.0);
    amplitude = pow(middleSin + middleCos, 0.5);
    amplitude = amplitude * 2.0;
    return amplitude;
}
double* TAmplitudeAnalyzer::getAmplitudeArray() {
    return amplitudeArray_;
}
double* TAmplitudeAnalyzer::getFrequancyArray() {
    return frequencyArray_;
}
int TAmplitudeAnalyzer::getArraySize() {
    return arrayLength_;
}
