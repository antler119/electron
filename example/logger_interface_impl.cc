#include "electron/example/logger_interface_impl.h"

#include "base/logging.h"

LoggerInterfaceImpl::LoggerInterfaceImpl() {}

LoggerInterfaceImpl::~LoggerInterfaceImpl() {}

void LoggerInterfaceImpl::Log(const std::string& message) {
  LOG(ERROR) << "[Logger] " << message;
  lines_.push_back(message);
}

void LoggerInterfaceImpl::GetTail(GetTailCallback callback) {
  std::move(callback).Run(lines_.back());
}