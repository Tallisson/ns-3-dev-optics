#include "wdm-mux-net-device.h"
#include "wdm-phy.h"

#include "ns3/log.h"
#include "ns3/packet.h"

#include <vector>
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("WdmMuxNetDevice");

namespace ns3
{

WdmMuxNetDevice::WdmMuxNetDevice (int numInputs)
  :
    WdmPassthroughNetDevice (numInputs, 1),
    m_attenuation (/*TODO*/)
{
  NS_LOG_FUNCTION (this << numInputs);
  NS_LOG_LOGIC ("m_attenuation=" << m_attenuation); /* TODO */
}

WdmMuxNetDevice::~WdmMuxNetDevice ()
{

}

void
WdmMuxNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (this << packet << inputPhy << signalFrequency);
  m_outputPhyInterfaces[0]->SendPacket (packet, signalFrequency);
}

}
