// .h
// Asynchronous Thread Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /** Represents a single asynchronous thread. */
    class Thread final {
        // THREAD

        /** The thread that makes up this thread. */
        std::thread MyThread;

        /** The lambda expression used for this function's execution. */
        std::function<void()> ThreadExecution;

        // NAME

        /** The name of this thread. */
        const std::string ThreadName;

        // THREAD STATUS

        /** Whether this thread has been started. */
        bool Started = false;

        /** Whether this thread is currently running. */
        bool Running = false;

        /** Whether this thread has been cancelled. */
        bool Cancelled = false;

        /** Whether this thread has been cancelled. */
        bool Completed = false;

        /** The total number of active threads. */
        static int ActiveThreads;

    public:
        // CONSTRUCTORS

        /** Creates and automatically runs a new asynchronous thread from the given lambda expression. */
        explicit Thread(const std::function<void()> &Execution);

        /** Creates a new asynchronous thread from the given lambda expression that will run when told. */
        explicit Thread(const bool &AutoStart, const std::function<void()> &Execution);

        /** Creates and automatically runs a new asynchronous thread from the given lambda expression. */
        explicit Thread(std::string Name, const std::function<void()> &Execution);

        /** Creates a new asynchronous thread from the given lambda expression that will run when told. */
        explicit Thread(const bool &AutoStart, std::string Name, const std::function<void()> &Execution);

        /** Creates a new asynchronous thread from the given lambda expression that will run when told. */
        explicit Thread(std::string Name, const bool &AutoStart, const std::function<void()> &Execution);

        // THREAD

        /** Runs the thread if it has not run or has been cancelled already. */
        bool Run();

        /** Cancels this thread if it has not completed or been cancelled yet. */
        bool Cancel();

        /**
         * Returns whether this thread has not completed its execution yet.<br/>
         * This is useful to check if a thread is still able to manage resources.
         */
        [[nodiscard]] bool IsActive() const;

        /**
         * Returns whether this thread has completed its execution.<br/>
         * This is useful to check if a thread is no longer managing resources.
         */
        explicit operator bool() const;

        // GETTERS

        /** Returns the underlying std::thread in this thread. */
        [[nodiscard]] std::thread &GetThread();

        /** Returns the lambda expression that this thread executed. */
        [[nodiscard]] std::function<void()> &Execution();

        /** Returns the name of this string. */
        [[nodiscard]] std::string Name() const;

        /** Returns whether the thread has started. */
        [[nodiscard]] bool IsStarted() const;

        /** Returns whether the thread is currently running. */
        [[nodiscard]] bool IsRunning() const;

        /** Returns whether the thread has been cancelled. */
        [[nodiscard]] bool IsCancelled() const;

        /** Returns whether the thread has successfully completed. */
        [[nodiscard]] bool IsCompleted() const;

        /** Returns the total number of active threads. */
        static int TotalThreads();

        // EXECUTION

        /** Yields the current thread. */
        static void Yield();

        /** Freezes the current thread for the given number of seconds. */
        static void Delay(const double &Seconds);

        /**
         * Freezes this thread until the given condition is met.<br/>
         * This is useful for creating mutexes.
         */
        static void Await(const bool &Condition);

        /**
         * Freezes this thread until the given thread is completed or cancelled.<br/>
         * This is useful for joining threads together.
         */
        static void Await(const Thread &AwaitedThread);

        // ASYNC

        /** Runs the given lambda expression asynchronously. */
        static std::shared_ptr<Thread> Async(const std::function<void()> &Execution);

        /** Runs the given lambda expression asynchronously after the given number of seconds. */
        static std::shared_ptr<Thread> AsyncDelay(const double &Delay, const std::function<void()> &Execution);

        // SYSTEM

        /** Returns the number of logical processors on this device. */
        static int NumberOfProcessors();
    };
}
