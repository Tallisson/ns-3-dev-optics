#include "utils.h"
#include <iostream>

using namespace boost;
using namespace std;

namespace ns3
{

Desc* Utils::ProcessFile(const char* filename) {
  Desc* d = new Desc();
  std::ifstream file(filename);
  boost::iostreams::filtering_istream in;
  in.push(file);
  int qtd_bus = 0;
  int qtd_branch = 0;

  for(std::string str; std::getline(in, str); )
  {
      if((qtd_bus == 0 && qtd_branch == 0) && ValidateInfSystem(str)) {
        container::vector<std::string> strs;
        split(strs, str, is_any_of("\t "));
        if(strs.size() == 3) {
          d->name_system = strs.at(1);
          d->s_base = atof(strs.at(2).c_str());
        }
      }

      if(qtd_bus > 0) {
        container::vector<std::string> strs;
        split(strs, str, is_any_of("\t "));
        unsigned size = strs.size();
        if(size == 9) {
          Ptr<Bus> b = CreateObject<Bus>();
          b->Init(strs);
          d->bars.push_back(b);
        }
        qtd_bus--;
      }

      if((qtd_bus == 0 && qtd_branch == 0) && ValidateBusData(str)) {
        container::vector<std::string> strs;
        split(strs, str, is_any_of("\t "));
        if(strs.size() == 3) {
          qtd_bus = atoi(strs.at(2).c_str());
        }
      }

      if(qtd_branch > 0) {
        container::vector<std::string> strs;
        split(strs, str, is_any_of("\t "));
        unsigned size = strs.size();
        if(size >= 8) {
          Ptr<Admitt> ad = CreateObject<Admitt>();
          ad->Init(strs);
          d->admmits.push_back(ad);
        }
        qtd_branch--;
      }

      if((qtd_bus == 0 && qtd_branch == 0) && ValidateBranchData(str)) {
        container::vector<std::string> strs;
        split(strs, str, is_any_of("\t "));
        if(strs.size() == 3) {
          qtd_branch = atoi(strs.at(2).c_str());
        }
      }
  }

  return d;
}

bool Utils::ValidateInfSystem(const std::string& s) {
  regex e("\\A(System[\\s]{1}[\\S]+[\\s]{1}[\\d]{2, 4})\\z");
  return regex_match(s, e);
}

bool Utils::ValidateBusData(const std::string& s) {
  regex e("\\A(Bus Data[\\s]{1}[\\d]{1, 3})\\z");
  return regex_match(s, e);
}

bool Utils::ValidateBranchData(const std::string& s) {
  regex e("\\A(Branch Data[\\s]{1}[\\d]{1, 3})\\z");
  return regex_match(s, e);
}
}
