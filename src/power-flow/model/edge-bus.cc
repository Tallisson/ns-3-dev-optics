#include "edge-bus.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("EdgeBus");
NS_OBJECT_ENSURE_REGISTERED(EdgeBus);

TypeId
EdgeBus::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EdgeBus")
    .SetParent<Object> ()
    .AddConstructor<EdgeBus> ()
    .AddAttribute ("Conductance",
                   "Conductance Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&EdgeBus::c),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Susceptance",
                   "Susceptance Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&EdgeBus::s),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SusceptanceShunt",
                    "Susceptance Shunt Value",
                    DoubleValue (0), /* TODO */
                    MakeDoubleAccessor (&EdgeBus::sh),
                    MakeDoubleChecker<double> ())
    .AddAttribute ("Tap",
                   "Tap Value",
                   DoubleValue (0),
                   MakeDoubleAccessor (&EdgeBus::tap),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Type",
                   "Connection Type",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&EdgeBus::type),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Bus Controlled",
                   "Bus Controlled",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&EdgeBus::crt_bar),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinimumLimitT",
                   "Minimum Limit for tap",
                   DoubleValue (0),
                   MakeDoubleAccessor (&EdgeBus::min_lim_tap),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaximumLimitT",
                    "Maximum Limit for tap",
                    DoubleValue (0),
                    MakeDoubleAccessor (&EdgeBus::max_lim_tap),
                    MakeDoubleChecker<double> ())
    .AddAttribute ("MinimumLimitA",
                   "Minimum Limit for angle",
                   DoubleValue (0),
                   MakeDoubleAccessor (&EdgeBus::min_lim_phi),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaximumLimitA",
                   "Maximum Limit for angle",
                   DoubleValue (0),
                   MakeDoubleAccessor (&EdgeBus::max_lim_phi),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("From",
                   "Bus from",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&EdgeBus::from),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("To",
                   "Bus to",
                   UintegerValue (0),
                   MakeUintegerAccessor (&EdgeBus::to),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

void EdgeBus::Init(double c, double s, double sh, double tap, uint32_t type,
    double angle_phi, double min_lim, double max_lim, uint32_t from, uint32_t to, uint32_t crt_bar)
{
  this->c = c;
  this->s = s;
  this->sh = sh;
  this->tap = tap;
  this->type = type;
  this->angle_phi = angle_phi;
  this->from = from;
  this->to = to;
  this->crt_bar = crt_bar;

  if(type == VARIABLE_TAP_MVAR || type == VARIABLE_TAP_VC)
  {
    min_lim_tap = min_lim;
    max_lim_tap = max_lim;
    min_lim_phi = max_lim_phi = 0;
  }

  if(type == VARIABLE_PHASE_ANGLE)
  {
    min_lim_tap = max_lim_tap = 0;
    min_lim_phi = min_lim;
    max_lim_phi = max_lim;
    }
}

EdgeBus::EdgeBus()
{
}

EdgeBus::~EdgeBus()
{
}

double EdgeBus::GetC() {
  return c;
}

double EdgeBus::GetS() {
  return s;
}

double EdgeBus::GetSh() {
  return sh;
}

void EdgeBus::SetC(double c) {
  this->c = c;
}

void EdgeBus::SetS(double s) {
  this->s = s;
}

void EdgeBus::SetSh(double sh) {
  this->sh = sh;
}

void EdgeBus::SetPhi(double angle) {
  this->angle_phi = angle;
}

bool EdgeBus::Exist() {
  return false;
}

void EdgeBus::SetTap(double t) {
  this->tap = t;
}

void EdgeBus::SetBar(uint32_t bar) {
  this->crt_bar = bar;
}

void EdgeBus::SetLim(uint32_t var, double value) {
  switch(var) {
  case MAX_TAP:
    this->max_lim_tap = value;
    break;
  case MIN_TAP:
    this->min_lim_tap = value;
    break;
  case MAX_PHI:
    this->max_lim_phi = value;
    break;
  case MIN_PHI:
    this->min_lim_phi = value;
    break;
  default:
    break;
  }
}

double EdgeBus::GetTap() {
  return tap;
}

double EdgeBus::GetPhi() {
  return angle_phi;
}

uint32_t EdgeBus::GetType() {
  return type;
}

uint32_t EdgeBus::GetCrtBar() {
  return crt_bar;
}

double EdgeBus::GetLim(uint32_t var) {
  switch(var) {
  case MAX_TAP:
    return max_lim_tap;
    break;
  case MIN_TAP:
    return min_lim_tap;
    break;
  case MAX_PHI:
    return max_lim_phi;
    break;
  case MIN_PHI:
    return min_lim_phi;
    break;
  default:
    break;
  }

  return 0;
}

uint32_t EdgeBus::GetFrom()
{
  return from;
}

uint32_t EdgeBus::GetTo()
{
  return to;
}

}
