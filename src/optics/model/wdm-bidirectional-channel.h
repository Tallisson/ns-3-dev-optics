#ifndef WDM_BIDIRECTIONAL_CHANNEL_H
#define WDM_BIDIRECTIONAL_CHANNEL_H

#include "wdm-channel.h"

namespace ns3
{

class WdmBidirectionalChannel : public WdmChannel
{
public:
  static TypeId GetTypeId (void);

  WdmBidirectionalChannel ();
  virtual ~WdmBidirectionalChannel ();

  void Attach (Ptr<WdmInputOutputPhy> inpuOutputPhy);
  Ptr<WdmInputOutputPhy> GetOnuInputOutputPhy (uint32_t i) const;

  virtual void Transmit (Ptr<Packet> packet, Ptr<WdmInputOutputPhy> inputOutputPhy, uint64_t signalFrequency);

  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  Ptr<WdmInputOutputPhy> m_inputOutputPhy[2];
};

}

#endif // WDM_BIDIRECTIONAL_CHANNEL_H
