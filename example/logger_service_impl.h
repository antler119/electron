/**
**********************************************************************************************************************************************************************************************************************************
* @file:	logger_service_impl.h
* @author:
* @date:	2021年02月09日 17:20:48 Tuesday
* @brief:
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __LOGGER_SERVICE_IMPL__H__
#define __LOGGER_SERVICE_IMPL__H__

#include "base/logging.h"

#include "services/service_manager/public/cpp/service.h"

#include "electron/example/logger_interface_impl.h"

class LoggerServiceImpl : public service_manager::Service {
 public:
  explicit LoggerServiceImpl();
  ~LoggerServiceImpl() override;

  void OnStart() override;

  void OnBindInterface(const service_manager::BindSourceInfo& source,
                       const std::string& interface_name,
                       mojo::ScopedMessagePipeHandle interface_pipe) override;

 private:
  std::unique_ptr<LoggerInterfaceImpl> logger_interface_;
};

#endif  //!__LOGGER_SERVICE_IMPL__H__