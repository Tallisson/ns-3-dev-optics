/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * TODO:
 * WdmMux;
 * WdmDemux;
 * WdmAddDropMux;
 */

#include "wdm-passthrough-net-device.h"
#include "wdm-oxc-net-device.h"
#include "wdm-channel.h"

#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/packet.h"

#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("WdmPassthroughNetDevice");

namespace ns3
{

TypeId
WdmPassthroughNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmTransportNetDevice")
      .SetParent<WdmNetDevice> ()
      .AddAttribute ("Address",
                     "The MAC address of this device.",
                     Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                     MakeMac48AddressAccessor (&WdmPassthroughNetDevice::m_address),
                     MakeMac48AddressChecker ())
      .AddAttribute ("ReceiveErrorModel",
                     "The receiver error model used to simulate packet loss",
                     PointerValue (),
                     MakePointerAccessor (&WdmPassthroughNetDevice::m_receiveErrorModel),
                     MakePointerChecker<ErrorModel> ())
      ;
  return tid;
}

WdmPassthroughNetDevice::WdmPassthroughNetDevice (uint32_t numInputPhyInterfaces, uint32_t numOutputPhyInterfaces)
  :
    WdmNetDevice (numInputPhyInterfaces, numOutputPhyInterfaces)
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmPassthroughNetDevice::WdmPassthroughNetDevice (uint32_t numInputOutputPhyInterfaces)
  :
    WdmNetDevice (numInputOutputPhyInterfaces)
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmPassthroughNetDevice::~WdmPassthroughNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

bool
WdmPassthroughNetDevice::SetMtu (uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  return false;
}

uint16_t
WdmPassthroughNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return 0;
}

bool
WdmPassthroughNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}

bool
WdmPassthroughNetDevice::Send (Ptr<Packet> packet,
                    const Address &dest,
                    uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  //AddHeader (packet, protocolNumber);
  //m_outputPhyInterfaces[0]->SendPacket (packet, 197300000000000); /* TODO */
  return false;
}

bool
WdmPassthroughNetDevice::SendFrom (Ptr<Packet> packet,
                        const Address &source,
                        const Address &dest,
                        uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << source << dest << protocolNumber);
  return false;
}

bool
WdmPassthroughNetDevice::SupportsSendFrom (void) const
{
  return false;
}

Ptr<Node>
WdmPassthroughNetDevice::GetNode (void) const
{
  return m_node;
}

void
WdmPassthroughNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool
WdmPassthroughNetDevice::NeedsArp (void) const
{
  return false; // FIXME je li tako?
}

void
WdmPassthroughNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
WdmPassthroughNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  m_promiscCallback = cb;
}

} // namespace ns3
