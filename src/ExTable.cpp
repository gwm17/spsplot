
/*

ExTable.cpp
Generates a map for nuclear excitation energies; fed from a file titled excitations.dat in the dir /data/. Creates a static global instance
of this map (EX) for use throughout code it is included into (i.e. its a singleton, but gave the option for it to not be)

Written by G.W. McCann Sep. 2020

*/

#include "ExTable.h"
#include <fstream>
#include <iostream>

ExTable::ExTable() {
	std::ifstream input("data/excitations.dat");
	if(!input.is_open()) {
		std::cerr<<"Unable to open excitations.dat! Check that it is in ./data/"<<std::endl;
		return;
	}

	std::string element, text;
	ExData temp;
	while(input>>element) {
		temp.ex_list.clear();
		temp.str_list.clear();
		while(true){
			input>>text;
			if(text == "end") break;
			temp.ex_list.emplace_back(stod(text));
			temp.str_list.emplace_back(text);
		}
		table[element] = temp;
	}
}

ExTable::~ExTable() {}

std::vector<double> ExTable::GetListOfExcitations(std::string& element) {
	auto iter = table.find(element);
	if(iter == table.end()) {
		std::cerr<<"Invalid element name at GetListOfExictations!"<<std::endl;
		return std::vector<double>({0});
	} else {
		return iter->second.ex_list;
	}
}

std::vector<std::string> ExTable::GetListOfExcitations_Strings(std::string& element) {
	auto iter = table.find(element);
	if(iter == table.end()) {
		std::cerr<<"Invalid element name at GetListOfExictations_Strings!"<<std::endl;
		return std::vector<std::string>({""});
	} else {
		return iter->second.str_list;
	}
}