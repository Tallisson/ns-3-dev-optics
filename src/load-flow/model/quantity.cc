#include "quantity.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Quantity");
NS_OBJECT_ENSURE_REGISTERED (Quantity);

TypeId
Quantity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Quantity")
    .SetParent<Object> ()
    .AddConstructor<Quantity> ()
    /*.AddAttribute ("PowerActiverG",
                   "Power Active Generation",
                   DoubleValue (),  TODO
                   MakeDoubleAccessor (&Quantity::pg),
                   MakeDoubleChecker<double> ())
     .AddAttribute ("PowerActiverL",
                    "Power Active Waste",
                    DoubleValue (),  TODO
                    MakeDoubleAccessor (&Quantity::pc),
                    MakeDoubleChecker<double> ())
      .AddAttribute ("PowerReactiverG",
                     "Power Reactive Generation",
                     DoubleValue (),  TODO
                     MakeDoubleAccessor (&Quantity::qg),
                     MakeDoubleChecker<double> ())
      .AddAttribute ("PowerReactiverL",
                     "Power Reactive Waste",
                     DoubleValue (),  TODO
                     MakeDoubleAccessor (&Quantity::qc),
                     MakeDoubleChecker<double> ())
*/  ;

  return tid;
}


Quantity::Quantity(): pg(0), pc(0), qg(0), qc(0) {
}

Quantity::~Quantity() {
}

void Quantity::SetAttr(uint32_t attr, double value) {
  switch(attr) {
  case PG:
    this->pg = value;
    break;
  case PC:
    this->pc= value;
    break;
  case QG:
    this->qg= value;
    break;
  case QC:
    this->qc= value;
    break;
  }
}

double Quantity::GetAttr(uint32_t attr) {
  switch(attr) {
  case PG:
    return this->pg;
    break;
  case PC:
    return this->pc;
    break;
  case QG:
    return this->qg;
    break;
  case QC:
    return this->qc;
    break;
  }

  return 0;
}
}
