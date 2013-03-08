/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/

// Disable "'this': used in base member initializer list"
#ifdef _MSC_VER
# pragma warning( push )
# pragma warning(disable: 4355)
#endif

#include <iostream>
#include <cstring>
#include <map>


#include <qi/log.hpp>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "transportsocket.hpp"
#include "tcptransportsocket.hpp"
#include "message_p.hpp"

#include "message.hpp"
#include <qi/buffer.hpp>
#include "transportsocket_p.hpp"

qiLogCategory("qimessaging.transportsocket");

namespace qi
{

  TransportSocket::TransportSocket(qi::EventLoop *eventLoop)
    : _p(new TransportSocketPrivate(this, eventLoop))
  {
    // Set messageReady signal to async mode to protect our network thread
    messageReady.setCallType(MetaCallType_Queued);
  }

  TransportSocket::TransportSocket(TransportSocketPrivate *p)
    : _p(p)
  {
    // Set messageReady signal to async mode to protect our network thread
    messageReady.setCallType(MetaCallType_Queued);
  }

  TransportSocket::~TransportSocket()
  {
    delete _p;
    _p = 0;
  }


  qi::Url TransportSocket::url() const {
    return _p->_url;
  }

  TransportSocket::Status TransportSocket::status() const {
    return _p->_status;
  }

  int TransportSocket::error() const
  {
    return _p->_err;
  }

  bool TransportSocket::isConnected() const
  {
    return _p->_status == qi::TransportSocket::Status_Connected;
  }

  qi::SignalBase::Link TransportSocket::messagePendingConnect(unsigned int serviceId, unsigned int objectId, boost::function<void (const qi::Message&)> fun) {
    return _p->_dispatcher.messagePendingConnect(serviceId, objectId, fun);
  }

  bool                 TransportSocket::messagePendingDisconnect(unsigned int serviceId, unsigned int objectId, qi::SignalBase::Link linkId) {
    return _p->_dispatcher.messagePendingDisconnect(serviceId, objectId, linkId);
  }

  TransportSocketPtr makeTransportSocket(const std::string &protocol, qi::EventLoop *eventLoop) {
    TransportSocketPtr ret;

    if (protocol == "tcp")
    {
      return TcpTransportSocketPtr(new TcpTransportSocket(eventLoop, false));
    }
#ifdef WITH_SSL
    else if (protocol == "tcps")
    {
      return TcpTransportSocketPtr(new TcpTransportSocket(eventLoop, true));
    }
#endif
    else
    {
      qiLogError() << "Unrecognized protocol to create the TransportSocket: " << protocol;
      return ret;
    }
  }
}

#ifdef _MSC_VER
# pragma warning( pop )
#endif
