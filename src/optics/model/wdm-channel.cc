/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * WdmChannel;
 * WdmCable;
 * WdmAmplifier;
 */

#include "wdm-channel.h"
#include "wdm-net-device.h"

#include "ns3/object.h" /* FIXME */
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/net-device-container.h"

#include <algorithm>
#include "ns3/double.h"

#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "ns3/net-device.h"

#include <limits>



#include "wdm-oxc-net-device.h"
#include "wdm-unidirectional-channel.h"

NS_LOG_COMPONENT_DEFINE ("WdmChannel");

namespace ns3 {

class NetDevice;

TypeId
WdmChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WdmChannel")
    .SetParent<Channel> ()
      .AddAttribute ("PropagationDelay",
                     "Propagation delay of the channel",
                     TimeValue (Seconds (0)),
                     MakeTimeAccessor (&WdmChannel::m_delay),
                     MakeTimeChecker ())
      .AddAttribute ("PropagationLossModel",
                     "Propagation loss of the channel",
                     DoubleValue (0.0),
                     MakeDoubleAccessor (&WdmChannel::m_loss),
                     MakeDoubleChecker<double> ())
      .AddTraceSource ("TxDrop",
                       "Trace source indicating a packet has been dropped",
                       MakeTraceSourceAccessor (&WdmChannel::m_txDropTrace))
  ;
  return tid;
}

TypeId
ChannelGroup::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ChannelGroup")
      .SetParent<Object> ()
      .AddAttribute ("Length",
                     "Physical length in meters",
                     DoubleValue (1.0),
                     MakeDoubleAccessor (&ChannelGroup::m_length),
                     MakeDoubleChecker<double> ())
  ;
  return tid;
}

ChannelGroup::ChannelGroup ()
  :
    m_failureRepair (CreateObject<FailureRepairModel> ())
{
  NS_LOG_FUNCTION_NOARGS ();
  m_failureRepair->AttachToObject (this);
}

ChannelGroup::~ChannelGroup ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
ChannelGroup::DoStop ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (std::vector<Ptr<Channel> >::iterator it = m_channels.begin (); it != m_channels.end (); it++)
    {
      (*it)->Stop ();
    }
  Object::DoStop ();
}

void
ChannelGroup::DoReset ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (std::vector<Ptr<Channel> >::iterator it = m_channels.begin (); it != m_channels.end (); it++)
    {
      (*it)->Reset ();
    }
  Object::DoReset ();
}

void
ChannelGroup::AddChannel (Ptr<Channel> ch)
{
  NS_LOG_FUNCTION (this << ch);
  m_channels.push_back (ch);
}

void
ChannelGroup::RemoveChannel (Ptr<Channel> ch)
{
  NS_LOG_FUNCTION (this << ch);
  m_channels.erase(std::remove(m_channels.begin(), m_channels.end(), ch), m_channels.end());
}

void
ChannelGroup::SetLength (double length)
{
  NS_LOG_FUNCTION (this << length);
  m_length = length;
}

double
ChannelGroup::GetLength ()
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_length;
}

TypeId
OxcToOxcPath::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EndToEndPaths")
      .SetParent<Object> ()
  ;
  return tid;
}

OxcToOxcPath::OxcToOxcPath (NetDeviceContainer devices, Ptr<WdmOxcNetDevice> device)
{
  NS_LOG_FUNCTION (this << &devices << device);
  NetDeviceContainer originalDevices = devices;
  for (uint32_t i = 0; i < devices.GetN (); i++)
    {
      m_dijkstraDistance[devices.Get (i)->GetObject<WdmOxcNetDevice> ()] = std::numeric_limits<double>::infinity();
    }
  for (uint32_t i = 0; i < device->GetNOutputPhyInterfaces (); i++)
    {
      if (device->GetOutputPhyInterface (i) != 0 && device->GetOutputPhyInterface (i)->GetOtherEndPhy () != 0)
        {
          NS_LOG_DEBUG ("OxcOutputPhyInterface " << i << " " << device->GetOutputPhyInterface (i));
          NS_LOG_DEBUG ("OxcOutputPhyInterface " << i << " OtherEndPhy " << device->GetOutputPhyInterface (i)->GetOtherEndPhy ());
          m_dijkstraDistance[device->GetOutputPhyInterface (i)->GetOtherEndPhy ()->GetDevice ()->GetObject<WdmOxcNetDevice> ()] = 1; // FIXME weight(ch)
        }
    }
  m_dijkstraDistance[device] = 0;

  while (devices.GetN () != 0)
    {
      Ptr<WdmOxcNetDevice> closestDevice;
      double closestDeviceDistance = std::numeric_limits<double>::infinity();
      for (uint32_t i = 0; i < devices.GetN (); i++)
        {
          if (m_dijkstraDistance[devices.Get (i)->GetObject<WdmOxcNetDevice> ()] < closestDeviceDistance)
            {
              closestDevice = devices.Get (i)->GetObject <WdmOxcNetDevice> ();
              closestDeviceDistance = m_dijkstraDistance[devices.Get (i)->GetObject<WdmOxcNetDevice> ()];
            }
        }
      if (closestDeviceDistance == std::numeric_limits<double>::infinity())
        {
          break;
        }
      devices.Remove (closestDevice);
      for (uint32_t i = 0; i < closestDevice->GetNOutputPhyInterfaces (); i++)
        {
          double alt = m_dijkstraDistance[closestDevice] + 1; // FIXME dist(u, v)
          if (closestDevice->GetOutputPhyInterface (i) != 0 && closestDevice->GetOutputPhyInterface (i)->GetOtherEndPhy () != 0 && alt < m_dijkstraDistance[closestDevice->GetOutputPhyInterface (i)->GetOtherEndPhy ()->GetDevice ()->GetObject<WdmOxcNetDevice> ()])
            {
              m_dijkstraDistance[devices.Get (i)->GetObject<WdmOxcNetDevice> ()] = alt;
              m_dijkstraPrevious[devices.Get (i)->GetObject<WdmOxcNetDevice> ()] = closestDevice;
            }
        }
    }
  NS_LOG_DEBUG ("Dijkstra devices " << originalDevices.GetN ());
  for (uint32_t i = 0; i < originalDevices.GetN (); i++)
    {
      NS_LOG_DEBUG ("Dijkstra device " << i << " " << originalDevices.Get (i)->GetObject<WdmOxcNetDevice> ());
      NS_LOG_DEBUG ("Dijkstra distance to device " << m_dijkstraDistance[originalDevices.Get (i)->GetObject<WdmOxcNetDevice> ()]);
      NS_LOG_DEBUG ("Dijkstra previous device " << m_dijkstraPrevious[originalDevices.Get (i)->GetObject<WdmOxcNetDevice> ()]);
    }
}

OxcToOxcPath::~OxcToOxcPath ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

std::list<Ptr<WdmOxcNetDevice> >
OxcToOxcPath::GetChannelList (Ptr<WdmOxcNetDevice> dev)
{
  std::list<Ptr<WdmOxcNetDevice> > channelList;
  while (m_dijkstraPrevious[dev] != 0)
    {
      channelList.push_front (dev);
      dev = m_dijkstraPrevious[dev];
    }
  return channelList;
}

double
OxcToOxcPath::GetDistance (Ptr<WdmOxcNetDevice> dev)
{
  return m_dijkstraDistance[dev];
}

} // namespace ns3
