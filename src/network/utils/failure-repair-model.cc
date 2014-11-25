#include "failure-repair-model.h"

#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("FailureRepairModel");

namespace ns3
{

TypeId
FailureRepairModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FailureRepairModel")
      .SetParent<Object> ()
      .AddAttribute ("MeanTimeToFailure",
                     "A RandomVariable used to pick the duration of the 'On' state.",
                     RandomVariableValue (ExponentialVariable (15552000.0)),
                     MakeRandomVariableAccessor (&FailureRepairModel::m_meanTimeToFailure),
                     MakeRandomVariableChecker ())
      .AddAttribute ("MeanTimeToRepair",
                     "A RandomVariable used to pick the duration of the 'Off' state.",
                     RandomVariableValue (ConstantVariable (28800.0)),
                     MakeRandomVariableAccessor (&FailureRepairModel::m_meanTimeToRepair),
                     MakeRandomVariableChecker ())
      .AddAttribute ("ActiveTime",
                     "The amount of time the model is active.",
                     TimeValue (Seconds (31556926.0)),
                     MakeTimeAccessor (&FailureRepairModel::m_activeTime),
                     MakeTimeChecker ())
  ;
  return tid;
}

FailureRepairModel::FailureRepairModel ()
  :
    m_totalUpTime (Seconds (0.0)),
    m_totalDownTime (Seconds (0.0)),
    m_failureRelatedVariable (UniformVariable (0.0, 1.0)),
    m_repairRelatedVariable (UniformVariable (0.0, 1.0))
{
}

FailureRepairModel::~FailureRepairModel ()
{
}

void
FailureRepairModel::StopObject ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_object->Stop ();
  NS_LOG_LOGIC ("At time " << Simulator::Now ().GetSeconds () << "s object " << this << " has been stopped");
  ScheduleRepairEvent ();
}

void
FailureRepairModel::ResetObject ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_object->Reset ();
  NS_LOG_LOGIC ("At time " << Simulator::Now ().GetSeconds () << "s object " << this << " has been reset");
  if (Simulator::Now () < m_activeTime)
    {
      ScheduleFailureEvent ();
    }
}

void
FailureRepairModel::AttachToObject (Ptr<Object> obj)
{
  NS_LOG_FUNCTION (this << obj);
  m_object = obj;
  ScheduleFailureEvent ();
}

void
FailureRepairModel::AddFailureRelatedObject (Ptr<FailureRepairModel> model, double dependency)
{
  NS_LOG_FUNCTION (this << model << dependency);
  m_failureRelatedObjects[model] = dependency;
}

void
FailureRepairModel::RemoveFailureRelatedObject (Ptr<FailureRepairModel> model)
{
  NS_LOG_FUNCTION (this << model);
  m_failureRelatedObjects.erase (model);
}

void
FailureRepairModel::AddRepairRelatedObject (Ptr<FailureRepairModel> model, double dependency)
{
  NS_LOG_FUNCTION (this << model << dependency);
  m_repairRelatedObjects[model] = dependency;
}

void
FailureRepairModel::RemoveRepairRelatedObject (Ptr<FailureRepairModel> model)
{
  NS_LOG_FUNCTION (this << model);
  m_repairRelatedObjects.erase (model);
}

void
FailureRepairModel::ScheduleFailureEvent ()
{
  NS_LOG_FUNCTION_NOARGS ();

  Time failureInterval = Seconds (m_meanTimeToFailure.GetValue ());
  m_totalUpTime += failureInterval;
  NS_LOG_LOGIC ("Failure of object " << m_object << " scheduled " << failureInterval.GetSeconds () << "s from now, total up time " << m_totalUpTime.GetSeconds () << "s");
  if (m_object->IsOperational ())
    {
      m_failureEvent = Simulator::Schedule (failureInterval, &FailureRepairModel::StopObject, this);
    }
  for (std::map<Ptr<FailureRepairModel>, double>::iterator it = m_failureRelatedObjects.begin (); it != m_failureRelatedObjects.end (); it++)
    {
      if (m_failureRelatedVariable.GetValue () < it->second)
        {
          it->first->ScheduleFailureEvent ();
        }
    }
}

void
FailureRepairModel::ScheduleRepairEvent ()
{
  NS_LOG_FUNCTION_NOARGS ();

  Time repairInterval = Seconds (m_meanTimeToRepair.GetValue ());
  m_totalDownTime += repairInterval;
  NS_LOG_LOGIC ("Repair of object " << m_object << " scheduled " << repairInterval.GetSeconds () << "s from now, total down time " << m_totalDownTime.GetSeconds () << "s");
  if (!m_object->IsOperational ())
    {
      m_failureEvent = Simulator::Schedule (repairInterval, &FailureRepairModel::ResetObject, this);
    }
  for (std::map<Ptr<FailureRepairModel>, double>::iterator it = m_repairRelatedObjects.begin (); it != m_repairRelatedObjects.end (); it++)
    {
      if (m_repairRelatedVariable.GetValue () < it->second)
        {
          it->first->ScheduleRepairEvent ();
        }
    }
}

}
