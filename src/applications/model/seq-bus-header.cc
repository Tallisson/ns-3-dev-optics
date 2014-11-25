/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 UFPI
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Thiago Allisson R. da Silva <allissonribeiro02@gmail.com>
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/header.h"
#include "ns3/simulator.h"
#include "seq-bus-header.h"
#include <iostream>

using namespace std;

NS_LOG_COMPONENT_DEFINE ("SeqBusHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SeqBusHeader);

SeqBusHeader::SeqBusHeader ()
  : m_seq (0),
    m_ts (Simulator::Now ().GetTimeStep ()),
    m_theta (0),
    m_voltage (0),
    m_active (0),
    m_reactive (0),
    m_id_node (0)
{
}

void
SeqBusHeader::SetSeq (uint32_t seq)
{
  m_seq = seq;
}
uint32_t
SeqBusHeader::GetSeq (void) const
{
  return m_seq;
}

Time
SeqBusHeader::GetTs (void) const
{
  return TimeStep (m_ts);
}

TypeId
SeqBusHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SeqBusHeader")
    .SetParent<Header> ()
    .AddConstructor<SeqBusHeader> ()
  ;
  return tid;
}
TypeId
SeqBusHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void
SeqBusHeader::Print (std::ostream &os) const
{
  os << "(seq=" << m_seq << " time=" << TimeStep (m_ts).GetSeconds () << ")";
}
uint32_t
SeqBusHeader::GetSerializedSize (void) const
{
  return 4+8+8+8+8+8+8+4;
}

void
SeqBusHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU32 (m_seq);
  i.WriteHtonU64 (m_ts);
  i.WriteHtonU64 (m_theta);
  i.WriteHtonU64 (m_voltage);
  i.WriteHtonU64 (m_active);
  i.WriteHtonU64 (m_reactive);
  i.WriteHtonU64 (m_losses);
  i.WriteHtonU32 (m_id_node);
}
uint32_t
SeqBusHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_seq = i.ReadNtohU32 ();
  m_ts = i.ReadNtohU64 ();
  m_theta = i.ReadNtohU64 ();
  m_voltage = i.ReadNtohU64 ();
  m_active = i.ReadNtohU64 ();
  m_reactive = i.ReadNtohU64 ();
  m_losses = i.ReadNtohU64 ();
  m_id_node = i.ReadNtohU32 ();
  return GetSerializedSize ();
}

void
SeqBusHeader::SetTheta(int theta)
{
  m_theta = theta;
}

int
SeqBusHeader::GetTheta ()
{
  return m_theta;
}

void
SeqBusHeader::SetVoltage (int voltage)
{
  m_voltage = voltage;
}

int
SeqBusHeader::GetVoltage ()
{
  return m_voltage;
}

void
SeqBusHeader::SetActive (int active)
{
  m_active = active;
}

int
SeqBusHeader::GetActive ()
{
  return m_active;
}

void
SeqBusHeader::SetReactive (int reactive)
{
  m_reactive = reactive;
}

int
SeqBusHeader::GetReactive ()
{
  return m_reactive;
}

void
SeqBusHeader::SetIdNode(uint32_t m_id_node)
{
  this->m_id_node = m_id_node;
}

uint64_t
SeqBusHeader::GetLosses ()
{
  return m_losses;
}

void
SeqBusHeader::SetLosses (uint64_t losses)
{
  this->m_losses = losses;
}

uint32_t
SeqBusHeader::GetIdNode()
{
  return m_id_node;
}

int
SeqBusHeader::DoubleToInt(double value)
{
  int v_int = (value * 10000);

  return v_int;
}

double
SeqBusHeader::IntToDouble(int value)
{
  double v_double = (value / 10000.0);

  return v_double;
}

} // namespace ns3
