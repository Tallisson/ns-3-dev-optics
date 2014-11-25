#include "utils.h"

#include "ns3/log.h"

using namespace boost;

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("Utils");
NS_OBJECT_ENSURE_REGISTERED (Utils);

Ptr<Utils> Utils::instance = NULL;


TypeId
Utils::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EdgeBus")
    .SetParent<Object> ();

  return tid;
}

Ptr<Utils> Utils::GetInstance() {
  if(!instance) {
    instance = CreateObject<Utils>();
  }

  return instance;
}

void Utils::Delete() {
  NS_LOG_FUNCTION_NOARGS ();
}

Desc* Utils::ProcessFile(const char* filename) {
  Desc* d = new Desc();
  std::ifstream file(filename);
  boost::iostreams::filtering_istream in;
  in.push(file);
  int qtd_bus = 0;
  int qtd_branch = 0;

  for(std::string str; std::getline(in, str); )
  {
      if((qtd_bus == 0 && qtd_branch == 0) && this->ValidateInfSystem(str)) {
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
          Ptr<Bus> b = CreateObject<Bus> ();
          b->Init(strs);
          d->bars.push_back(b);
        }
        qtd_bus--;
      }

      if((qtd_bus == 0 && qtd_branch == 0) && this->ValidateBusData(str)) {
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

      if((qtd_bus == 0 && qtd_branch == 0) && this->ValidateBranchData(str)) {
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
