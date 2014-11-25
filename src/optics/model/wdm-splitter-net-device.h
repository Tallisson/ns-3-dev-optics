#ifndef WDM_SPLITTER_NETDEVICE_H
#define WDM_SPLITTER_NETDEVICE_H

#include "wdm-passthrough-net-device.h"

namespace ns3
{
class WdmSplitterNetDevice : public WdmPassthroughNetDevice
{
public:
  WdmSplitterNetDevice (int numOutputs = 2);
  ~WdmSplitterNetDevice ();

  virtual void Receive (Ptr<Packet> packet, Ptr<WdmInputPhy> inputPhy, uint64_t signalFrequency);
  virtual bool Send (Ptr<Packet> packet, Ptr<WdmOutputPhy> outputPhy, uint64_t signalFrequency);

private:
  double m_attenuation;
};

}

#endif // WDM_SPLITTER_NETDEVICE_H
