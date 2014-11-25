/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

using namespace std;

#include "load-flow.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("LoadFlow");
NS_OBJECT_ENSURE_REGISTERED (LoadFlow);

#define ANGLE_INIT 0
#define VOLTAGE_INIT 1
#define ERROR 0.0001
#define S_BASE 100

TypeId
LoadFlow::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoadFlow")
    .SetParent<Object> ()
    .AddConstructor<LoadFlow> ()
    .AddAttribute ("NumB",
                   "Bus Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::numB),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumPQ",
                   "PQ Bus Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nPQ),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumPV",
                   "PV Bus Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nPV),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ContVar",
                   "Auxiliar Variable",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::cont),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumLT",
                   "Line Transmission Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nLT),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumTapFixed",
                   "Tap Fixed Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nTAP_Fixed),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumTapVC",
                   "Tap VC Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nTap_VC),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumTapMVAR",
                   "Tap MVAR Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nTap_MVAR),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumTapPhase",
                   "Tap MVAR Number",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&LoadFlow::nTAP_PHASE),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Error",
                   "Error Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&LoadFlow::error),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SBase",
                   "SBase Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&LoadFlow::sBase),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxError",
                   "Max Error Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&LoadFlow::max_error),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("UseBase",
                   "Use Base Flag",
                   BooleanValue (true), /* TODO */
                   MakeBooleanAccessor (&LoadFlow::max_error),
                   MakeBooleanChecker ())
    .AddAttribute ("Verbose",
                   "Verbose Flag",
                   BooleanValue (true), /* TODO */
                   MakeBooleanAccessor (&LoadFlow::verbose),
                   MakeBooleanChecker ())
    .AddAttribute ("SAlpha",
                   "Alpha Value",
                   DoubleValue (0), /* TODO */
                   MakeDoubleAccessor (&LoadFlow::s_alpha),
                   MakeDoubleChecker<double> ())
  ;

  return tid;
}

/*LoadFlow::LoadFlow(double error):
    numB(0), nPV(0), nPQ(0), cont(0), nLT(0), nTAP_Fixed(0), nTap_VC(0), nTap_MVAR(0), nTAP_PHASE(0),
    error(error), sBase(S_BASE), use_base(true), verbose(true), s_alpha(1)
{
  bars = CreateObject<Graph>();
  report = CreateObject<Report>();
}

LoadFlow::LoadFlow(double error, double sBase):
    numB(0), nPV(0), nPQ(0), cont(0), error(error), sBase(sBase), use_base(true), verbose(true), s_alpha(1)
{
  bars = CreateObject<Graph>();
}*/

LoadFlow::LoadFlow():
    numB(0), nPV(0), nPQ(0),
    cont(0), nLT(0), nTAP_Fixed(0),
    nTap_VC(0), nTap_MVAR(0), nTAP_PHASE(0),
    error(ERROR), use_base(true), s_alpha(1)
{
  bars = CreateObject<Graph>();
  report = CreateObject<Report>();
}

LoadFlow::~LoadFlow() {
  bars->Dispose();
  report->Dispose();
  delete description;

  NS_LOG_FUNCTION_NOARGS ();
}

void LoadFlow::AddBar(Ptr<Bus> bar) {
  if(use_base) {
    bar->SetAPower(bar->GetAPower() / sBase);
    bar->SetRPower(bar->GetRPower() / sBase);

    bar->SetAPowerG(bar->GetAPowerG() / sBase);
    bar->SetRPowerG(bar->GetRPowerG() / sBase);
    bar->SetAPowerL(bar->GetAPowerL() / sBase);
    bar->SetRPowerL(bar->GetRPowerL() / sBase);
    bar->SetBSh(bar->GetBSh() / sBase);
  }

  if(verbose) {
    cout << "Bar " << bar->GetId() << ": (P, Q, Bsh): " << bar->GetAPowerG() << ", " << bar->GetRPowerG() << ", " << bar->GetBSh() << endl;
  }

  if(bar->GetType() == GENERATION) {
    nPV++;
    ord.insert(pair<uint32_t, uint32_t>(bar->GetId(), cont++));
  }

  if(bar->GetType() == LOAD) {
    uint32_t id = bar->GetId();
    ord.insert(pair<uint32_t, uint32_t>(id, cont++));
    ordPQ.insert(pair<uint32_t, uint32_t>(id, nPQ));
    ++nPQ;
  }

  bars->AddV(bar);
  numB++;
}

Ptr<Bus> LoadFlow::GetBar(uint32_t v) {
  return bars->at(v);
}

Ptr<Graph> LoadFlow::GetGraph() {
  return this->bars;
}

void LoadFlow::AssocBars(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus> node) {
  switch(node->GetType()) {
  case FIXED_TAP:
    nTAP_Fixed++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_TAP_VC:
    nTap_VC++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_TAP_MVAR:
    nTap_MVAR++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_PHASE_ANGLE:
    nTAP_PHASE++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 0) );
    break;
  default:
    break;
  }

  bars->AddEdge(v, w, node);
}

