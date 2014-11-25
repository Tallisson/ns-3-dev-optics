#ifndef WDM_UNIDIRECTIONAL_CHANNEL_H
#define WDM_UNIDIRECTIONAL_CHANNEL_H

#include "wdm-channel.h"

namespace ns3
{

class WdmUnidirectionalChannel : public WdmChannel
{
public:
  static TypeId GetTypeId (void);

  WdmUnidirectionalChannel ();
  virtual ~WdmUnidirectionalChannel ();

  void Attach (Ptr<WdmInputPhy> inputPhy);
  Ptr<WdmInputPhy> GetInputPhy ();
  void Attach (Ptr<WdmOutputPhy> outputPhy);
  Ptr<WdmOutputPhy> GetOutputPhy ();

  virtual void Transmit (Ptr<Packet> packet, uint64_t signalFrequency);

  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  virtual void DoStop ();

private:
  Ptr<WdmInputPhy> m_inputPhy;
  Ptr<WdmOutputPhy> m_outputPhy;
};

}

#endif // WDM_UNIDIRECTIONAL_CHANNEL_H
