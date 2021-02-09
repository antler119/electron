#include <iostream>
#include <string>

#include "base/at_exit.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/run_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/task/single_thread_task_executor.h"
#include "base/threading/thread.h"

#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"

int main() {
  base::AtExitManager exit_manager;

  base::CommandLine::Init(0, nullptr);

  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_ALL;
  settings.delete_old = logging::DELETE_OLD_LOG_FILE;
  settings.log_file_path = L"mojo.log";
  logging::InitLogging(settings);

  base::SingleThreadTaskExecutor main_thread_task_executor(
      base::MessagePumpType::IO);

  base::RunLoop run_loop;

  mojo::core::Init();

  base::Thread ipc_thread("ipc");
  ipc_thread.StartWithOptions(
      base::Thread::Options(base::MessagePumpType::IO, 0));

  mojo::core::ScopedIPCSupport ipc_support(
      ipc_thread.task_runner(),
      mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);

  run_loop.Run();
  return 0;
}