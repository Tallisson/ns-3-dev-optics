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
 * Author: Thiago Allisson R. da Silva <allissonribeiro02@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "seq-bus-header.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/load-flow.h"
#include "packet-loss-counter.h"

#include "bus-server.h"

namespace ns3 {

#define FILE_SIMULATION "/home/thiago/workspace/LoadFlow/examples/14-bus.txt"

double diff_P[] = {0.10, 0.25, -0.10, -0.25};
double diff_Q[] = {0.10, 0.25, -0.10, -0.25};
int barras[] = {6};
double diff_V[] = {0.10, 0.25, -0.10, -0.25, -0.5};

NS_LOG_COMPONENT_DEFINE ("BusServer");
NS_OBJECT_ENSURE_REGISTERED (BusServer);

TypeId
BusServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BusServer")
    .SetParent<Application> ()
    .AddConstructor<BusServer> ()
    .AddAttribute ("Port",
                   "Port on which we listen for incoming packets.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&BusServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("PacketWindowSize",
                   "The size of the window used to compute the packet loss. This value should be a multiple of 8.",
                   UintegerValue (32),
                   MakeUintegerAccessor (&BusServer::GetPacketWindowSize,
                                         &BusServer::SetPacketWindowSize),
                   MakeUintegerChecker<uint16_t> (8,256))
    .AddAttribute ("IsCentralNode",
                   "Flag to inform if node is or no central node",
                   BooleanValue (false),
                   MakeBooleanAccessor(&BusServer::GetIsCentralNode,
                                      &BusServer::SetIsCentralNode),
                   MakeBooleanChecker ())
    .AddAttribute ("CounterPackets",
                   "Flag to inform if node is or no central node",
                   UintegerValue (14),
                   MakeUintegerAccessor (&BusServer::GetCount,
                                         &BusServer::SetCount),
                   MakeUintegerChecker<uint32_t> (1,256))
  ;
  return tid;
}

BusServer::BusServer ()
  : m_sent (0),
    m_lossCounter (0),
    m_interval(Seconds (1.0))
{
  NS_LOG_FUNCTION (this);
  m_received=0;
  m_socket = 0;
  m_socket6 = 0;
  cont_p = 0;
  cont_q = 0;
  id_bus = 0;
  cont_b = 0;
  cont_v = 0;

}

BusServer::~BusServer ()
{
  NS_LOG_FUNCTION (this);
}

uint16_t
BusServer::GetPacketWindowSize () const
{
  return m_lossCounter.GetBitMapSize ();
}

void
BusServer::SetPacketWindowSize (uint16_t size)
{
  m_lossCounter.SetBitMapSize (size);
}

uint32_t
BusServer::GetLost (void) const
{
  return m_lossCounter.GetLost ();
}

uint32_t
BusServer::GetReceived (void) const
{

  return m_received;

}

bool
BusServer::GetIsCentralNode (void) const
{
  return m_isCentralNode;
}

void
BusServer::SetIsCentralNode (bool isCentralNode)
{
  m_isCentralNode = isCentralNode;
}

void
BusServer::ServerHandleConnectionCreated (Ptr<Socket> s, const Address & addr)
{
  s->SetRecvCallback (MakeCallback (&BusServer::HandleRead, this));

  uint32_t index = m_clients.size();
  m_clients.insert (std::make_pair(index, addr));
  socket_clients.insert (std::make_pair (addr, s));

  Simulator::Schedule (m_interval, &BusServer::Send, this, index);
  //s->SetSendCallback (MakeCallback (&TcpTestCase::ServerHandleSend, this));
}

void
BusServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
BusServer::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), TcpSocketFactory::GetTypeId());
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
                                                   m_port);
      m_socket->Bind (local);
      m_socket->Listen ();
      m_socket->SetAcceptCallback (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
                                     MakeCallback (&BusServer::ServerHandleConnectionCreated, this));
    }


  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (),
                                                   m_port);
      m_socket6->Bind (local);
      m_socket6->Listen ();
      m_socket6->SetAcceptCallback (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
          MakeCallback (&BusServer::ServerHandleConnectionCreated, this));
    }

}

