#include "electron/example/service_manifest.h"

#include "base/no_destructor.h"

#include "electron/example/public/mojom/logger.mojom.h"

#include "services/service_manager/public/cpp/manifest_builder.h"
#include "services/service_manager/tests/background.test-mojom.h"

const char kDummyService[] = "dummy_service";

const char kLoggerService[] = "logger_service";
const char kLoggerCapability[] = "logger_capability";

const char kBackgroundTestService[] = "background_service_manager_test_service";
const char kBackgroundTestCapability[] = "test_service";

const std::vector<service_manager::Manifest>& GetManifests() {
  static base::NoDestructor<std::vector<service_manager::Manifest>> manifests{
      {service_manager::ManifestBuilder()
           .WithServiceName(kDummyService)
           //  .WithOptions(service_manager::ManifestOptionsBuilder()
           //                   .CanConnectToInstancesInAnyGroup(true)
           //                   .CanConnectToInstancesWithAnyId(true)
           //                   .WithInstanceSharingPolicy(
           //                       service_manager::Manifest::
           //                           InstanceSharingPolicy::kSingleton)
           //                   .Build())
           .RequireCapability(kLoggerService, kLoggerCapability)
           .RequireCapability(kBackgroundTestService, kBackgroundTestCapability)
           .Build(),
       service_manager::ManifestBuilder()
           .WithServiceName(kLoggerService)
           .WithOptions(
               service_manager::ManifestOptionsBuilder()
                   .WithExecutionMode(service_manager::Manifest::ExecutionMode::
                                          kStandaloneExecutable)
                   .WithSandboxType("none")
                   .Build())
           .ExposeCapability(kLoggerCapability,
                             service_manager::Manifest::InterfaceList<
                                 logger::mojom::Logger>())
           .Build(),
       service_manager::ManifestBuilder()
           .WithServiceName(kBackgroundTestService)
           .WithOptions(
               service_manager::ManifestOptionsBuilder()
                   .WithExecutionMode(service_manager::Manifest::ExecutionMode::
                                          kStandaloneExecutable)
                   .WithSandboxType("none")
                   .Build())
           .ExposeCapability(kBackgroundTestCapability,
                             service_manager::Manifest::InterfaceList<
                                 service_manager::mojom::TestService>())
           .Build()}};
  return *manifests;
}