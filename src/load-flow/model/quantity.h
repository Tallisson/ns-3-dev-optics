/*
 * quantity.h
 *
 *  Created on: 09/08/2014
 *      Author: thiago
 */

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

class Quantity: public Object {
private:
  double pg;
  double pc;
  double qg;
  double qc;
public:
  void SetAttr(int attr, double value);
  double GetAttr(int attr);

  static TypeId GetTypeId (void);

  Quantity();
  virtual ~Quantity();
};
}

#endif /* QUANTITY_H_ */
