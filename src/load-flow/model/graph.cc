#include "graph.h"
#include <iostream>
#include <string.h>
#include <math.h>

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Graph");
NS_OBJECT_ENSURE_REGISTERED (Graph);

TypeId
Graph::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Graph")
    .SetParent<Object> ()
    .AddConstructor<Graph> ()
    .AddAttribute ("Number Vertices",
                   "Number Vertices Value",
                   UintegerValue (0), /* TODO */
                   MakeUintegerAccessor (&Graph::numV),
                   MakeUintegerChecker<uint32_t> ())
     .AddAttribute ("Number Edges",
                    "Number Vertices Value",
                    UintegerValue (0), /* TODO */
                    MakeUintegerAccessor (&Graph::numE),
                    MakeUintegerChecker<uint32_t> ())
     .AddAttribute ("Simetric",
                    "Graph is simetric",
                    BooleanValue (true), /* TODO */
                    MakeBooleanAccessor (&Graph::simetric),
                    MakeBooleanChecker ());
 ;
 return tid;
}

Graph::Graph():
    numV(0),
    numE(0),
    simetric(true)
{
}

Graph::~Graph() {
  Ptr<Bus> b;
  for(container::map<uint32_t, Ptr<Bus> >::iterator it = bars.begin(); it != bars.end(); it++) {
    b = it->second;
    b->Clear();
  }
  bars.clear();
}

void Graph::AddEdge(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus> impd) {
  uint32_t idV = v->GetId();
  uint32_t idW = w->GetId();

  if(idV != idW) {
    this->numE++;
    Assoc(v, w, impd);

    if(simetric) {
      Assoc(w, v, impd);
      this->numE++;
    }
  }
}

void Graph::Assoc(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus> impd) {
  uint32_t idV = v->GetId();
  Ptr<Bus> tmp = bars.at(idV);

  if(tmp != NULL) {
    tmp->AddN(w, impd);
  }
}

bool Graph::HasEdge(uint32_t v, uint32_t w) {
  Ptr<Bus> bV = bars.at(v);
  if(bV != NULL) {
    if(bV->HasN(w) != NULL) {
      return true;
    }
  }

  return false;
}

Ptr<EdgeBus> Graph::GetEdge(uint32_t v, uint32_t w) {
  Ptr<Bus> bV;
  Ptr<EdgeBus> edge;

  bV = bars.at(v);
  if(bV != NULL) {
    edge = bV->HasN(w);
    return edge;
  }

  return NULL;
}

void Graph::SetSimetric(bool simetric) {
  this->simetric = simetric;
}

void Graph::AddV(Ptr<Bus> v) {
  uint32_t idV = v->GetId();
  bars.insert(make_pair(idV, v));
  numV++;
}

Ptr<Bus> Graph::at(uint32_t v) {
  Ptr<Bus> bV = bars.at(v);

  return bV;
}

Ptr<EdgeBus> Graph::AddEdge(Ptr<Bus> v, Ptr<Bus> w, Ptr<Admitt> admitt) {
  double r = admitt->GetR();
  double x = admitt->GetX();
  double sh = (admitt->GetSh() ? admitt->GetSh() : 0);

  double c = r / (pow(r, 2) + pow(x, 2));
  double s = -x / (pow(r, 2) + pow(x, 2));

  Ptr<EdgeBus> node = CreateObject<EdgeBus>();
  //c, s, sh, admitt->GetTap(), admitt->GetAngle(), admitt->GetFrom(), admitt->GetTo())
  node->Init(c, s, sh, admitt->GetTap(), admitt->GetType(), admitt->GetAngle(), admitt->GetMinLim(), admitt->GetMaxLim(),
             admitt->GetFrom(), admitt->GetTo(), admitt->GetBar());

  AddEdge(v, w, node);

  return node;
}

container::map<uint32_t, Ptr<Bus> > Graph::GetBars() {
  return bars;
}

uint32_t Graph::GetSize() {
  return bars.size();
}
}
