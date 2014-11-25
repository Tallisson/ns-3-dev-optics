/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WDM_NET_DEVICE_H
#define WDM_NET_DEVICE_H

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/data-rate.h"
#include "ns3/error-model.h"
#include "ns3/uinteger.h"
#include "ns3/traced-callback.h"
#include "ns3/node.h"
#include "ns3/ppp-header.h"

#include <vector>

#include <stdint.h>

namespace ns3 {

class WdmBidirectionalChannel;
class WdmUnidirectionalChannel;
class WdmPhy;
class WdmInputPhy;
class WdmOutputPhy;
class WdmInputOutputPhy;
class Queue;

class WdmNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);
  WdmNetDevice (uint32_t numInputPhyInterfaces, uint32_t numOutputPhyInterfaces); /* FIXME */
  WdmNetDevice (uint32_t numInputOutputPhyInterfaces);
  virtual ~WdmNetDevice ();


  virtual void Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency) = 0;
  //virtual void SendOut (Ptr<Packet> packet, Ptr<WdmPhy> outputPhy, uint64_t signalFrequency) = 0;

  virtual Ptr<Channel> GetChannel (void) const;

  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;

  virtual void AddLinkChangeCallback (Callback<void> callback);

  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;

  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual Address GetMulticast (Ipv6Address addr) const;

  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;

  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);

  virtual bool NeedsArp (void) const;

  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);

  virtual uint32_t GetInputPhyIfIndex (Ptr<WdmInputPhy> inputPhyInterface) const;
  virtual uint32_t GetOutputPhyIfIndex (Ptr<WdmOutputPhy> outputPhyInterface) const;
  //virtual uint32_t GetInputOutputIfIndex (Ptr<WdmInputOutputPhy> inputOutputPhyInterface) const; TODO
  virtual Ptr<WdmInputPhy> GetInputPhyInterface (uint32_t index) const;
  virtual Ptr<WdmOutputPhy> GetOutputPhyInterface (uint32_t index) const;
  //virtual Ptr<WdmInputOutputPhy> GetInputOutputPhyInterface (uint32_t index) const; TODO

  virtual uint32_t GetNInputPhyInterfaces () const;
  virtual uint32_t GetNOutputPhyInterfaces () const;

  virtual bool AttachInput (Ptr<WdmUnidirectionalChannel> inputCh, uint32_t index);
  virtual bool AttachInput (Ptr<WdmUnidirectionalChannel> ch);
  virtual bool AttachOutput (Ptr<WdmUnidirectionalChannel> outputCh, uint32_t index);
  virtual bool AttachOutput (Ptr<WdmUnidirectionalChannel> ch);
  virtual bool AttachInputOutput (Ptr<WdmBidirectionalChannel> inputOutputCh, uint32_t index);
  virtual bool AttachInputOutput (Ptr<WdmBidirectionalChannel> inputOutputCh);

  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;

  void AddPppHeader (Ptr<Packet> packet, uint16_t protocolNumber);
  bool ProcessPppHeader (Ptr<Packet> packet, uint16_t& protocolNumber);
  void AddEthernetHeader (Ptr<Packet> packet, Mac48Address source, Mac48Address dest, uint16_t protocolNumber);
  bool ProcessEthernetHeader (Ptr<Packet> packet, Mac48Address& source, Mac48Address& dest, uint16_t& protocolNumber);

  virtual void NotifyChannelStopped (Ptr<WdmPhy> phy);

protected:
  uint32_t m_ifIndex;
  std::vector<Ptr<WdmInputPhy> > m_inputPhyInterfaces;
  std::vector<Ptr<WdmOutputPhy> > m_outputPhyInterfaces;
  std::vector<Ptr<WdmInputOutputPhy> > m_inputOutputPhyInterfaces;
  Mac48Address m_address;
  TracedCallback<> m_linkChangeCallbacks;
  Ptr<ErrorModel> m_receiveErrorModel;
  Ptr<Node> m_node;
  NetDevice::ReceiveCallback m_rxCallback;
  NetDevice::PromiscReceiveCallback m_promiscCallback;
  bool m_workingState;
};

} // namespace ns3

#endif // WDM_NET_DEVICE_H
