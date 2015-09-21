#ifndef INSTRUMENTATIONMAP_H
#define INSTRUMENTATIONMAP_H

#include <map>
#include <string>

using namespace std;

namespace clang {
namespace auto_instrument {

struct InstrumentInfoEntry {
  string varName;
  string instrumentation;
  InstrumentInfoEntry();
  InstrumentInfoEntry(InstrumentInfoEntry& entry);
  InstrumentInfoEntry(string varName, string instrumentation);
};

class InstrumentationMap {
 public:
  InstrumentationMap(const char *filepath);
  ~InstrumentationMap();
  map<int,InstrumentInfoEntry*>& getMap();
  void printMap();
  string getHeader();
  string getInit();
  string getEnd();

 private:
  static const string KEY_HEADER;
  static const string KEY_INIT;
  static const string KEY_END;

  void constructInstrumentationMap(const char *filepath);
  void destructInstrumentationMap();

  map<int,InstrumentInfoEntry*> instrumentationMap;
  string headerSnippet;
  string initSnippet;
  string endSnippet;
};

}
}

#endif