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
#include "ns3/type-id.h"
#include "ns3/object.h"

namespace ns3
{

typedef struct Desc {
public:
  std::string name_system;
  double s_base;
  container::vector<Ptr<Bus> > bars;
  container::vector<Ptr<Admitt> > admmits;
  Desc(): name_system(""), s_base(0) {};
} Desc;

class Utils: public Object {
private:
  Utils(Utils const&){};             // copy constructor is private
  virtual ~Utils() {};

  static Ptr<Utils> instance;
public:
  Utils() {};
  static Ptr<Utils> GetInstance();
  Desc* ProcessFile(const char* file);
  void Delete();
  bool ValidateInfSystem(const std::string& s);
  bool ValidateBusData(const std::string& s);
  bool ValidateBranchData(const std::string& s);

  static TypeId GetTypeId (void);
};
}

#endif /* UTILS_H_ */
