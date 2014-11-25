/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WDM_CHANNEL_H
#define WDM_CHANNEL_H

#include "wdm-phy.h"

#include "ns3/channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"


#include "ns3/failure-repair-model.h"
#include "ns3/event-id.h" /* FIXME */

#include <map>
#include <vector>

namespace ns3 {

class NetDevice;
class WdmOxcNetDevice;
class NetDeviceContainer;

class WdmChannel : public Channel
{
public:
  static TypeId GetTypeId (void);

  //virtual void Transmit (Ptr<Packet> packet, uint64_t signalFrequency) = 0;

protected:
  Time m_delay;
  double m_loss;
  TracedCallback<Ptr<const Packet>, uint64_t> m_txDropTrace;
};

class ChannelGroup : public Object
{
public:
  static TypeId GetTypeId (void);
  ChannelGroup ();
  ~ChannelGroup ();

  void AddChannel (Ptr<Channel> ch);
  void RemoveChannel (Ptr<Channel> ch);

  void SetLength (double length);
  double GetLength ();

private:
  void DoStop ();
  void DoReset ();

  std::vector<Ptr<Channel> > m_channels;
  double m_length;
  Ptr<FailureRepairModel> m_failureRepair;
};

class OxcToOxcPath : public Object
{
public:
  static TypeId GetTypeId (void);
  OxcToOxcPath (NetDeviceContainer devices, Ptr<WdmOxcNetDevice> dev);
  ~OxcToOxcPath ();

  std::list<Ptr<WdmOxcNetDevice> > GetChannelList (Ptr<WdmOxcNetDevice> dev);
  double GetDistance (Ptr<WdmOxcNetDevice> dev);

  /*
  void UpdateUpTime ();
  void UpdateDownTime ();

  double GetUpTime ();
  double GetDownTime ();
  */

private:
  std::map<Ptr<WdmOxcNetDevice>, double > m_dijkstraDistance;
  std::map<Ptr<WdmOxcNetDevice>, Ptr<WdmOxcNetDevice> > m_dijkstraPrevious;
  /*
  std::map<int, double > m_pathUpTime;
  std::map<int, double > m_pathDownTime;
  */
};

} // namespace ns3

#endif // WDM_CHANNEL_H
