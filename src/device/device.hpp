//
//  device.hpp
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#ifndef device_hpp
#define device_hpp

#include <string>

#include "bytestream.hpp"
#include "eslog.hpp"

namespace excelsecu {

template <class Transferable> class device {
public:
  device() { m_trans = std::make_unique<Transferable>(); }

  ~device() {}

public:
  bool connect(const std::string &sn) {

    try {
      m_trans->connect(sn);
    } catch (std::exception &e) {
      return false;
    }
    return true;
  }

  bool disconnect() {

    try {
      m_trans->disconnect();
    } catch (std::exception &e) {
      return false;
    }
    return true;
  }

  bytestream send(const bytestream &apdu) {

    try {
      return m_trans->send(apdu);
    } catch (std::exception &e) {
      LOG_ERROR("send apdu failed");
    }
    return bytestream();
  }

private:
  std::unique_ptr<Transferable> m_trans;
};
}; // namespace excelsecu

#endif /* device_hpp */
