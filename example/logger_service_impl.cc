#include "electron/example/logger_service_impl.h"

#include <memory>

#include "base/callback.h"

#include "electron/example/public/mojom/logger.mojom.h"

LoggerServiceImpl::LoggerServiceImpl(
    mojo::PendingReceiver<service_manager::mojom::Service> receiver)
    : service_receiver_(this, std::move(receiver)) {}

LoggerServiceImpl::~LoggerServiceImpl() {}

void LoggerServiceImpl::OnStart() {
  LOG(INFO) << __FUNCTION__;
  registry_.AddInterface<logger::mojom::Logger>(
      base::BindRepeating(&LoggerServiceImpl::Create, base::Unretained(this)));
}

void LoggerServiceImpl::OnBindInterface(
    const service_manager::BindSourceInfo& source,
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  LOG(INFO) << __FUNCTION__ << ", interface_name: " << interface_name;
  registry_.BindInterface(interface_name, std::move(interface_pipe));
}

void LoggerServiceImpl::Create(
    mojo::PendingReceiver<logger::mojom::Logger> receiver) {
  receivers_.Add(this, std::move(receiver));
}

void LoggerServiceImpl::Log(const std::string& message) {
  LOG(INFO) << __FUNCTION__ << ", message: " << message;
  lines_.push_back(message);
}

void LoggerServiceImpl::GetTail(GetTailCallback callback) {
  LOG(INFO) << __FUNCTION__;
  std::move(callback).Run(lines_.back());
}