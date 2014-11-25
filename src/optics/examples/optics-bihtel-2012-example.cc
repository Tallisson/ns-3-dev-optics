/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <ctime>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/optics-module.h"

#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("OpticsBihtel2012");

int main()
{
  return 0;
}

/*
int g_droppedPackets = 0;
int g_sentPackets = 0;
int g_receivedPackets = 0;

void
PrintChannelStatus (Ptr<ChannelGroup> chg1, Ptr<ChannelGroup> chg2, Ptr<ChannelGroup> chg3, Ptr<ChannelGroup> chg4)
{
  NS_LOG_UNCOND ("Status chg12 " << chg1->IsOperational ());
  NS_LOG_UNCOND ("Status chg13 " << chg2->IsOperational ());
  NS_LOG_UNCOND ("Status chg24 " << chg3->IsOperational ());
  NS_LOG_UNCOND ("Status chg34 " << chg4->IsOperational ());
  if (Simulator::Now () < Seconds (1000))
    {
      Simulator::Schedule (Seconds (1), &PrintChannelStatus, chg1, chg2, chg3, chg4);
    }
}

void
PrintDroppedPacket (Ptr<const Packet> p, uint64_t signalFrequency)
{
  //NS_LOG_UNCOND ("Drop paket " << p << " frekvencija " << signalFrequency);
  g_droppedPackets++;
}

void
PrintTxPacket (Ptr<const Packet> p)
{
  g_sentPackets++;
}

void
PrintRxPacket (Ptr<const Packet> p, const Address& addr)
{
  g_receivedPackets++;
}

int
main (int argc, char *argv[])
{
  std::time_t start, end;
  std::time (&start);

  //LogComponentEnable ("FailureRepairModel", LOG_LEVEL_ALL);
  //LogComponentEnable ("WdmChannel", LOG_LEVEL_ALL);
  //LogComponentEnable ("WdmUnidirectionalChannel", LOG_LEVEL_ALL);

  //LogComponentEnable ("WdmNetDevice", LOG_LEVEL_ALL);

  //LogComponentEnable ("WdmEdgeNetDevice", LOG_LEVEL_ALL);
  //LogComponentEnable ("WdmPassthroughNetDevice", LOG_LEVEL_ALL);
  //LogComponentEnable ("WdmOxcNetDevice", LOG_LEVEL_ALL);

  //LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  //LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);

  //Ptr<Node> n0 = CreateObject<Node> ();
  Ptr<Node> n1 = CreateObject<Node> ();
  Ptr<Node> n2 = CreateObject<Node> ();
  Ptr<Node> n3 = CreateObject<Node> ();
  Ptr<Node> n4 = CreateObject<Node> ();
  Ptr<Node> n5 = CreateObject<Node> ();
  Ptr<Node> n6 = CreateObject<Node> ();
  Ptr<Node> n7 = CreateObject<Node> ();
  Ptr<Node> n8 = CreateObject<Node> ();

  Ptr<WdmOxcNetDevice> dev1 = CreateObject<WdmOxcNetDevice> ();
  Ptr<WdmOxcNetDevice> dev2 = CreateObject<WdmOxcNetDevice> ();
  Ptr<WdmOxcNetDevice> dev3 = CreateObject<WdmOxcNetDevice> ();
  Ptr<WdmOxcNetDevice> dev4 = CreateObject<WdmOxcNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev5 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev6 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev7 = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> dev8 = CreateObject<WdmEdgeNetDevice> ();

  Ptr<WdmUnidirectionalChannel> ch12a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch12b = CreateObject<WdmUnidirectionalChannel> ();
  ch12a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch12b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<WdmUnidirectionalChannel> ch13a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch13b = CreateObject<WdmUnidirectionalChannel> ();
  ch13a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch13b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<WdmUnidirectionalChannel> ch24a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch24b = CreateObject<WdmUnidirectionalChannel> ();
  ch24a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch24b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<WdmUnidirectionalChannel> ch34a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch34b = CreateObject<WdmUnidirectionalChannel> ();
  ch34a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch34b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));



  Ptr<WdmUnidirectionalChannel> ch15a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch15b = CreateObject<WdmUnidirectionalChannel> ();
  ch15a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch15b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<WdmUnidirectionalChannel> ch26a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch26b = CreateObject<WdmUnidirectionalChannel> ();
  ch26a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch26b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<WdmUnidirectionalChannel> ch37a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch37b = CreateObject<WdmUnidirectionalChannel> ();
  ch37a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch37b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<WdmUnidirectionalChannel> ch48a = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> ch48b = CreateObject<WdmUnidirectionalChannel> ();
  ch48a->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));
  ch48b->SetAttribute ("PropagationDelay", TimeValue (MilliSeconds (10)));

  Ptr<ChannelGroup> chg12 = CreateObject<ChannelGroup> ();
  chg12->AddChannel (ch12a);
  chg12->AddChannel (ch12b);

  Ptr<ChannelGroup> chg13 = CreateObject<ChannelGroup> ();
  chg13->AddChannel (ch13a);
  chg13->AddChannel (ch13b);

  Ptr<ChannelGroup> chg24 = CreateObject<ChannelGroup> ();
  chg24->AddChannel (ch24a);
  chg24->AddChannel (ch24b);

  Ptr<ChannelGroup> chg34 = CreateObject<ChannelGroup> ();
  chg34->AddChannel (ch34a);
  chg34->AddChannel (ch34b);

  dev1->AttachInput (ch12a);
  //NS_LOG_UNCOND ("Ond dev1 ch12a index is " << dev1->GetInputPhyIfIndex (ch12a->GetInputPhy ()));
  dev1->AttachOutput (ch12b);
  //NS_LOG_UNCOND ("Ond dev1 ch12b index is " << dev1->GetOutputPhyIfIndex (ch12b->GetOutputPhy ()));
  dev1->AttachInput (ch13a);
  //NS_LOG_UNCOND ("Ond dev1 ch13a index is " << dev1->GetInputPhyIfIndex (ch13a->GetInputPhy ()));
  dev1->AttachOutput (ch13b);
  //NS_LOG_UNCOND ("Ond dev1 ch13b index is " << dev1->GetOutputPhyIfIndex (ch13b->GetOutputPhy ()));

  dev2->AttachInput (ch12b);
  dev2->AttachOutput (ch12a);
  dev2->AttachInput (ch24a);
  dev2->AttachOutput (ch24b);

  dev3->AttachInput (ch13b);
  dev3->AttachOutput (ch13a);
  dev3->AttachInput (ch34a);
  dev3->AttachOutput (ch34b);

  dev4->AttachInput (ch24b);
  dev4->AttachOutput (ch24a);
  dev4->AttachInput (ch34b);
  dev4->AttachOutput (ch34a);

  dev5->AttachInput (ch15a);
  dev5->AttachOutput (ch15b);
  dev5->SetAddress (Mac48Address::Allocate ());
  dev5->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev5->SetAttribute ("SignalFrequency", UintegerValue (190100000000000));

  dev1->AttachInput (ch15b);
  dev1->AttachOutput (ch15a);

  dev6->AttachInput (ch26a);
  dev6->AttachOutput (ch26b);
  dev6->SetAddress (Mac48Address::Allocate ());
  dev6->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev6->SetAttribute ("SignalFrequency", UintegerValue (190200000000000));

  dev2->AttachInput (ch26b);
  dev2->AttachOutput (ch26a);

  dev7->AttachInput (ch37a);
  dev7->AttachOutput (ch37b);
  dev7->SetAddress (Mac48Address::Allocate ());
  dev7->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev7->SetAttribute ("SignalFrequency", UintegerValue (190200000000000));

  dev3->AttachInput (ch37b);
  dev3->AttachOutput (ch37a);

  dev8->AttachInput (ch48a);
  dev8->AttachOutput (ch48b);
  dev8->SetAddress (Mac48Address::Allocate ());
  dev8->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  dev8->SetAttribute ("SignalFrequency", UintegerValue (190100000000000));

  dev4->AttachInput (ch48b);
  dev4->AttachOutput (ch48a);

  n1->AddDevice (dev1);
  n2->AddDevice (dev2);
  n3->AddDevice (dev3);
  n4->AddDevice (dev4);
  n5->AddDevice (dev5);
  n6->AddDevice (dev6);
  n7->AddDevice (dev7);
  n8->AddDevice (dev8);

  Ptr<WdmOxcCentralControl> control = CreateObject<WdmOxcCentralControl> ();
  control->AddOxc (dev1);
  control->AddOxc (dev2);
  control->AddOxc (dev3);
  control->AddOxc (dev4);
  control->AddChannelGroup (chg12);
  control->AddChannelGroup (chg13);
  control->AddChannelGroup (chg24);
  control->AddChannelGroup (chg34);
  control->ReconfigureToFailed3OrNone (); // FIXME to tako zasad mora biti

  ch12a->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch12b->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch13a->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch13b->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch24a->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch24b->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch34a->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));
  ch34b->TraceConnectWithoutContext ("TxDrop", MakeCallback (&PrintDroppedPacket));

  InternetStackHelper stack;
  stack.InstallAll ();
*/
  /*
  NodeContainer n0n1, n1n2, n1n3, n2n4, n3n4, n4n5;
  n0n1.Add (n0);
  n0n1.Add (n1);
  n1n2.Add (n1);
  n1n2.Add (n2);
  n1n3.Add (n1);
  n1n3.Add (n3);
  n2n4.Add (n2);
  n2n4.Add (n4);
  n3n4.Add (n3);
  n3n4.Add (n4);
  n4n5.Add (n4);
  n4n5.Add (n5);
  */

