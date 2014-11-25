#ifndef WDM_MUX_NET_DEVICE_H
#define WDM_MUX_NET_DEVICE_H

#include "wdm-passthrough-net-device.h"

#include <map>

namespace ns3
{

class WdmMuxNetDevice : public WdmPassthroughNetDevice
{
public:
  WdmMuxNetDevice (int numInputs = 8);
  ~WdmMuxNetDevice ();

  virtual void Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency);

private:
  double m_attenuation;
};

}

#endif // WDM_MUX_NET_DEVICE_H
