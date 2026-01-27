// reports/shared/factories/generic_formatter_factory.hpp
#ifndef REPORTS_SHARED_FACTORIES_GENERIC_FORMATTER_FACTORY_HPP_
#define REPORTS_SHARED_FACTORIES_GENERIC_FORMATTER_FACTORY_HPP_

#include <memory>
#include <functional>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

// 引入 toml++ 用于序列化
#include <toml++/toml.hpp>
// 引入独立的序列化器头文件
#include "reports/shared/serialization/report_config_serializer.hpp"

#include "reports/shared/types/report_format.hpp"
#include "reports/shared/interfaces/i_report_formatter.hpp"
#include "reports/shared/traits/report_traits.hpp"
#include "reports/shared/factories/dll_formatter_wrapper.hpp"




// --- Factory ---

template <typename ReportDataType>
class GenericFormatterFactory {
public:
    using ConfigType = typename ReportTraits<ReportDataType>::ConfigType;
    using FormatterType = IReportFormatter<ReportDataType>;
    using Creator = std::function<std::unique_ptr<FormatterType>(const ConfigType&)>;

private:
    static std::map<ReportFormat, Creator>& get_registry() {
        static std::map<ReportFormat, Creator> registry;
        return registry;
    }

public:
    static void register_formatter(ReportFormat format, Creator creator) {
        get_registry()[format] = std::move(creator);
    }

    // 注册 DLL 格式化器
    static void register_dll_formatter(ReportFormat format, const std::string& dll_base_name) {
        
        auto creator_lambda = [dll_base_name](const ConfigType& config) -> std::unique_ptr<FormatterType> {
            // 1. 自动补全 DLL 后缀
            std::string dll_path = dll_base_name;
            #ifdef _WIN32
                if (dll_path.find(".dll") == std::string::npos) dll_path += ".dll";
            #else
                if (dll_path.find("lib") != 0 && dll_path.find("/") == std::string::npos) dll_path = "lib" + dll_path;
                if (dll_path.find(".so") == std::string::npos) dll_path += ".so";
            #endif

            // 2. [核心重构] 使用 ConfigTomlSerializer 生成 TOML 字符串
            // 变量名改为 serialized_toml_config 以消除歧义
            std::string serialized_toml_config = ConfigTomlSerializer::to_toml_string(config);

            // 3. 创建 Wrapper
            try {
                auto wrapper = std::make_unique<DllFormatterWrapper<ReportDataType>>(dll_path, serialized_toml_config);
                if (!wrapper) {
                     throw std::runtime_error("Unknown error creating DLL wrapper");
                }
                return wrapper;
            } catch (const std::exception& e) {
                std::cerr << "[Factory] Failed to load DLL formatter '" << dll_path << "': " << e.what() << std::endl;
                throw; 
            }
        };
        
        register_formatter(format, creator_lambda);
    }

    static std::unique_ptr<FormatterType> create(ReportFormat format, const ConfigType& config) {
        auto& registry = get_registry();
        if (registry.find(format) == registry.end()) {
            throw std::runtime_error("No formatter registered for format: " + std::to_string(static_cast<int>(format)));
        }
        return registry[format](config);
    }
};

#endif // REPORTS_SHARED_FACTORIES_GENERIC_FORMATTER_FACTORY_HPP_