/*

MassLookup.h
Generates a map for isotopic masses using AMDC data; subtracts away
electron mass from the atomic mass by default. Creates a static global instance
of this map (MASS) for use throughout code it is included into.

Written by G.W. McCann Aug. 2020

*/
#include "MassLookup.h"

using namespace std;

/*
  Read in AMDC mass file, preformated to remove excess info. Here assumes that by default
  the file is in a local directory etc/
*/
MassLookup::MassLookup() {
  ifstream massfile("data/mass.txt");
  if(massfile.is_open()) {
    string junk, A, element;
    int Z;
    double atomicMassBig, atomicMassSmall;
    getline(massfile,junk);
    getline(massfile,junk);
    while(massfile>>junk) {
      massfile>>Z>>A>>element>>atomicMassBig>>atomicMassSmall;
      string key = "("+to_string(Z)+","+A+")";
      massTable[key] = (atomicMassBig +atomicMassSmall*1e-6 - Z*electron_mass)*u_to_mev;
      elementTable[Z] = element;
    }
  } else {
    cerr<<"Unable to open mass.txt. Make sure it is present."<<endl;
  }
}

MassLookup::~MassLookup() {}

//Returns nuclear mass in MeV
double MassLookup::FindMass(int Z, int A) {
  string key = "("+to_string(Z)+","+to_string(A)+")";
  try {
    double mass = massTable.at(key);
    return mass;
  } catch (out_of_range& oor) {
    cerr<<"Mass of "<<key<<" (Z,A) not found in Mass Table! Returning 1"<<endl;
    return 1;
  }
}

//returns element symbol
string MassLookup::FindElement(int Z) {
  try {
    string element = elementTable.at(Z);
    return element;
  } catch (out_of_range& oor) {
    cerr<<"Atomic number: "<<Z<<" not found in Element Table! Returning void."<<endl;
    string element = "void";
    return element;
  }
}
