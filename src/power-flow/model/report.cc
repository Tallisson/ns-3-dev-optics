#include "report.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Report");
NS_OBJECT_ENSURE_REGISTERED (Report);

TypeId
Report::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Report")
    .SetParent<Object> ()
    .AddConstructor<Report> ()
  ;

  return tid;
}


void Report::Insert(Ptr<Bus>  bar, Ptr<Quantity>  q) {
  power.insert(pair<Ptr<Bus> , Ptr<Quantity> >(bar, q));
}

void Report::Insert(Ptr<EdgeBus>   n, Ptr<Loss> l) {
  losses.insert(pair<Ptr<EdgeBus>  , Ptr<Loss> >(n, l));
}

Ptr<Quantity> Report::At(Ptr<Bus>  bar) {
  return power.at(bar);
}

Ptr<Loss> Report::At(Ptr<EdgeBus>   n) {
  return losses.at(n);
}

container::map<Ptr<Bus> , Ptr<Quantity> > Report::GetPower() {
  return power;
}

container::map<Ptr<EdgeBus>  , Ptr<Loss>   > Report::GetLosses() {
  return losses;
}

Report::Report() {
}

Report::~Report() {
  Clear();
}

void Report::Clear()
{
  container::map<Ptr<Bus> , Ptr<Quantity> >::iterator itP;
  for(itP = power.begin(); itP != power.end(); itP++)
  {
    delete itP->second;
  }
  container::map<Ptr<EdgeBus>  , Ptr<Loss> >::iterator itL;
  for(itL = losses.begin(); itL != losses.end(); itL++)
  {
    delete itL->second;
  }

  power.clear();
  losses.clear();
}

}
