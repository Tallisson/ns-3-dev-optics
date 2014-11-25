/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/optics-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Whatever");

void
SendOnePacket (Ptr<WdmNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  device->Send (p, device->GetBroadcast (), 0x0800);
}

void
SendIpPacket (Ptr<WdmNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  Ipv4Header ipHeader;
  ipHeader.SetSource (Ipv4Address ("10.0.0.1"));
  ipHeader.SetDestination (Ipv4Address ("10.0.0.2"));
  ipHeader.SetProtocol (0);
  ipHeader.SetPayloadSize (0);
  p->AddHeader (ipHeader);
  device->Send (p, device->GetBroadcast (), 1490);
}

int
main (int argc, char *argv[])
{
  //LogComponentEnable ("WdmChannel", LOG_LEVEL_ALL);
 // LogComponentEnable ("WdmBidirectionalChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmOltNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmOnuNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmPonSplitterNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WdmPhy", LOG_LEVEL_ALL);

  Ptr<Node> n1 = CreateObject<Node> ();
  Ptr<Node> n2 = CreateObject<Node> ();
  Ptr<Node> n3 = CreateObject<Node> ();
  Ptr<Node> n4 = CreateObject<Node> ();
  Ptr<Node> n5 = CreateObject<Node> ();

  Ptr<WdmOltNetDevice> dev1 = CreateObject<WdmOltNetDevice> ();
  Ptr<WdmPonSplitterNetDevice> dev2 = CreateObject<WdmPonSplitterNetDevice> (4);
  Ptr<WdmOnuNetDevice> dev3 = CreateObject<WdmOnuNetDevice> ();
  Ptr<WdmOnuNetDevice> dev4 = CreateObject<WdmOnuNetDevice> ();
  Ptr<WdmOnuNetDevice> dev5 = CreateObject<WdmOnuNetDevice> ();

  Ptr<WdmBidirectionalChannel> ch1 = CreateObject<WdmBidirectionalChannel> ();
  Ptr<WdmBidirectionalChannel> ch2 = CreateObject<WdmBidirectionalChannel> ();
  Ptr<WdmBidirectionalChannel> ch3 = CreateObject<WdmBidirectionalChannel> ();
  Ptr<WdmBidirectionalChannel> ch4 = CreateObject<WdmBidirectionalChannel> ();




  dev1->AttachInputOutput (ch1);
  dev1->SetAddress (Mac48Address::Allocate ());
  dev1->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev2->AttachInputOutput (ch1);
  dev2->SetAddress (Mac48Address::Allocate ());

  dev2->AttachInputOutput (ch2);
  dev3->AttachInputOutput (ch2);
  dev3->SetAddress (Mac48Address::Allocate ());
  dev3->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));

  dev2->AttachInputOutput (ch3);
  dev4->AttachInputOutput (ch3);
  dev4->SetAddress (Mac48Address::Allocate ());
  dev4->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));

  dev2->AttachInputOutput (ch4);
  dev5->AttachInputOutput (ch4);
  dev5->SetAddress (Mac48Address::Allocate ());
  dev5->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));






  n1->AddDevice (dev1);
  n2->AddDevice (dev2);
  n3->AddDevice (dev3);
  n4->AddDevice (dev4);
  n5->AddDevice (dev5);

  InternetStackHelper stack;
  stack.Install (n1);
  stack.Install (n2);
  stack.Install (n3);
  stack.Install (n4);
  stack.Install (n5);


  Simulator::Schedule (Seconds (0.5), &SendIpPacket, dev1);
  Simulator::Schedule (Seconds (1.5), &SendIpPacket, dev1);
  Simulator::Schedule (Seconds (2.5), &SendIpPacket, dev1);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
