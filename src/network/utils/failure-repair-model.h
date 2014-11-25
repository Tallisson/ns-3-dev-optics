#ifndef FAILURE_REPAIR_MODEL_H
#define FAILURE_REPAIR_MODEL_H

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/random-variable.h"
#include "ns3/event-id.h"
#include <map>

namespace ns3
{

class FailureRepairModel : public Object
{
public:
  static TypeId GetTypeId (void);
  FailureRepairModel ();
  virtual ~FailureRepairModel ();

  virtual void AttachToObject (Ptr<Object> obj);

  virtual void StopObject ();
  virtual void ResetObject ();

  void AddFailureRelatedObject (Ptr<FailureRepairModel> model, double dependency);
  void RemoveFailureRelatedObject (Ptr<FailureRepairModel> model);
  void AddRepairRelatedObject (Ptr<FailureRepairModel> model, double dependency);
  void RemoveRepairRelatedObject (Ptr<FailureRepairModel> model);

private:
  void ScheduleFailureEvent ();
  void ScheduleRepairEvent ();
  RandomVariable m_meanTimeToFailure;
  RandomVariable m_meanTimeToRepair;
  Time m_totalUpTime;
  Time m_totalDownTime;
  Time m_activeTime;
  Ptr<Object> m_object;
  EventId m_failureEvent;
  EventId m_repairEvent;
  std::map<Ptr<FailureRepairModel>, double> m_failureRelatedObjects;
  RandomVariable m_failureRelatedVariable;
  std::map<Ptr<FailureRepairModel>, double> m_repairRelatedObjects;
  RandomVariable m_repairRelatedVariable;
};

}

#endif // FAILURE_REPAIR_MODEL_H
