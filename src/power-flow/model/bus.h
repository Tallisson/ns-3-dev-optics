#ifndef BAR_H_
#define BAR_H_

#include <boost/container/map.hpp>
#include <boost/container/vector.hpp>
#include "node.h"
#include <string>
#include <boost/cstdint.hpp>

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
#define ANGLE 400

class Bus : public Object {
private:
  double angle;
  double voltage;
  double aPower;
  double rPower;
  double aPowerL;
  double rPowerL;
  double aPowerG;
  double rPowerG;
  int type;
  int id;
  double c;
  double s;
  double bSh;

  double actual_angle;
  double actual_voltage;
  double erroQ;
  double erroP;

  container::map<int, Bus*> neighbors;
  container::map<int, Node*> impd;

public:
  void Init();
  Bus();
  virtual ~Bus();

  double GetAngle();
  double GetActualAngle();
  double GetVoltage();
  double GetActualVoltage();
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

  int GetType();
  int GetId();

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

  void AddN(Bus* n, Node* i);
  Node * HasN(int w);
  container::map<int, Bus*> GetNs();
  container::map<int, Node*> GetWeight();
  Node * GetEdge(int n);
  void Clear();
};
}
#endif /* BAR_H_ */
