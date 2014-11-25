#include "wdm-control-net-device.h"
#include "wdm-oxc-net-device.h"
#include <ns3/callback.h>
#include <ns3/simulator.h>

NS_LOG_COMPONENT_DEFINE ("WdmControlNetDevice");

namespace ns3
{

bool
Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  /* TODO: uspostava puta i slanje
   * routing - IP
   * signalling - MPLS
   * stiže novi paket s IP sloja koji je uz pomoć routinga odredio da ga mora spucat van na to sučelje
   *   ako ne postoji red čekanja za taj paket,
   *     stvori novi red čekanja, dodaj paket u njega,
   *     vrati višem sloju true kao da je paket poslan
   *     pokreni uspostavu puta (PATH)
   *     opcionalno krcaj nove pakete u isti/druge redove, pokreni druge uspostave putova itd.
   *     dobij natrag odgovor (RESV) dodaj atribut na red čekanja tipa "ima oznaku, oznaka je X"
   *   ako već postoji red čekanja za taj paket
   *     savršeno, samo ga dodaj na kraj
   *   ako još taj red čekanja ima oznaku da je put uspostavljen
   *     GetDataDevice ()->Send (packet) redom za pakete iz tog reda čekanja
   */
  return false;
}

Ptr<WdmNetDevice>
WdmControlNetDevice::GetDataDevice () const
{
  return m_dataDevice;
}

WdmOxcControl::WdmOxcControl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmOxcControl::~WdmOxcControl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WdmOxcControl::SetOxcControlIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_oxcControlIndex = index;
}

uint32_t
WdmOxcControl::GetOxcControlIndex (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_oxcControlIndex;
}

WdmOxcCentralControl::WdmOxcCentralControl ()
  :
    m_master (this)
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmOxcCentralControl::~WdmOxcCentralControl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WdmOxcCentralControl::SetMaster (Ptr<WdmOxcCentralControl> centralControlMaster)
{
  NS_LOG_FUNCTION (this << centralControlMaster);
  m_master = centralControlMaster;
}

bool
WdmOxcCentralControl::IsMaster ()
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_master == this;
}

void
WdmOxcCentralControl::AddSlave (Ptr<WdmOxcCentralControl> slave)
{
  NS_LOG_FUNCTION (this << slave);
  m_slaves.push_back (slave);
  slave->SetMaster (this);
}

void
WdmOxcCentralControl::ConfigureOxcCrossConnectionTable (WdmOxcControlAction actionType, std::pair<uint32_t, uint64_t> source, std::pair<uint32_t, uint64_t> dest)
{
  NS_LOG_FUNCTION (this << actionType << source.first << source.second << dest.first << dest.second);
  switch (actionType)
    {
    case WDM_OXC_CONTROL_ACTION_SET:
      m_oxc->SetCrossConnectionTableEntry (source, dest);
      break;
    case WDM_OXC_CONTROL_ACTION_UNSET:
      m_oxc->UnsetCrossConnectionTableEntry (source);
      break;
    default:
      NS_FATAL_ERROR ("Unknown action.");
    }
}

void
WdmOxcCentralControl::NotifyObjectStoppedRestarted (WdmOxcControlStopRestartEvent eventType, uint32_t stopIndex1, uint32_t stopIndex2)
{
  NS_LOG_FUNCTION (this << eventType << stopIndex1 << stopIndex2);
  if (IsMaster ())
    {
      switch (eventType)
        {
        case WDM_OXC_CONTROL_EVENT_STOP_CHANNEL:
          break;
        case WDM_OXC_CONTROL_EVENT_STOP_MEMS:
          break;
        case WDM_OXC_CONTROL_EVENT_STOP_PHY:
          break;
        case WDM_OXC_CONTROL_EVENT_RESTART_CHANNEL:
          break;
        case WDM_OXC_CONTROL_EVENT_RESTART_MEMS:
          break;
        case WDM_OXC_CONTROL_EVENT_RESTART_PHY:
          break;
        default:
          NS_FATAL_ERROR ("Unknown event.");
        }
    }
  else
    {
      m_master->NotifyObjectStoppedRestarted (eventType, stopIndex1, stopIndex2);
    }
  /*
  else
    {
      for (std::vector<Ptr<WdmOxcControl> >::iterator it = m_peers.begin (); it != m_peers.end (); it++)
        {
          (*it)->NotifyObjectStoppedRestarted (eventType, stopIndex1, stopIndex2);
        }
    }
    */
}

