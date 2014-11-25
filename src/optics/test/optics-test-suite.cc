/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/wdm-unidirectional-channel.h"
#include "ns3/wdm-edge-net-device.h"

// An essential include is test.h
#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/pointer.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// This is an example TestCase.
class OpticsTestCase1 : public TestCase
{
public:
  OpticsTestCase1 ();
  virtual ~OpticsTestCase1 ();
  void SendOnePacket (Ptr<WdmEdgeNetDevice> device);

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
OpticsTestCase1::OpticsTestCase1 ()
  : TestCase ("Optics test case 1")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
OpticsTestCase1::~OpticsTestCase1 ()
{
}

void
OpticsTestCase1::SendOnePacket (Ptr<WdmEdgeNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  device->Send (p, device->GetBroadcast (), 0x800);
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
OpticsTestCase1::DoRun (void)
{
  Ptr<Node> a = CreateObject<Node> ();
  Ptr<Node> b = CreateObject<Node> ();
  Ptr<WdmEdgeNetDevice> devA = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmEdgeNetDevice> devB = CreateObject<WdmEdgeNetDevice> ();
  Ptr<WdmUnidirectionalChannel> channelAB = CreateObject<WdmUnidirectionalChannel> ();
  Ptr<WdmUnidirectionalChannel> channelBA = CreateObject<WdmUnidirectionalChannel> ();

  devA->AttachInput (channelAB);
  devA->AttachOutput (channelBA);
  devA->SetAddress (Mac48Address::Allocate ());
  devA->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));
  devB->AttachInput (channelBA);
  devB->AttachOutput (channelAB);
  devB->SetAddress (Mac48Address::Allocate ());
  devB->SetAttribute ("TxQueue", PointerValue (CreateObject<DropTailQueue> ()));

  a->AddDevice (devA);
  b->AddDevice (devB);

  Simulator::Schedule (Seconds (1.0), &OpticsTestCase1::SendOnePacket, this, devA);

  Simulator::Run ();
  Simulator::Destroy ();
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, true, "true doesn't equal true for some reason");
  // Use this one for floating point comparisons
  NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class OpticsTestSuite : public TestSuite
{
public:
  OpticsTestSuite ();
};

OpticsTestSuite::OpticsTestSuite ()
  : TestSuite ("optics", UNIT)
{
  AddTestCase (new OpticsTestCase1);
}
// Do not forget to allocate an instance of this TestSuite
static OpticsTestSuite opticsTestSuite;