void LoadFlow::AssocBars(Ptr<Bus>  v, Ptr<Bus>  w, Ptr<Admitt>  admitt) {
  Ptr<EdgeBus> node = bars->AddEdge(v, w, admitt);

  switch(admitt->GetType()) {
  case TRANSMISSION_LINE:
    nLT++;
    break;
  case FIXED_TAP:
    nTAP_Fixed++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_TAP_VC:
    nTap_VC++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_TAP_MVAR:
    nTap_MVAR++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_PHASE_ANGLE:
    nTAP_PHASE++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 0) );
    break;
  default:
    break;
  }
}

void LoadFlow::AssocBars(Ptr<Admitt>  admitt) {
  Ptr<EdgeBus> node = bars->AddEdge(bars->at(admitt->GetFrom()), bars->at(admitt->GetTo()), admitt);

  switch(admitt->GetType()) {
  case TRANSMISSION_LINE:
    nLT++;
    break;
  case FIXED_TAP:
    nTAP_Fixed++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_TAP_VC:
    nTap_VC++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_TAP_MVAR:
    nTap_MVAR++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 1) );
    break;
  case VARIABLE_PHASE_ANGLE:
    nTAP_PHASE++;
    estCrtlVar.insert( std::pair<Ptr<EdgeBus>, double>(node, 0) );
    break;
  default:
    break;
  }
}

Ptr<EdgeBus> LoadFlow::GetEdge(uint32_t v, uint32_t w) {
  Ptr<EdgeBus> edge = bars->GetEdge(v, w);
  return edge;
}

void LoadFlow::SetSimetric(bool s) {
  bars->SetSimetric(s);
}

void LoadFlow::InitState() {
  Ptr<Bus>  bar;
  container::map<uint32_t, Ptr<Bus> > nodes = bars->GetBars();

  for (container::map<uint32_t, Ptr<Bus> >::iterator it=nodes.begin(); it!=nodes.end(); ++it) {
    bar = it->second;

    if(bar->GetType() != SLACK) {
      bar->SetAngle(ANGLE_INIT);
    }

    if(bar->GetType() == LOAD) {
      bar->SetVoltage(VOLTAGE_INIT);
    }
  }
}

void LoadFlow::InitState(double aInitial, double vInitial) {
  Ptr<Bus>  bar;
  container::map<uint32_t, Ptr<Bus> > nodes = bars->GetBars();

  for (container::map<uint32_t, Ptr<Bus> >::iterator it=nodes.begin(); it!=nodes.end(); ++it) {
    bar = it->second;

    if(bar->GetType() != SLACK) {
      bar->SetAngle(aInitial);
    }

    if(bar->GetType() == LOAD) {
      bar->SetVoltage(vInitial);
    }
  }
}

void LoadFlow::Configure() {
  int s = nPV + (nPQ << 1);

  jacobian = new Solve();
  jacobian->Initialize(s, s, TYPE_DOUBLE);
  y_bus = new Solve();
  y_bus->Initialize(numB, numB, TYPE_COMPLEX);
  y_bus->Zeros();

  for(uint32_t j = 0; j < numB; j++) {
    Ptr<Bus> bus = bars->at(j+1);
    cx_double value(bus->GetC(), abs(bus->GetS()));

    y_bus->SetValue(j, j, value);
    container::map<uint32_t, Ptr<EdgeBus> > neighbors = bus->GetWeight();
    for (container::map<uint32_t, Ptr<EdgeBus> >::iterator it=neighbors.begin(); it!=neighbors.end(); ++it) {
      Ptr<EdgeBus> n = it->second;
      int index_n = (it->first-1);
      cx_double value(n->GetC(), n->GetS());
      y_bus->SetValue(j, index_n, value);
    }
  }

  calcP.zeros(s);
  calcQ.zeros(s);
  errorP.zeros(s);
  errorQ.zeros(s);
}

void LoadFlow::Configure(const char* file) {
  numB = 0;
  nPV = 0;
  nPQ = 0,
  cont = 0;
  nLT = 0;
  nTAP_Fixed = 0;
  nTap_VC = 0;
  nTap_MVAR = 0;
  nTAP_PHASE = 0;

  Ptr<Utils> ut = Utils::GetInstance();
  description = ut->ProcessFile(file);
  ut->Delete();

  (description->s_base > 0) ? this->sBase = description->s_base : 0;

  for(unsigned i = 0; i < description->bars.size(); i++) {
    AddBar(description->bars.at(i));
  }

  container::vector<Ptr<Admitt> > branchs = description->admmits;
  for(unsigned i = 0; i < branchs.size(); i++) {
    this->AssocBars(branchs.at(i));
  }
  delete description;

  this->Configure();
  InitState();
}

void LoadFlow::initJ() {
  /*uint32_t s = nPV + (nPQ << 1);
  jacobian = zeros<mat>(s, s);*/
  jacobian->Zeros();
}

void LoadFlow::updateState() {
  Ptr<Bus>  barK;

  container::map<uint32_t, uint32_t>::iterator it=ord.begin();
  for(uint32_t k = 0; it != ord.end(); k++) {
    barK = bars->at(it->first);

    if(barK->GetType() != SLACK)
      barK->SetAngle(barK->GetAngle() + errorQ(k));

    if(barK->GetType() == LOAD) {
      uint32_t i = ordPQ.at(barK->GetId()) + (nPV + nPQ);
      barK->SetVoltage(barK->GetVoltage() + errorQ(i));
    }

    it++;
  }
}

bool LoadFlow::nextIterate() {
  vec t = abs(errorP);
  max_error = max(t);
  return  max_error > error;
}

