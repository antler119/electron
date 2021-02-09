#include <cmath>
#include <iostream>
#include <string>
#include <thread>

#include "base/bind.h"
#include "base/callback.h"
#include "base/run_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/task/single_thread_task_executor.h"
#include "base/threading/thread.h"

#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"

#include "mojo/public/cpp/bindings/remote.h"

#include "electron/example/logger_interface_impl.h"

int Floor(float f) {
  std::cout << "tid: " << std::this_thread::get_id() << std::endl;
  return std::floor(f);
}

std::string IntToString(int i) {
  std::cout << "tid: " << std::this_thread::get_id() << std::endl;
  return base::NumberToString(i);
}

void Closure(float f) {
  std::cout << "tid: " << std::this_thread::get_id() << ", f: " << f
            << std::endl;
}

int main() {
  mojo::core::Init();

  base::SingleThreadTaskExecutor main_thread_task_executor(
      base::MessagePumpType::IO);

  base::RunLoop run_loop;

  base::Thread ipc_thread("ipc");
  ipc_thread.StartWithOptions(
      base::Thread::Options(base::MessagePumpType::IO, 0));

  mojo::core::ScopedIPCSupport ipc_support(
      ipc_thread.task_runner(),
      mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);

  base::OnceCallback<int(float)> first = base::BindOnce(&Floor);
  base::OnceCallback<std::string(int)> second = base::BindOnce(&IntToString);

  std::string r = std::move(second).Run(std::move(first).Run(3.5f));
  std::cout << "result: " << r << std::endl;

  //   base::SingleThreadTaskExecutor main_thread_task_executor(
  //       base::MessagePumpType::IO);

  //   base::RunLoop run_loop;

  //   std::cout << "bg_thread begin: " << std::endl;
  //   base::Thread bg_thread("bg_thread");
  //   //   bg_thread.task_runner()->PostTask(FROM_HERE,
  //   //   base::BindOnce(&Closure, 6.6));
  //   std::cout << "bg_thread end: " << std::endl;

  //   std::cout << "begin: " << std::endl;
  //   run_loop.Run();
  //   std::cout << "end: " << std::endl;

  mojo::Remote<logger::mojom::Logger> logger;
  LoggerInterfaceImpl impl(logger.BindNewPipeAndPassReceiver(),
                           run_loop.QuitClosure());

  logger->Log("OK cool");
  logger.reset();

  run_loop.Run();
  return 0;
}