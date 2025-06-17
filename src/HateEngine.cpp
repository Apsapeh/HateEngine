#include <chrono>
#include <cstdint>
#include <glad/gl.h>

#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Log.hpp>
#include <HateEngine/Render/OpenGL_1_3.hpp>
#include <HateEngine/AudioServer.hpp>
#include <thread>

#include "HateEngine/AudioServer.hpp"
#include "HateEngine/Input.hpp"
#include "HateEngine/Level.hpp"
#include "HateEngine/OSDriverInterface.hpp"
#include "HateEngine/Render/RenderInterface.hpp"
#include "HateEngine/Types/Result.hpp"
#include "glm/ext/vector_float2.hpp"
#include "globalStaticParams.hpp"

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#define SET_THREAD_HIGH_PRIORITY                                                                        \
    sched_param sch_params;                                                                             \
    sch_params.sched_priority = 99;                                                                     \
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#elif __APPLE__
#include <pthread.h>
#include <sched.h>
#define SET_THREAD_HIGH_PRIORITY                                                                        \
    sched_param sch_params;                                                                             \
    sch_params.sched_priority = 99;                                                                     \
    pthread_setschedparam(pthread_self(), SCHED_RR, &sch_params);
#elif _WIN32
#include <windows.h>
#define SET_THREAD_HIGH_PRIORITY SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
// SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
// #define SET_THREAD_HIGH_PRIORITY ;
#endif

bool glad_is_initialized = false;
std::thread::id HateEngine::Engine::mainThreadId;
bool HateEngine::Engine::isMainThreadIdInitialized = false;

using namespace HateEngine;

std::thread::id Engine::getMainThreadId() {
    return HateEngine::Engine::mainThreadId;
}

Engine::Engine(std::string window_lbl, int width, int height) : Input(&this->OSDriver) {
    if (not this->isMainThreadIdInitialized) {
        this->mainThreadId = std::this_thread::get_id();
        this->isMainThreadIdInitialized = true;
    }

    this->mainWindow = OSDriver.createWindow(width, height, window_lbl).takeOk();

    // float xscale, yscale;
    this->Input.changeInputWindow(this->mainWindow);

    __updateResolution(width, height);

    OSDriver.onFramebufferSizeChanged.connect([this](std::shared_ptr<OSDriverInterface::OSWindow> win,
                                                     int w, int h) {
        if (win == this->mainWindow)
            this->__updateResolution(w, h);
    });

    OSDriver.onCursorMoved.connect([this](std::shared_ptr<OSDriverInterface::OSWindow> win,
                                          InputEventInfo event) {
        if (win == this->mainWindow)
            this->_inputEvent(event);
    });

    OSDriver.onKeyPressed.connect([this](std::shared_ptr<OSDriverInterface::OSWindow> win,
                                         InputEventInfo event) {
        if (win == this->mainWindow)
            this->_inputEvent(event);
    });

    OSDriver.onMouseButtonPressed.connect([this](std::shared_ptr<OSDriverInterface::OSWindow> win,
                                                 InputEventInfo event) {
        if (win == this->mainWindow)
            this->_inputEvent(event);
    });

    OSDriver.onMouseWheelScrolled.connect([this](std::shared_ptr<OSDriverInterface::OSWindow> win,
                                                 InputEventInfo event) {
        if (win == this->mainWindow)
            this->_inputEvent(event);
    });

    // Load Glad
    int version = gladLoadGL(OSDriver.getProcAddress);
    if (not version) {
        HATE_FATAL("Failed to initialize GLAD");
    }
    glad_is_initialized = true;


    // Calculating the delay between FixedProcessLoop iterations
    SET_THREAD_HIGH_PRIORITY

    int64_t fixDelay = 1000000 / fixedLoopRefreshRate;
    int64_t physDelay = 1000000 / physicsEngineIterateLoopRefreshRate;
    int64_t audioDelay = 1000000 / audioEngineIterateLoopRefreshRate;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int o = 0; o < 10; ++o)
        std::this_thread::sleep_for(std::chrono::microseconds(fixDelay));
    auto t2 = std::chrono::high_resolution_clock::now();
    int64_t d_fp =
            std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 10 - fixDelay;
    this->fixedProcessDelayMCS = fixDelay - d_fp;

    if (physicsEngineIterateLoopRefreshRate == fixedLoopRefreshRate) {
        this->physicsEngineIterateDelayMCS = this->fixedProcessDelayMCS;
        return;
    }

    t1 = std::chrono::high_resolution_clock::now();
    for (int o = 0; o < 10; ++o)
        std::this_thread::sleep_for(std::chrono::microseconds(physDelay));
    t2 = std::chrono::high_resolution_clock::now();
    int64_t d_pp =
            std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 10 - physDelay;
    this->physicsEngineIterateDelayMCS = physDelay - d_pp;

    AudioServer::Reinit();

    this->renderInterface = new class OpenGL_1_3(this);
}

