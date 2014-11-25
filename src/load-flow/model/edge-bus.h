#ifndef EDGE_BUS_H_
#define EDGE_BUS_H_

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

class EdgeBus: public Object {
private:
  double c;
  double s;
  double sh;
  double tap;
  int type;
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
  virtual ~EdgeBus ();

  static TypeId GetTypeId (void);

  /*EdgeBus(double c, double s, int from, int to):
    c(c), s(s), sh(0), tap(1), type(TRANSMISSION_LINE), angle_phi(0), crt_bar(0),
    min_lim_tap(0), max_lim_tap(0), min_lim_phi(0), max_lim_phi(0), from(from), to(to)
  {};
  EdgeBus(double c, double s, double sh, int from, int to):
    c(c), s(s), sh(sh), tap(1), type(TRANSMISSION_LINE), angle_phi(0), crt_bar(0),
    min_lim_tap(0), max_lim_tap(0), min_lim_phi(0), max_lim_phi(0), from(from), to(to) {};
  EdgeBus(double c, double s, double sh, double tap, double angle_phi, int from, int to):
    c(c), s(s), sh(sh), tap(tap), angle_phi(angle_phi), crt_bar(0),
    min_lim_tap(0), max_lim_tap(0), min_lim_phi(0), max_lim_phi(0), from(from), to(to) {};
  EdgeBus(double c, double s, double sh, double tap, double angle_phi, int crt_bar,
       double min_tap, double max_tap, double min_phi, double max_phi, int from, int to):
    c(c), s(s), sh(sh), tap(tap), angle_phi(angle_phi), crt_bar(crt_bar),
    min_lim_tap(min_tap), max_lim_tap(max_tap), min_lim_phi(min_phi), max_lim_phi(max_phi), from(from), to(to)
  {};
  EdgeBus(): c(0), s(0), sh(0), tap(1), type(TRANSMISSION_LINE), angle_phi(0), crt_bar(0), from(from), to(to)
  {};*/

  void Init(double c, double s, double sh, double tap, uint32_t type, double angle_phi, double min_lim, double max_lim,
            uint32_t from, uint32_t to, uint32_t crt_bar);

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
  void SetFrom(uint32_t from);
  void SetTo(uint32_t to);
  void SetType(uint32_t type);

  bool Exist();

  uint32_t GetFrom();
  uint32_t GetTo();
};
}

#endif /* EDGE_BUS_H_ */