/*

WdmOxcCentralControl::WdmOxcCentralControl ()
  :
    m_lastReconfigurationTime (Seconds (0))
{
  NS_LOG_FUNCTION_NOARGS ();
}

WdmOxcCentralControl::~WdmOxcCentralControl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WdmOxcCentralControl::AddOxc (Ptr<WdmOxcNetDevice> oxc)
{
  NS_LOG_FUNCTION (this << oxc);
  m_oxcList.push_back (oxc);
  oxc->SetCentralControl (this);
}

void
WdmOxcCentralControl::AddChannelGroup (Ptr<ChannelGroup> channelGroup)
{
  NS_LOG_FUNCTION (this << channelGroup);
  m_channelGroupList.push_back (channelGroup);
}

void
WdmOxcCentralControl::NotifyChannelStopped ()
{
  NS_LOG_FUNCTION_NOARGS ();
  ReconfigureOxcs ();
}

void
WdmOxcCentralControl::ReconfigureOxcs ()
{
  NS_LOG_FUNCTION_NOARGS ();
  // FIXME bolja metoda za ovaj uvjet
  if (Simulator::Now () != m_lastReconfigurationTime)
    {
      m_lastReconfigurationTime = Simulator::Now ();
      if (!m_channelGroupList[0]->IsOperational () && m_lastReconfigurationUsedPath != 0)
        {
          m_lastReconfigurationUsedPath = 0;
          ReconfigureToFailed0 ();
        }
      else if (!m_channelGroupList[1]->IsOperational () && m_lastReconfigurationUsedPath != 1)
        {
          m_lastReconfigurationUsedPath = 1;
          ReconfigureToFailed1 ();
        }
      else if (!m_channelGroupList[2]->IsOperational () && m_lastReconfigurationUsedPath != 2)
        {
          m_lastReconfigurationUsedPath = 2;
          ReconfigureToFailed2 ();
        }
      else if (m_lastReconfigurationUsedPath != 3)
        {
          //!m_channelGroupList[3]->IsOperational () || m_channelGroupList[i]->IsOperational ()
          m_lastReconfigurationUsedPath = 3;
          ReconfigureToFailed3OrNone ();
        }
      else
        {
          NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << " central control " << this << " is not reconfiguring anything, configuration " << m_lastReconfigurationUsedPath << " is used");
        }
    }
}

void
WdmOxcCentralControl::ReconfigureToFailed0 ()
{
  NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << " central control " << this << " reconfiguring cross-connections to failed channel group 0");
  ReconfigureToBackupPath1 ();
  ReconfigureToBackupPath2 ();
}

void
WdmOxcCentralControl::ReconfigureToFailed1 ()
{
  NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << " central control " << this << " reconfiguring cross-connections to failed channel group 1");
  ReconfigureToMainPath1 ();
  ReconfigureToBackupPath2 ();
}

void
WdmOxcCentralControl::ReconfigureToFailed2 ()
{
  NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << " central control " << this << " reconfiguring cross-connections to failed channel group 2");
  ReconfigureToBackupPath1 ();
  ReconfigureToMainPath2 ();
}

void
WdmOxcCentralControl::ReconfigureToFailed3OrNone ()
{
  NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << " central control " << this << " reconfiguring cross-connections to failed channel group 3 or none");
  ReconfigureToMainPath1 ();
  ReconfigureToMainPath2 ();
}

void
WdmOxcCentralControl::ReconfigureToMainPath1 ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_oxcList[0]->SetCrossConnectionTableEntry (std::make_pair (2, 190100000000000), std::make_pair (0, 190100000000000));
  m_oxcList[0]->SetCrossConnectionTableEntry (std::make_pair (0, 190100000000000), std::make_pair (2, 190100000000000));
  m_oxcList[1]->SetCrossConnectionTableEntry (std::make_pair (0, 190100000000000), std::make_pair (1, 190100000000000));
  m_oxcList[1]->SetCrossConnectionTableEntry (std::make_pair (1, 190100000000000), std::make_pair (0, 190100000000000));
  m_oxcList[3]->SetCrossConnectionTableEntry (std::make_pair (0, 190100000000000), std::make_pair (2, 190100000000000));
  m_oxcList[3]->SetCrossConnectionTableEntry (std::make_pair (2, 190100000000000), std::make_pair (0, 190100000000000));
}

void
WdmOxcCentralControl::ReconfigureToBackupPath1 ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_oxcList[0]->SetCrossConnectionTableEntry (std::make_pair (2, 190100000000000), std::make_pair (1, 190100000000000));
  m_oxcList[0]->SetCrossConnectionTableEntry (std::make_pair (1, 190100000000000), std::make_pair (2, 190100000000000));
  m_oxcList[2]->SetCrossConnectionTableEntry (std::make_pair (0, 190100000000000), std::make_pair (1, 190100000000000));
  m_oxcList[2]->SetCrossConnectionTableEntry (std::make_pair (1, 190100000000000), std::make_pair (0, 190100000000000));
  m_oxcList[3]->SetCrossConnectionTableEntry (std::make_pair (1, 190100000000000), std::make_pair (2, 190100000000000));
  m_oxcList[3]->SetCrossConnectionTableEntry (std::make_pair (2, 190100000000000), std::make_pair (1, 190100000000000));
}

void
WdmOxcCentralControl::ReconfigureToMainPath2 ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_oxcList[1]->SetCrossConnectionTableEntry (std::make_pair (2, 190200000000000), std::make_pair (0, 190200000000000));
  m_oxcList[1]->SetCrossConnectionTableEntry (std::make_pair (0, 190200000000000), std::make_pair (2, 190200000000000));
  m_oxcList[0]->SetCrossConnectionTableEntry (std::make_pair (0, 190200000000000), std::make_pair (1, 190200000000000));
  m_oxcList[0]->SetCrossConnectionTableEntry (std::make_pair (1, 190200000000000), std::make_pair (0, 190200000000000));
  m_oxcList[2]->SetCrossConnectionTableEntry (std::make_pair (0, 190200000000000), std::make_pair (2, 190200000000000));
  m_oxcList[2]->SetCrossConnectionTableEntry (std::make_pair (2, 190200000000000), std::make_pair (0, 190200000000000));
}

void
WdmOxcCentralControl::ReconfigureToBackupPath2 ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_oxcList[1]->SetCrossConnectionTableEntry (std::make_pair (2, 190200000000000), std::make_pair (1, 190200000000000));
  m_oxcList[1]->SetCrossConnectionTableEntry (std::make_pair (1, 190200000000000), std::make_pair (2, 190200000000000));
  m_oxcList[3]->SetCrossConnectionTableEntry (std::make_pair (0, 190200000000000), std::make_pair (1, 190200000000000));
  m_oxcList[3]->SetCrossConnectionTableEntry (std::make_pair (1, 190200000000000), std::make_pair (0, 190200000000000));
  m_oxcList[2]->SetCrossConnectionTableEntry (std::make_pair (1, 190200000000000), std::make_pair (2, 190200000000000));
  m_oxcList[2]->SetCrossConnectionTableEntry (std::make_pair (2, 190200000000000), std::make_pair (1, 190200000000000));
}

*/

}
