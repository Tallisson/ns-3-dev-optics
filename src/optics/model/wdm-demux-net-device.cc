#include "wdm-demux-net-device.h"
#include "wdm-phy.h"

#include "ns3/log.h"
#include "ns3/packet.h"

#include <vector>
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("WdmDemuxNetDevice");

namespace ns3
{

WdmDemuxNetDevice::WdmDemuxNetDevice (int numOutputs)
  :
    WdmPassthroughNetDevice (1, numOutputs),
    m_attenuation (/*TODO*/)
{
  NS_LOG_FUNCTION (this << numOutputs);
  NS_LOG_LOGIC ("m_attenuation=" << m_attenuation); /* TODO */
  /*TODO frequency assignment */
  m_outputFrequencyAssignement[0] = std::make_pair (190100000000000, 193400000000000);
  m_outputFrequencyAssignement[1] = std::make_pair (193400000000000, 197400000000000);
}

WdmDemuxNetDevice::~WdmDemuxNetDevice ()
{

}

void
WdmDemuxNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmInputPhy> inputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << inputPhy << signalFrequency);
  for (std::vector<Ptr<WdmOutputPhy> >::iterator it; it != m_outputPhyInterfaces.end (); it++)
    {
      if (m_outputFrequencyAssignement[GetOutputPhyIfIndex(*it)].first <= signalFrequency && signalFrequency < m_outputFrequencyAssignement[GetOutputPhyIfIndex(*it)].second)
        {
          Send (packet, *it, signalFrequency);
          /* TODO -m_attenuation */
          break; /* prevent packet duplication */
        }
    }
}

bool
WdmDemuxNetDevice::Send (Ptr<Packet> packet, Ptr<WdmOutputPhy> outputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << outputPhy << signalFrequency);
  outputPhy->SendPacket (packet, signalFrequency);
  return true;
}

}