void LoadFlow::mismatches() {
  double theta;
  container::map<uint32_t, Ptr<Bus> > nodes = bars->GetBars();

  container::map<uint32_t, uint32_t>::iterator it=ord.begin();
  for(uint32_t k = 0; it != ord.end(); k++) {
    Ptr<Bus>  barK = bars->at(it->first);
    Ptr<Bus>  barM;

    if(barK->GetType() != SLACK) {
      double vK = barK->GetVoltage();
      errorP(k) = barK->GetAPower();

      container::map<uint32_t, Ptr<EdgeBus> > neighbors = barK->GetWeight();
      Ptr<EdgeBus> edge;
      uint32_t m;
      for (container::map<uint32_t, Ptr<EdgeBus> >::iterator it=neighbors.begin(); it!=neighbors.end(); ++it) {
        m = it->first;
        barM = bars->at(m);
        edge = it->second;
        theta = barK->GetAngle() - barM->GetAngle();

        // (g(km)*V(k)^2 - V(k)*V(m)*(g(km)*cos(akm)+b(km)*sin(akm)));
        if(edge->GetFrom() == barK->GetId())
        {
          errorP(k) = errorP(k) - (edge->GetC() * pow((vK * 1/edge->GetTap()), 2) - (vK * 1/edge->GetTap()) * barM->GetVoltage() *
                   (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi())));
        } else if(edge->GetFrom() == barM->GetId())
        {
          errorP(k) = errorP(k) - (edge->GetC() * pow(vK, 2) - (vK * 1/edge->GetTap()) * barM->GetVoltage() *
                             (edge->GetC() * cos(theta + edge->GetPhi()) + edge->GetS() * sin(theta + edge->GetPhi())));
        }
      }
    }

    if(barK->GetType() == LOAD) {
      uint32_t i = ordPQ.at(barK->GetId()) + (nPQ + nPV);
      double vK = barK->GetVoltage();

      // Incluindo Susceptância Nodal na Potência reativa
      // Qg(k) + Bsh(k)*V(k)^2 - Qc(k)
      errorP(i) = (barK->GetRPower() + barK->GetBSh() * pow(vK, 2));
      container::map<uint32_t, Ptr<EdgeBus> > neighbors = barK->GetWeight();

      Ptr<EdgeBus> edge;
      uint32_t m;
      for (container::map<uint32_t, Ptr<EdgeBus> >::iterator it=neighbors.begin(); it!=neighbors.end(); ++it) {
        m = it->first;
        barM = bars->at(m);
        edge = it->second;
        theta = barK->GetAngle() - barM->GetAngle();

        // mis(nb-1+ordPQ(k)) = mis(nb-1+ordPQ(k)) - (-(b(km)+bsh(km))*V(k)^2 + V(k)*V(m)*(b(km)*cos(akm)-g(km)*sin(akm)));
        if(edge->GetFrom() == barK->GetId())
        {
          errorP(i) -= -((edge->GetS() * pow(1/edge->GetTap(), 2)) + edge->GetSh())  * pow(vK, 2) + (vK * 1/edge->GetTap()) * barM->GetVoltage() *
                    (edge->GetS() * cos(theta - edge->GetPhi()) - edge->GetC() * sin(theta - edge->GetPhi()));
        } else if(edge->GetFrom() == barM->GetId())
        {
          errorP(i) -= -(edge->GetS() + edge->GetSh())  * pow(vK, 2) + (vK * 1/edge->GetTap()) * barM->GetVoltage() *
                              (edge->GetS() * cos(theta + edge->GetPhi()) - edge->GetC() * sin(theta + edge->GetPhi()));
        }
      }

    }

    it++;
  }

}

void LoadFlow::solveSys() {
  //mat m = inv(jacobian);
  errorQ = jacobian->Product(errorP);
}

