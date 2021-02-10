/**
**********************************************************************************************************************************************************************************************************************************
* @file:	service_manifest.h
* @author:
* @date:	2021年02月10日 09:47:41 Wednesday
* @brief:
**********************************************************************************************************************************************************************************************************************************
**/
#ifndef __SERVICE_MANIFEST__H__
#define __SERVICE_MANIFEST__H__

#include <vector>

#include "services/service_manager/public/cpp/manifest.h"

extern const char kLoggerService[];
extern const char kLoggerCapability[];

const std::vector<service_manager::Manifest>& GetManifests();

#endif  //!__SERVICE_MANIFEST__H__