void Engine::Run() {
    SET_THREAD_HIGH_PRIORITY
    this->isRunned = true;
    std::thread* fixedProcessThread = nullptr;
    std::thread* physicsEngineProcessThread = nullptr;
    if (not this->isOneThread) {
        if (this->fixedProcessLoop != nullptr)
            fixedProcessThread = new std::thread(&Engine::threadFixedProcessLoop, this);

        physicsEngineProcessThread = new std::thread(&Engine::threadPhysicsEngineIterateLoop, this);
    }

    // glfwSwapBuffers(this->window);
    double oldTime = OSDriver.getTime();
    double fixed_process_loop_delta = 0;
    double physics_engine_iterate_loop_delta = 0;
    double audio_engine_iterate_loop_delta = 0;
    double fixed_process_loop_delay = 1.0 / this->fixedLoopRefreshRate;
    double physics_engine_iterate_loop_delay = 1.0 / this->physicsEngineIterateLoopRefreshRate;
    double audio_engine_iterate_loop_delay = 1.0 / this->audioEngineIterateLoopRefreshRate;
    while (not this->mainWindow->isShouldBeClosed()) {
        auto time_start = std::chrono::high_resolution_clock::now();
        this->frameDelta = OSDriver.getTime() - oldTime;
        oldTime = OSDriver.getTime();
        OSDriver.poll();
        pollDeferredTasks();

#ifdef __HATE_ENGINE_PROFILER
        if (this->Input.isKeyPressed(KeyF8)) {
            this->Exit();
            continue;
        }
#endif

        // meshesMutex.lock();
        // std::lock_guard<std::mutex> lock(this->levelMutex);
        if (this->processLoop != nullptr)
            this->processLoop(this, frameDelta);

        if (this->level != nullptr)
            this->level->Update(this, frameDelta);

        if (this->isOneThread) {
            fixed_process_loop_delta += frameDelta;
            physics_engine_iterate_loop_delta += frameDelta;

            if (fixed_process_loop_delta >= fixed_process_loop_delay) {
                if (this->fixedProcessLoop != nullptr)
                    this->fixedProcessLoop(this, fixed_process_loop_delta);
                if (this->level != nullptr)
                    this->level->UpdateFixed(this, fixed_process_loop_delta);
                fixed_process_loop_delta = 0.0;
            }

            if (physics_engine_iterate_loop_delta >= physics_engine_iterate_loop_delay) {

                if (this->level != nullptr) {
                    while (physics_engine_iterate_loop_delta - 0.001 > 0.0f) {
                        float d = physics_engine_iterate_loop_delay;
                        if (physics_engine_iterate_loop_delta < d)
                            d = physics_engine_iterate_loop_delta;
                        this->level->getPhysEngine()->IteratePhysics(d);
                        physics_engine_iterate_loop_delta -= d;
                    }
                }
                physics_engine_iterate_loop_delta = 0.0;
            }
        }

        if (this->level != nullptr) {
            audio_engine_iterate_loop_delta += frameDelta;

            if (audio_engine_iterate_loop_delta >= audio_engine_iterate_loop_delay) {
                if (this->level != nullptr and this->level->camera != nullptr) {
                    AudioServer::setListener3DPosition(this->level->camera->getGlobalPosition());
                    AudioServer::setListener3DDirection(this->level->camera->getGlobalDirection());
                    AudioServer::Update3D();
                }
                audio_engine_iterate_loop_delta = 0.0;
            }
            this->renderInterface->Render();
        }


        auto time_end = std::chrono::high_resolution_clock::now();
        long time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start).count();
        lastCPUTime = time - this->renderInterface->last_gpu_time;

        this->mainWindow->swapBuffers();
    }

    if (fixedProcessThread != nullptr and fixedProcessThread->joinable())
        fixedProcessThread->join();
    if (physicsEngineProcessThread != nullptr and physicsEngineProcessThread->joinable())
        physicsEngineProcessThread->join();
    AudioServer::Deinit();
}

void Engine::Exit() {
    this->mainWindow->RequireClose();
}


void Engine::_inputEvent(const InputEventInfo& event) {
    if (this->inputEventFunc != nullptr)
        this->inputEventFunc(this, event);

    if (this->level != nullptr)
        this->level->UpdateInput(this, event);
}


void Engine::__updateResolution(int width, int height) {
    const auto scale = this->mainWindow->getScale();
    const auto old_scale = this->getDisplayScale();
    const auto old_res = this->getResolution();
    const auto old_aspect_ratio = this->getAspectRatio();
    this->displayScale = scale;
    this->resolution = glm::ivec2((float) width / scale.x, (float) height / scale.y);
    this->aspectRatio = (float) width / (float) height;

    this->onResolutionChanged.emit(this, getResolution(), old_res);
    this->onDisplayScaleChanged.emit(this, getDisplayScale(), old_scale);
    this->onAspectRatioChanged.emit(this, getAspectRatio(), old_aspect_ratio);
}


