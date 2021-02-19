/**
**********************************************************************************************************************************************************************************************************************************
* @file:	logger_impl.h
* @author:
* @date:	2021年02月08日 13:55:57 Monday
* @brief:
**********************************************************************************************************************************************************************************************************************************
**/
#ifndef __LOGGER_INTERFACE_IMPL__H__
#define __LOGGER_INTERFACE_IMPL__H__

#include <memory.h>
#include <vector>

#include "base/macros.h"

#include "electron/example/public/mojom/logger.mojom.h"

#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote.h"

class LoggerInterfaceImpl : public logger::mojom::Logger {
 public:
  explicit LoggerInterfaceImpl();

  ~LoggerInterfaceImpl() override;

  void Log(const std::string& message) override;
  void GetTail(GetTailCallback callback) override;

 protected:
  mojo::ReceiverSet<logger::mojom::Logger> receivers_;
  std::vector<std::string> lines_;

 private:
  DISALLOW_COPY_AND_ASSIGN(LoggerInterfaceImpl);
};

#endif  //!__LOGGER_INTERFACE_IMPL__H__
