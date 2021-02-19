#include "services/service_manager/public/cpp/service_executable/service_main.h"

#include "base/logging.h"
#include "base/task/single_thread_task_executor.h"

#include "electron/example/logger_service_impl.h"

void ServiceMain(
    mojo::PendingReceiver<service_manager::mojom::Service> receiver) {
  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_ALL;
  settings.delete_old = logging::DELETE_OLD_LOG_FILE;
  settings.log_file_path = FILE_PATH_LITERAL("logger_service_main.log");
  logging::InitLogging(settings);

  LOG(INFO) << __FUNCTIONW__;
  base::SingleThreadTaskExecutor main_task_executor;
  LoggerServiceImpl(std::move(receiver)).RunUntilTermination();
}