/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */ /*
 * WdmNetDevice;
 */

#include "wdm-net-device.h"
#include "wdm-bidirectional-channel.h"
#include "wdm-unidirectional-channel.h"

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/ppp-header.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"

#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("WdmNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WdmNetDevice);

TypeId
WdmNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmNetDevice")
    .SetParent<NetDevice> ()
  ;
  return tid;
}

WdmNetDevice::WdmNetDevice (uint32_t numInputPhyInterfaces, uint32_t numOutputPhyInterfaces)
  :
    NetDevice ()
{
  NS_LOG_FUNCTION (this << numInputPhyInterfaces << numOutputPhyInterfaces);
  for (uint32_t i = 0; i < numInputPhyInterfaces; i++)
    {
      Ptr<WdmInputPhy> inputPhyInterface = CreateObject<WdmInputPhy> ();
      inputPhyInterface->SetDevice (this);
      m_inputPhyInterfaces.push_back (inputPhyInterface);
    }
  for (uint32_t i = 0; i < numOutputPhyInterfaces; i++)
    {
      Ptr<WdmOutputPhy> outputPhyInterface = CreateObject<WdmOutputPhy> ();
      outputPhyInterface->SetDevice (this);
      m_outputPhyInterfaces.push_back (outputPhyInterface);
    }
}

WdmNetDevice::WdmNetDevice (uint32_t numInputOutputPhyInterfaces)
    :
      NetDevice ()
{
    NS_LOG_FUNCTION (this << numInputOutputPhyInterfaces);
    for (uint32_t i = 0; i < numInputOutputPhyInterfaces; i++)
      {
        Ptr<WdmInputOutputPhy> inputOutputPhyInterfaces = CreateObject<WdmInputOutputPhy> ();
        inputOutputPhyInterfaces->SetDevice (this);
        m_inputOutputPhyInterfaces.push_back (inputOutputPhyInterfaces);
      }

}


WdmNetDevice::~WdmNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}




uint32_t
WdmNetDevice::GetInputPhyIfIndex (Ptr<WdmInputPhy> inputPhyInterface) const
{
  NS_LOG_FUNCTION (this << inputPhyInterface);
  return std::find (m_inputPhyInterfaces.begin(), m_inputPhyInterfaces.end(), inputPhyInterface) - m_inputPhyInterfaces.begin();
}

uint32_t
WdmNetDevice::GetOutputPhyIfIndex (Ptr<WdmOutputPhy> outputPhyInterface) const
{
  NS_LOG_FUNCTION (this << outputPhyInterface);
  /*
  for (uint32_t i = 0; i < m_outputPhyInterfaces.size (); i++)
    {
      NS_LOG_UNCOND ("OutputPhyIf[" << i << "] " << m_outputPhyInterfaces[i]);
      NS_LOG_UNCOND ("OutputPhyIface " << outputPhyInterface);
      if (m_outputPhyInterfaces[i] == outputPhyInterface)
        {
          return i;
        }
      else
        {
          NS_LOG_UNCOND ("wut");
        }
    }
  */
  return std::find (m_outputPhyInterfaces.begin(), m_outputPhyInterfaces.end(), outputPhyInterface) - m_outputPhyInterfaces.begin();
}

Ptr<WdmInputPhy>
WdmNetDevice::GetInputPhyInterface (uint32_t index) const
{
  return m_inputPhyInterfaces[index];
}

Ptr<WdmOutputPhy>
WdmNetDevice::GetOutputPhyInterface (uint32_t index) const
{
  return m_outputPhyInterfaces[index];
}

uint32_t
WdmNetDevice::GetNInputPhyInterfaces () const
{
  return m_inputPhyInterfaces.size ();
}

uint32_t
WdmNetDevice::GetNOutputPhyInterfaces () const
{
  return m_outputPhyInterfaces.size ();
}

bool
WdmNetDevice::AttachInput (Ptr<WdmUnidirectionalChannel> inputCh, uint32_t index)
{
  NS_LOG_FUNCTION (this << inputCh << index);
  /* FALI NS_ASSERT */
  inputCh->Attach (m_inputPhyInterfaces[index]);
  return true;
}

bool
WdmNetDevice::AttachInput (Ptr<WdmUnidirectionalChannel> inputCh)
{
  NS_LOG_FUNCTION (this << inputCh);
  uint32_t index = m_inputPhyInterfaces.size ();
  for (std::vector<Ptr<WdmInputPhy> >::iterator it = m_inputPhyInterfaces.begin (); it != m_inputPhyInterfaces.end (); it++)
    {
      if ((*it)->GetChannel () == 0)
        {
          index = it - m_inputPhyInterfaces.begin();
          NS_LOG_INFO ("Attaching channel " << inputCh << " to input physical interface " << *it << " with index " << index);
          AttachInput (inputCh, index);
          return true;
        }
    }
  return false;
}

