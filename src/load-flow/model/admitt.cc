#include "admitt.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Admitt");
NS_OBJECT_ENSURE_REGISTERED (Admitt);

TypeId
Admitt::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Admitt")
    .SetParent<Object> ()
    .AddConstructor<Admitt> ()
    .AddAttribute ("From",
                   "Bus from",
                   UintegerValue (), /* TODO */
                   MakeUintegerAccessor (&Admitt::from),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("To",
                   "Bus to",
                   UintegerValue (),
                   MakeUintegerAccessor (&Admitt::to),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Resistance",
                   "Resistance Value",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::r),
                   MakeDoubleChecker<double>())
    .AddAttribute ("Reactance",
                   "Reactance Value",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::x),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Susceptance",
                   "Susceptance Value",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::sh),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Tap",
                   "Tap Value",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::tap),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Phi",
                   "Angle of lag",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::angle_phi),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Type",
                   "Connection Type",
                   UintegerValue (), /* TODO */
                   MakeUintegerAccessor (&Admitt::type),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinimumLimit",
                   "Minimum Limit for angle or tap",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::min_lim),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaximumLimit",
                   "Maximum Limit for angle or tap",
                   DoubleValue (),
                   MakeDoubleAccessor (&Admitt::max_lim),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

Admitt::Admitt()
  : from(0), to(0), r(0),
    x(0), sh(0), tap(0),
    angle_phi(angle_phi), type(TRANSMISSION_LINE),
    min_lim(0), max_lim(0), crt_bar(0)
{

}

Admitt::~Admitt()
{
}

void Admitt::Init(container::vector<string> data)
{
  from = (atof(data.at(0).c_str()));
  to = (atof(data.at(1).c_str()));
  r = (atof(data.at(2).c_str()));
  x = (atof(data.at(3).c_str()));
  sh = (atof(data.at(4).c_str()) / 2);
  tap = (atof(data.at(5).c_str()));
  angle_phi = (atof(data.at(6).c_str()));
  type = (atoi(data.at(7).c_str()));
  min_lim = (atof(data.at(8).c_str()));
  max_lim = (atof(data.at(9).c_str()));
  crt_bar = (atoi(data.at(10).c_str()));
};

void Admitt::SetR(double r) {
  this->r = r;
}

void Admitt::SetX(double x) {
  this->r = x;
}

void Admitt::SetSh(double sh) {
  this->sh = sh;
}

void Admitt::SetTap(double tap) {
  this->tap  = tap;
}

void Admitt::SetType(uint32_t type) {
  this->type = type;
}

void Admitt::SetAngle(double angle) {
  this->angle_phi = angle;
}

void Admitt::SetMaxLim(double max) {
  max_lim = max;
}

void Admitt::SetMinLim(double min) {
  min_lim = min;
}

double Admitt::GetR() {
  return r;
}

double Admitt::GetX() {
  return x;
}

double Admitt::GetSh() {
  return sh;
}

uint32_t Admitt::GetType() {
  return type;
}

double Admitt::GetAngle() {
  return angle_phi;
}

double Admitt::GetTap() {
  return tap;
}

uint32_t Admitt::GetTo() {
  return to;
}

uint32_t Admitt::GetFrom() {
  return from;
}

double Admitt::GetMaxLim() {
  return max_lim;
}

double Admitt::GetMinLim() {
  return min_lim;
}

uint32_t Admitt::GetCrtBar() {
  return crt_bar;
}
}
