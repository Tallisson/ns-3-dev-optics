#ifndef NODE_H_
#define NODE_H_

#include "define.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

namespace ns3
{

#define MIN_TAP 100
#define MAX_TAP 200
#define MIN_PHI 300
#define MAX_PHI 400

class EdgeBus : public Object {
private:
  double c;
  double s;
  double sh;
  double tap;
  uint32_t type;
  double angle_phi;

  double min_lim_tap;
  double max_lim_tap;
  double min_lim_phi;
  double max_lim_phi;
  uint32_t from;
  uint32_t to;
  uint32_t crt_bar;

public:
  EdgeBus();

  void Init(double c, double s, double sh, double tap, uint32_t type,
       double angle_phi, double min_lim, double max_lim, uint32_t from, uint32_t to, uint32_t crt_bar);

  virtual ~EdgeBus();
  double GetS();
  double GetC();
  double GetSh();
  double GetTap();
  double GetPhi();
  double GetLim(uint32_t var);
  uint32_t GetId();
  uint32_t GetType();
  uint32_t GetCrtBar();

  void SetS(double s);
  void SetC(double c);
  void SetSh(double sh);
  void SetTap(double t);
  void SetPhi(double angle);
  void SetBar(uint32_t bar);
  void SetLim(uint32_t var, double value);
  bool Exist();

  static TypeId EdgeBus::GetTypeId (void);

  uint32_t GetFrom();
  uint32_t GetTo();
};
}

#endif /* NODE_H_ */
