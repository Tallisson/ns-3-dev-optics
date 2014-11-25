/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WDM_PHY_H
#define WDM_PHY_H

#include "wdm-net-device.h"
#include "wdm-channel.h"

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

#include <map>
#include <string>

namespace ns3 {

class Packet;
class Channel;
class WdmChannel;
class NetDevice; /* TODO */

class WdmPhy : public Object
{
public:
  static TypeId GetTypeId (void);

  WdmPhy (int numWavelengths = 4); /* TODO PROVJERI JAKO ČUDNO */
  virtual ~WdmPhy ();

  // TODO ATTACH
  Ptr<Channel> GetChannel (void) const;
  void SetChannel (Ptr<WdmChannel>);
  Ptr<WdmNetDevice> GetDevice (void) const; /* TODO generaliziraj da radi za Object umjesto NetDevice */
  //std::string GetDeviceType (void) const;
  void SetDevice (Ptr<WdmNetDevice> netDevice);

  void NotifyChannelStopped ();

protected:
  Ptr<WdmChannel> m_channel;
  uint64_t m_channelFloorFrequency;
  uint64_t m_channelCeilFrequency;
  uint64_t m_channelSpacing;
  std::map<uint64_t, bool> m_channelBusy;
  uint32_t m_numWavelengths;
  Ptr<WdmNetDevice> m_device; /* TODO */
  //std::string m_deviceType;
};

class OpticalSignalFrequency
{
public:
  OpticalSignalFrequency (uint64_t frequency);
  ~OpticalSignalFrequency ();
  static OpticalSignalFrequency ConvertWavelengthToFrequency (double signalWavelength);

private:
  uint64_t m_frequency;
};

class OpticalSignalWavelength
{
public:
  OpticalSignalWavelength (double wavelength);
  ~OpticalSignalWavelength ();
  static OpticalSignalWavelength ConvertFrequencyToWavelength (uint64_t signalFrequency);

private:
  double m_wavelength;
};

class WdmInputPhy : public WdmPhy
{
public:
  void ReceivePacket (Ptr<Packet> packet, uint64_t signalFrequency); // FIXME isto kao gore
  // TODO: Define
  //void ReceivePacket (Ptr<Packet> packet, double signalWavelength);
  Ptr<WdmOutputPhy> GetOtherEndPhy () const;
};

class WdmOutputPhy : public WdmPhy
{
public:
  void SendPacket (Ptr<Packet> packet, uint64_t signalFrequency); // FIXME valna duljina, jačina, itd.
  // TODO: Define
  //void SendPacket (Ptr<Packet> packet, double signalWavelength);
  Ptr<WdmInputPhy> GetOtherEndPhy () const; /* TODO preimenuj */
};

class WdmInputOutputPhy : public WdmPhy
{
public:
  void SendPacket (Ptr<Packet> packet, uint64_t signalFrequency);
  // TODO: Define
  //void SendPacket (Ptr<Packet> packet, double signalWavelength);
  void ReceivePacket (Ptr<Packet> packet, uint64_t signalFrequency);
  // TODO: Define
  //void ReceivePacket (Ptr<Packet> packet, OpticalSignalWavelength signalWavelength);
};

} // namespace ns3

#endif // WDM_PHY_H
