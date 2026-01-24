// core/application.hpp
#pragma once
#ifndef CORE_APPLICATION_HPP_
#define CORE_APPLICATION_HPP_

namespace Core {

    class Application {
    public:
        Application() = default;
        ~Application() = default;

        /**
         * @brief 应用程序主入口
         * @return 退出代码 (0 为成功)
         */
        int run(int argc, char* argv[]);
    };

}

#endif // CORE_APPLICATION_HPP_