void LoadFlow::DpDer() {
  Ptr<Bus>  barK, barM;

  container::map<uint32_t, uint32_t>::iterator it=ord.begin();
  uint32_t noSlack = nPQ + nPV;
  for(uint32_t k = 0; k < noSlack; k++) {
    barK = bars->at(it->first);
    uint32_t m = 0;

    if(barK->GetType() != SLACK) {
      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();
      for(container::map<uint32_t, Ptr<Bus> >::iterator itN=neighbors.begin(); itN!=neighbors.end(); ++itN) {
        barM = itN->second;
        double theta = barK->GetAngle() - barM->GetAngle();
        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());

        // dPk em relação a 'ak '.
        // -V(k)*V(m)*(g(km)*sin(akm)-b(km)*cos(akm)) + Jac(k-1, k-1);
        if(edge->GetFrom() == barK->GetId())
        {
          double value = (-(barK->GetVoltage()* 1/edge->GetTap()) * barM->GetVoltage() *
              (edge->GetC() * sin(theta - edge->GetPhi()) - edge->GetS() * cos(theta - edge->GetPhi())) + jacobian->GetValue(k, k));
          jacobian->SetValue(k, k, value);
        } else if(edge->GetFrom() == barM->GetId())
        {
          double value = (-barK->GetVoltage() * barM->GetVoltage() * (edge->GetC() * sin(theta - edge->GetPhi()) -
                            edge->GetS() * cos(theta - edge->GetPhi())) + jacobian->GetValue(k, k));
          jacobian->SetValue(k, k, value);
        }

        // dPk em relação a 'am' (exceto quando m for a barra slack).
        if(barM->GetType() != SLACK) {
          // V(k)*V(m)*(g(km)*sin(akm)-b(km)*cos(akm));
          //cout << "M: " << m << endl;
          m = ord.at(barM->GetId());
          (jacobian->SetValue(k, m, (barK->GetVoltage() * barM->GetVoltage() * (edge->GetC() * sin(theta) - edge->GetS() * cos(theta)))));
        }

        // dPk em relação a 'vk'.
        if(barK->GetType() == LOAD) {
          // -2*g(km)*V(k) + V(m)*(g(km)*cos(akm)+b(km)*sin(akm)) + Jac(k-1, nb-1+ordPQ(k));
          m = noSlack + ordPQ.at(barK->GetId());
          if(edge->GetFrom() == barK->GetId())
          {
            double value = (-2 * edge->GetC() * (barK->GetVoltage() * 1 / edge->GetTap()) + barM->GetVoltage() *
                  (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi())) + jacobian->GetValue(k, m));
            jacobian->SetValue(k, m, value);
          } else if(edge->GetFrom() == barM->GetId())
          {
            (jacobian->SetValue(k, m, (-2 * edge->GetC() * (barK->GetVoltage() * 1 / edge->GetTap()) + barM->GetVoltage() *
                             (edge->GetC() * cos(theta + edge->GetPhi()) + edge->GetS() * sin(theta + edge->GetPhi())) + jacobian->GetValue(k, m))));
          }
        }

        // dPk em relação a 'vm'.
        if(barM->GetType() == LOAD) {
          // V(k)*(g(km)*cos(akm)+b(km)*sin(akm));
          m = noSlack + ordPQ.at(barM->GetId());

          if(edge->GetFrom() == barK->GetId())
          {
            double value = (barK->GetVoltage() * edge->GetTap()) * (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi()));
            jacobian->SetValue(k, m, value);
          } else if(edge->GetFrom() == barM->GetId()) {
            double value = barK->GetVoltage() * edge->GetTap() * (edge->GetC() * cos(theta + edge->GetPhi())  + edge->GetS() * sin(theta + edge->GetPhi()));
            jacobian->SetValue(k, m, value);
          }
        }
      }
    }

    it++;
  }
}

void LoadFlow::DqDer() {
  Ptr<Bus>  barK, barM;

  container::map<uint32_t, uint32_t>::iterator it=ordPQ.begin();
  uint32_t s = nPV+ (nPQ << 1);
  uint32_t noSlack = nPQ + nPV;
  for(uint32_t k = nPQ + nPV; k < s; ) {
    barK = bars->at(it->first);
    uint32_t m = 0;

    if(barK->GetType() == LOAD) {
      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();
      //std::map<uint32_t, uint32_t>::iterator it = ord.find(barK->GetId());

      for(container::map<uint32_t, Ptr<Bus> >::iterator itN=neighbors.begin(); itN!=neighbors.end(); ++itN) {
        barM = itN->second;
        double theta = barK->GetAngle() - barM->GetAngle();
        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());

        // dQk em relaçao a 'ak'.
        // V(k)*V(m)*(b(km)*sin(akm)+g(km)*cos(akm)) + Jac(nb-1+ordPQ(k), k-1);
        m = ord.at(barK->GetId());
        if(edge->GetFrom() == barK->GetId())
        {
          double value = (barK->GetVoltage() * 1/edge->GetTap()) * barM->GetVoltage() *
              (edge->GetS() * sin(theta - edge->GetPhi()) + edge->GetC() * cos(theta - edge->GetPhi())) + jacobian->GetValue(k, m);
          jacobian->SetValue(k, m, value);
        } else if(edge->GetFrom() == barM->GetId())
        {
          double value = (barK->GetVoltage() * 1/edge->GetTap()) * barM->GetVoltage() *
              (edge->GetS() * sin(theta + edge->GetPhi()) + edge->GetC() * cos(theta + edge->GetPhi())) + jacobian->GetValue(k, m);
          jacobian->SetValue(k, m, value);
        }

        // dQk em relaçao a 'am' (exceto quando m for a barra slack).
        if(barM->GetType() != SLACK) {
          // -V(k)*V(m)*(g(km)*cos(akm)+b(km)*sin(akm));
          m = ord.at(barM->GetId());
          if(edge->GetFrom() == barK->GetId())
          {
            double value = -(barK->GetVoltage() * 1/edge->GetTap()) * barM->GetVoltage() *
                (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi()));
            jacobian->SetValue(k, m, value);
          } else if(edge->GetFrom() == barM->GetId())
          {
            double value = -(barK->GetVoltage() * 1/edge->GetTap()) * barM->GetVoltage() *
                (edge->GetC() * cos(theta + edge->GetPhi()) + edge->GetS() * sin(theta + edge->GetPhi()));
            jacobian->SetValue(k, m, value);
          }
        }

        // dQk em relaçao a 'vk'.
        // 2*(b(km)+bsh(km))*V(k) - V(m)*(b(km)*cos(akm)-g(km)*sin(akm)) + Jac(nb-1+ordPQ(k), nb-1+ordPQ(k));
        if(edge->GetFrom() == barK->GetId())
        {
          double value = 2 * (edge->GetS()+edge->GetSh()) * (barK->GetVoltage() * 1/edge->GetTap()) - barM->GetVoltage() *
              (edge->GetS() * cos(theta - edge->GetPhi()) - edge->GetC() * sin(theta - edge->GetPhi())) + jacobian->GetValue(k, k);
          jacobian->SetValue(k, k, value);
        } else if(edge->GetFrom() == barM->GetId())
        {
          double value = 2 * (edge->GetS()+edge->GetSh()) * (barK->GetVoltage() * 1/edge->GetTap()) -
              barM->GetVoltage() * (edge->GetS() * cos(theta + edge->GetPhi()) - edge->GetC() * sin(theta + edge->GetPhi())) + jacobian->GetValue(k, k);
          jacobian->SetValue(k, k, value);
        }
        // dQk em relacao a 'vm'.
        if(barM->GetType() == LOAD) {
          // -V(k)*(b(km)*cos(akm)-g(km)*sin(akm));
          /*m = ord.at(barM->GetId());
          int index = m + nPQ;
          if(index < nPV + nPQ) {
            index++;
          }*/
          m = ordPQ.at(barM->GetId()) + noSlack;
          if(edge->GetFrom() == barK->GetId())
          {
            double value = -(barK->GetVoltage()*1/edge->GetTap()) * (edge->GetS() * cos(theta - edge->GetPhi()) -
                edge->GetC() * sin(theta - edge->GetPhi()));
            jacobian->SetValue(k, m, value);
          } else if(edge->GetFrom() == barM->GetId())
          {
            double value = -(barK->GetVoltage()*1/edge->GetTap()) * (edge->GetS() * cos(theta + edge->GetPhi()) -
                edge->GetC() * sin(theta + edge->GetPhi()));
            jacobian->SetValue(k, m, value);
          }
        }
      }

      // dQk em relaçao a 'vk' (continuação).
      // 2*Bsh(k)*V(k)
      double value = 2*barK->GetBSh() * barK->GetVoltage() + jacobian->GetValue(k, k);
      jacobian->SetValue(k, k, value);
      k++;
    }

    it++;
  }
}

