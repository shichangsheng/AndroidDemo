#include <jni.h>
#include "AndroidOut.h"
#include "Renderer.h"

// 替换 GameActivity 头文件
//#include <android_native_app_glue.h>
#include "native_app_glue/android_native_app_glue.h"

//#include <android/native_activity.h>
//#include <android/log.h>

// 删除 GameActivity 相关代码
extern "C" {

// 保留 handle_cmd 函数（但移除 GameActivity 依赖）
void handle_cmd(android_app* pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            pApp->userData = new Renderer(pApp);
            break;
        case APP_CMD_TERM_WINDOW:
            if (pApp->userData) {
                auto* pRenderer = reinterpret_cast<Renderer*>(pApp->userData);
                pApp->userData = nullptr;
                delete pRenderer;
            }
            break;
        default:
            break;
    }
}

// 删除 motion_event_filter_func（改用默认输入处理）

// Android 主入口
void android_main(struct android_app* pApp) {
    aout << "Welcome to android_main (NativeActivity)" << std::endl;

    // 注册事件回调
    pApp->onAppCmd = handle_cmd;

    // 删除 GameActivity 输入过滤器
    // android_app_set_motion_event_filter(pApp, motion_event_filter_func);

    // 主循环
    int events;
    android_poll_source* pSource;
    do {
        if (ALooper_pollAll(0, nullptr, &events, (void**)&pSource) >= 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }

        if (pApp->userData) {
            auto* pRenderer = reinterpret_cast<Renderer*>(pApp->userData);
            pRenderer->handleInput();  // 需适配输入处理
            pRenderer->render();
        }
    } while (!pApp->destroyRequested);
}
}
