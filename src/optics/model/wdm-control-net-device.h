#ifndef WDM_CONTROL_NETDEVICE_H
#define WDM_CONTROL_NET_DEVICE_H

#include <ns3/log.h>
#include <ns3/point-to-point-net-device.h>

namespace ns3
{

class WdmNetDevice;
class WdmOxcNetDevice;
class ChannelGroup;
class WdmOutputPhy;

class WdmControlNetDevice : private PointToPointNetDevice
{
public:
  virtual bool Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber);
  virtual Ptr<WdmNetDevice> GetDataDevice () const;

private:
  Ptr<WdmNetDevice> m_dataDevice;
};

class WdmOxcControl : public Object
{
public:
  static TypeId GetTypeId (void);
  WdmOxcControl ();
  ~WdmOxcControl ();

  void SetOxcControlIndex (const uint32_t index);
  uint32_t GetOxcControlIndex (void) const;

  enum WdmOxcControlAction
  {
    WDM_OXC_CONTROL_ACTION_SET,
    WDM_OXC_CONTROL_ACTION_UNSET
  };

  enum WdmOxcControlStopRestartEvent
  {
    WDM_OXC_CONTROL_EVENT_STOP_CHANNEL,
    WDM_OXC_CONTROL_EVENT_STOP_MEMS,
    WDM_OXC_CONTROL_EVENT_STOP_PHY,
    WDM_OXC_CONTROL_EVENT_RESTART_CHANNEL,
    WDM_OXC_CONTROL_EVENT_RESTART_MEMS,
    WDM_OXC_CONTROL_EVENT_RESTART_PHY
  };

protected:
  uint32_t m_oxcControlIndex;
  //std::vector<Ptr<WdmOxcControl> > m_peers;
  Ptr<WdmOxcNetDevice> m_oxc;
};

class WdmOxcCentralControl : public WdmOxcControl
{
public:
  WdmOxcCentralControl ();
  ~WdmOxcCentralControl ();

  void SetMaster (Ptr<WdmOxcCentralControl> centralControlMaster);
  bool IsMaster ();

  void AddSlave (Ptr<WdmOxcCentralControl> slave);

  void ConfigureOxcCrossConnectionTable (WdmOxcControlAction actionType, std::pair<uint32_t, uint64_t> source, std::pair<uint32_t, uint64_t> dest = std::make_pair (0, 0));
  void NotifyObjectStoppedRestarted (WdmOxcControlStopRestartEvent eventType, uint32_t stopIndex1, uint32_t stopIndex2 = 0);
  /*
  void AddOxc (Ptr<WdmOxcNetDevice> oxc);
  void AddChannelGroup (Ptr<ChannelGroup> channelGroup);

  void NotifyChannelStopped ();
  void ReconfigureOxcs ();
  void ReconfigureToMainPath1 ();
  void ReconfigureToBackupPath1 ();
  void ReconfigureToMainPath2 ();
  void ReconfigureToBackupPath2 ();

  void ReconfigureToFailed0 ();
  void ReconfigureToFailed1 ();
  void ReconfigureToFailed2 ();
  void ReconfigureToFailed3OrNone ();
  */

private:
  Ptr<WdmOxcCentralControl> m_master;
  std::list<Ptr<WdmOxcCentralControl> > m_slaves;
  /*
  std::vector<Callback<void> > m_oxcCallbackList;
  std::vector<Ptr<WdmOxcNetDevice> > m_oxcList;
  std::vector<Ptr<ChannelGroup> > m_channelGroupList;
  Time m_lastReconfigurationTime;
  uint16_t m_lastReconfigurationUsedPath;
  */
};

class WdmOxcDistributedControl : public WdmOxcControl
{
  // ...
};

}

#endif // WDM_CONTROL_NET_DEVICE_H
