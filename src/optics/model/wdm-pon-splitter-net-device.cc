#include "wdm-pon-splitter-net-device.h"
#include "wdm-passthrough-net-device.h"
#include "wdm-channel.h"

#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/type-id.h"
#include "ns3/traced-callback.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/packet.h"
#include "ns3/ppp-header.h"

#include <string>
#include <map>

NS_LOG_COMPONENT_DEFINE ("WdmPonSplitterNetDevice");

namespace ns3 {

TypeId
WdmPonSplitterNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmPonSplitterNetDevice")
      .SetParent<WdmPassthroughNetDevice> ()
    .AddConstructor<WdmPonSplitterNetDevice> ()
    ;
  return tid;
}


WdmPonSplitterNetDevice::WdmPonSplitterNetDevice (uint32_t numInputOutputs)
  :
    WdmPassthroughNetDevice (numInputOutputs) // 1:16 ili 1:32
{
  NS_LOG_FUNCTION (this << numInputOutputs);
}


WdmPonSplitterNetDevice::~WdmPonSplitterNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}



void
WdmPonSplitterNetDevice::Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputOutputPhy, uint64_t signalFrequency)
{
  NS_LOG_FUNCTION (packet << inputOutputPhy << signalFrequency);

  NS_LOG_UNCOND (this << " receive on input-output physical interface " << inputOutputPhy);
  if (inputOutputPhy == m_inputOutputPhyInterfaces[0])
  {
    for(std::vector<Ptr<WdmInputOutputPhy> >::iterator it = m_inputOutputPhyInterfaces.begin(); it != m_inputOutputPhyInterfaces.end(); it++)
    {
        if (*it != m_inputOutputPhyInterfaces[0])
          {
            Ptr<Packet> packetCopy = packet->Copy ();
            (*it)->SendPacket (packetCopy, signalFrequency);
          }
    }
/*
        for(uint32_t i = 1; i < m_inputOutputPhyInterfaces.size (); i++)
                {
                    m_inputOutputPhyInterfaces[i]->SendPacket (packet, signalFrequency);
                }
*/
    }
    else
    {
       m_inputOutputPhyInterfaces[0]->SendPacket (packet, signalFrequency);
    }
}


} // namespace ns3


