#ifndef WDM_DEMUX_NET_DEVICE_H
#define WDM_DEMUX_NET_DEVICE_H

#include "wdm-passthrough-net-device.h"

#include <map>

class WdmInputPhy;
class WdmOutputPhy;

namespace ns3
{

class WdmDemuxNetDevice : public WdmPassthroughNetDevice
{
public:
  WdmDemuxNetDevice (int numOutputs = 8);
  ~WdmDemuxNetDevice ();

  virtual void Receive (Ptr<Packet> packet, Ptr<WdmInputPhy> inputPhy, uint64_t signalFrequency);
  virtual bool Send (Ptr<Packet> packet, Ptr<WdmOutputPhy> outputPhy, uint64_t signalFrequency);

private:
  double m_attenuation;
  std::map<uint32_t, std::pair<uint64_t, uint64_t> > m_outputFrequencyAssignement;
};

}

#endif // WDM_DEMUX_NET_DEVICE_H
