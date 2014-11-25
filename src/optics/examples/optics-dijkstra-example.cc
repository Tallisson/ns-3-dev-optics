/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/optics-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DijkstraJeKralj");

int main (int argc, char *argv[])
{
  LogComponentEnable ("WdmChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmPhy", LOG_LEVEL_ALL);

  int nDevices = 6;

  NodeContainer nodes;
  nodes.Create (nDevices);
  NetDeviceContainer devices;
  for (int i = 0; i < nDevices; i++)
    {
      Ptr<WdmOxcNetDevice> dev = CreateObject<WdmOxcNetDevice> ();
      devices.Add (dev);
      nodes.Get (i)->AddDevice (dev);
    }

  int nChannels = 24;
  std::vector<Ptr<WdmUnidirectionalChannel> > channels;
  for (int i = 0; i < nChannels; i++)
    {
      Ptr<WdmUnidirectionalChannel> ch = CreateObject<WdmUnidirectionalChannel> ();
      channels.push_back (ch);
    }

  devices.Get (0)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[0], 0);
  devices.Get (1)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[0], 0);
  devices.Get (0)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[1], 0);
  devices.Get (1)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[1], 0);

  devices.Get (1)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[2], 1);
  devices.Get (2)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[2], 1);
  devices.Get (1)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[3], 1);
  devices.Get (2)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[3], 1);

  devices.Get (2)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[4], 0);
  devices.Get (3)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[4], 0);
  devices.Get (2)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[5], 0);
  devices.Get (3)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[5], 0);

  devices.Get (2)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[6], 2);
  devices.Get (4)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[6], 2);
  devices.Get (2)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[7], 2);
  devices.Get (4)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[7], 2);

  devices.Get (4)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[8], 0);
  devices.Get (5)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[8], 0);
  devices.Get (4)->GetObject<WdmOxcNetDevice> ()->AttachOutput (channels[9], 0);
  devices.Get (5)->GetObject<WdmOxcNetDevice> ()->AttachInput (channels[9], 0);

  Ptr<OxcToOxcPath> paths0 = CreateObject<OxcToOxcPath> (devices, devices.Get (0)->GetObject<WdmOxcNetDevice> ());

  /*
  std::vector<std::list<Ptr<WdmOxcNetDevice> > > paths0vec;
  for (int i = 1; i < nDevices; i++)
    {
      std::list<Ptr<WdmOxcNetDevice> > path = paths0->GetChannelList (devices.Get (i)->GetObject<WdmOxcNetDevice> ());
      paths0vec.push_back (path);
    }

  std::cout << "paths0vec size " << paths0vec.size () << std::endl;
  for (unsigned int i = 0; i < paths0vec.size (); i++)
    {
      std::cout << paths0vec[i].size() << std::endl;
    }
  */

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
