//
// Created by ghost on 6/11/25.
//

#pragma once

#include <memory>
#include <functional>
#include <queue>
#include <future>
#include <thread>
#include <mutex>


namespace HateEngine {

    class HateEngineCallDeferred {
    protected:
        class TaskBase {
        public:
            virtual ~TaskBase() {
            }
            virtual void execute() = 0;
        };

        template<typename R>
        class Task : public TaskBase {
        public:
            std::packaged_task<R()> task;
            Task(std::packaged_task<R()> t) : task(std::move(t)) {
            }
            void execute() override {
                task();
            };
        };

        std::mutex taskQueueMutex;
        std::vector<std::unique_ptr<TaskBase>> taskQueue;

        void pollDeferredTasks() {
            std::lock_guard<std::mutex> lock(taskQueueMutex);

            if (not taskQueue.empty()) {
                for (const auto& task: taskQueue) {
                    task->execute();
                }
                taskQueue.clear();
            }
        }

    public:
        template<typename Func, typename... Args>
        std::future<typename std::result_of<Func(Args...)>::type> callDeferredAsync(
                Func&& func, Args&&... args
        ) {
            typedef typename std::result_of<Func(Args...)>::type ReturnType;

            auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
            std::packaged_task<ReturnType()> task(boundTask);
            std::future<ReturnType> result = task.get_future();

            {
                std::lock_guard<std::mutex> lock(taskQueueMutex);
                taskQueue.push_back(std::unique_ptr<TaskBase>(new Task<ReturnType>(std::move(task))));
            }

            return result;
        }

        template<typename Func, typename... Args>
        typename std::result_of<Func(Args...)>::type callDeferred(Func&& func, Args&&... args) {
            auto result = callDeferredAsync(std::forward<Func>(func), std::forward<Args>(args)...);
            return result.get();
        }
    };

} // namespace HateEngine
