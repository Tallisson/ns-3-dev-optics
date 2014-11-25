/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
//#include "ns3/load-flow-helper.h"
#include "ns3/load-flow.h"
#include <iostream>

using namespace ns3;
using namespace std;

int
main (int argc, char **argv)
{
  cout << "Oi" << endl;
  Ptr<LoadFlow> load = CreateObject<LoadFlow>();
  load->SetError(0.0001);
  load->Configure("/home/thiago/workspace/ler.txt");
  load->Execute ();
  cout << "Meus ovos" << endl;
  /*bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);



  Simulator::Run ();
  Simulator::Destroy ();*/
  return 0;
}


