/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008,2009 INRIA, UDCAST
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
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/load-flow.h"
#include "packet-loss-counter.h"
#include "seq-bus-header.h"
#include "bus-server.h"

namespace ns3 {

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
  ;
  return tid;
}

BusServer::BusServer ()
  : m_lossCounter (0)
{
  NS_LOG_FUNCTION (this);
  m_received=0;
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
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
                                                   m_port);
      m_socket->Bind (local);
    }

  m_socket->SetRecvCallback (MakeCallback (&BusServer::HandleRead, this));

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (),
                                                   m_port);
      m_socket6->Bind (local);
    }

  m_socket6->SetRecvCallback (MakeCallback (&BusServer::HandleRead, this));

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


/*void
BusServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
          SeqTsHeader seqTs;
          packet->RemoveHeader (seqTs);
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

          m_lossCounter.NotifyReceived (currentSequenceNumber);
          m_received++;
        }
    }
}*/

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
BusServer::Send (uint32_t peer)
{
  NS_LOG_FUNCTION_NOARGS ();
  SeqBusHeader seqTs;
  seqTs.SetSeq (m_sent);
  Address m_peerAddress = m_clients.at(peer);

  std::map<Address, uint32_t >::iterator it = m_index.find(m_peerAddress);
  if(it != m_index.end())
  {
    Ptr<LoadFlow> load_flow = CreateObject<LoadFlow> ();
    uint32_t index = it->second;

    load_flow->Execute ();
    Ptr<Bus> bus = load_flow->GetBar(index);
    seqTs.SetVoltage(seqTs.DoubleToInt(load_flow->GetBar(index)->GetVoltage()) );
    seqTs.SetActive(seqTs.DoubleToInt(load_flow->GetBar(index)->GetAPower()) );
    seqTs.SetReactive(seqTs.DoubleToInt(load_flow->GetBar(index)->GetRPower()) );
    seqTs.SetTheta(seqTs.DoubleToInt(load_flow->GetBar(index)->GetAngle()) );
    seqTs.SetLosses(seqTs.DoubleToInt(load_flow->GetTotalLoss()) );

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

} // Namespace ns3
