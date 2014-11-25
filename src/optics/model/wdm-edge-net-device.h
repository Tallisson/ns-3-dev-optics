/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WDM_HOST_NET_DEVICE_H
#define WDM_HOST_NET_DEVICE_H

#include "wdm-net-device.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/data-rate.h"
#include "ns3/error-model.h"
#include "ns3/uinteger.h"
#include "ns3/traced-callback.h"
#include "ns3/node.h"

#include <vector>

#include <stdint.h>

namespace ns3 {

class WdmUnidirectionalChannel;
class WdmPhy;
class Queue;

class WdmEdgeNetDevice : public WdmNetDevice
{
public:
  static TypeId GetTypeId (void);

  WdmEdgeNetDevice ();
  virtual ~WdmEdgeNetDevice ();

  bool Attach (Ptr<WdmUnidirectionalChannel> inputCh, Ptr<WdmUnidirectionalChannel> outputCh);
  bool AttachInput (Ptr<WdmUnidirectionalChannel> inputCh);
  bool AttachOutput (Ptr<WdmUnidirectionalChannel> outputCh);
  void CheckIsLinkUp (void);

  /*void SetQueue (Ptr<Queue> queue);
  Ptr<Queue> GetQueue (void) const;*/

  virtual void Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency);

  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;

  virtual bool IsLinkUp (void) const;

  virtual bool Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);

  virtual bool SupportsSendFrom (void) const;

  void SetSignalFrequency (uint64_t signalFrequency);
  uint64_t GetSignalFrequency (void) const;

private:
  uint16_t m_mtu;
  uint64_t m_signalFrequency;
  bool m_linkUp;
  Ptr<Queue> m_queue;
  DataRate m_bps;
};

} // namespace ns3

#endif // WDM_HOST_NET_DEVICE_H
