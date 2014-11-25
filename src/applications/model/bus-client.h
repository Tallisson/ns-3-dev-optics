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
#ifndef BUS_CLIENT_H_
#define BUS_CLIENT_H_

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class BusClient : public Application {
public:
  static TypeId GetTypeId (void);
  BusClient ();

  virtual ~BusClient ();

  /**
   * \brief set the remote address and port
   * \param ip remote IP address
   * \param port remote port
   */
  void SetRemote (Ipv4Address ip, uint16_t port);
  void SetRemote (Ipv6Address ip, uint16_t port);
  void SetRemote (Address ip, uint16_t port);
  void SetId(uint32_t id);

  uint32_t GetId();
  bool is_write_md;
protected:
  virtual void DoDispose (void);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTransmit (Time dt);
  void Send (void);
  void HandleRead (Ptr<Socket> socket);

  uint32_t m_count;
  Time m_interval;
  uint32_t m_size;

  uint32_t m_id;


  uint32_t m_sent;
  Ptr<Socket> m_socket;
  Address m_peerAddress;
  uint16_t m_peerPort;
  EventId m_sendEvent;

};

}

#endif /* BUS_CLIENT_H_ */