void LoadFlow::calcJ() {
  initJ();
  DpDer();
  DqDer();
}

void LoadFlow::calcS2() {
  Ptr<Bus>  barK, barM;
  container::map<uint32_t, Ptr<Bus> > nodes = bars->GetBars();

  for(container::map<uint32_t, Ptr<Bus> >::iterator it = nodes.begin(); it != nodes.end(); it++) {
    barK = it->second;

    if(barK->GetType() == SLACK) {
      double sum = 0;
      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();

      for(container::map<uint32_t, Ptr<Bus> >::iterator itN = neighbors.begin(); itN != neighbors.end(); itN++) {
        barM = itN->second;
        double theta = barK->GetAngle() - barM->GetAngle();
        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());

        // (gkm(km)*V(k)^2 - V(k)*V(m)*(gkm(km)*cos(akm)+bkm(km)*sin(akm)));
        double vK = barK->GetVoltage();
        if(edge->GetFrom() == barK->GetId())
        {
          sum += (edge->GetC() * pow((vK * 1/edge->GetTap()), 2) - (vK * 1/edge->GetTap()) *
                  barM->GetVoltage() * (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi())) );
        } else if(edge->GetFrom() == barM->GetId())
        {
          sum += (edge->GetC() * pow(vK, 2) - (vK * 1/edge->GetTap()) *
                 barM->GetVoltage() * (edge->GetC() * cos(theta + edge->GetPhi()) + edge->GetS() * sin(theta + edge->GetPhi())) );
        }
      }
      barK->SetAPower(sum);

    }

    if(barK->GetType() != LOAD) {
      double sum = 0;
      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();

      for(container::map<uint32_t, Ptr<Bus> >::iterator itN = neighbors.begin(); itN != neighbors.end(); itN++) {
        barM = itN->second;
        double theta = barK->GetAngle() - barM->GetAngle();
        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());

        // (-(bkm(km)+bkm_sh(km))*V(k)^2 + V(k)*V(m)*(bkm(km)*cos(akm)-gkm(km)*sin(akm)));
        double vK = barK->GetVoltage();
        if(edge->GetFrom() == barK->GetId())
        {
          sum += (-(edge->GetS() + edge->GetSh()) * pow((vK * 1/edge->GetTap()),2) + (vK * 1/edge->GetTap()) * barM->GetVoltage() *
                  (edge->GetS() * cos(theta - edge->GetPhi()) - edge->GetC() * sin(theta - edge->GetPhi())) );
        } else if(edge->GetFrom() == barM->GetId())
        {
          sum += (-(edge->GetS() + edge->GetSh()) * pow(vK,2) + (vK * 1/edge->GetTap()) * barM->GetVoltage() *
                  (edge->GetS() * cos(theta + edge->GetPhi()) - edge->GetC() * sin(theta + edge->GetPhi())) );
        }
      }
      barK->SetRPower(sum);

    }
  }
}

/*uint32_t LoadFlow::Execute() {
  return Execute("");
}*/

