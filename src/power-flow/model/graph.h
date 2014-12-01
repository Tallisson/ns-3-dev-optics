#include <boost/container/map.hpp>
#include "bus.h"
#include "edge-bus.h"
#include "admitt.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace boost;

namespace ns3
{

class Graph : public Object {
private:
  uint32_t numV;
  uint32_t numE;
  bool simetric;
  void Assoc(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus>  impd);
  container::map<uint32_t, Ptr<Bus> > bars;

public:
  Graph(uint32_t numV);
  Graph();
  ~Graph();

  void AddV(Ptr<Bus> v);
  void AddEdge(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus>  impd);
  bool HasEdge(uint32_t v, uint32_t w);
  Ptr<EdgeBus>  GetEdge(uint32_t v, uint32_t w);
  void SetSimetric(bool simetric);
  Ptr<Bus> at(uint32_t v);
  Ptr<EdgeBus>  AddEdge(Ptr<Bus> v, Ptr<Bus> w, Ptr<Admitt>  admitt);
  container::map<uint32_t, Ptr<Bus> > GetBars();
  uint32_t GetSize();

  static TypeId Graph::GetTypeId (void);
};
}
