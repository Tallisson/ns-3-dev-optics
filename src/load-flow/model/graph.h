#include <boost/container/map.hpp>
#include "admitt.h"
#include "bus.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

//using namespace std;
using namespace boost;

namespace ns3
{

class Graph: public Object {
private:
  uint32_t numV;
  uint32_t numE;
  bool simetric;
  void Assoc(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus> impd);
  container::map<uint32_t, Ptr<Bus> > bars;

public:
  Graph();
  virtual ~Graph();
  static TypeId GetTypeId (void);

  void AddV(Ptr<Bus> v);
  void AddEdge(Ptr<Bus> v, Ptr<Bus> w, Ptr<EdgeBus> impd);
  void SetSimetric(bool simetric);
  void SetNumV(uint32_t numV);
  void SetNumE(uint32_t numE);

  bool HasEdge(uint32_t v, uint32_t w);

  Ptr<EdgeBus> GetEdge(uint32_t v, uint32_t w);
  Ptr<Bus> at(uint32_t v);
  Ptr<EdgeBus> AddEdge(Ptr<Bus> v, Ptr<Bus> w, Ptr<Admitt> admitt);

  container::map<uint32_t, Ptr<Bus> > GetBars();
  uint32_t GetSize();
};
}