bool
WdmNetDevice::AttachOutput (Ptr<WdmUnidirectionalChannel> outputCh, uint32_t index)
{
  NS_LOG_FUNCTION (this << outputCh << index);
  outputCh->Attach (m_outputPhyInterfaces[index]);
  return true;
}

bool
WdmNetDevice::AttachOutput (Ptr<WdmUnidirectionalChannel> outputCh)
{
  NS_LOG_FUNCTION (this << outputCh);
  uint32_t index = m_outputPhyInterfaces.size ();
  for (std::vector<Ptr<WdmOutputPhy> >::iterator it = m_outputPhyInterfaces.begin (); it != m_outputPhyInterfaces.end (); it++)
    {
      if ((*it)->GetChannel () == 0)
        {
          index = it - m_outputPhyInterfaces.begin();
          NS_LOG_INFO ("Attaching channel " << outputCh << " to output physical interface " << *it << " with index " << index);
          AttachOutput (outputCh, index);
          return true;
        }
    }
  return false;
}



bool
WdmNetDevice::AttachInputOutput (Ptr<WdmBidirectionalChannel> inputOutputCh, uint32_t index)
{
  NS_LOG_FUNCTION (this << inputOutputCh << index);
  /* FALI NS_ASSERT */
  inputOutputCh->Attach (m_inputOutputPhyInterfaces[index]);
  return true;
}

bool
WdmNetDevice::AttachInputOutput (Ptr<WdmBidirectionalChannel> inputOutputCh)
{
  NS_LOG_FUNCTION (this << inputOutputCh);
  uint32_t index = m_inputOutputPhyInterfaces.size ();
  for (std::vector<Ptr<WdmInputOutputPhy> >::iterator it = m_inputOutputPhyInterfaces.begin (); it != m_inputOutputPhyInterfaces.end (); it++)
    {
      if ((*it)->GetChannel () == 0)
        {
          index = it - m_inputOutputPhyInterfaces.begin();
          AttachInputOutput (inputOutputCh, index);
          return true;
        }
    }
  return false;

}

void
WdmNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_ifIndex = index;
}

uint32_t
WdmNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifIndex;
}

Ptr<Channel>
WdmNetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return 0;
}

void
WdmNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_address = Mac48Address::ConvertFrom (address);
}

Address
WdmNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_address;
}

void
WdmNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_linkChangeCallbacks.ConnectWithoutContext (callback);
}

bool
WdmNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
WdmNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
WdmNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
WdmNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  // TODO
  NS_LOG_FUNCTION (this << multicastGroup);
  return Mac48Address ("01:00:5e:00:00:00");
}

Address
WdmNetDevice::GetMulticast (Ipv6Address addr) const
{
  // TODO
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address ("33:33:00:00:00:00");
}

bool
WdmNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}

bool
WdmNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}
Ptr<Node>
WdmNetDevice::GetNode (void) const
{
  return m_node;
}

void
WdmNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool
WdmNetDevice::NeedsArp (void) const
{
  return false;
}

void
WdmNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
WdmNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  m_promiscCallback = cb;
}

void
WdmNetDevice::AddPppHeader (Ptr<Packet> packet, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << protocolNumber);
  PppHeader ppp;
  switch(protocolNumber)
    {
    case 0x0800: ppp.SetProtocol (0x0021); break; //IPv4
    case 0x86DD: ppp.SetProtocol (0x0057); break; //IPv6
    default: NS_ASSERT_MSG (false, "PPP Protocol number not defined!");
    }

  packet->AddHeader (ppp);
}

bool
WdmNetDevice::ProcessPppHeader (Ptr<Packet> packet, uint16_t& protocolNumber)
{
  NS_LOG_FUNCTION (packet << protocolNumber);
  PppHeader ppp;
  packet->RemoveHeader (ppp);
  switch(ppp.GetProtocol ())
    {
    case 0x0021: protocolNumber = 0x0800; break; //IPv4
    case 0x0057: protocolNumber = 0x86DD; break; //IPv6
    default: NS_ASSERT_MSG (false, "PPP Protocol number not defined!");
    }

  return true;
}

