/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WDM_SWITCH_NET_DEVICE_H
#define WDM_SWITCH_NET_DEVICE_H

#include "wdm-phy.h"
#include "wdm-net-device.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/traced-callback.h"

#include <string>
#include <map>

#include <stdint.h>

namespace ns3 {

class WdmOxcNetDevice;

class WdmPassthroughNetDevice : public WdmNetDevice
{
public:
  static TypeId GetTypeId (void);

  WdmPassthroughNetDevice (uint32_t numInputPhyInterfaces, uint32_t numOutputPhyInterfaces);
  WdmPassthroughNetDevice (uint32_t numInputOutputPhyInterfaces);
  virtual ~WdmPassthroughNetDevice ();

  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;

  virtual bool IsLinkUp (void) const;

  virtual bool Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);

  virtual bool SupportsSendFrom (void) const;

  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);

  virtual bool NeedsArp (void) const;

  virtual void SetReceiveCallback (ReceiveCallback cb);

  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);

protected:

};

} // namespace ns3

#endif // WDM_SWITCH_NET_DEVICE_H
