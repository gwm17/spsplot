#include "SPSPlot.h"
#include <TAxis.h>
#include <TLatex.h>
#include <TStyle.h>

//Default constructor
SPSPlot::SPSPlot() {
	graph_array = nullptr;
	ngraphs = 0;
	validFlag = false;
}

//Overload for use as standalone (no gui)
SPSPlot::SPSPlot(std::string& filename) {
	validFlag = ReadInputFile(filename);
	graph_array = nullptr;
	ngraphs = 0;
}

SPSPlot::~SPSPlot() {
	if(graph_array != nullptr) {
		for(int i=0; i<ngraphs; i++)
			delete graph_array[i];
		delete[] graph_array;
	}
}

//Called to load data
void SPSPlot::AttachFile(std::string& filename) {
	validFlag = ReadInputFile(filename);
}

//Handler for input data
bool SPSPlot::ReadInputFile(std::string& filename) {
	ifstream input(filename);
	if(!input.is_open()) {
		std::cerr<<"Unable to open file at SPSPlot::ReadInputFile()!"<<std::endl;
		return false;
	}

	//Clear reactions so that file can be loaded even if there is already loaded data
	m_Reactions.clear();

	std::string junk;
	double bke, b, theta, rhomin, rhomax;
	int zt, at, zp, ap, ze, ae;

	input>>junk>>bke>>junk>>b>>junk>>theta;
	input>>junk>>rhomin>>junk>>rhomax;

	std::getline(input, junk);
	std::getline(input, junk);
	std::getline(input, junk);
	while(input>>at) {
		Reaction rxn;
		input>>zt>>ap>>zp>>ae>>ze;
		rxn.SetReactionData(at, zt, ap, zp, ae, ze);
		rxn.SetKinematicParams(bke, theta, b);
		m_Reactions.push_back(rxn);
	}

	m_rhoMin = rhomin; m_rhoMax = rhomax;
	m_beamKE = bke; m_theta = theta; m_B = b;
	input.close();
	return true;
}


void SPSPlot::UpdateReactions() {
	for(auto& rxn: m_Reactions) {
		rxn.SetKinematicParams(m_beamKE, m_theta, m_B);
	}
}

//Main way to update data
void SPSPlot::SetParameters(double bke, double theta, double b) {
	if(!IsValid()) { return; }
	m_beamKE = bke;
	m_theta = theta;
	m_B = b;
	UpdateReactions();
}

/*UNUSED*/
void SPSPlot::SetB(double b) {
	if(!IsValid()) { return; }
	m_B = b;
	UpdateReactions();
}

/*UNUSED*/
void SPSPlot::SetTheta(double theta) {
	if(!IsValid()) { return; }
	m_theta = theta;
	UpdateReactions();
}

/*UNUSED*/
void SPSPlot::SetBeamKE(double bke) {
	if(!IsValid()) { return; }
	m_beamKE = bke;
	UpdateReactions();
}

/*Alter plotting range*/
void SPSPlot::SetRhoRange(double rhoMin, double rhoMax) {
	if(!IsValid()) { return; }
	m_rhoMin = rhoMin;
	m_rhoMax = rhoMax;
}

/*Workhorse function; generates an array of graphs (one for each reaction)
  If the array has already been created, array is deleted and re-created
  Locally data is gathered over the relevant range (rhoMin to rhoMax) and fed
  to the proper root objects. Graphs are formated. Note the y-axis is an arbitrary
  variable (rxn index) used to space out the reactions for visibility
 */
