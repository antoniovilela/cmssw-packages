#include "TFile.h"

#include "PlottingTools.h"
#include "Plotter.h"

#include <iostream>
#include <vector>

void plot(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, const std::string& option = "DefaultNorm"){
  if(option == "DefaultNorm"){
     Plotter<DefaultNorm> plotter;
     plotter.plot(variables,directories);
  } else if(option == "NumberEntriesNorm"){
     Plotter<NumberEntriesNorm> plotter;
     plotter.plot(variables,directories);
  } else{
     std::cout << "Not valid option: " << option << std::endl;
  }  
}

void plot(std::vector<std::string>& variables, const std::string& file1, const std::string& file2, const std::string& option = "DefaultNorm"){

  std::vector<std::pair<std::string,TDirectory*> > directories;
  
  directories.push_back(std::make_pair("file1",TFile::Open(file1.c_str())));
  directories.push_back(std::make_pair("file2",TFile::Open(file2.c_str())));

  //Plotter<DefaultNorm> plotter;
  //plotter.plot(variables,directories);
  plot(variables,directories,option);
}

void plot(const std::string& variable, const std::string& file1, const std::string& file2, const std::string& option = "DefaultNorm"){
  std::vector<std::string> variables;
  variables.push_back(variable);

  plot(variables,file1,file2,option);
}
