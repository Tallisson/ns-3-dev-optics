/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * WdmPhy;
 */

#include "wdm-phy.h"
#include "wdm-channel.h"
#include "wdm-bidirectional-channel.h"
#include "wdm-unidirectional-channel.h"
#include "wdm-net-device.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"

namespace ns3 {

class Packet;
class Object;

NS_LOG_COMPONENT_DEFINE ("WdmPhy");

TypeId
WdmPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmPhy")
    .SetParent<Object> ()
    .AddConstructor<WdmPhy> ()
    .AddAttribute ("NumWavelengths",
                   "Number of usable wavelengths on the fiber",
                   UintegerValue (4), /* TODO */
                   MakeUintegerAccessor (&WdmPhy::m_numWavelengths),
                   MakeUintegerChecker<uint32_t> ())
    /* TODO
    .AddAttribute ("WavelengthBand",
                   "L C whatever",
                   EnumValue (40),
                   MakeEnumAccessor (&WdmPhy::m_wavelengthBand),
                   MakeEnumChecker ())
    */
  ;
  return tid;
}

WdmPhy::WdmPhy (int numWavelenghts) /* TODO CWDM DWDM */
  :
    // FIXME
    m_channelFloorFrequency (190100000000000),
    m_channelCeilFrequency (197400000000000),
    m_channelSpacing (100000000000),
    m_numWavelengths (numWavelenghts)
{
  NS_LOG_FUNCTION (this << numWavelenghts);
}

WdmPhy::~WdmPhy ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<Channel>
WdmPhy::GetChannel (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_channel;
}

void
WdmPhy::SetChannel (Ptr<WdmChannel> ch)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_channel = ch;
}

Ptr<WdmNetDevice>
WdmPhy::GetDevice(void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_device;
}

/*
std::string
WdmPhy::GetDeviceType (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_deviceType;
}
*/

void
WdmPhy::SetDevice (Ptr<WdmNetDevice> netDevice)
{
  NS_LOG_FUNCTION (this << netDevice);
  m_device = netDevice;
}

void
WdmPhy::NotifyChannelStopped ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_device->NotifyChannelStopped (this);
}

void
WdmInputPhy::ReceivePacket (Ptr<Packet> packet, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << signalFrequency);
  NS_ASSERT (m_device != 0);
  //m_device->GetTypeId ();
  m_device->Receive (packet, this, signalFrequency);
}

Ptr<WdmOutputPhy>
WdmInputPhy::GetOtherEndPhy () const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_DEBUG ("WdmInputPhy m_channel=" << m_channel);
  if (m_channel != 0)
    {
      return m_channel->GetObject<WdmUnidirectionalChannel> ()->GetOutputPhy ();
    }
  else
    {
      return 0;
    }
}

void
WdmOutputPhy::SendPacket (Ptr<Packet> packet, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << signalFrequency);
  NS_ASSERT (m_channel != 0);
  m_channel->GetObject<WdmUnidirectionalChannel> ()->Transmit (packet, signalFrequency);
}

Ptr<WdmInputPhy>
WdmOutputPhy::GetOtherEndPhy () const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_DEBUG ("WdmOutputPhy m_channel=" << m_channel);
  if (m_channel != 0)
    {
      return m_channel->GetObject<WdmUnidirectionalChannel> ()->GetInputPhy ();
    }
  else
    {
      return 0;
    }
}

OpticalSignalWavelength::OpticalSignalWavelength (double wavelength)
  :
    m_wavelength (wavelength)
{}

OpticalSignalFrequency::OpticalSignalFrequency (uint64_t frequency)
  :
    m_frequency (frequency)
{}

void
WdmInputOutputPhy::SendPacket (Ptr<Packet> packet, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << signalFrequency);
  NS_ASSERT (m_channel != 0);
  m_channel->GetObject<WdmBidirectionalChannel> ()->Transmit (packet, this, signalFrequency); /* FIXME */
}

void
WdmInputOutputPhy::ReceivePacket (Ptr<Packet> packet, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << signalFrequency);
  NS_ASSERT (m_device != 0);
  //m_device->GetTypeId ();
  m_device->Receive (packet, this, signalFrequency);
}

} // namespace ns3
