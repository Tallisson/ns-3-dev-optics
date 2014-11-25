/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __LOAD_FLOW_H__
#define __LOAD_FLOW_H__

#include <armadillo>
#include <iostream>
#include <stdio.h>
#include <boost/algorithm/string.hpp>

#include "bus.h"
#include "graph.h"
#include "report.h"
#include "define.h"
#include "utils.h"
#include "math.h"
#include "solve.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace arma;
using namespace std;
using namespace boost;

namespace ns3
{

class LoadFlow: public Object {
private:
  uint32_t numB;
  uint32_t nPV;
  uint32_t nPQ;
  uint32_t cont;

  uint32_t nLT;
  uint32_t nTAP_Fixed;
  uint32_t nTap_VC;
  uint32_t nTap_MVAR;
  uint32_t nTAP_PHASE;

  //mat jacobian;
  Solve* jacobian;
  Solve* y_bus;

  /*vec estP;
  vec estS;
  vec diffP;
  vec diffS;*/

  vec calcP;
  vec calcQ;
  vec errorP;
  vec errorQ;

  container::map<uint32_t, uint32_t> ord;
  container::map<uint32_t, uint32_t> ordPQ;
  container::map<Ptr<EdgeBus>, double> estCrtlVar;
  double error;
  double sBase;

  double max_error;
  bool use_base;
  bool verbose;
  double s_alpha;
  double total_loss;

  Ptr<Report> report;
  Desc* description;
  Ptr<Graph> bars;

  void mismatches();
  void solveSys();
  void calcJ();
  void InitState();
  void InitState(double aInitial, double vInitial);
  void initJ();
  void updateState();
  void calcS2();
  bool nextIterate();
  void setControlVariables();
  //void insertLoss(Node* edge, double vK, double vM, double theta);
  void insertLoss(Ptr<EdgeBus> edge, Ptr<Bus> barK, Ptr<Bus> barM);
public:
  /*LoadFlow(double error);
  LoadFlow(double error, double sBase);*/
  LoadFlow();
  virtual ~LoadFlow();

  //uint32_t Execute();
  uint32_t Execute();
  void Execute(double aInitial, double vInitial);
  void AddBar(Ptr<Bus> bar);
  void AssocBars(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus> node);
  void AssocBars(Ptr<Bus> v, Ptr<Bus> w, Ptr<Admitt> admitt);
  void AssocBars(Ptr<Admitt> admitt);
  Ptr<Graph> GetGraph();
  Ptr<Bus> GetBar(uint32_t v);
  Ptr<EdgeBus> GetEdge(uint32_t v, uint32_t w);
  void SetSimetric(bool s);
  void DpDer();
  void DqDer();
  void SetUseBase(bool use_base);
  void CalcReport();

  void SetBase(double base);
  void SetVerbose(bool verbose);
  void SetError(double error);
  void ResetReport();
  void Clear();

  double GetTotalLoss();

  void Configure(const char* file);
  void Configure();

  static TypeId GetTypeId (void);
};
}

#endif  // __LOAD_FLOW_H__