/*
  NetDeviceContainer devices;
  devices.Add (dev1);
  devices.Add (dev2);
  devices.Add (dev3);
  devices.Add (dev4);
  devices.Add (dev5);
  devices.Add (dev6);
  devices.Add (dev7);
  devices.Add (dev8);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4interfaces = ipv4.Assign (devices);

  Ptr<Ipv4> ipv4n5 = n5->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4n6 = n6->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4n7 = n7->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4n8 = n8->GetObject<Ipv4> ();

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> staticRouting5 = ipv4RoutingHelper.GetStaticRouting (ipv4n5);
  staticRouting5->AddHostRouteTo (Ipv4Address ("10.0.0.9"), Ipv4Address ("10.0.0.2"), 1);

  Ptr<Ipv4StaticRouting> staticRouting6 = ipv4RoutingHelper.GetStaticRouting (ipv4n6);
  staticRouting6->AddHostRouteTo (Ipv4Address ("10.0.0.8"), Ipv4Address ("10.0.0.3"), 1);

  Ptr<Ipv4StaticRouting> staticRouting7 = ipv4RoutingHelper.GetStaticRouting (ipv4n7);
  staticRouting7->AddHostRouteTo (Ipv4Address ("10.0.0.7"), Ipv4Address ("10.0.0.4"), 1);

  Ptr<Ipv4StaticRouting> staticRouting8 = ipv4RoutingHelper.GetStaticRouting (ipv4n8);
  staticRouting8->AddHostRouteTo (Ipv4Address ("10.0.0.6"), Ipv4Address ("10.0.0.5"), 1);
*/
      /*
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (n8);
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (500.0));

  UdpEchoClientHelper echoClient (ipv4interfaces.GetAddress (7), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (50));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (n5);
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (500.0));


  UdpEchoServerHelper echoServer2 (9);

  ApplicationContainer serverApps2 = echoServer2.Install (n7);
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (500.0));

  UdpEchoClientHelper echoClient2 (ipv4interfaces.GetAddress (6), 9);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (50));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps2 = echoClient2.Install (n6);
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (500.0));
    */

