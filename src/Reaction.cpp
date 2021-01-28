/*

Reaction.cpp
Reaction class for the SPSPlot environment. Calculations are primarily based
on kinematics from Iliadis, Nuclear Physics of Stars, Appendix D.

Based on code by D.W. Visser written at Yale for the original SPANC

Modified from the SPANCRedux environment to be suitable for SPSPlot

Written by G.W. McCann Aug. 2020

Modified Sep 2020 by G.W. McCann

*/
#include "Reaction.h"

/*Set all flags to start values*/
Reaction::Reaction() {
  target_initialized = false;
  kinematics_initialized = false;
}

Reaction::~Reaction() {
}

/*initialize reactants*/
void Reaction::SetReactionData(int At, int Zt, int Ap, int Zp, int Ae, int Ze) {

  target.A = At; target.Z = Zt;
  target.mass_gs = MASS.FindMass(target.Z, target.A);
  target.sym = to_string(target.A)+MASS.FindElement(target.Z);

  projectile.A = Ap; projectile.Z = Zp;
  projectile.mass_gs = MASS.FindMass(projectile.Z, projectile.A);
  projectile.sym = to_string(projectile.A) + MASS.FindElement(projectile.Z);

  ejectile.A = Ae; ejectile.Z = Ze;
  ejectile.mass_gs = MASS.FindMass(ejectile.Z, ejectile.A);
  ejectile.sym = to_string(ejectile.A) + MASS.FindElement(ejectile.Z);

  residual.A = At+Ap - Ae;
  residual.Z = Zt+Zp - Ze;
  if(residual.A<=0 || residual.Z<=0) {
    std::cerr<<"Invalid reaction at SetReactionData()!"<<std::endl;
    return;
  }
  residual.sym = to_string(residual.A) + MASS.FindElement(residual.Z);
  residual.mass_gs = MASS.FindMass(residual.Z, residual.A);

  SetExcitations(); //Find listed exictation energies

  name = target.sym + "(" + projectile.sym+ "," + ejectile.sym + ")" + residual.sym;
  target_initialized = true;
}

/*initialize the kinematic (SPS) parameters*/
void Reaction::SetKinematicParams(double beamKE, double lab_angle, double mag_field) {
  if(!target_initialized) {
    std::cerr<<"Target not set correctly! Unable to initialize kinematics."<<std::endl;
    return;
  }

  beamE = beamKE;
  projectile.KE = beamE;
  theta = lab_angle*DEG2RAD;
  B = mag_field;

  projectile.E = projectile.KE + projectile.mass_gs;
  projectile.p = sqrt(pow(projectile.E, 2.)-pow(projectile.mass_gs, 2.));

  target.E = target.mass_gs;
  target.KE = 0.;
  target.p = 0.;

  kinematics_initialized = true;
  CalculateRhos(); //Calculate rho values for the given excitations
}

/*Calculates the bending radius (in cm) of the ejectile, given an excitation energy of the residual (in MeV)*/
double Reaction::CalculateRho(double excitation) {
  if(!kinematics_initialized) {
    std::cerr<<"Attempting calculation with uninitialized parameters at GetRho! Return 0"<<std::endl;
    return 0.0;
  }

  double Q = projectile.mass_gs+target.mass_gs - (ejectile.mass_gs+residual.mass_gs+excitation);
  double r = sqrt(projectile.mass_gs*ejectile.mass_gs*projectile.KE)/(ejectile.mass_gs+residual.mass_gs)*cos(theta);
  double s = (projectile.KE*(residual.mass_gs-projectile.mass_gs)+residual.mass_gs*Q)/(ejectile.mass_gs+residual.mass_gs);

  double ejectKE1 = r + sqrt(r*r + s);
  double ejectKE2 = r - sqrt(r*r + s);

  double ejectKE;
  if(ejectKE2 < 0. || std::isnan(ejectKE2)) {
    ejectKE =  ejectKE1*ejectKE1;
  } else if (ejectKE1 < 0. || std::isnan(ejectKE1)) {
    ejectKE = ejectKE2*ejectKE2;
  } else {
    ejectKE = ejectKE1*ejectKE1;
  }  

  double ejectP = sqrt(ejectKE*(ejectKE+2.0*ejectile.mass_gs));
  double qbrho = ejectP/QBRHO2P;

  return qbrho/(ejectile.Z*B);
}


/*Getters and setters*/

void Reaction::SetExcitations() {
  excitations = EX.GetListOfExcitations(residual.sym);
  ex_strings = EX.GetListOfExcitations_Strings(residual.sym);
}

void Reaction::CalculateRhos() {  
  rhos.clear();
  rhos.reserve(excitations.size());
  for(unsigned int i=0; i<excitations.size(); i++) {
    rhos.emplace_back(CalculateRho(excitations[i]));
  }
}

vector<double>* Reaction::GetRhos() {
  return &rhos;
}

vector<double>* Reaction::GetExs() {
  return &excitations;
}

vector<string>* Reaction::GetEx_Strings() {
  return &ex_strings;
}

const nucleus& Reaction::GetTarget() {
  return target;
}

const nucleus& Reaction::GetProjectile() {
  return projectile;
}

const nucleus& Reaction::GetEjectile() {
  return ejectile;
}

const nucleus& Reaction::GetResidual() {
  return residual;
}

double Reaction::GetAngle() {
  return theta/DEG2RAD;
}

double Reaction::GetBfield() {
  return B;
}

double Reaction::GetBeamKE() {
  return beamE;
}

std::string& Reaction::GetName() {
  return name;
}

