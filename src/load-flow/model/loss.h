#ifndef LOSS_H_
#define LOSS_H_

#include "define.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

namespace ns3
{

class Loss: public Object {
private:
  double p_in_branch;
  double p_out_branch;
  double q_in_branch;
  double q_out_branch;
  double loss;
public:
  Loss();
  virtual ~Loss();

  static TypeId GetTypeId (void);
  void SetAttr(int attr, double value);
  double GetAttr(int attr);
};

}

#endif /* LOSS_H_ */