/*
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));

  ApplicationContainer apps = sink.Install (n8);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (31556927.0));

  OnOffHelper onOffApp ("ns3::UdpSocketFactory", InetSocketAddress (ipv4interfaces.GetAddress (7), 9));
  onOffApp.SetAttribute ("DataRate", StringValue ("1Kbps"));
  onOffApp.SetAttribute ("PacketSize", UintegerValue (1400));
  onOffApp.SetAttribute ("OnTime", RandomVariableValue (ExponentialVariable (20)));
  onOffApp.SetAttribute ("OffTime", RandomVariableValue (UniformVariable (1, 3)));

  ApplicationContainer clientApps = onOffApp.Install (n5);
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (31556926.0));

  Ptr<Application> app = n5->GetApplication (0);
  app->TraceConnectWithoutContext ("Tx", MakeCallback (&PrintTxPacket));

  app = n8->GetApplication (0);
  app->TraceConnectWithoutContext ("Rx", MakeCallback (&PrintRxPacket));


  PacketSinkHelper sink2 ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));

  ApplicationContainer apps2 = sink2.Install (n7);
  apps2.Start (Seconds (1.0));
  apps2.Stop (Seconds (31556927.0));

  OnOffHelper onOffApp2 ("ns3::UdpSocketFactory", InetSocketAddress (ipv4interfaces.GetAddress (6), 9));
  onOffApp2.SetAttribute ("DataRate", StringValue ("1Kbps"));
  onOffApp2.SetAttribute ("PacketSize", UintegerValue (1400));
  onOffApp2.SetAttribute ("OnTime", RandomVariableValue (ExponentialVariable (20)));
  onOffApp2.SetAttribute ("OffTime", RandomVariableValue (UniformVariable (1, 3)));

  ApplicationContainer clientApps2 = onOffApp2.Install (n6);
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (31556926.0));

  app = n6->GetApplication (0);
  app->TraceConnectWithoutContext ("Tx", MakeCallback (&PrintTxPacket));

  app = n7->GetApplication (0);
  app->TraceConnectWithoutContext ("Rx", MakeCallback (&PrintRxPacket));



  //PrintChannelStatus (chg12, chg13, chg24, chg34);

  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_UNCOND ("Sent: " << g_sentPackets << " Received: " << g_receivedPackets << " Dropped: " << g_droppedPackets);

  std::time (&end);
  double dif = std::difftime (end, start);
  NS_LOG_UNCOND ("Program took " << dif << " seconds.");

  return 0;
}
*/
