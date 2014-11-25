/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef BUS_H_
#define BUS_H_

#include <boost/container/map.hpp>
#include <boost/container/vector.hpp>
#include <string>
#include <boost/cstdint.hpp>

#include "edge-bus.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace boost;
using namespace std;

namespace ns3
{

#define SLACK 3
#define GENERATION 2
#define LOAD 0

#define VOLTAGE 100
#define A_POWER 200
#define R_POWER 300

class Bus: public Object {
private:
  double angle;
  double voltage;
  double aPower;
  double rPower;
  double aPowerL;
  double rPowerL;
  double aPowerG;
  double rPowerG;
  uint32_t type;
  uint32_t id;
  double c;
  double s;
  double bSh;

  double actual_angle;
  double actual_voltage;
  double erroQ;
  double erroP;

  container::map<uint32_t, Ptr<Bus> > neighbors;
  container::map<uint32_t, Ptr<EdgeBus> > impd;

public:
  void Init(container::vector<string> data);

  /*Bus(double angle, double voltage, double aPower, double rPower, int type, int id, double bSh) :
      angle(angle), voltage(voltage), aPower(aPower), rPower(rPower), aPowerL(0), rPowerL(0), aPowerG(0), rPowerG(0), type(type), id(id),
      bSh(bSh)
  {};

  Bus(double angle, double voltage, double aPowerL, double rPowerL, double aPowerG, double rPowerG, int type, int id, double bSh) :
    angle(angle), voltage(voltage), aPowerL(aPowerL), rPowerL(rPowerL), aPowerG(aPowerG), rPowerG(rPowerG), type(type), id(id), c(0), s(0), bSh(bSh)
  {
    aPower = aPowerG - aPowerL;
    rPower = rPowerG - rPowerL;
  };*/

  Bus ();
  virtual ~Bus();

  static TypeId GetTypeId (void);

  double GetAngle();
  double GetVoltage();
  double GetAPower();
  double GetRPower();
  double GetAPowerG();
  double GetAPowerL();
  double GetRPowerG();
  double GetRPowerL();
  double GetC();
  double GetS();
  double GetBSh();
  double GetEst(int variable);

  uint32_t GetType();
  uint32_t GetId();

  void SetAngle(double angle);
  void SetVoltage(double voltage);
  void SetAPower(double aPower);
  void SetRPower(double rPower);
  void SetAPowerG(double aPowerG);
  void SetAPowerL(double aPowerL);
  void SetRPowerG(double rPowerG);
  void SetRPowerL(double rPowerL);
  void SetC(double c);
  void SetS(double s);
  void SetBSh(double bSh);
  void SetEst(int variable, double value);
  void SetType(uint32_t type);
  void SetId(uint32_t id);

  void AddN(Ptr<Bus> n, Ptr<EdgeBus> i);
  Ptr<EdgeBus> HasN(int w);
  container::map<uint32_t, Ptr<Bus> > GetNs();
  container::map<uint32_t, Ptr<EdgeBus> > GetWeight();
  Ptr<EdgeBus> GetEdge(uint32_t n);
  void Clear();

  double GetActualAngle();
  double GetActualVoltage();
};
}
#endif /* BUS_H_ */
