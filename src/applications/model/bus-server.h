/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 UFPI
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
 * Author: Thiago Allisson Ribeiro da Silva <allissonribeiro02@gmail.com>
 */

#ifndef BUS_SERVER_H_
#define BUS_SERVER_H_

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "packet-loss-counter.h"
#include <map>

using namespace std;

namespace ns3 {

class BusServer : public Application {
public:
  static TypeId GetTypeId (void);
  BusServer ();
  virtual ~BusServer ();
  /**
   * returns the number of lost packets
   * \return the number of lost packets
   */
  uint32_t GetLost (void) const;

  /**
   * \brief returns the number of received packets
   * \return the number of received packets
   */
  uint32_t GetReceived (void) const;

  /**
   * \return the size of the window used for checking loss.
   */
  uint16_t GetPacketWindowSize () const;

  /**
   * \return if node is central
   */
  bool GetIsCentralNode () const;

  /**
   * \return if node is central
   */
  uint32_t GetCount() const;

  /**
   * \brief Set the size of the window used for checking loss. This value should
   *  be a multiple of 8
   * \param size the size of the window used for checking loss. This value should
   *  be a multiple of 8
   */
  void SetPacketWindowSize (uint16_t size);

  void SetIsCentralNode (bool isCentralNode);

  void SetCount(uint32_t count);

  void SetId(uint32_t id);
protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void HandleRead (Ptr<Socket> socket);
  void ServerHandleConnectionCreated (Ptr<Socket> s, const Address & addr);
  void Send (uint32_t peer);

  uint16_t m_port;
  Address m_local;
  uint32_t m_received;
  uint32_t m_sent;
  uint32_t m_count;
  Ptr<Socket> m_socket;
  Ptr<Socket> m_socket6;
  PacketLossCounter m_lossCounter;
  bool m_isCentralNode;

  map<uint32_t, Address> m_clients;
  map<Address, uint32_t> m_index;
  map<Address, Ptr<Socket> > socket_clients;

  Time m_interval;

  uint32_t m_id;

  int cont_p;
  int cont_q;
  int id_bus;
  int cont_b;
  int cont_v;
};

}
#endif /* BUS_SERVER_H_ */
