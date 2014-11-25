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
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "bus-client.h"
#include "seq-bus-header.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <ctime>

using namespace std;

namespace ns3 {

#define FILE_LOG "log-buses"

NS_LOG_COMPONENT_DEFINE ("BusClient");
NS_OBJECT_ENSURE_REGISTERED (BusClient);

TypeId
BusClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BusClient")
    .SetParent<Application> ()
    .AddConstructor<BusClient> ()
    .AddAttribute ("MaxPackets",
                   "The maximum number of packets the application will send",
                   UintegerValue (100),
                   MakeUintegerAccessor (&BusClient::m_count),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Interval",
                   "The time to wait between packets", TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&BusClient::m_interval),
                   MakeTimeChecker ())
    .AddAttribute (
      "RemoteAddress",
      "The destination Address of the outbound packets",
      AddressValue (),
      MakeAddressAccessor (&BusClient::m_peerAddress),
      MakeAddressChecker ())
    .AddAttribute ("RemotePort", "The destination port of the outbound packets",
                   UintegerValue (100),
                   MakeUintegerAccessor (&BusClient::m_peerPort),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("PacketSize",
                   "Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&BusClient::m_size),
                   MakeUintegerChecker<uint32_t> (12,1500))
    .AddAttribute ("ID",
                   "Id client",
                   UintegerValue (),
                   MakeUintegerAccessor (&BusClient::m_id),
                   MakeUintegerChecker<uint32_t> (1, 1500))
  ;
  return tid;
}

BusClient::BusClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_sent = 0;
  m_socket = 0;
  m_sendEvent = EventId ();
  is_write_md =false;
}

BusClient::~BusClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
BusClient::SetRemote (Ipv4Address ip, uint16_t port)
{
  m_peerAddress = Address(ip);
  m_peerPort = port;
}

void
BusClient::SetRemote (Ipv6Address ip, uint16_t port)
{
  m_peerAddress = Address(ip);
  m_peerPort = port;
}

void
BusClient::SetId(uint32_t id)
{
  m_id = id;
}

void
BusClient::SetRemote (Address ip, uint16_t port)
{
  m_peerAddress = ip;
  m_peerPort = port;
}

void
BusClient::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Application::DoDispose ();
}

void
BusClient::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind ();
          m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort));

        }
      else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind6 ();
          m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&BusClient::HandleRead, this));
  m_sendEvent = Simulator::Schedule (Seconds (0.0), &BusClient::Send, this);
}

void
BusClient::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  Simulator::Cancel (m_sendEvent);
}

void
BusClient::Send (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_sendEvent.IsExpired ());
  SeqBusHeader seqTs;
  seqTs.SetSeq (m_sent);
  seqTs.SetIdNode(this->m_id);

  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (seqTs);


  //NS_LOG_INFO("Client " << seqTs.GetIdNode());

  std::stringstream peerAddressStringStream;
  if (Ipv4Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
    }
  else if (Ipv6Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
    }

  if ((m_socket->Send (p)) >= 0)
    {
      ++m_sent;
      if(m_id)
      {
        NS_LOG_INFO ("TraceDelay TX " << m_size << " bytes to "
                                      << peerAddressStringStream.str () << " Uid: "
                                      << p->GetUid () << " Time: "
                                      << (Simulator::Now ()).GetSeconds ());
      }

    }
  else
    {
      NS_LOG_INFO ("Error while sending " << m_size << " bytes to "
                                          << peerAddressStringStream.str ());
    }

  if (m_sent < m_count)
    {
      //m_sendEvent = Simulator::Schedule (m_interval, &BusClient::Send, this);
    }
}

void
BusClient::HandleRead(Ptr<Socket> socket)
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
          uint32_t currentSequenceNumber = seqTs.GetSeq ();
          if (InetSocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () <<
                           " bytes from "<< InetSocketAddress::ConvertFrom (from).GetIpv4 () <<
                           " Sequence Number: " << currentSequenceNumber <<
                           " Uid: " << packet->GetUid () <<
                           " TXtime: " << seqTs.GetTs () <<
                           " RXtime: " << Simulator::Now () <<
                           " Delay: " << Simulator::Now ().GetMilliSeconds() - seqTs.GetTs ().GetMilliSeconds());
            }
          else if (Inet6SocketAddress::IsMatchingType (from))
            {
              NS_LOG_INFO ("TraceDelay: RX " << packet->GetSize () <<
                           " bytes from "<< Inet6SocketAddress::ConvertFrom (from).GetIpv6 () <<
                           " Sequence Number: " << currentSequenceNumber <<
                           " Uid: " << packet->GetUid () <<
                           " TXtime: " << seqTs.GetTs () <<
                           " RXtime: " << Simulator::Now () <<
                           " Delay: " << Simulator::Now().GetMilliSeconds() - seqTs.GetTs ().GetMilliSeconds());
            }
        }
    }
}

uint32_t
BusClient::GetId()
{
  return m_id;
}

}
