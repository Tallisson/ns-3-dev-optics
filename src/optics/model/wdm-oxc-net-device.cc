/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * WdmOxc; // abstract base class
 * WdmOpaqueOxc;
 * WdmTransparentOxc;
 * WdmTranslucentOxc;
 */

#include "wdm-oxc-net-device.h"
#include "wdm-passthrough-net-device.h"
#include "wdm-unidirectional-channel.h"
#include "wdm-control-net-device.h"

#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/type-id.h"
#include "ns3/traced-callback.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/packet.h"
#include "ns3/ppp-header.h"
#include "ns3/simulator.h"

#include <string>
#include <map>

NS_LOG_COMPONENT_DEFINE ("WdmOxcNetDevice");

namespace ns3 {

TypeId
WdmOxcNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmOxcNetDevice")
      .SetParent<WdmPassthroughNetDevice> ()
      .AddConstructor<WdmOxcNetDevice> ()
      /*.AddAttribute ("Address",
                     "The MAC address of this device.",
                     Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                     MakeMac48AddressAccessor (&WdmOxcNetDevice::m_address),
                     MakeMac48AddressChecker ())
      .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                     UintegerValue (1500),
                     MakeUintegerAccessor (&WdmOxcNetDevice::SetMtu,
                                           &WdmOxcNetDevice::GetMtu),
                     MakeUintegerChecker<uint16_t> ())
      .AddAttribute ("ReceiveErrorModel",
                     "The receiver error model used to simulate packet loss",
                     PointerValue (),
                     MakePointerAccessor (&WdmOxcNetDevice::m_receiveErrorModel),
                     MakePointerChecker<ErrorModel> ())*/
      ;
  return tid;
}

WdmOxcNetDevice::WdmOxcNetDevice (uint32_t numInputs, uint32_t numOutputs)
  :
    WdmPassthroughNetDevice (numInputs, numOutputs)
{
  NS_LOG_FUNCTION (this << numInputs << numOutputs);
}

WdmOxcNetDevice::~WdmOxcNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WdmOxcNetDevice::SetCrossConnectionTableEntry (std::pair<uint32_t, uint64_t> source, std::pair<uint32_t, uint64_t> dest)
{
  NS_LOG_FUNCTION (this << source.first << source.second << dest.first << dest.second);
  m_crossConnectionTable.insert(std::make_pair(source, dest));
}

void
WdmOxcNetDevice::UnsetCrossConnectionTableEntry (std::pair<uint32_t, uint64_t> source)
{
  NS_LOG_FUNCTION (this << source.first << source.second);
  m_crossConnectionTable.erase (source);
}

std::pair<uint32_t, uint64_t>
WdmOxcNetDevice::GetDestinationForSource (std::pair<uint32_t, uint64_t> source) const
{
  NS_LOG_FUNCTION (this << source.first << source.second);
  std::map<std::pair<uint32_t, uint64_t>, std::pair<uint32_t, uint64_t> >::const_iterator iter = m_crossConnectionTable.find (source);
  if (iter != m_crossConnectionTable.end())
    {
      return iter->second;
    }
  else
    {
      return std::make_pair (m_outputPhyInterfaces.size (), 0);
    }
}

std::pair<uint32_t, uint64_t>
WdmOxcNetDevice::GetSourceForDestination (std::pair<uint32_t, uint64_t> destination) const
{
  NS_LOG_FUNCTION (this << destination.first << destination.second);
  for (std::map<std::pair<uint32_t, uint64_t>, std::pair<uint32_t, uint64_t> >::const_iterator it = m_crossConnectionTable.begin (); it != m_crossConnectionTable.end (); it++)
    {
      if (it->second == destination)
        {
          return it->first;
        }
      else
        {
          return std::make_pair (m_inputPhyInterfaces.size (), 0);
        }
    }
  // FIXME: to make compiler happy
  return std::make_pair (m_inputPhyInterfaces.size (), 0);
}

void
WdmOxcNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << inputPhy << signalFrequency);
  uint32_t phyIfIndex = GetInputPhyIfIndex (inputPhy->GetObject<WdmInputPhy> ());
  NS_LOG_INFO ("WdmOxcNetDevice " << this << " received packet " << packet << " at input physical interface index " << phyIfIndex << " signal frequency " << signalFrequency);
  /*if (signalFrequency == 197300000000000)
    {
      uint16_t protocol;
      ProcessHeader (packet, protocol);
      m_rxCallback (this, packet, protocol, inputPhy->GetOutputPhy ()->GetDevice ()->GetAddress ());
    }*/
  std::pair<uint32_t, uint64_t> entry = GetDestinationForSource (std::make_pair (phyIfIndex, signalFrequency));
  if (entry.first < m_outputPhyInterfaces.size ())
    {
      GetOutputPhyInterface (entry.first)->SendPacket (packet, entry.second);
      NS_LOG_INFO ("WdmOxcNetDevice " << this << " transmitted packet " << packet << " at PHY interface index " << entry.first << " signal frequency " << entry.second);
    }
  else
    {
      NS_LOG_INFO ("Cross connection table entry for input physical interface index " << phyIfIndex << " signal frequency " << signalFrequency << " is missing, droppping packet " << packet);
      // m_rxDropTrace... FIXME TODO
    }
}

void
WdmOxcNetDevice::NotifyChannelStopped (Ptr<WdmPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  //FIXME NOWm_centralControl->NotifyChannelStopped ();
  /*
  uint32_t index;
  if (GetInputPhyIfIndex (phy->GetObject<WdmInputPhy> ()) < m_inputPhyInterfaces.size ())
    {
      index = GetInputPhyIfIndex (phy->GetObject<WdmInputPhy> ());
      NS_LOG_INFO ("Input physical interface " << phy << " index is " << index);
    }
  else if (GetOutputPhyIfIndex (phy->GetObject<WdmOutputPhy> ()) < m_outputPhyInterfaces.size ())
    {
      index = GetOutputPhyIfIndex (phy->GetObject<WdmOutputPhy> ());
      NS_LOG_INFO ("Output physical interface " << phy << " index is " << index);
      //std::pair<uint32_t, uint64_t> source = GetSourceForDestination (std::make_pair (index, 190100000000000));
    }
  else
    {
      NS_LOG_ERROR ("Physical interface " << phy << " is neither input nor output physical interface.");
    }
    */
}

void
WdmOxcNetDevice::SetCentralControl (Ptr<WdmOxcCentralControl> control)
{
  NS_LOG_FUNCTION (this << control);
  m_centralControl = control;
}

} // namespace ns3
