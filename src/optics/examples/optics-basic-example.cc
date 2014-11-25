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
  ipHeader.SetDestination (Ipv4Address ("10.0.0.2"));
  ipHeader.SetProtocol (0);
  ipHeader.SetPayloadSize (0);
  p->AddHeader (ipHeader);
  device->Send (p, device->GetBroadcast (), 0x0800);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable ("FailureRepairModel", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmUnidirectionalChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmEdgeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmNetDevice", LOG_LEVEL_ALL);

  Ptr<Node> n1 = CreateObject<Node> ();
  Ptr<Node> n2 = CreateObject<Node> ();

  Ptr<WdmEdgeNetDevice> dev1 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev2 = CreateObject<WdmEdgeNetDevice> ();

  Ptr<WdmUnidirectionalChannel> ch1a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch1b = CreateObject<WdmUnidirectionalChannel> ();

  Ptr<ChannelGroup> chg = CreateObject<ChannelGroup> ();
  chg->AddChannel (ch1a);
  chg->AddChannel (ch1b);
  Ptr<FailureRepairModel> frm = CreateObject<FailureRepairModel> ();
  frm->AttachToObject (chg);

  dev1->AttachInput (ch1b);
  dev1->AttachOutput (ch1a);
  dev1->SetAddress (Mac48Address::Allocate ());
  dev1->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev2->AttachInput (ch1a);
  dev2->AttachOutput (ch1b);
  dev2->SetAddress (Mac48Address::Allocate ());
  dev2->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));

  n1->AddDevice (dev1);
  n2->AddDevice (dev2);

  InternetStackHelper stack;
  stack.Install (n1);
  stack.Install (n2);

  Simulator::Schedule (Seconds (0.5), &SendIpPacket, dev1);
  Simulator::Schedule (Seconds (1.5), &SendIpPacket, dev1);
  Simulator::Schedule (Seconds (2.5), &SendIpPacket, dev2);
  Simulator::Schedule (Seconds (101.5), &SendIpPacket, dev1);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
