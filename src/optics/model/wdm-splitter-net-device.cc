#include "wdm-splitter-net-device.h"
#include "wdm-phy.h"

#include "ns3/log.h"
#include "ns3/packet.h"

#include <vector>
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("WdmSplitterNetDevice");

namespace ns3
{

WdmSplitterNetDevice::WdmSplitterNetDevice (int numOutputs)
  :
    WdmPassthroughNetDevice (1, numOutputs),
    m_attenuation (10 * std::log10 (numOutputs))
{
  NS_LOG_FUNCTION (this << numOutputs);
  NS_LOG_LOGIC ("m_attenuation=" << m_attenuation); /* TODO */
}

WdmSplitterNetDevice::~WdmSplitterNetDevice ()
{

}

void
WdmSplitterNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmInputPhy> inputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << inputPhy << signalFrequency);
  for (std::vector<Ptr<WdmOutputPhy> >::iterator it; it != m_outputPhyInterfaces.end (); it++)
    {
      Send (packet, *it, signalFrequency);
      /* TODO -m_attenuation */
    }
}

bool
WdmSplitterNetDevice::Send (Ptr<Packet> packet, Ptr<WdmOutputPhy> outputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << outputPhy << signalFrequency);
  outputPhy->SendPacket (packet, signalFrequency);
  return true;
}

}
