#include <iostream>
#include <string>
#include <vector>

#include "base/at_exit.h"
#include "base/base_switches.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/debug/stack_trace.h"
#include "base/feature_list.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/no_destructor.h"
#include "base/run_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/task/single_thread_task_executor.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "base/threading/thread.h"

#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"

#include "electron/example/public/mojom/logger.mojom.h"
#include "electron/example/service_manifest.h"

#include "services/service_manager/background_service_manager.h"
#include "services/service_manager/public/cpp/constants.h"
#include "services/service_manager/public/cpp/manifest.h"
#include "services/service_manager/public/cpp/manifest_builder.h"
#include "services/service_manager/public/cpp/service.h"
#include "services/service_manager/public/cpp/service_receiver.h"
#include "services/service_manager/tests/background.test-mojom.h"

class DummyServiceImpl : public service_manager::Service {
 public:
  explicit DummyServiceImpl(
      mojo::PendingReceiver<service_manager::mojom::Service> receiver)
      : service_receiver_(this, std::move(receiver)) {
    LOG(INFO) << __FUNCTION__;
  }

  ~DummyServiceImpl() override = default;

  service_manager::Connector* connector() {
    LOG(INFO) << __FUNCTION__;
    return service_receiver_.GetConnector();
  }

 private:
  service_manager::ServiceReceiver service_receiver_;

  DISALLOW_COPY_AND_ASSIGN(DummyServiceImpl);
};

void SetFlagAndRunClosure(bool* flag, base::OnceClosure closure) {
  //   base::debug::StackTrace().Print();
  *flag = true;
  std::move(closure).Run();
}

void ReceiveGetTail(base::OnceClosure closure, const std::string& message) {
  LOG(INFO) << __FUNCTION__ << ", message: " << message;
  std::move(closure).Run();
}

int main() {
  base::AtExitManager exit_manager;

  base::CommandLine::Init(0, nullptr);

  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_ALL;
  settings.delete_old = logging::DELETE_OLD_LOG_FILE;
  settings.log_file_path = FILE_PATH_LITERAL("test_service_manager2.log");
  logging::InitLogging(settings);

  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  base::FeatureList::InitializeInstance(
      command_line->GetSwitchValueASCII(switches::kEnableFeatures),
      command_line->GetSwitchValueASCII(switches::kDisableFeatures));

  mojo::core::Init();

  base::ThreadPoolInstance::CreateAndStartWithDefaultParams(
      "StandaloneService");

  base::Thread ipc_thread("ipc");
  ipc_thread.StartWithOptions(
      base::Thread::Options(base::MessagePumpType::IO, 0));

  mojo::core::ScopedIPCSupport ipc_support(
      ipc_thread.task_runner(),
      mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);

  base::SingleThreadTaskExecutor main_thread_task_executor(
      base::MessagePumpType::IO);

  service_manager::BackgroundServiceManager background_service_manager(
      GetManifests());

  mojo::PendingRemote<service_manager::mojom::Service> service_remote;
  DummyServiceImpl dummy_service_impl(
      service_remote.InitWithNewPipeAndPassReceiver());
  background_service_manager.RegisterService(
      service_manager::Identity(kDummyService, base::Token::CreateRandom(),
                                base::Token{}, base::Token::CreateRandom()),
      std::move(service_remote), mojo::NullReceiver());

  mojo::Remote<service_manager::mojom::TestService> background_test_service;
  dummy_service_impl.connector()->Connect(
      service_manager::ServiceFilter::ByName(kBackgroundTestService),
      background_test_service.BindNewPipeAndPassReceiver());

  mojo::Remote<logger::mojom::Logger> logger_service;
  dummy_service_impl.connector()->Connect(
      service_manager::ServiceFilter::ByName(kLoggerService),
      logger_service.BindNewPipeAndPassReceiver());

  LOG(INFO) << "START";
  base::RunLoop run_loop;

  logger_service->GetTail(
      base::BindOnce(&ReceiveGetTail, base::BindOnce([] {})));

  bool got_result = false;
  background_test_service->Test(base::BindOnce(
      &SetFlagAndRunClosure, &got_result, base::BindOnce([] {})));

  run_loop.Run();

  base::ThreadPoolInstance::Get()->Shutdown();
  LOG(INFO) << "STOP";
  return 0;
}