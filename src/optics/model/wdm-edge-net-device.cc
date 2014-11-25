/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "wdm-edge-net-device.h"

#include "wdm-net-device.h"
#include "wdm-unidirectional-channel.h"
#include "wdm-bidirectional-channel.h"

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("WdmEdgeNetDevice");

namespace ns3 {

TypeId
WdmEdgeNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmEdgeNetDevice")
    .SetParent<WdmNetDevice> ()
    .AddConstructor<WdmEdgeNetDevice> ()
    .AddAttribute ("Address",
                   "The MAC address of this device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&WdmEdgeNetDevice::m_address),
                   MakeMac48AddressChecker ())
    .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&WdmEdgeNetDevice::SetMtu,
                                         &WdmEdgeNetDevice::GetMtu),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("DataRate",
                   "The default data rate for each wavelength channel",
                   DataRateValue (DataRate ("10Gbps")),
                   MakeDataRateAccessor (&WdmEdgeNetDevice::m_bps),
                   MakeDataRateChecker ())
    .AddAttribute ("ReceiveErrorModel",
                   "The receiver error model used to simulate packet loss",
                   PointerValue (),
                   MakePointerAccessor (&WdmEdgeNetDevice::m_receiveErrorModel),
                   MakePointerChecker<ErrorModel> ())
    .AddAttribute ("TxQueue",
                   "A queue to use as the transmit queue in the device",
                   PointerValue (),
                   MakePointerAccessor (&WdmEdgeNetDevice::m_queue),
                   MakePointerChecker<Queue> ())
    .AddAttribute ("SignalFrequency",
                   "Signal frequency used by edge net device",
                   UintegerValue (190100000000000),
                   MakeUintegerAccessor (&WdmEdgeNetDevice::SetSignalFrequency,
                                         &WdmEdgeNetDevice::GetSignalFrequency),
                   MakeUintegerChecker<uint64_t> ())
  ;
  return tid;
}

WdmEdgeNetDevice::WdmEdgeNetDevice ()
  :
    WdmNetDevice (1, 1)
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmEdgeNetDevice::~WdmEdgeNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_queue = 0;
  /* TODO isprazni listu Phy */
}

bool
WdmEdgeNetDevice::Attach (Ptr<WdmUnidirectionalChannel> inputCh, Ptr<WdmUnidirectionalChannel> outputCh)
{
  NS_LOG_FUNCTION (this << inputCh << outputCh);
  AttachInput (inputCh);
  AttachOutput (outputCh);
  return true;
}

bool
WdmEdgeNetDevice::AttachInput (Ptr<WdmUnidirectionalChannel> inputCh)
{
  NS_LOG_FUNCTION (this << inputCh);
  inputCh->Attach (m_inputPhyInterfaces[0]); /* TODO ako je više sučelja nađi prvo koje nije spojeno */
  //CheckIsLinkUp ();
  return true;
}

bool
WdmEdgeNetDevice::AttachOutput (Ptr<WdmUnidirectionalChannel> outputCh)
{
  NS_LOG_FUNCTION (this << outputCh);
  outputCh->Attach (m_outputPhyInterfaces[0]); /* TODO ako je više sučelja nađi prvo koje nije spojeno */
  //CheckIsLinkUp ();
  return true;
}

/*void
WdmEdgeNetDevice::CheckIsLinkUp (void)
{
  if (m_inputPhyInterfaces[0] != 0 && m_outputPhyInterfaces[0] != 0)
    {
      m_linkUp = true;
    }
  //m_linkChangeCallbacks (); TODO
}*/

void
WdmEdgeNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << inputPhy << signalFrequency);
  NS_LOG_INFO ("WdmEdgeNetDevice receiving packet with UID " << packet->GetUid () << " size " << packet->GetSize ());

  uint16_t protocol = 0;

  if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt (packet))
    {
      //
      // If we have an error model and it indicates that it is time to lose a
      // corrupted packet, don't forward this packet up, let it go.
      //
      /* TODO: m_phyRxDropTrace (packet); */
    }
  else
    {
      //
      // Hit the trace hooks.  All of these hooks are in the same place in this
      // device becuase it is so simple, but this is not usually the case in
      // more complicated devices.
      //
      /*
      m_snifferTrace (packet);
      m_promiscSnifferTrace (packet);
      m_phyRxEndTrace (packet);
      TODO */

      //
      // Strip off the point-to-point protocol header and forward this packet
      // up the protocol stack.  Since this is a simple point-to-point link,
      // there is no difference in what the promisc callback sees and what the
      // normal receive callback sees.
      //
      ProcessPppHeader (packet, protocol);

      if (!m_promiscCallback.IsNull ())
        {
          //m_macPromiscRxTrace (packet);
          m_promiscCallback (this, packet, protocol, GetAddress (), GetAddress (), NetDevice::PACKET_HOST); /* VELIKI MASNI FIXME */
        }

      //m_macRxTrace (packet);
      m_rxCallback (this, packet, protocol, GetAddress ()); /* VELIKI MASNI FIXME */
    }
}

bool
WdmEdgeNetDevice::Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  NS_LOG_INFO ("WdmEdgeNetDevice sending packet with UID " << packet->GetUid () << " size " << packet->GetSize ());

  //
  // If IsLinkUp() is false it means there is no channel to send any packet
  // over so we just hit the drop trace on the packet and return an error.
  //
  /*if (IsLinkUp () == false)
    {
      //m_macTxDropTrace (packet); TODO
      return false;
    }*/

  //
  // Stick a point to point protocol header on the packet in preparation for
  // shoving it out the door.
  //
  AddPppHeader (packet, protocolNumber);

  //m_macTxTrace (packet); TODO

  //
  // If there's a transmission in progress, we enque the packet for later
  // transmission; otherwise we send it now.
  //
  if (true) /* TODO m_txMachineState == READY, odnosno MPLS put uspostavljen */
    {
      //
      // Even if the transmitter is immediately available, we still enqueue and
      // dequeue the packet to hit the tracing hooks.
      //
      if (m_queue->Enqueue (packet) == true)
        {
          packet = m_queue->Dequeue ();
          //m_snifferTrace (packet); TODO
          //m_promiscSnifferTrace (packet); TODO
          m_outputPhyInterfaces[0]->SendPacket (packet, m_signalFrequency); // 197300000000000
          return true;
        }
      else
        {
          // Enqueue may fail (overflow)
          //m_macTxDropTrace (packet); TODO
          return false;
        }
    }
  else
    {
      return m_queue->Enqueue (packet);
    }
  return true;
}

bool
WdmEdgeNetDevice::SendFrom (Ptr<Packet> packet,
                                 const Address &source,
                                 const Address &dest,
                                 uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << source << dest << protocolNumber);
  /* TODO */
  return true;
}

bool
WdmEdgeNetDevice::SupportsSendFrom (void) const
{
  return true;
}

bool
WdmEdgeNetDevice::SetMtu (uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  m_mtu = mtu;
  return true;
}

uint16_t
WdmEdgeNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mtu;
}

bool
WdmEdgeNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkUp;
}

void
WdmEdgeNetDevice::SetSignalFrequency (uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << signalFrequency);
  m_signalFrequency = signalFrequency;
}

uint64_t
WdmEdgeNetDevice::GetSignalFrequency (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_signalFrequency;
}

} // namespace ns3
