#ifndef WDMOLTNETDEVICE_H
#define WDMOLTNETDEVICE_H

#include "wdm-net-device.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/data-rate.h"
#include "ns3/error-model.h"
#include "ns3/uinteger.h"
#include "ns3/traced-callback.h"
#include "ns3/node.h"

#include <vector>

#include <stdint.h>

namespace ns3 {

class WdmBidirectionalChannel;
class WdmPhy;
class Queue;

class WdmOltNetDevice : public WdmNetDevice
{
public:
  static TypeId GetTypeId (void);

  WdmOltNetDevice ();
  virtual ~WdmOltNetDevice ();

  bool AttachInputOutput (Ptr<WdmBidirectionalChannel> inputOutputCh);
  void CheckIsLinkUp (void);

  /*void SetQueue (Ptr<Queue> queue);
  Ptr<Queue> GetQueue (void) const;*/

  virtual void Receive (Ptr<Packet> packet, Ptr<WdmPhy> inputOutputPhy, uint64_t wavelength);

  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;

  virtual bool IsLinkUp (void) const;

  virtual bool Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& src, const Address& dest, uint16_t protocolNumber);

  virtual bool SupportsSendFrom (void) const;

private:
  uint16_t m_mtu;
  bool m_linkUp;
  Ptr<Queue> m_queue;
  DataRate m_bps;
};


} // namespace ns3

#endif // WDMOLTNETDEVICE_H
