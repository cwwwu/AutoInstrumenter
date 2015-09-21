#include "InstrumentationMap.h"

#include <fstream>
#include <sstream>
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace clang {
namespace auto_instrument {

InstrumentInfoEntry::InstrumentInfoEntry() : varName(""), instrumentation("") {}
InstrumentInfoEntry::InstrumentInfoEntry(InstrumentInfoEntry& entry) 
  : varName(entry.varName), instrumentation(entry.instrumentation) {}
InstrumentInfoEntry::InstrumentInfoEntry(string varName, string instrumentation)
  : varName(varName), instrumentation(instrumentation) {}



InstrumentationMap::InstrumentationMap(const char *filepath) {
  instrumentationMap.clear();
  constructInstrumentationMap(filepath);
}

InstrumentationMap::~InstrumentationMap() {
  destructInstrumentationMap();
}

map<int,InstrumentInfoEntry*>& InstrumentationMap::getMap() {
  return instrumentationMap;
}

void InstrumentationMap::printMap() {
  llvm::errs() << "Instrumentation Map:\n";
  for (map<int,InstrumentInfoEntry*>::iterator it=instrumentationMap.begin();
        it != instrumentationMap.end();
        it++) {
    llvm::errs() << (*it).first << " => <" << (*it).second->varName << "," << (*it).second->instrumentation << ">\n";
  }
}

string InstrumentationMap::getHeader()  { return headerSnippet; }
string InstrumentationMap::getInit()    { return initSnippet; }
string InstrumentationMap::getEnd()     { return endSnippet; }

void InstrumentationMap::constructInstrumentationMap(const char *filepath) {
  if (filepath) {
    ifstream inputFile(filepath);
    string line;
    while(getline(inputFile,line)) {
      string functionName, varName, lineno, code;
      istringstream lineStream(line);
      
      getline(lineStream, functionName, ',');
      if (functionName.compare(KEY_HEADER) == 0) {
        headerSnippet = line.substr(line.find_first_of("<")+1,
                                    line.find_last_of(">")-line.find_first_of("<")-1);
      } else if (functionName.compare(KEY_INIT) == 0) {
        initSnippet = line.substr(line.find_first_of("<")+1,
                                  line.find_last_of(">")-line.find_first_of("<")-1);
      } else if (functionName.compare(KEY_END) == 0) {
        endSnippet = line.substr(line.find_first_of("<")+1,
                                 line.find_last_of(">")-line.find_first_of("<")-1);
      } else {
        getline(lineStream, varName, ',');
        getline(lineStream, lineno, ',');
        code = line.substr(line.find_first_of("<")+1,
                            line.find_last_of(">")-line.find_first_of("<")-1);
        InstrumentInfoEntry *entry = new InstrumentInfoEntry(varName,code);
        instrumentationMap[atoi(lineno.c_str())] = entry;
      }
    }
  }
 
  printMap(); 
}

void InstrumentationMap::destructInstrumentationMap() {
  for (map<int,InstrumentInfoEntry*>::iterator it=instrumentationMap.begin();
        it != instrumentationMap.end();
        it++) {
    delete (*it).second;
  }
}

const string InstrumentationMap::KEY_HEADER = "GOOMF_Header";
const string InstrumentationMap::KEY_INIT = "GOOMF_Init";
const string InstrumentationMap::KEY_END = "GOOMF_Dest";

}
}