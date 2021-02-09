#include "logger_impl.h"

#include "base/logging.h"

LoggerImpl::LoggerImpl(mojo::PendingReceiver<logger::mojom::Logger> receiver,
                       base::OnceClosure disconnect_handler)
    : receiver_(this, std::move(receiver)) {
  receiver_.set_disconnect_handler(std::move(disconnect_handler));
}

LoggerImpl::~LoggerImpl() {}

void LoggerImpl::Log(const std::string &message) {
  LOG(ERROR) << "[Logger] " << message;
  lines_.push_back(message);
}

void LoggerImpl::GetTail(GetTailCallback callback) {
  std::move(callback).Run(lines_.back());
}

void LoggerImpl::OnError() {
  LOG(ERROR) << "Client disconnected! Purging log lines.";
  lines_.clear();
}