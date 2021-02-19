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
  base::SingleThreadTaskExecutor main_thread_task_executor(
      base::MessagePumpType::IO);

  base::RunLoop run_loop;

  base::OnceCallback<int(float)> first = base::BindOnce(&Floor);
  base::OnceCallback<std::string(int)> second = base::BindOnce(&IntToString);

  std::string r = std::move(second).Run(std::move(first).Run(3.5f));
  std::cout << "result: " << r << std::endl;

  run_loop.Run();
  return 0;
}