uint32_t LoadFlow::Execute() {
  ResetReport();
  uint32_t counter = 0;

  mismatches();

  if(verbose == true) {
    NS_LOG_INFO (numB);
    for(uint32_t i = 0; i < numB; i++) {
      Ptr<Bus>  tmp = bars->at(i+1);
      cout << "Bar(" << tmp->GetId() << ")=> (v, a): " << tmp->GetVoltage()  << ", "  << tmp->GetAngle()<< endl;
    }

    cout << "Erro: " << endl << errorP << endl;
  }

  while(true) {
    calcJ();
    solveSys();
    updateState();
    counter++;
    mismatches();
    setControlVariables();

    if(verbose == true) {
      cout << "Jac" << endl << jacobian << endl;

      cout << "dx" << endl << errorQ << endl;

      for(uint32_t i = 0; i < numB; i++) {
        Ptr<Bus>  tmp = bars->at(i+1);
        cout << "Bar(" << tmp->GetId() << ")=> (v, a): " << tmp->GetVoltage()  << ", "  << tmp->GetAngle()<< endl;
      }

      cout << "Erro: " << endl << errorP << endl;
    }

    if(nextIterate() == false) {
      break;
    }
  }

  CalcReport();
  if(verbose == true) {
    cout << "O processo convergiu com " << counter << " iterações\n" << endl;

    for(uint32_t i = 0; i < numB; i++) {
      cout << i << " ";
      Ptr<Bus>  b = bars->at(i+1);

      //cout << "Barra(" << b->GetId()+1 << "): Estado(a, v, p, q): (" << b->GetAngle() << ", " << b->GetVoltage() << ", " << b->GetAPower() << ", " << b->GetRPower() << ")" << endl;
      printf("Barra(%d)=> Angulo: %.5f, Voltagem: %.5f\n", b->GetId(), b->GetAngle(), b->GetVoltage());
    }
  }

  ostringstream os;
  os << endl << "Grandezas Calculdas: " << endl;
  container::map<Ptr<Bus> , Ptr<Quantity> > data = report->GetPower();
  for(container::map<Ptr<Bus> , Ptr<Quantity> >::iterator it = data.begin(); it != data.end(); it++) {
    Ptr<Bus>  barK = it->first;
    Ptr<Quantity> qt = it->second;

    if(verbose)
    {
      os << "Barra " << barK->GetId() << ": { PG [p.u.] = " << qt->GetAttr(PG) << ", QG [p.u.] = " << qt->GetAttr(QG)
         << ", PC [p.u.] = " << qt->GetAttr(PC) <<  ", QC [p.u.] = " << qt->GetAttr(QC) << " }" << endl;
    }
  }

  if(verbose)
  {
    os << endl << "Perdas: " << endl;
    os << "Pkm[p.u.] | " << " Pmk[p.u.] | " << "Perdas | " << "Qkm[p.u.] | " << "Qmk[p.u.]" << endl;
  }
  container::map<Ptr<EdgeBus>, Ptr<Loss> > losses = report->GetLosses();
  double loss_total = 0;
  for(container::map<Ptr<EdgeBus>, Ptr<Loss> >::iterator it = losses.begin(); it != losses.end(); it++) {
    Ptr<Loss>  l = it->second;
    loss_total += l->GetAttr(LOSS);
    double loss = l->GetAttr(LOSS);

    if(verbose)
    {
      os << l->GetAttr(P_IN) << " | " << l->GetAttr(P_OUT) << " | " << (loss) << " | " << l->GetAttr(Q_IN) << " | " << l->GetAttr(Q_OUT) << endl;
    }
  }

  total_loss = loss_total * sBase;
  os << "Perda Total: " << total_loss << endl;
  if(verbose)
  {
    cout << os.str();
  }

  return counter;
}

void LoadFlow::SetUseBase(bool use_base) {
  this->use_base = use_base;
}

/*
 * Controles implementados:
 * 1) Controle de magnitude de tensão nodal (local e remota) por injeção de reativos
 * 2) Controle de magnitude de tensão nodal (local e remota) por ajuste de tap (transformadores em-fase)
 * 3) Controle de fluxo de potência (transformadores defasadores)
 *
 * Ajuste alternado:
 * correção na variavel de controle = diferença entre valores calculados e especificados da variável controlada * sensibilidade
 */
void LoadFlow::setControlVariables() {
  for( container::map<Ptr<EdgeBus>, double>::iterator it = estCrtlVar.begin(); it != estCrtlVar.end(); it++ ) {
    Ptr<EdgeBus> edge = it->first;
    Ptr<Bus> crt_bar = bars->at(edge->GetCrtBar() - 1);

    // deltaU = alfa * deltaZ
    // deltaZ = z_esp - z_cal
    switch(edge->GetType()) {
    //case FIXED_TAP:

    case VARIABLE_TAP_VC:
    {
      double old_tap = it->second;
      double delta_z = abs(crt_bar->GetAngle() - crt_bar->GetActualAngle());

      if(delta_z > error) {
        double new_tap = old_tap + s_alpha * (delta_z);

        double limit = edge->GetLim(MAX_TAP);
        if(new_tap > limit) {
          new_tap = limit;
        } else {
          limit = edge->GetLim(MIN_TAP);
          if(new_tap < limit) {
            new_tap = limit;
          }
        }

        it->second = new_tap;
      }
      break;
    }
    case VARIABLE_TAP_MVAR:
    {
      double old_tap = it->second;
      double delta_z = abs(crt_bar->GetEst(R_POWER));
      if(delta_z > error) {
        double new_tap = old_tap + s_alpha * (delta_z);

        double limit = edge->GetLim(MAX_TAP);
        if(new_tap > limit) {
          new_tap = limit;
        } else {
          limit = edge->GetLim(MIN_TAP);
          if(new_tap < limit) {
            new_tap = limit;
          }
        }

        it->second = new_tap;
      }
      break;
    }
    case VARIABLE_PHASE_ANGLE:
    {
      double old_angle = it->second;
      double delta_z = abs(crt_bar->GetEst(A_POWER));

      if(delta_z > error) {
        double new_angle = old_angle + s_alpha * (delta_z);

        double limit = edge->GetLim(MAX_PHI);
        if(new_angle > limit) {
          new_angle = limit;
        } else {
          limit = edge->GetLim(MIN_PHI);
          if(new_angle < limit) {
            new_angle = limit;
          }
        }

        it->second = new_angle;
      }
      break;
    }
    default:
    {
    }
    break;
    }
  }
}