TGraph** SPSPlot::GetGraphs() {
	if(!IsValid()) { return nullptr; }

	if(ngraphs == 0) ngraphs = m_Reactions.size(); //The first call makes the number of graphs the number of rxns

	/*eliminate memory leaks*/
	if(graph_array == nullptr) {
		graph_array = new TGraph*[ngraphs];
	} else {
		for(int i=0; i<ngraphs; i++)
			delete graph_array[i];
	}

	//local memory for tgraphs
	std::vector<std::vector<double>> rxn_labels;
	std::vector<std::vector<double>> valid_rhos;
	std::vector<std::vector<std::string>> ex_labels;
	int localSize=0; //indexing variable
	rxn_labels.reserve(ngraphs);
	valid_rhos.reserve(ngraphs);
	ex_labels.reserve(ngraphs);

	int nRxns = m_Reactions.size();
	for(int i=0; i<nRxns; i++) {
		//set graph data
		localSize = m_Reactions[i].GetRhos()->size();
		rxn_labels.emplace_back(std::vector<double>());
		valid_rhos.emplace_back(std::vector<double>());
		ex_labels.emplace_back(std::vector<std::string>());
		for(int j=0; j<localSize; j++) {
			rxn_labels[i].push_back((double)i);
			double& this_rho = m_Reactions[i].GetRhos()->at(j);
			std::string& this_label = m_Reactions[i].GetEx_Strings()->at(j);
			if(this_rho >= m_rhoMin && this_rho <= m_rhoMax) {
				valid_rhos[i].push_back(this_rho);
				ex_labels[i].push_back(this_label);
			}
		}

		//create and format the graphs with tlatex labels on each point
		graph_array[i] = new TGraph(valid_rhos[i].size(), &(valid_rhos[i][0]), &(rxn_labels[i][0]));
		graph_array[i]->SetName(m_Reactions[i].GetName().c_str());
		graph_array[i]->SetTitle(m_Reactions[i].GetName().c_str());
		graph_array[i]->SetMarkerColor(i+1);
		graph_array[i]->SetMarkerSize(1);
		for(unsigned int j=0; j<valid_rhos[i].size(); j++) {
			TLatex *label = new TLatex(graph_array[i]->GetX()[j], graph_array[i]->GetY()[j]+0.1, ex_labels[i][j].c_str());
			label->SetTextSize(0.02);
			graph_array[i]->GetListOfFunctions()->Add(label); //graphs then own the labels (i.e. deleted when graph is deleted)
		}
		graph_array[i]->GetXaxis()->SetLimits(m_rhoMin, m_rhoMax);
		graph_array[i]->SetMinimum(-1); //obnoxius root difference between setting x and y axis ranges
		graph_array[i]->SetMaximum(ngraphs);
		graph_array[i]->GetXaxis()->SetTitle("#rho (cm)"); //latex style labels
		graph_array[i]->GetYaxis()->SetTitle("Reaction Index");
	}
	ngraphs = nRxns;

	return graph_array;

}

/*
	Mostly exists on the idea that eventually will implement active loading of additional
	reactions through the gui
*/
void SPSPlot::SaveToFile(std::string& name) {
	std::ofstream output(name);
	if(!output.is_open()) {
		std::cerr<<"Unable to create configuration file!"<<std::endl;
		return;
	}

	output<<"BeamKE(MeV): "<<m_beamKE<<std::endl;
	output<<"Bfield(kG): "<<m_B<<std::endl;
	output<<"Theta(deg): "<<m_theta<<std::endl;
	output<<"RhoMin(cm): "<<m_rhoMin<<" RhoMax(cm): "<<m_rhoMax<<std::endl;
	output<<std::endl;
	output<<"AT\tZT\tAP\tZP\tAE\tZE"<<std::endl;
	for(auto rxn: m_Reactions) {
		output<<rxn.GetTarget().A<<"\t"<<rxn.GetTarget().Z;
		output<<"\t"<<rxn.GetProjectile().A<<"\t"<<rxn.GetProjectile().Z;
		output<<"\t"<<rxn.GetEjectile().A<<"\t"<<rxn.GetEjectile().Z;
		output<<std::endl;
	}
	output.close();
}

void SPSPlot::AddReaction(Reaction rxn) {
	rxn.SetKinematicParams(m_beamKE, m_theta, m_B);
	m_Reactions.push_back(rxn);
}