void
BusServer::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0)
    {
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void
BusServer::SetCount (uint32_t count)
{
  m_count = count;
}

uint32_t
BusServer::GetCount(void) const
{
  return m_count;
}

void
BusServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;

  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
          SeqBusHeader seqTs;
          packet->RemoveHeader (seqTs);
          uint32_t index_client = seqTs.GetIdNode();
          NS_LOG_INFO("Client " << index_client);
          std::map<Address, Ptr<Socket> >::iterator it = socket_clients.find(from);
          if((it != socket_clients.end()))
          {
            m_index.insert(std::make_pair(it->first, index_client));
          }

          uint32_t currentSequenceNumber = seqTs.GetSeq ();
          if (InetSocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () <<
                           " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
                           " Sequence Number: " << currentSequenceNumber <<
                           " Uid: " << packet->GetUid () <<
                           " TXtime: " << seqTs.GetTs () <<
                           " RXtime: " << Simulator::Now () <<
                           " Delay: " << Simulator::Now () - seqTs.GetTs ());
            }
          else if (Inet6SocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () <<
                           " bytes from "<< Inet6SocketAddress::ConvertFrom (from).GetIpv6 () <<
                           " Sequence Number: " << currentSequenceNumber <<
                           " Uid: " << packet->GetUid () <<
                           " TXtime: " << seqTs.GetTs () <<
                           " RXtime: " << Simulator::Now () <<
                           " Delay: " << Simulator::Now () - seqTs.GetTs ());
            }

          /*packet->RemoveAllPacketTags ();
          packet->RemoveAllByteTags ();
          NS_LOG_INFO(socket->SendTo (p, 0, from));*/

          m_lossCounter.NotifyReceived (currentSequenceNumber);
          m_received++;
        }
    }
}

void
BusServer::Send (uint32_t peer)
{
  NS_LOG_FUNCTION_NOARGS ();
  SeqBusHeader seqTs;
  seqTs.SetSeq (m_sent);
  Address m_peerAddress = m_clients.at(peer);

  Ptr<LoadFlow> load_flow = CreateObject<LoadFlow>();
  load_flow->SetVerbose (false);
  load_flow->SetError (0.0001);
  load_flow->Configure(FILE_SIMULATION);
  std::map<Address, uint32_t >::iterator it = m_index.find(m_peerAddress);
  if(it != m_index.end())
  {
    uint32_t index = it->second;

    load_flow->Execute ();
    Ptr<Bus> bus = load_flow->GetBar(index);
    seqTs.SetVoltage(seqTs.DoubleToInt(load_flow->GetBar(index)->GetVoltage()) );
    seqTs.SetActive(seqTs.DoubleToInt(load_flow->GetBar(index)->GetAPower()) );
    seqTs.SetReactive(seqTs.DoubleToInt(load_flow->GetBar(index)->GetRPower()) );
    seqTs.SetTheta(seqTs.DoubleToInt(load_flow->GetBar(index)->GetAngle()) );
    seqTs.SetLosses(seqTs.DoubleToInt(load_flow->GetTotalLoss()) );

    /*if (cont_p < 4)
    {
      //Ptr<Bus> bus = load_flow->GetBar(index);
      if(cont_p > 0) {
        bus->SetAPower(bus->initial_apower);
      }
      bus->SetAPower(bus->GetAPower() + diff_P[cont_p]);
      bus->SetAPowerG(bus->GetAPowerG() + diff_P[cont_p]);
      cont_p++;
    } else if (cont_q < 4)
    {
      //Ptr<Bus> bus = load_flow->GetBar(index);
      if(cont_q > 0)
      {
        bus->SetRPower(bus->initial_rpower);
      } else {
        bus->SetAPower(bus->initial_apower);
      }

      bus->SetRPower(bus->GetRPower() + diff_Q[cont_q]);
      bus->SetRPowerG(bus->GetRPowerG() + diff_Q[cont_q]);
      cont_q++;
    } else if (cont_v < 12)
    {
      if(cont_v > 0)
      {
        bus->SetVoltage(bus->initial_voltage);
      } else
      {
        bus->SetRPower(bus->initial_rpower);
      }

      if (cont_b < 6)
      {
        bus = load_flow->GetBar(barras[id_bus]);
        bus->SetVoltage(bus->GetVoltage() + diff_V[cont_b]);
        cont_b++;
      } else
      {
        id_bus++;
        cont_b = 0;
      }
      cont_v++;
    }*/
    load_flow->Dispose();
    seqTs.SetIdNode (m_id);
  }

  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (seqTs);

  std::stringstream peerAddressStringStream;

  if (Ipv4Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
    }
  else if (Ipv6Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
    }

  Ptr<Socket> s = socket_clients.at (m_peerAddress);
  if ((s->SendTo (p, 0, m_peerAddress)) >= 0)
    {
      ++m_sent;
      NS_LOG_INFO ("TraceDelay TX " << p->GetSize() << " bytes to "
                                    << peerAddressStringStream.str () << " Uid: "
                                    << p->GetUid () << " Time: "
                                    << (Simulator::Now ()).GetSeconds ());

    }
  else
    {
      NS_LOG_INFO ("Error while sending " << p->GetSize() << " bytes to "
                                          << peerAddressStringStream.str ());
    }

  if (m_sent < m_count)
    {
      Simulator::Schedule (m_interval, &BusServer::Send, this, peer);
    }
}

void
BusServer::SetId (uint32_t id)
{
  m_id = id;
}

}
