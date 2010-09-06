/*
 * connection_handler.cpp
 *
 *  Created on: Oct 13, 2009 at 2:26:49 PM
 *      Author: Jean-Charles DELAY
 * 			Mail  : jdelay@aldebaran-robotics.com
 */

#include <alcommon-ng/transport/shm/server/connection_handler.hpp>
#include <alcommon-ng/transport/common/handlers_pool.hpp>

#include <alcommon-ng/transport/shm/memory/mapped_device.hpp>
#include <alcommon-ng/transport/shm/memory/mapped_segment_selector.hpp>
#include <boost/iostreams/stream_buffer.hpp>

#include <boost/bind.hpp>

#include <boost/interprocess/shared_memory_object.hpp>

#include <alcommon-ng/collections/variables_list.hpp>
#include <alcommon-ng/transport/shm/client/shmconnection.hpp>

namespace AL {
  namespace Transport {

  ConnectionHandler::ConnectionHandler(const std::string & rdv_name, IDataHandler *dataHandler, ResultHandler &resultHandler)
    : rdv_name(rdv_name),
      fDataHandler(dataHandler),
      resultHandler(resultHandler)
  {
    this->setTaskName("ConnectionHandler");
  }

  ConnectionHandler::~ConnectionHandler () {
    MappedSegmentSelector::instance().free(rdv_name.c_str());
  }

  void ConnectionHandler::run() {
    unsigned int id;
    std::string  def;

    try {
      io::stream_buffer<MappedDevice> buf(MappedSegmentSelector::instance().get(rdv_name.c_str(),
        MappedSegmentSelector::MS_OPEN | MappedSegmentSelector::MS_REMOVE));
      std::iostream stream(&buf);
      stream >> id;
      stream >> def;
    } catch (const std::exception & e) {
      std::cerr << "[IppcThread] Exception caught : " << e.what() << std::endl;
    }

    boost::interprocess::shared_memory_object::remove(rdv_name.c_str());

    if (fDataHandler) {
      try
      {
        std::string result;
        fDataHandler->dataHandler(def, result);
        //std::cout << "sending result:" << id << "size:" << result.size() << std::endl;
        ShmConnection connection("clientserv", resultHandler);
        connection.sendResult(id, result);
        //TODO:CTAF:

//        AL::ALPtr<ResultDefinition> result = callback->ippcCallback(def);
//        if (response_delegate)
//          response_delegate->serverResponseHandler(def, result);
      }
      catch (...)
      {
        std::cout << "Exception catch at ConnectionHandler::run level" << std::endl;
      }
    }
  }

}
}
