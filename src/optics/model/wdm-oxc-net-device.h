/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WDM_OXC_NET_DEVICE_H
#define WDM_OXC_NET_DEVICE_H

#include "wdm-passthrough-net-device.h"

namespace ns3 {

class WdmOxcCentralControl;
class WdmUnidirectionalChannel;

class WdmOxcNetDevice : public WdmPassthroughNetDevice
{
public:
  static TypeId GetTypeId (void);

  WdmOxcNetDevice (uint32_t numInputs = 4, uint32_t numOutputs = 4); /* FIXME */
  virtual ~WdmOxcNetDevice ();

  virtual void Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency);

  void SetCrossConnectionTableEntry (std::pair<uint32_t, uint64_t> source, std::pair<uint32_t, uint64_t> dest);
  void UnsetCrossConnectionTableEntry (std::pair<uint32_t, uint64_t> source);
  std::pair<uint32_t, uint64_t> GetDestinationForSource (std::pair<uint32_t, uint64_t> source) const;
  std::pair<uint32_t, uint64_t> GetSourceForDestination (std::pair<uint32_t, uint64_t> destination) const;

  virtual void NotifyChannelStopped (Ptr<WdmPhy> phy);
  void SetCentralControl (Ptr<WdmOxcCentralControl> control);

private:
  std::map<std::pair<uint32_t, uint64_t>, std::pair<uint32_t, uint64_t> > m_crossConnectionTable;
  //std::vector<Ptr<WdmSwitchNetDevice> > m_wdmSwitchNetDevices;
  Ptr<WdmOxcCentralControl> m_centralControl;
};

} // namespace ns3

#endif // WDM_OXC_NET_DEVICE_H
