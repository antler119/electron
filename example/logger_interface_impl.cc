#include "electron/example/logger_interface_impl.h"

#include "base/logging.h"

LoggerInterfaceImpl::LoggerInterfaceImpl(
    mojo::PendingReceiver<logger::mojom::Logger> receiver,
    base::OnceClosure disconnect_handler)
    : receiver_(this, std::move(receiver)) {
  receiver_.set_disconnect_handler(std::move(disconnect_handler));
}

LoggerInterfaceImpl::~LoggerInterfaceImpl() {}

void LoggerInterfaceImpl::Log(const std::string& message) {
  LOG(ERROR) << "[Logger] " << message;
  lines_.push_back(message);
}

void LoggerInterfaceImpl::GetTail(GetTailCallback callback) {
  std::move(callback).Run(lines_.back());
}

void LoggerInterfaceImpl::OnError() {
  LOG(ERROR) << "Client disconnected! Purging log lines.";
  lines_.clear();
}