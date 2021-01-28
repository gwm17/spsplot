/*

SPSPlot.h
Master class of the SPSPlot program. Takes in an input file and creates reaction data which is then
used to calculate and plot the rho values for different excitations. Conceptually based on the program
Specplot written by D.W. Visser at Yale. Uses the  ROOT graphics environment.

Written by G.W. McCann Sep. 2020

*/

#ifndef SPSPLOT_H
#define SPSPLOT_H

#include <vector>
#include <string>
#include <TGraph.h>
#include "Reaction.h"

class SPSPlot {
public:
	SPSPlot();
	SPSPlot(std::string& filename);
	~SPSPlot();

	void AttachFile(std::string& filename);
	void SetParameters(double B, double theta, double beamKE);
	void SetB(double B);
	void SetTheta(double theta);
	void SetBeamKE(double beamKE);
	void SetRhoRange(double rhoMin, double rhoMax);

	void AddReaction(Reaction rxn);

	double inline GetRhoMin() {return m_rhoMin;};
	double inline GetRhoMax() {return m_rhoMax;};
	double inline GetTheta() {return m_theta;};
	double inline GetBeamKE() {return m_beamKE;};
	double inline GetB() {return m_B;};

	TGraph** GetGraphs();

	int inline GetNGraphs() { return ngraphs; };
	bool inline IsValid() { return validFlag; };

	void SaveToFile(std::string& name);

private:
	bool ReadInputFile(std::string& filename);
	void UpdateReactions();

	std::vector<Reaction> m_Reactions;

	double m_B;
	double m_theta;
	double m_beamKE;
	double m_rhoMin, m_rhoMax;

	int ngraphs;
	bool validFlag;

	TGraph** graph_array; //owned by SPSPlot
};

#endif