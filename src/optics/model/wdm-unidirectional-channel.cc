#include "wdm-phy.h"
#include "wdm-unidirectional-channel.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("WdmUnidirectionalChannel");

namespace ns3 {

TypeId
WdmUnidirectionalChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmUnidirectionalChannel")
    .SetParent<WdmChannel> ()
    .AddConstructor<WdmUnidirectionalChannel> ()
  ;
  return tid;
}

WdmUnidirectionalChannel::WdmUnidirectionalChannel ()
  :
    WdmChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmUnidirectionalChannel::~WdmUnidirectionalChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WdmUnidirectionalChannel::Attach (Ptr<WdmInputPhy> inputPhy)
{
  NS_LOG_FUNCTION (this << inputPhy);
  NS_ASSERT (inputPhy != 0);
  m_inputPhy = inputPhy;
  NS_LOG_INFO ("WDM channel " << this << " attaching input physical interface " << m_inputPhy);
  inputPhy->SetChannel (this);
}

Ptr<WdmInputPhy>
WdmUnidirectionalChannel::GetInputPhy ()
{
  return m_inputPhy;
}

void
WdmUnidirectionalChannel::Attach (Ptr<WdmOutputPhy> outputPhy)
{
  NS_LOG_FUNCTION (this << outputPhy);
  NS_ASSERT (outputPhy != 0);
  m_outputPhy = outputPhy;
  NS_LOG_INFO ("WDM channel " << this << " attaching output physical interface " << m_outputPhy);
  outputPhy->SetChannel (this);
}

Ptr<WdmOutputPhy>
WdmUnidirectionalChannel::GetOutputPhy ()
{
  return m_outputPhy;
}

void
WdmUnidirectionalChannel::Transmit (Ptr<Packet> packet, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << signalFrequency);
  /*
  NS_LOG_UNCOND (this << " " << m_outputPhy << " " << m_inputPhy);
  NS_ASSERT (m_outputPhy != 0);
  NS_ASSERT (m_inputPhy != 0);
  */

  /*
  Simulator::ScheduleWithContext (m_link[wire].m_dst->GetNode ()->GetId (),
                                  Seconds (0), &PointToPointNetDevice::Receive,
                                  m_link[wire].m_dst, p);
  */

  if (IsOperational ())
    {
      Simulator::ScheduleWithContext (m_inputPhy->GetDevice ()->GetNode ()->GetId (),
                                      m_delay, &WdmInputPhy::ReceivePacket, m_inputPhy,
                                      packet, signalFrequency);
      NS_LOG_INFO ("WDM channel is in operational state, transmitting packet " << packet << " sent at frequency " << signalFrequency << " with delay " << m_delay.GetSeconds ());
    }
  else
    {
      m_txDropTrace (packet, signalFrequency);
      NS_LOG_INFO ("WDM channel is in failed state, dropping packet " << packet << " sent at frequency " << signalFrequency);
    }

 // m_phy[m_phy[0] == phy ? 1 : 0]->ReceivePacket (packet, signalFrequency);
  /* TODO WRITE FUNCTION BODY receive na suprotnoj strani od one koja radi send */
}

uint32_t
WdmUnidirectionalChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  int nDevices = 0;
  if (m_inputPhy != 0)
    {
      nDevices++;
    }
  if (m_outputPhy != 0)
    {
      nDevices++;
    }
  NS_LOG_FUNCTION ("WdmChannel:GetNDevices nDevices " << nDevices);
  return nDevices;
}

Ptr<NetDevice>
WdmUnidirectionalChannel::GetDevice (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  NS_ASSERT (index < 2);
  if (index == 0)
    {
      return m_inputPhy->GetDevice ()->GetObject<NetDevice> ();
    }
  else
    {
      return m_outputPhy->GetDevice ()->GetObject<NetDevice> ();
    }
}

void
WdmUnidirectionalChannel::DoStop ()
{
  NS_LOG_FUNCTION_NOARGS ();
  // FIXME
  m_inputPhy->NotifyChannelStopped ();
  m_outputPhy->NotifyChannelStopped ();
  Object::DoStop ();
}

} // namespace ns3
