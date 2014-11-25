/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef ADMITT_H_
#define ADMITT_H_

#include "define.h"
#include <boost/container/vector.hpp>
#include <boost/cstdlib.hpp>

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace std;
using namespace boost;

namespace ns3
{

class Admitt: public Object {
private:
  uint32_t from;
  uint32_t to;
  double r;
  double x;
  double sh;
  double tap;
  double angle_phi;
  uint32_t type;
  double min_lim;
  double max_lim;
  uint32_t crt_bar;
public:
  Admitt();

  virtual ~Admitt ();

  static TypeId GetTypeId (void);
  void SetR(double r);
  void SetX(double x);
  void SetSh(double sh);
  void SetTap(double tap);
  void SetType(uint32_t type);
  void SetTo(uint32_t to);
  void SetFrom(uint32_t from);
  void SetAngle(double angle);
  void SetMaxLim(double max);
  void SetMinLim(double min);
  void Init(container::vector<string> data);

  double GetR();
  double GetX();
  double GetSh();
  double GetTap();
  double GetAngle();
  uint32_t GetType();
  uint32_t GetTo();
  uint32_t GetFrom();
  uint32_t GetBar();
  double GetMaxLim();
  double GetMinLim();
};
}


#endif /* ADMITT_H_ */
