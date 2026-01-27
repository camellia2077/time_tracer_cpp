// reports/shared/factories/dll_formatter_wrapper.hpp
#ifndef REPORTS_SHARED_FACTORIES_DLL_FORMATTER_WRAPPER_HPP_
#define REPORTS_SHARED_FACTORIES_DLL_FORMATTER_WRAPPER_HPP_

#include "reports/shared/interfaces/i_report_formatter.hpp"
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h> 
#endif

// 辅助宏：用于消除 -Wcast-function-type 警告
template<typename Target, typename Source>
Target cast_func_ptr(Source src) {
    return reinterpret_cast<Target>(reinterpret_cast<void(*)()>(src));
}

template<typename ReportDataType>
class DllFormatterWrapper : public IReportFormatter<ReportDataType> {
public:
    // [重命名] 参数名改为 serialized_config_content，表明这是传输用的序列化数据
    DllFormatterWrapper(const std::string& dll_path, const std::string& serialized_config_content) {
#ifdef _WIN32
        dll_handle_ = LoadLibraryA(dll_path.c_str());
        if (!dll_handle_) throw std::runtime_error("Failed to load DLL: " + dll_path);
        
        create_func_ = cast_func_ptr<CreateFormatterFunc>(GetProcAddress(dll_handle_, "create_formatter"));
        destroy_func_ = cast_func_ptr<DestroyFormatterFunc>(GetProcAddress(dll_handle_, "destroy_formatter"));

        if constexpr (std::is_same_v<ReportDataType, DailyReportData>) {
            format_func_day_ = cast_func_ptr<FormatReportFunc_Day>(GetProcAddress(dll_handle_, "format_report"));
        } else if constexpr (std::is_same_v<ReportDataType, RangeReportData>) {
            format_func_range_ = cast_func_ptr<FormatReportFunc_Range>(GetProcAddress(dll_handle_, "format_report"));
        }
#else
        dll_handle_ = dlopen(dll_path.c_str(), RTLD_LAZY);
        if (!dll_handle_) throw std::runtime_error("Failed to load library: " + dll_path);
        
        create_func_ = reinterpret_cast<CreateFormatterFunc>(dlsym(dll_handle_, "create_formatter"));
        destroy_func_ = reinterpret_cast<DestroyFormatterFunc>(dlsym(dll_handle_, "destroy_formatter"));

        if constexpr (std::is_same_v<ReportDataType, DailyReportData>) {
            format_func_day_ = reinterpret_cast<FormatReportFunc_Day>(dlsym(dll_handle_, "format_report"));
        } else if constexpr (std::is_same_v<ReportDataType, RangeReportData>) {
            format_func_range_ = reinterpret_cast<FormatReportFunc_Range>(dlsym(dll_handle_, "format_report"));
        }
#endif

        bool format_func_loaded = false;
        if constexpr (std::is_same_v<ReportDataType, DailyReportData>) {
            format_func_loaded = (format_func_day_ != nullptr);
        } else if constexpr (std::is_same_v<ReportDataType, RangeReportData>) {
            format_func_loaded = (format_func_range_ != nullptr);
        }

        if (!create_func_ || !destroy_func_ || !format_func_loaded) {
            throw std::runtime_error("Failed to get function pointers from DLL: " + dll_path);
        }

        // 传递序列化后的配置字符串
        formatter_handle_ = create_func_(serialized_config_content.c_str());
        if (!formatter_handle_) {
            throw std::runtime_error("create_formatter from DLL returned null.");
        }
    }

    ~DllFormatterWrapper() override {
        if (formatter_handle_ && destroy_func_) {
            destroy_func_(formatter_handle_);
        }
#ifdef _WIN32
        if (dll_handle_) FreeLibrary(dll_handle_);
#else
        if (dll_handle_) dlclose(dll_handle_);
#endif
    }

    std::string format_report(const ReportDataType& data) const override {
        if (formatter_handle_) {
            if constexpr (std::is_same_v<ReportDataType, DailyReportData>) {
                if (format_func_day_) {
                    const char* res = format_func_day_(formatter_handle_, data);
                    return res ? std::string(res) : "";
                }
            } else if constexpr (std::is_same_v<ReportDataType, RangeReportData>) {
                if (format_func_range_) {
                    const char* res = format_func_range_(formatter_handle_, data);
                    return res ? std::string(res) : "";
                }
            }
        }
        return "Error: Formatter handle or format function is null.";
    }

private:
#ifdef _WIN32
    HINSTANCE dll_handle_ = nullptr;
#else
    void* dll_handle_ = nullptr;
#endif
    FormatterHandle formatter_handle_ = nullptr;
    CreateFormatterFunc create_func_ = nullptr;
    DestroyFormatterFunc destroy_func_ = nullptr;
    
    FormatReportFunc_Day format_func_day_ = nullptr;
    FormatReportFunc_Range format_func_range_ = nullptr;
};

#endif // REPORTS_SHARED_FACTORIES_DLL_FORMATTER_WRAPPER_HPP_