void LoadFlow::insertLoss(Ptr<EdgeBus> edge, Ptr<Bus>  barK, Ptr<Bus>  barM) {
  double vK = barK->GetVoltage();
  double vM = barM->GetVoltage();
  double theta = barK->GetAngle() - barM->GetAngle();
  double p_km, p_mk, q_km, q_mk, perdas;

  if(edge->GetFrom() == barK->GetId())
  {
    p_km = edge->GetC() * pow((vK * 1/edge->GetTap()), 2) - (vK * 1/edge->GetTap()) * vM *
                  (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi()));
    p_mk = edge->GetC() * pow(vM, 2) - (1/edge->GetTap() * vK) * vM *
                  (edge->GetC() * cos(theta - edge->GetPhi()) - edge->GetS() * sin(theta - edge->GetPhi()));
    q_km = -(pow(edge->GetS() * 1/edge->GetTap(), 2) + edge->GetSh()) * pow(vK, 2) + (1 / edge->GetTap() * vK) * vM *
                  (edge->GetS() * cos(theta - edge->GetPhi()) - edge->GetC() * sin(theta - edge->GetPhi()));
    q_mk = -(edge->GetS() + edge->GetSh()) * pow(vM, 2) + (1/edge->GetTap() * vK) * vM *
                  (edge->GetS() * cos(theta) + edge->GetC() * sin(theta));
    perdas = edge->GetC() * (pow((vK * 1/edge->GetTap()), 2) + pow(vM, 2) - 2 * (1/edge->GetTap() * vK) * vM *
                    cos(theta - edge->GetPhi()));
  } else if(edge->GetFrom() == barM->GetId())
  {
    p_km = edge->GetC() * pow(vK, 2) - (vK * 1/edge->GetTap()) * vM *
                  (edge->GetC() * cos(theta + edge->GetPhi()) + edge->GetS() * sin(theta + edge->GetPhi()));
    p_mk = edge->GetC() * pow(vM, 2) - (1/edge->GetTap() * vK) * vM *
                  (edge->GetC() * cos(theta + edge->GetPhi()) - edge->GetS() * sin(theta + edge->GetPhi()));
    q_km = -(edge->GetS() + edge->GetSh()) * pow((vK * 1/edge->GetTap()), 2) + (1 / edge->GetTap() * vK) * vM *
                  (edge->GetS() * cos(theta + edge->GetPhi()) - edge->GetC() * sin(theta + edge->GetPhi()));
    q_mk = -(edge->GetS() + edge->GetSh()) * pow(vM, 2) + (1/edge->GetTap() * vK) * vM *
                  (edge->GetS() * cos(theta) + edge->GetC() * sin(theta));
    perdas = edge->GetC() * (pow(vK, 2) + pow(vM, 2) - 2 * (1/edge->GetTap() * vK) * vM *
                    cos(theta + edge->GetPhi()));
  }

  Ptr<Loss> l = CreateObject<Loss>();
  l->SetAttr(P_IN, p_km);
  l->SetAttr(P_OUT, p_mk);
  l->SetAttr(Q_IN, q_km);
  l->SetAttr(Q_OUT, q_mk);
  l->SetAttr(LOSS, perdas);

  report->Insert(edge, l);
}

/*
 * 1) Cálculo de Grandezas: Potência ativa gerada e Potência Reativa Gerada
 * 2) Cálculo de Perdas de Potência:
 *      Pkm(i) = gkm(i)*V(k)^2 - V(k)*V(m)*(gkm(i)*cos(akm)+bkm(i)*sin(akm));
 *      Pmk(i) = gkm(i)*V(m)^2 - V(k)*V(m)*(gkm(i)*cos(akm)-bkm(i)*sin(akm));
 *      Qkm(i) = -(bkm(i)+bkm_sh(i))*V(k)^2 + V(k)*V(m)*(bkm(i)*cos(akm)-gkm(i)*sin(akm));
 *      Qmk(i) = -(bkm(i)+bkm_sh(i))*V(m)^2 + V(k)*V(m)*(bkm(i)*cos(akm)+gkm(i)*sin(akm));
 *
 *      Perdas(i) = gkm(i)*(V(k)^2 + V(m)^2 - 2*V(k)*V(m)*cos(akm));
 */
