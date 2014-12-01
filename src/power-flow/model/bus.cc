#include "bus.h"
#include <iostream>
#include <stdio.h>

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

using namespace std;

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Bus");
NS_OBJECT_ENSURE_REGISTERED (Bus);

void Bus::Init(container::vector<string> data)
{
  id = atoi(data.at(0).c_str());
  type = atoi(data.at(1).c_str());
  actual_voltage = voltage = atof(data.at(2).c_str());
  actual_angle = (atof(data.at(3).c_str()) * M_PI / 180);
  aPowerL = atof(data.at(4).c_str());
  rPowerL = atof(data.at(5).c_str());
  aPowerG =  atof(data.at(6).c_str());
  rPowerG = atof(data.at(7).c_str());
  bSh = atof(data.at(8).c_str());

  if(type == SLACK)
  {
    angle = actual_angle;
  }

  erroQ = 0;
  erroP = 0;

  aPower = aPowerG - aPowerL;
  rPower = rPowerG - rPowerL;
};

TypeId
Bus::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Bus")
      .SetParent<Object> ()
      .AddConstructor<Bus> ()
      .AddAttribute("Angle",
                    "Angle Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::angle),
                    MakeDoubleChecker<double> ())
      .AddAttribute("ActivePower",
                    "Active Power Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::aPower),
                    MakeDoubleChecker<double> ())
      .AddAttribute("ReactivePower",
                    "Reactive Power Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::rPower),
                    MakeDoubleChecker<double> ())
      .AddAttribute("ActivePowerG",
                    "Active Power Generation Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::aPowerG),
                    MakeDoubleChecker<double> ())
      .AddAttribute("ActivePowerL",
                    "Active Power Load Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::aPowerL),
                    MakeDoubleChecker<double> ())
      .AddAttribute("ReactivePowerG",
                    "Reactive Power Generation Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::rPowerG),
                    MakeDoubleChecker<double> ())
      .AddAttribute("ReactivePowerL",
                    "Reactive Power Load Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::rPowerL),
                    MakeDoubleChecker<double> ())
      .AddAttribute("Conductance",
                    "Conductance Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::c),
                    MakeDoubleChecker<double> ())
      .AddAttribute("Susceptance",
                    "Susceptance Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::s),
                    MakeDoubleChecker<double> ())
      .AddAttribute("Type",
                    "Type Value",
                    UintegerValue (0), /* TODO */
                    MakeUintegerAccessor (&Bus::type),
                    MakeUintegerChecker<uint32_t> ())
      .AddAttribute("Id",
                    "Bus Identifier",
                    UintegerValue (0), /* TODO */
                    MakeUintegerAccessor (&Bus::id),
                    MakeDoubleChecker<uint32_t> ())
      .AddAttribute("FinalAngle",
                    "Final Angle Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::actual_angle),
                    MakeDoubleChecker<double> ())
      .AddAttribute("FinalVoltage",
                    "Final Voltage Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::actual_voltage),
                    MakeDoubleChecker<double> ())
      .AddAttribute("EstimatedActive",
                    "Estimated Active Power Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::erroP),
                    MakeDoubleChecker<double> ())
      .AddAttribute("EstimatedReactive",
                    "Estimated Reactive Power Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&Bus::erroQ),
                    MakeDoubleChecker<double> ())
  ;
  return tid;
}

Bus::Bus()
{
}

Bus::~Bus() {
  impd.clear();
  neighbors.clear();
}

double Bus::GetAngle() {
  return angle;
}

double Bus::GetVoltage() {
  return voltage;

}

double Bus::GetAPower() {
  return aPower;
}

double Bus::GetRPower() {
  return rPower;
}

int Bus::GetType() {
  return type;
}

int Bus::GetId() {
  return id;
}

void Bus::SetAngle(double angle) {
  this->angle = angle;
}

void Bus::SetVoltage(double voltage) {
  this->voltage = voltage;
}

void Bus::SetAPower(double aPower) {
  this->aPower = aPower;
}

void Bus::SetRPower(double rPower) {
  this->rPower = rPower;
}

void Bus::AddN(Ptr<Bus> n, Ptr<EdgeBus> i) {
  c += i->GetC();
  s += i->GetS() + i->GetSh();

  int id = n->GetId();
  neighbors.insert(pair<int, Ptr<Bus> >(id, n));
  impd.insert(pair<int, Ptr<EdgeBus> >(id, i));
}

Ptr<EdgeBus> Bus::HasN(int w) {
  Bus* bW = neighbors.at(w);
  if(bW != NULL) {
    Node* imp = impd.at(w);
    return imp;
  }

  return NULL;
}

double Bus::GetC() {
  return c;
}

double Bus::GetS() {
  return s;
}

void Bus::SetS(double s) {
  this->s = s;
}

void Bus::SetC(double c) {
  this->c = c;
}

container::map<int, Ptr<EdgeBus> > Bus::GetWeight() {
  return impd;
}

container::map<int, Ptr<Bus> > Bus::GetNs() {
  return neighbors;
}

double Bus::GetBSh() {
  return bSh;
}

Node* Bus::GetEdge(int n) {
  return impd.at(n);
}

void Bus::SetAPowerG(double aPowerG) {
  this->aPowerG = aPowerG;
}

void Bus::SetAPowerL(double aPowerL) {
  this->aPowerL = aPowerL;
}

void Bus::SetRPowerG(double rPowerG) {
  this->rPowerG = rPowerG;
}

void Bus::SetRPowerL(double rPowerL) {
  this->rPowerL = rPowerL;
}

void Bus::SetEst(int variable, double value) {
  switch(variable) {
  case A_POWER:
    this->erroP = value;
    break;
  case R_POWER:
    this->erroQ = value;
    break;
  }
}

double Bus::GetAPowerG() {
  return this->aPowerG;
}

double Bus::GetAPowerL() {
  return this->aPowerL;
}

double Bus::GetRPowerG() {
  return this->rPowerG;
}

double Bus::GetRPowerL() {
  return this->rPowerL;
}

double Bus::GetEst(int variable) {
  switch(variable) {
  case VOLTAGE:
  case A_POWER:
    return this->erroP;
    break;
  case R_POWER:
    return this->erroQ;
    break;
  }

  return 0;
}

void Bus::SetBSh(double bSh) {
  this->bSh = bSh;
}

void Bus::Clear() {
  neighbors.clear();
  impd.clear();

  delete this;
}

double Bus::GetActualAngle()
{
  return actual_angle;
}

double Bus::GetActualVoltage()
{
  return actual_voltage;
}

void Bus::Clear()
{
  neighbors.clear();
  container::map<int, Ptr<EdgeBus> >::iterator it;
  for(it = impd.begin(); it != impd.end(); it++)
  {
    delete it->second;
  }
  impd.clear();
}

}
