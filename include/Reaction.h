/*

Reaction.h
Reaction class for the SPSPlot environment. Calculations are primarily based
on kinematics from Iliadis, Nuclear Physics of Stars, Appendix D.

Based on code by D.W. Visser written at Yale for the original SPANC

Written by G.W. McCann Aug. 2020

Modified from the SPANCRedux environment to be suitable for SPSPlot

Written by G.W. McCann Aug. 2020

Modified Sep 2020 by G.W. McCann

*/
#ifndef REACTION_H
#define REACTION_H

#include <TROOT.h>
#include <TMath.h>
#include <iostream>
#include <string>
#include <vector>
#include "MassLookup.h"
#include "ExTable.h"

struct nucleus {
  double E;
  double KE;
  double p;
  int A, Z;
  double mass_gs;
  std::string sym;
};

class Reaction {
  
  public:
    Reaction();
    ~Reaction();
    void SetReactionData(int At, int Zt, int Ap, int Zp, int Ae, int Ze);
    void SetKinematicParams(double beamKE, double lab_angle, double mag_field);
    vector<double>* GetRhos();
    vector<double>* GetExs();
    vector<std::string>* GetEx_Strings();
    const nucleus& GetTarget();
    const nucleus& GetProjectile();
    const nucleus& GetEjectile();
    const nucleus& GetResidual();
    double GetAngle();
    double GetBfield();
    double GetBeamKE();
    std::string& GetName();

  private:
    void SetExcitations();
    double CalculateRho(double excitation);
    void CalculateRhos();
    nucleus target, projectile, ejectile, residual;
    double theta, B, beamE;
    std::string name;
    vector<double> excitations;
    vector<std::string> ex_strings;
    vector<double> rhos;

    bool target_initialized, kinematics_initialized; 

    static constexpr double C = 299792458;
    static constexpr double QBRHO2P = 1.0E-9*C; //converts QBrho to momentum (cm*kG -> MeV/c)
    static constexpr double DEG2RAD = TMath::Pi()/180.0; //converts degrees to radians
    static constexpr double UNIT_CHARGE = 1.602176643E-19;
    static constexpr double MEV2J = 1.602176643E-13; //MeV to Joules

    static constexpr double KG2T = 0.1;
};

#endif