void LoadFlow::CalcReport() {
  Ptr<Bus> barK, barM;
  container::map<uint32_t, Ptr<Bus> > nodes = bars->GetBars();

  for(container::map<uint32_t, Ptr<Bus> >::iterator it = nodes.begin(); it != nodes.end(); it++) {
    barK = it->second;
    Ptr<Quantity> qt = NULL;

    /*
     * Para todas as barras do tipo slack (tipo 3):
     * 1) Pg(k) = Pc(k)
     * 2) Pg(k) = Pg(k) + G(km) * (V(k) ^ 2 - V(k) * V(m) *(G(km) * cos(theta) + Bkm * sen(theta)))
     */
    if(barK->GetType() == SLACK) {
      qt = CreateObject<Quantity>();
      double generate_active = barK->GetAPowerL();
      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();

      for(container::map<uint32_t, Ptr<Bus> >::iterator itN = neighbors.begin(); itN != neighbors.end(); itN++) {
        barM = itN->second;
        double theta = barK->GetAngle() - barM->GetAngle();
        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());

        // (gkm(km)*V(k)^2 - V(k)*V(m)*(gkm(km)*cos(akm)+bkm(km)*sin(akm)));
        double vK = barK->GetVoltage();
        if(edge->GetFrom() == barK->GetId())
        {
          generate_active += (edge->GetC() * pow((vK * 1/edge->GetTap()), 2) - (vK * 1/edge->GetTap()) *
                  barM->GetVoltage() * (edge->GetC() * cos(theta - edge->GetPhi()) + edge->GetS() * sin(theta - edge->GetPhi())) );
        } else if(edge->GetFrom() == barM->GetId())
        {
          generate_active += (edge->GetC() * pow(vK, 2) - (vK * 1/edge->GetTap()) *
                 barM->GetVoltage() * (edge->GetC() * cos(theta + edge->GetPhi()) + edge->GetS() * sin(theta + edge->GetPhi())) );
        }
      }
      barK->SetAPowerG(generate_active);
      barK->SetAPower(generate_active - barK->GetAPowerL());

      // Configurando potência ativa gerada e consumida
      //qt->SetAttr(PG, generate_active);
      //qt->SetAttr(PC, barK->GetAPowerL());
    }

    /*
     * Para todas as barras diferentes de LOAD:
     * 1) Qg(k) = -Bsh(k) * V(k)^2 + Qc(k);
     * 2) Qg(k) = Qg(k) + (-(bkm(km)+bkm_sh(km))*V(k)^2 + V(k)*V(m)*(bkm(km)*cos(akm)-gkm(km)*sin(akm)));
     */
    if(barK->GetType() != LOAD) {
      double generate_power = -barK->GetBSh() * pow(barK->GetVoltage(), 2) + barK->GetRPowerL();
      if(qt == NULL)
      {
        qt = CreateObject<Quantity>();
      }

      qt->SetAttr(PG, barK->GetAPowerG());
      qt->SetAttr(PC, barK->GetAPowerL());
      qt->SetAttr(QC, barK->GetRPowerL());

      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();
      for(container::map<uint32_t, Ptr<Bus> >::iterator itN = neighbors.begin(); itN != neighbors.end(); itN++) {
        barM = itN->second;
        double theta = barK->GetAngle() - barM->GetAngle();
        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());

        double vK = barK->GetVoltage();
        double vM = barM->GetVoltage();

        generate_power += (-(edge->GetS() * pow(1/edge->GetTap(), 2) + edge->GetSh()) * pow(vK, 2) +
                          (vK * 1/edge->GetTap()) * vM * (edge->GetS() * cos(theta) - edge->GetC() * sin(theta)));

        //this->insertLoss(edge, vK, vM, theta);
        this->insertLoss(edge, barK, barM);
      }
      barK->SetRPowerG(generate_power);
      barK->SetRPower(generate_power - barK->GetAPowerL());

      qt->SetAttr(QG, generate_power);
    }

    if(barK->GetType() == LOAD) {
      if(qt == NULL)
      {
        qt = CreateObject<Quantity>();
      }

      qt->SetAttr(PG, barK->GetAPowerG());
      qt->SetAttr(QG, barK->GetRPowerG());
      qt->SetAttr(PC, barK->GetAPowerL());
      qt->SetAttr(QC, barK->GetRPowerL());

      //report->Insert(barK, qt);

      container::map<uint32_t, Ptr<Bus> > neighbors = barK->GetNs();
      for(container::map<uint32_t, Ptr<Bus> >::iterator itN = neighbors.begin(); itN != neighbors.end(); itN++) {
        barM = itN->second;

        Ptr<EdgeBus> edge = barK->GetEdge(barM->GetId());
        this->insertLoss(edge, barK, barM);
      }
    }
    if(qt != NULL)
    {
      report->Insert(barK, qt);
    }
  }
}

void
LoadFlow::SetBase(double base)
{
  this->sBase = base;
}

void
LoadFlow::SetError(double error)
{
  this->error = error;
}

void
LoadFlow::SetVerbose(bool verbose)
{
  this->verbose = verbose;
}

double LoadFlow::GetTotalLoss()
{
  return total_loss;
}

void LoadFlow::ResetReport()
{
  report->Clear();
}

void LoadFlow::Clear()
{
  InitState();
}

}
