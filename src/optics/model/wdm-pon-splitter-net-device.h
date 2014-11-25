#ifndef WDM_PON_SPLITTER_NET_DEVICE_H
#define WDM_PON_SPLITTER_NET_DEVICE_H

#include "wdm-passthrough-net-device.h"

namespace ns3 {

class WdmPonSplitterNetDevice : public WdmPassthroughNetDevice
{
public:
  static TypeId GetTypeId (void);

  WdmPonSplitterNetDevice (uint32_t numInputOutputs = 4); /* FIXME */
  virtual ~WdmPonSplitterNetDevice ();


  virtual void Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputOutputPhy, uint64_t signalFrequency);

};

} // namespace ns3

#endif // WDM_SPLITTER_NET_DEVICE_H