void
WdmNetDevice::AddEthernetHeader (Ptr<Packet> packet, Mac48Address source, Mac48Address dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << source << dest << protocolNumber);

  EthernetHeader header (false);
  header.SetSource (source);
  header.SetDestination (dest);

  EthernetTrailer trailer;

  NS_LOG_LOGIC ("p->GetSize () = " << packet->GetSize ());
  NS_LOG_LOGIC ("m_mtu = " << GetMtu ());

        NS_LOG_LOGIC ("Encapsulating packet as LLC (length interpretation)");

        LlcSnapHeader llc;
        llc.SetType (protocolNumber);
        packet->AddHeader (llc);

        //
        // This corresponds to the length interpretation of the lengthType
        // field but with an LLC/SNAP header added to the payload as in
        // IEEE 802.2
        //
          uint16_t lengthType = packet->GetSize ();

        //
        // All Ethernet frames must carry a minimum payload of 46 bytes.  The
        // LLC SNAP header counts as part of this payload.  We need to padd out
        // if we don't have enough bytes.  These must be real bytes since they
        // will be written to pcap files and compared in regression trace files.
        //
        if (packet->GetSize () < 46)
          {
            uint8_t buffer[46];
            memset (buffer, 0, 46);
            Ptr<Packet> padd = Create<Packet> (buffer, 46 - packet->GetSize ());
            NS_LOG_LOGIC ("PDU "<<46 - packet->GetSize ());
            packet->AddAtEnd (padd);

          }

        NS_ASSERT_MSG (packet->GetSize () <= GetMtu (),
                       "CsmaNetDevice::AddHeader(): 802.3 Length/Type field with LLC/SNAP: "
                       "length interpretation must not exceed device frame size minus overhead");

  NS_LOG_LOGIC ("header.SetLengthType (" << lengthType << ")");
  header.SetLengthType (lengthType);
  packet->AddHeader (header);

  if (Node::ChecksumEnabled ())
    {
      trailer.EnableFcs (true);
    }
  trailer.CalcFcs (packet);
  packet->AddTrailer (trailer);
}

bool
WdmNetDevice::ProcessEthernetHeader (Ptr<Packet> packet, Mac48Address& source, Mac48Address& dest, uint16_t& protocolNumber)
{
  NS_LOG_FUNCTION (packet << source << dest << protocolNumber);
  uint16_t protocol = 0;
  NS_LOG_UNCOND ("Packet size before " << packet->GetSize ());

  //
  // Trace sinks will expect complete packets, not packets without some of the
  // headers.
  //
  //Ptr<Packet> originalPacket = packet->Copy ();

  EthernetTrailer trailer;
  packet->RemoveTrailer (trailer);

  if (Node::ChecksumEnabled ())
    {
      trailer.EnableFcs (true);
    }

  trailer.CheckFcs (packet);
  bool crcGood = trailer.CheckFcs (packet);
  if (!crcGood)
    {
      NS_LOG_INFO ("CRC error on Packet " << packet);
      //m_phyRxDropTrace (packet);
      return false;
    }

  EthernetHeader header (false);
  packet->RemoveHeader (header);

  NS_LOG_LOGIC ("Pkt source is " << header.GetSource ());
  NS_LOG_LOGIC ("Pkt destination is " << header.GetDestination ());


  //
  // If the length/type is less than 1500, it corresponds to a length
  // interpretation packet.  In this case, it is an 802.3 packet and
  // will also have an 802.2 LLC header.  If greater than 1500, we
  // find the protocol number (Ethernet type) directly.
  //

  NS_LOG_UNCOND ("Packet size after " << header.GetLengthType () << " " << packet->GetSize ());
      NS_ASSERT (packet->GetSize () >= header.GetLengthType ());
      uint32_t padlen = packet->GetSize () - header.GetLengthType ();
      NS_ASSERT (padlen <= 46);
      if (padlen > 0)
        {
          packet->RemoveAtEnd (padlen);
        }

      LlcSnapHeader llc;
      packet->RemoveHeader (llc);
      protocol = llc.GetType ();

  //
  // For all kinds of packetType we receive, we hit the promiscuous sniffer
  // hook and pass a copy up to the promiscuous callback.  Pass a copy to
  // make sure that nobody messes with our packet.
  //
  /*m_promiscSnifferTrace (originalPacket);
  if (!m_promiscRxCallback.IsNull ())
    {
      m_macPromiscRxTrace (originalPacket);
      m_promiscRxCallback (this, packet, protocol, header.GetSource (), header.GetDestination (), packetType);
    }*/

  //
  // If this packet is not destined for some other host, it must be for us
  // as either a broadcast, multicast or unicast.  We need to hit the mac
  // packet received trace hook and forward the packet up the stack.
  //

      //m_snifferTrace (originalPacket);
      //m_macRxTrace (originalPacket);
      m_rxCallback (this, packet, protocol, header.GetSource ());
  return true;
}

void
WdmNetDevice::NotifyChannelStopped (Ptr<WdmPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
}


} // namespace ns3
