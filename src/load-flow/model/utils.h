#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <boost/cstdint.hpp>
#include <boost/regex.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include "bus.h"
#include "admitt.h"
#include "define.h"

#include "ns3/ptr.h"

namespace ns3
{

typedef struct Desc {
public:
  std::string name_system;
  double s_base;
  container::vector<Ptr<Bus> > bars;
  container::vector<Ptr<Admitt> > admmits;
  Desc(): name_system(""), s_base(0) {};
  ~Desc()
  {
    bars.clear();
    admmits.clear();
  }
} Desc;

class Utils{

public:
  static Desc* ProcessFile(const char* file);
  static bool ValidateInfSystem(const std::string& s);
  static bool ValidateBusData(const std::string& s);
  static bool ValidateBranchData(const std::string& s);
};
}

#endif /* UTILS_H_ */
