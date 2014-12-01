#include "loss.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Loss");
NS_OBJECT_ENSURE_REGISTERED (Loss);

TypeId
Loss::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Loss")
    .SetParent<Object> ()
    .AddConstructor<Loss> ()
    .AddAttribute ("PowerActiveBranch",
                   "Power Active Branch Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&Loss::p_in_branch),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("PowerActiveBranchOut",
                   "Power Active Branch Out Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&Loss::p_out_branch),
                   MakeDoubleChecker<double> ())
     .AddAttribute ("PowerReactiveBranch",
                    "Power Reactive Branch Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Loss::q_in_branch),
                    MakeDoubleChecker<double> ())
     .AddAttribute ("PowerReactiveBranchOut",
                    "Power Reactive Branch Out Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Loss::q_out_branch),
                    MakeDoubleChecker<double> ())
      .AddAttribute ("LossPower",
                     "Losses Power",
                     DoubleValue (0), /* TODO */
                     MakeDoubleAccessor (&Loss::loss),
                     MakeDoubleChecker<double> ())
  ;

  return tid;
}


Loss::Loss() {
}

Loss::~Loss() {
}

void Loss::SetAttr(uint32_t attr, double value) {
  switch(attr) {
  case P_IN:
    this->p_in_branch = value;
    break;
  case P_OUT:
    this->p_out_branch = value;
  case Q_IN:
    this->q_in_branch= value;
    break;
  case Q_OUT:
    this->q_out_branch = value;
    break;
  case LOSS:
    this->loss = value;
    break;
  }
}

double Loss::GetAttr(uint32_t attr) {
  switch(attr) {
  case P_IN:
    return this->p_in_branch;
    break;
  case P_OUT:
    return this->p_out_branch;
    break;
  case Q_IN:
    return this->q_in_branch;
    break;
  case Q_OUT:
    return this->q_out_branch;
    break;
  case LOSS:
    return this->loss;
    break;
  }

  return 0;
}

}
