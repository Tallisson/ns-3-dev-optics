/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/optics-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Whatever");

void
SendOnePacket (Ptr<WdmEdgeNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  device->Send (p, device->GetBroadcast (), 0x0800);
}

void
SendIpPacket (Ptr<WdmEdgeNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  Ipv4Header ipHeader;
  ipHeader.SetSource (Ipv4Address ("10.0.0.1"));
  ipHeader.SetDestination (Ipv4Address ("10.0.5.1"));
  ipHeader.SetProtocol (0);
  ipHeader.SetPayloadSize (0);
  p->AddHeader (ipHeader);
  device->Send (p, device->GetBroadcast (), 0x0800);
}

bool ReceiveCallback (Ptr<NetDevice> b, Ptr<const Packet> packet, uint16_t d, const Address &address)
{
  NS_LOG_LOGIC ("LUKA3 " << packet);
  return true;
}

int main (int argc, char *argv[])
{
  LogComponentEnable ("WdmNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmEdgeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmTransportNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmOxcNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmUnidirectionalChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("Node", LOG_LEVEL_ALL);
  //LogComponentEnable ("Simulator", LOG_LEVEL_ALL);
  //LogComponentEnable ("DefaultSimulatorImpl", LOG_LEVEL_ALL);
  //LogComponentEnable ("GlobalRouter", LOG_LEVEL_ALL);

  Ptr<Node> n1 = CreateObject<Node> ();
  Ptr<Node> nSw = CreateObject<Node> ();
  Ptr<Node> n2 = CreateObject<Node> ();
  Ptr<Node> n3 = CreateObject<Node> ();
  Ptr<Node> n4 = CreateObject<Node> ();
  Ptr<Node> nMux = CreateObject<Node> ();
  //Ptr<Node> nDeMux = CreateObject<Node> ();
  Ptr<WdmEdgeNetDevice> dev1 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev2 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev3 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev4 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmOxcNetDevice> devOxc = CreateObject<WdmOxcNetDevice> ();
  Ptr<WdmMuxNetDevice> devMux = CreateObject<WdmMuxNetDevice> ();
  //Ptr<WdmDeMux> devDeMux = CreateObject<WdmDeMux> ();
  Ptr<WdmUnidirectionalChannel> ch1a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch1b = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch2a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch2b = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch3a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch3b = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch4a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch4b = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch5a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch5b = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch6a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch6b = CreateObject<WdmUnidirectionalChannel> ();

  /*
   * dev1 -- 1 -- \       / -- 3 -- \
   *              -- Oxc --         Mux -- dev3
   * dev2 -- 2 -- /       \ -- 4 -- /
   *                                           dev4
   */


  devOxc->AttachInput (ch1a, 0);
  devOxc->AttachOutput (ch1b, 0);
  devOxc->AttachInput (ch2a, 1);
  devOxc->AttachOutput (ch2b, 1);
  devOxc->AttachInput (ch3a, 2);
  devOxc->AttachOutput (ch3b, 2);
  devOxc->AttachInput (ch4a, 3);
  devOxc->AttachOutput (ch4b, 3);
  devOxc->AttachInput (ch5b, 3);
  devOxc->SetCrossConnectionTableEntry (std::make_pair (0, 190100000000000), std::make_pair (3, 190200000000000));
  devOxc->SetCrossConnectionTableEntry (std::make_pair (3, 190200000000000), std::make_pair (1, 190300000000000));

  dev1->AttachInput (ch1b);
  dev1->AttachOutput (ch1a);
  dev1->SetAddress (Mac48Address::Allocate ());
  dev1->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev2->AttachInput (ch2b);
  dev2->AttachOutput (ch2a);
  dev2->SetAddress (Mac48Address::Allocate ());
  dev2->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev3->AttachInput (ch6b);
  //dev3->AttachOutput (ch3a);
  dev3->SetAddress (Mac48Address::Allocate ());
  dev3->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  //dev4->AttachInput (ch4b);
  //dev4->AttachOutput (ch4a);
  dev4->SetAddress (Mac48Address::Allocate ());
  dev4->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  devMux->AttachOutput (ch5b);
  devMux->AttachInput (ch3b);
  devMux->AttachInput (ch4b);

  n1->AddDevice (dev1);
  n2->AddDevice (dev2);
  n3->AddDevice (dev3);
  n4->AddDevice (dev4);
  nSw->AddDevice (devOxc);
  nMux->AddDevice (devMux);
  //nDeMux->AddDevice (devDeMux);

  InternetStackHelper stack;
  stack.Install (n1);
  stack.Install (nSw);
  stack.Install (n2);
  stack.Install (n3);
  stack.Install (n4);

  /*
  NetDeviceContainer devices1, devices2, devices3, devices4;
  devices1.Add (dev1);
  devices1.Add (devOxc->GetDevice (0));
  devices2.Add (dev2);
  devices2.Add (devOxc->GetDevice (1));
  devices3.Add (dev3);
  devices3.Add (devOxc->GetDevice (2));
  devices4.Add (dev4);
  devices4.Add (devOxc->GetDevice (3));

  Ipv4AddressHelper address1, address2, address3, address4;
  address1.SetBase ("10.0.0.0", "255.255.255.0");
  address2.SetBase ("10.0.1.0", "255.255.255.0");
  address3.SetBase ("10.0.2.0", "255.255.255.0");
  address4.SetBase ("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);
  Ipv4InterfaceContainer interfaces3 = address3.Assign (devices3);
  Ipv4InterfaceContainer interfaces4 = address4.Assign (devices4);
  */

  /*
  A test = CreateObject<A>;

  Simulator::Schedule (Seconds (1.0), &SendOnePacket, test, devA);
  */
  //SendOnePacket (devA);
  //devB->SetReceiveCallback (MakeCallback (&ReceiveCallback));
  //Simulator::Schedule (Seconds (1.0), &SendOnePacket, dev1);

  /*
  NS_LOG_UNCOND ("SIMULACIJA " << dev1->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev2->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev3->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev4->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev1->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev2->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev3->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << dev4->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (0)->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (1)->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (2)->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (3)->GetInputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (0)->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (1)->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (2)->GetOutputPhyInterface (0)->GetChannel ());
  NS_LOG_UNCOND ("SIMULACIJA " << devOxc->GetDevice (3)->GetOutputPhyInterface (0)->GetChannel ());
  */

  Simulator::Schedule (Seconds (2.0), &SendIpPacket, dev1);

  //Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
