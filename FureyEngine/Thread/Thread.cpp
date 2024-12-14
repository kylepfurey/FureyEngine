// .cpp
// Asynchronous Thread Script
// by Kyle Furey

#include "Thread.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates and automatically runs a new asynchronous thread from the given lambda expression.
    Thread::Thread(const std::function<void()> &Execution)
        : ThreadExecution(Execution), ThreadName(std::string("New Thread #") + std::to_string(ActiveThreads + 1)) {
        this->Started = true;
        this->MyThread = std::thread([&] {
            ++ActiveThreads;
            this->Running = true;
            this->ThreadExecution();
            this->Running = false;
            --ActiveThreads;
            if (!this->Cancelled) {
                this->Completed = true;
            }
        });
        this->MyThread.detach();
    }

    // Creates a new asynchronous thread from the given lambda expression that will run when told.
    Thread::Thread(const bool &AutoStart, const std::function<void()> &Execution)
        : ThreadExecution(Execution), ThreadName(std::string("New Thread #") + std::to_string(ActiveThreads + 1)),
          Started(AutoStart) {
        this->MyThread = std::thread([&] {
            while (!this->Started && !this->Cancelled) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            if (!this->Cancelled) {
                ++ActiveThreads;
                this->Running = true;
                this->ThreadExecution();
                this->Running = false;
                --ActiveThreads;
                if (!this->Cancelled) {
                    this->Completed = true;
                }
            }
        });
        this->MyThread.detach();
    }

    // Creates and automatically runs a new asynchronous thread from the given lambda expression.
    Thread::Thread(std::string Name, const std::function<void()> &Execution)
        : ThreadExecution(Execution), ThreadName(std::move(Name)) {
        this->Started = true;
        this->MyThread = std::thread([&] {
            ++ActiveThreads;
            this->Running = true;
            this->ThreadExecution();
            this->Running = false;
            --ActiveThreads;
            if (!this->Cancelled) {
                this->Completed = true;
            }
        });
        this->MyThread.detach();
    }

    // Creates a new asynchronous thread from the given lambda expression that will run when told.
    Thread::Thread(const bool &AutoStart, std::string Name, const std::function<void()> &Execution)
        : ThreadExecution(Execution), ThreadName(std::move(Name)), Started(AutoStart) {
        this->MyThread = std::thread([&] {
            while (!this->Started && !this->Cancelled) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            if (!this->Cancelled) {
                ++ActiveThreads;
                this->Running = true;
                this->ThreadExecution();
                this->Running = false;
                --ActiveThreads;
                if (!this->Cancelled) {
                    this->Completed = true;
                }
            }
        });
        this->MyThread.detach();
    }

    // Creates a new asynchronous thread from the given lambda expression that will run when told.
    Thread::Thread(std::string Name, const bool &AutoStart, const std::function<void()> &Execution)
        : ThreadExecution(Execution), ThreadName(std::move(Name)), Started(AutoStart) {
        this->MyThread = std::thread([&] {
            while (!this->Started && !this->Cancelled) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            if (!this->Cancelled) {
                ++ActiveThreads;
                this->Running = true;
                this->ThreadExecution();
                this->Running = false;
                --ActiveThreads;
                if (!this->Cancelled) {
                    this->Completed = true;
                }
            }
        });
        this->MyThread.detach();
    }

    // THREAD

    // Runs the thread if it has not run or has been cancelled already.
    bool Thread::Run() {
        if (!Started && !Cancelled) {
            Started = true;
            return true;
        }

        return false;
    }

    // Cancels this thread if it has not completed or been cancelled yet.
    bool Thread::Cancel() {
        if (!Cancelled && !Completed) {
            Cancelled = true;
            return true;
        }
        return false;
    }

    // Returns whether this thread has not completed its execution yet.
    // This is useful to check if a thread is still able to manage resources.
    bool Thread::IsActive() const {
        return !IsCompleted() && !IsCancelled();
    }

    // Returns whether this thread has completed its execution.
    // This is useful to check if a thread is no longer managing resources.
    Thread::operator bool() const {
        return !IsActive();
    }

    // GETTERS

    // Returns the underlying std::thread in this thread.
    std::thread &Thread::GetThread() {
        return MyThread;
    }

    // Returns the lambda expression that this thread executed.
    std::function<void()> &Thread::Execution() {
        return ThreadExecution;
    }

    // Returns the name of this string.
    std::string Thread::Name() const {
        return ThreadName;
    }

    // Returns whether the thread has started.
    bool Thread::IsStarted() const {
        return Started;
    }

    // Returns whether the thread is currently running.
    bool Thread::IsRunning() const {
        return Running;
    }

    // Returns whether the thread has been cancelled.
    bool Thread::IsCancelled() const {
        return Cancelled;
    }

    // Returns whether the thread has successfully completed.
    bool Thread::IsCompleted() const {
        return Completed;
    }

    // Returns the total number of active threads.
    int Thread::TotalThreads() {
        return ActiveThreads;
    }

    // EXECUTION

    // Yields the current thread.
    void Thread::Yield() {
        std::this_thread::yield();
    }

    // Freezes the current thread for the given number of seconds.
    void Thread::Delay(const double &Seconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(Seconds * 1000)));
    }

    // Freezes this thread until the given condition is met.
    // This is useful for creating mutexes.
    void Thread::Await(const bool &Condition) {
        while (!Condition) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    // Freezes this thread until the given thread is completed or cancelled.
    // This is useful for joining threads together.
    void Thread::Await(const Thread &AwaitedThread) {
        while (AwaitedThread.IsActive()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    // ASYNC

    // Runs the given lambda expression asynchronously.
    std::shared_ptr<Thread> Thread::Async(const std::function<void()> &Execution) {
        auto AsyncThread = std::make_shared<Thread>("Async Thread", true, [Execution] {
            Execution();
        });
        return AsyncThread;
    }

    // Runs the given lambda expression asynchronously after the given number of seconds.
    std::shared_ptr<Thread> Thread::AsyncDelay(const double &Delay, const std::function<void()> &Execution) {
        auto AsyncThread = std::make_shared<Thread>("Async Thread", true, [Delay, Execution] {
            Thread::Delay(Delay);
            Execution();
        });
        return AsyncThread;
    }

    // SYSTEM

    // Returns the number of logical processors on this device.
    int Thread::NumberOfProcessors() {
        return static_cast<int>(std::thread::hardware_concurrency());
    }

    // STATIC VARIABLE INITIALIZATION

    // The total number of active threads.
    int Thread::ActiveThreads = 1;
}
