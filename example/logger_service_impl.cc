#include "electron/example/logger_service_impl.h"

#include <memory>

#include "base/callback.h"

#include "electron/example/public/mojom/logger.mojom.h"

LoggerServiceImpl::LoggerServiceImpl() {}

LoggerServiceImpl::~LoggerServiceImpl() {}

void LoggerServiceImpl::OnStart() {
  LOG(INFO) << __FUNCTION__;
}

void LoggerServiceImpl::OnBindInterface(
    const service_manager::BindSourceInfo& source,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  LOG(INFO) << __FUNCTION__ << ", interface_name: " << interface_name;
  if (interface_name == LoggerInterfaceImpl::Name_) {
    logger_interface_ = std::make_unique<LoggerInterfaceImpl>(
        mojo::PendingReceiver<logger::mojom::Logger>(std::move(interface_pipe)),
        base::BindOnce([] {}));
  }
}