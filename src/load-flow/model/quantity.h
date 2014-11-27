#ifndef QUANTITY_H_
#define QUANTITY_H_

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

namespace ns3
{

#define PG 10
#define PC 15
#define QG 20
#define QC 25

class Quantity : public Object {
private:
  double pg;
  double pc;
  double qg;
  double qc;
public:
  void SetAttr(uint32_t attr, double value);
  double GetAttr(uint32_t attr);
  Quantity();
  virtual ~Quantity();

  static TypeId GetTypeId (void);
};
}

#endif /* QUANTITY_H_ */
