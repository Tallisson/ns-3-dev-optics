#include "wdm-phy.h"
#include "wdm-bidirectional-channel.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("WdmBidirectionalChannel");

namespace ns3 {

TypeId
WdmBidirectionalChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmBidirectionalChannel")
    .SetParent<WdmChannel> ()
    .AddConstructor<WdmBidirectionalChannel> ()
  ;
  return tid;
}

WdmBidirectionalChannel::WdmBidirectionalChannel()
  :
    WdmChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmBidirectionalChannel::~WdmBidirectionalChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}


void
WdmBidirectionalChannel::Attach (Ptr<WdmInputOutputPhy> inputOutputPhy)
{
  NS_LOG_FUNCTION (this << inputOutputPhy);
  NS_ASSERT (inputOutputPhy != 0);

  if (m_inputOutputPhy[0] == 0)
  {
      m_inputOutputPhy[0] = inputOutputPhy;
      NS_LOG_UNCOND ("WDM channel " << this << " attaching input-output physical interface 0 " << inputOutputPhy);
      inputOutputPhy->SetChannel (this);
  }
  else if (m_inputOutputPhy[1] == 0)
  {
      m_inputOutputPhy[1] = inputOutputPhy;
      NS_LOG_UNCOND ("WDM channel " << this << " attaching input-output physical interface 1 " << inputOutputPhy);
      inputOutputPhy->SetChannel (this);
  }
  else
  {
      NS_LOG_UNCOND ("WDM channel " << this << " has both input output physical interfaces already attached");
  }
}

Ptr<WdmInputOutputPhy>
WdmBidirectionalChannel::GetOnuInputOutputPhy (uint32_t index) const
{
    return m_inputOutputPhy[index];
}

void
WdmBidirectionalChannel::Transmit (Ptr<Packet> packet, Ptr<WdmInputOutputPhy> inputOutputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << signalFrequency);
  NS_ASSERT (m_inputOutputPhy[0] != 0);
  NS_ASSERT (m_inputOutputPhy[1] != 0);

  if (IsOperational ()){
    if (m_inputOutputPhy[0] == inputOutputPhy)
    {


         Simulator::ScheduleWithContext (m_inputOutputPhy[1]->GetDevice ()->GetNode ()->GetId (),
                                      Seconds (0), &WdmInputOutputPhy::ReceivePacket, m_inputOutputPhy[1],
                                      packet, signalFrequency);
         NS_LOG_UNCOND (this << " transmits from " << m_inputOutputPhy[0] << " to " << m_inputOutputPhy[1]);
    }

    else if (m_inputOutputPhy[1] == inputOutputPhy)
    {


         Simulator::ScheduleWithContext (m_inputOutputPhy[0]->GetDevice ()->GetNode ()->GetId (),
                                      Seconds (0), &WdmInputOutputPhy::ReceivePacket, m_inputOutputPhy[0],
                                      packet, signalFrequency);
         NS_LOG_UNCOND (this << " transmits from " << m_inputOutputPhy[1] << " to " << m_inputOutputPhy[0]);
    }
  }
  else
  {
    NS_LOG_INFO ("WDM channel is in failed state, dropping packet " << packet << " sent at frequency " << signalFrequency);
  }
}



uint32_t
WdmBidirectionalChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  int nDevices = 0;
  if (m_inputOutputPhy[0] != 0)
    {
      nDevices++;
    }
  if (m_inputOutputPhy[1] != 0)
    {
      nDevices++;
    }
  NS_LOG_DEBUG ("nDevices " << nDevices);
  return nDevices;
}

Ptr<NetDevice>
WdmBidirectionalChannel::GetDevice (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  NS_ASSERT (index < 2);
  return m_inputOutputPhy[index]->GetDevice ()->GetObject<NetDevice> ();
}

} // namespace ns3
