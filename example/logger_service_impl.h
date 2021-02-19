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

#include "mojo/public/cpp/bindings/receiver_set.h"

#include "services/service_manager/public/cpp/binder_registry.h"
#include "services/service_manager/public/cpp/service.h"
#include "services/service_manager/public/cpp/service_receiver.h"

#include "electron/example/public/mojom/logger.mojom.h"

class LoggerServiceImpl : public service_manager::Service,
                          public logger::mojom::Logger {
 public:
  explicit LoggerServiceImpl(
      mojo::PendingReceiver<service_manager::mojom::Service> receiver);

  ~LoggerServiceImpl() override;

  void OnStart() override;

  void OnBindInterface(const service_manager::BindSourceInfo& source,
                       const std::string& interface_name,
                       mojo::ScopedMessagePipeHandle interface_pipe) override;

  void Create(mojo::PendingReceiver<logger::mojom::Logger> receiver);

  // logger::mojom::Logger
  void Log(const std::string& message) override;
  void GetTail(GetTailCallback callback) override;

 private:
  service_manager::ServiceReceiver service_receiver_;
  service_manager::BinderRegistry registry_;
  mojo::ReceiverSet<logger::mojom::Logger> receivers_;

  std::vector<std::string> lines_;

  DISALLOW_COPY_AND_ASSIGN(LoggerServiceImpl);
};

#endif  //!__LOGGER_SERVICE_IMPL__H__