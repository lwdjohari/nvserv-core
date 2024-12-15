
#include "nvserv/threads/event_loop_executor.h"

#include <atomic>
#include <chrono>
#include <thread>

#include "absl/time/time.h"
#include "catch2/catch_all.hpp"
#include "nvm/stopwatch.h"

using namespace std::chrono_literals;
using namespace nvserv::threads;

TEST_CASE("EventLoopExecutor::RunOnce Test") {
  auto executor = std::make_shared<EventLoopExecutor>();

  try {
    std::cout << "Running section: Task RunOnce test\n";
    bool task_executed = false;
    auto task_to_execute = [&]() {
      std::cout << "Working for for 50ms\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      task_executed = true;
    };

    auto fn_task_to_execute = MakeTaskPtr(task_to_execute);

    // Submit the task with a short deadline
    executor->SubmitTask(fn_task_to_execute,
                         EventLoopExecutor::TaskType::RunOnce,
                         absl::Milliseconds(100));

    // Give enough time for task to be processed
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Check if the task was executed
    if (task_executed) {
      executor->Stop();
      std::cout << "Stopped the EventLoopExecutor.\n";
    }

    REQUIRE(task_executed == true);
  } catch (const std::exception& e) {
    std::cerr << "Test failed with exception: " << e.what() << "\n";
    std::cout << "Stopped the EventLoopExecutor.\n";

    executor->Stop();
    REQUIRE(false);  // Mark test as failed
  } catch (...) {
    std::cerr << "Test failed with an unknown exception.\n";
    std::cout << "Stopped the EventLoopExecutor.\n";
    executor->Stop();
    REQUIRE(false);  // Mark test as failed
  }

  std::cout << "Final Line: Stopped the EventLoopExecutor.\n";

  executor->Stop();
}

TEST_CASE("EventLoopExecutor::Interval Test") {
  auto executor = std::make_shared<EventLoopExecutor>();
  auto sw = nvm::Stopwatch();

  try {
    std::cout << "Running section: Task Interval test\n";
    uint32_t flag1 = 0;
    uint32_t flag2 = 0;
    bool state = false;

    auto task_fn1 = [&]() {
      std::cout << "Task 1: Starting for for 100ms@"
                << std::to_string(sw.ElapsedMilliseconds()) << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      flag1++;
      std::cout << "Task 1: Finished for for 100ms@"
                << std::to_string(sw.ElapsedMilliseconds()) << "\n\n";
    };

    auto task_fn2 = [&]() {
      std::cout << "Task 2: Starting for for 100ms@"
                << std::to_string(sw.ElapsedMilliseconds()) << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      flag2++;
      std::cout << "Task 2: Finished for for 100ms@"
                << std::to_string(sw.ElapsedMilliseconds()) << "\n\n";
    };

    auto task_ptr1 = MakeTaskPtr(task_fn1);
    auto task_ptr2 = MakeTaskPtr(task_fn2);

    sw.Reset();
    // Submit the task with a short deadline
    executor->SubmitTask(task_ptr1, EventLoopExecutor::TaskType::RunAtInterval,
                         absl::Milliseconds(0), absl::Milliseconds(300));

    executor->SubmitTask(task_ptr2, EventLoopExecutor::TaskType::RunAtInterval,
                         absl::Milliseconds(200), absl::Milliseconds(100));

    // Give enough time for task to be processed
    std::this_thread::sleep_for(std::chrono::milliseconds(550));

    // Check if the task was executed
    if (flag1 >= 2 && flag2 >= 2) {
      state = true;
      std::cout << "Stopping the EventLoopExecutor @"
                << std::to_string(sw.ElapsedMilliseconds()) << ".\n";
      executor->Stop();
      std::cout << "Stopped the EventLoopExecutor @"
                << std::to_string(sw.ElapsedMilliseconds()) << ".\n";
    }

    std::cout << "Flags: f1=" << std::to_string(flag1)
              << "; f2=" << std::to_string(flag2) << "\n";

    REQUIRE(state == true);
  } catch (const std::exception& e) {
    std::cerr << "Test failed with exception: " << e.what() << "\n";
    std::cout << "Stopped the EventLoopExecutor.\n";

    executor->Stop();
    REQUIRE(false);  // Mark test as failed
  } catch (...) {
    std::cerr << "Test failed with an unknown exception.\n";
    std::cout << "Stopped the EventLoopExecutor.\n";
    executor->Stop();
    REQUIRE(false);  // Mark test as failed
  }

  std::cout << "Final Line: Stopped the EventLoopExecutor.\n";

  executor->Stop();
}
