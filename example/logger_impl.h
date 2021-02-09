/**
**********************************************************************************************************************************************************************************************************************************
* @file:	logger_impl.h
* @author:
* @date:	2021年02月08日 13:55:57 Monday
* @brief:
**********************************************************************************************************************************************************************************************************************************
**/
#ifndef __LOGGER_IMPL__H__
#define __LOGGER_IMPL__H__
#include <memory.h>
#include <vector>

#include "base/macros.h"
#include "mojo/public/cpp/bindings/remote.h"

#include "electron/example/public/mojom/logger.mojom.h"

class LoggerImpl : public logger::mojom::Logger {
public:
  explicit LoggerImpl(mojo::PendingReceiver<logger::mojom::Logger> receiver,
                      base::OnceClosure disconnect_handler);
  ~LoggerImpl() override;

  void Log(const std::string &message) override;
  void GetTail(GetTailCallback callback) override;

  void OnError();

protected:
  mojo::Receiver<logger::mojom::Logger> receiver_;
  std::vector<std::string> lines_;

private:
  DISALLOW_COPY_AND_ASSIGN(LoggerImpl);
};

#endif //!__LOGGER_IMPL__H__