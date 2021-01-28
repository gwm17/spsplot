
/*

ExTable.h
Generates a map for nuclear excitation energies; fed from a file titled excitations.dat in the dir /data/. Creates a static global instance
of this map (EX) for use throughout code it is included into (i.e. its a singleton, but gave the option for it to not be)

Written by G.W. McCann Sep. 2020

*/
#ifndef EXTABLE_H
#define EXTABLE_H

#include <unordered_map>
#include <string>
#include <vector>

struct ExData {
	std::vector<double> ex_list;
	std::vector<std::string> str_list;
};

class ExTable {
public:
	ExTable();
	~ExTable();
	std::vector<double> GetListOfExcitations(std::string& name);
	std::vector<std::string> GetListOfExcitations_Strings(std::string& element);

private:
	std::unordered_map<std::string, ExData> table;

};

//static global instance
static ExTable EX;
#endif