void Engine::setResolution(int width, int height) {
    HATE_WARNING("Engine::setResolution not implemented");

    // FIXME: Implement setResolution
}

void Engine::setOneThreadMode(bool mode) {
    if (this->isRunned) {
        HATE_ERROR("Engine::setOneThreadMode should be called before Engine::Run");
        return;
    }
    this->isOneThread = mode;
}

void Engine::setVSync(bool vsync) {
    this->mainWindow->setSwapInterval(vsync);
}

RenderInterface* Engine::getRenderInterface() {
    return this->renderInterface;
}

Engine::RenderAPI Engine::getRenderAPI() {
    return this->renderApi;
}

double Engine::getGPUTimeMS() {
    return this->renderInterface->getGPUTimeMS();
}

double Engine::getCPUTimeMS() {
    return double(this->lastCPUTime) / 10000000.0;
}

double Engine::getFrameDelta() {
    return this->frameDelta;
}

glm::ivec2 Engine::getResolution() {
    return this->resolution;
}

glm::vec2 Engine::getDisplayScale() {
    return this->displayScale;
}

float Engine::getAspectRatio() {
    return this->aspectRatio;
}

bool Engine::getOneThreadMode() {
    return this->isOneThread;
}

bool Engine::getVSync() {
    return mainWindow->getSwapInterval();
}


void Engine::threadFixedProcessLoop() {
    SET_THREAD_HIGH_PRIORITY

    double oldTime = OSDriver.getTime();
    double delta = (float) this->fixedProcessDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not this->mainWindow->isShouldBeClosed()) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(std::chrono::microseconds(a));
        // std::cout << d / 10<< "\n";
        delta = OSDriver.getTime() - oldTime;
        oldTime = OSDriver.getTime();
        std::lock_guard<std::mutex> lock(this->levelMutex);
        if (this->level != nullptr)
            this->level->UpdateFixed(this, delta);

        // meshesMutex.lock();
        fixedProcessLoop(this, delta);
        // meshesMutex.unlock();
        func_delta = OSDriver.getTime() - oldTime;
    }
}

void Engine::threadPhysicsEngineIterateLoop() {
    SET_THREAD_HIGH_PRIORITY

    double oldTime = OSDriver.getTime();
    double delta = (double) this->physicsEngineIterateDelayMCS / 1000000;
    double func_delta = 0.0;

    while (not this->mainWindow->isShouldBeClosed()) {
        int64_t a = this->fixedProcessDelayMCS - int64_t(func_delta * 1000000);
        std::this_thread::sleep_for(std::chrono::microseconds(a));

        delta = OSDriver.getTime() - oldTime;
        oldTime = OSDriver.getTime();
        std::lock_guard<std::mutex> lock(this->levelMutex);
        if (this->level != nullptr and this->level->getPhysEngine() != nullptr)
            this->level->getPhysEngine()->IteratePhysics((float) delta);
        func_delta = OSDriver.getTime() - oldTime;
    }
}

void Engine::setProcessLoop(void (*func)(Engine*, double)) {
    this->processLoop = func;
}
void Engine::setFixedProcessLoop(void (*func)(Engine*, double)) {
    this->fixedProcessLoop = func;
}
void Engine::setInputEvent(void (*func)(Engine*, const InputEventInfo&)) {
    this->inputEventFunc = func;
}

void Engine::setLevel(Level* lvl) {
    std::lock_guard<std::mutex> lock(this->levelMutex);
    Level* old = this->level;
    if (this->level != nullptr) {
        this->level->onLightAdded.disconnect(level_callback_added_uuid);
        this->level->onLightRemoved.disconnect(level_callback_removed_uuid);
        for (auto& l: *this->level->getLights()) {
            this->renderInterface->removeLight(l.second);
        }
    }

    this->level = lvl;
    this->level_callback_added_uuid =
            this->level->onLightAdded.connect([this](Light* l) { this->renderInterface->addLight(l); });
    this->level_callback_removed_uuid = this->level->onLightRemoved.connect([this](Light* l) {
        this->renderInterface->removeLight(l);
    });
    for (auto& l: *lvl->getLights()) {
        this->renderInterface->addLight(l.second);
    }

    onLevelChanged.emit(this, this->level, old);
}

Level* Engine::getLevel() {
    return this->level;
}

/*===========> THIS CODE SECTION CANNOT BE MODIFIED <===========*/

/*
 R.I.P Alexey Navalny
 #РоссияБудетСвободной
 #RussiaWillBeFree
*/

/*=====> END OF CODE SECTION PROHIBITED FROM MODIFICATION <=====*/
