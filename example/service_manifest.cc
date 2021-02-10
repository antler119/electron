#include "electron/example/service_manifest.h"

#include "base/no_destructor.h"

#include "services/service_manager/public/cpp/manifest_builder.h"

#include "electron/example/public/mojom/logger.mojom.h"

const char kLoggerService[] = "logger_service";
const char kLoggerCapability[] = "logger_capability";

const std::vector<service_manager::Manifest>& GetManifests() {
  static base::NoDestructor<std::vector<service_manager::Manifest>> manifests{
      {service_manager::ManifestBuilder()
           .WithServiceName(kLoggerService)
           .ExposeCapability(kLoggerCapability,
                             service_manager::Manifest::InterfaceList<
                                 logger::mojom::Logger>())
           .Build()}};
  return *manifests;
}