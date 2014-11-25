#include "wdm-onu-net-device.h"

#include "wdm-net-device.h"
#include "wdm-unidirectional-channel.h"
#include "wdm-bidirectional-channel.h"

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"

#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"

NS_LOG_COMPONENT_DEFINE ("WdmOnuNetDevice");

namespace ns3 {

TypeId
WdmOnuNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmOnuNetDevice")
    .SetParent<WdmNetDevice> ()
    .AddConstructor<WdmOnuNetDevice> ()
    .AddAttribute ("Address",
                   "The MAC address of this device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&WdmOnuNetDevice::m_address),
                   MakeMac48AddressChecker ())
    .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&WdmOnuNetDevice::SetMtu,
                                         &WdmOnuNetDevice::GetMtu),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("DataRate",
                   "The default data rate for each wavelength channel",
                   DataRateValue (DataRate ("10Gbps")),
                   MakeDataRateAccessor (&WdmOnuNetDevice::m_bps),
                   MakeDataRateChecker ())
    .AddAttribute ("ReceiveErrorModel",
                   "The receiver error model used to simulate packet loss",
                   PointerValue (),
                   MakePointerAccessor (&WdmOnuNetDevice::m_receiveErrorModel),
                   MakePointerChecker<ErrorModel> ())
    .AddAttribute ("TxQueue",
                   "A queue to use as the transmit queue in the device.",
                   PointerValue (),
                   MakePointerAccessor (&WdmOnuNetDevice::m_queue),
                   MakePointerChecker<Queue> ())
  ;
  return tid;
}

WdmOnuNetDevice::WdmOnuNetDevice()
:
  WdmNetDevice (1)
  {
    NS_LOG_FUNCTION_NOARGS ();
  }

WdmOnuNetDevice::~WdmOnuNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_queue = 0;
  /* TODO isprazni listu Phy */
}

bool
WdmOnuNetDevice::AttachInputOutput(Ptr<WdmBidirectionalChannel> inputOutputCh)
{
  NS_LOG_FUNCTION (this << inputOutputCh);
  inputOutputCh->Attach (m_inputOutputPhyInterfaces[0]);


  return true;
}

/*void
WdmOnuNetDevice::CheckIsLinkUp (void)
{
  if (m_inputPhyInterfaces[0] != 0 && m_outputPhyInterfaces[0] != 0)
    {
      m_linkUp = true;
    }
  //m_linkChangeCallbacks (); TODO
}*/

void
WdmOnuNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputOutputPhy, uint64_t wavelength)
{
  NS_LOG_UNCOND (this << " receive on input-output physical interface " << inputOutputPhy);
  NS_LOG_FUNCTION (this << packet << inputOutputPhy << wavelength);
  uint16_t protocol = 0;

  //
  // Trace sinks will expect complete packets, not packets without some of the
  // headers.
  //
  Ptr<Packet> originalPacket = packet->Copy ();

  Mac48Address src;
  Mac48Address dest;
  ProcessEthernetHeader (packet, src, dest, protocol);
  m_rxCallback (this, packet, protocol, src);
}

bool
WdmOnuNetDevice::Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  return SendFrom (packet, m_address, dest, protocolNumber);
}

bool
WdmOnuNetDevice::SendFrom (Ptr<Packet> packet,
                                 const Address &src,
                                 const Address &dest,
                                 uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << src << dest << protocolNumber);
  Mac48Address destination = Mac48Address::ConvertFrom (dest);
  Mac48Address source = Mac48Address::ConvertFrom (src);
  AddEthernetHeader (packet, source, destination, protocolNumber);

  m_inputOutputPhyInterfaces[0]->SendPacket (packet, 1310);
  return true;
}

bool
WdmOnuNetDevice::SupportsSendFrom (void) const
{
  return true;
}

bool
WdmOnuNetDevice::SetMtu (uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  m_mtu = mtu;
  return true;
}

uint16_t
WdmOnuNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mtu;
}

bool
WdmOnuNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_linkUp;
}








} // namespace ns3
