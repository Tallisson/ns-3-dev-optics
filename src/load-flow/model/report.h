#ifndef REPORT_H_
#define REPORT_H_

#include <boost/container/map.hpp>
#include "bus.h"
#include "quantity.h"
#include "loss.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace boost;

namespace ns3
{

class Report: public Object {
private:
  container::map<Ptr<Bus>, Ptr<Quantity> > power;
  container::map<Ptr<EdgeBus>, Ptr<Loss> > losses;
public:
  void Insert(Ptr<Bus> bar, Ptr<Quantity>  q);
  void Insert(Ptr<EdgeBus> n, Ptr<Loss>  l);
  void Clear();

  Ptr<Quantity>  At(Ptr<Bus> bar);
  Ptr<Loss>  At(Ptr<EdgeBus> n);
  container::map<Ptr<Bus>, Ptr<Quantity> > GetPower();
  container::map<Ptr<EdgeBus>, Ptr<Loss> > GetLosses();

  static TypeId GetTypeId (void);
  Report();
  virtual ~Report();
};
}

#endif /* REPORT_H_ */
