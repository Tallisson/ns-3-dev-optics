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

#ifndef SEQ_BUS_HEADER_H
#define SEQ_BUS_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \ingroup bus application
 * \class SeqBusHeader
 * \brief Packet header for Udp client/server application
 * The header is made of a 32bits sequence number followed by
 * a 64bits time stamp.
 */
class SeqBusHeader : public Header
{
public:
  SeqBusHeader ();

  /**
   * \param seq the sequence number
   */
  void SetSeq (uint32_t seq);
  /**
   * \return the sequence number
   */
  uint32_t GetSeq (void) const;
  /**
   * \return the time stamp
   */
  Time GetTs (void) const;

  void SetTheta (int theta);
  int GetTheta();
  void SetVoltage (int voltage);
  int GetVoltage();
  void SetActive (int theta);
  int GetActive();
  void SetReactive (int theta);
  int GetReactive ();
  void SetIdNode (uint32_t m_id_node);
  uint32_t GetIdNode ();
  void SetLosses (uint64_t losses);
  uint64_t GetLosses ();

  static TypeId GetTypeId (void);

  int DoubleToInt (double value);
  double IntToDouble (int value);
private:
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  uint32_t m_seq;
  uint64_t m_ts;
  uint64_t m_theta;
  uint64_t m_voltage;
  uint64_t m_active;
  uint64_t m_reactive;
  uint64_t m_losses;
  uint32_t m_id_node;
};

} // namespace ns3

#endif /* SEQ_BUS_HEADER_H */
