// importer/data_importer.cpp
#include "importer/data_importer.hpp"
#include "importer/import_service.hpp" 
#include "importer/storage/repository.hpp"
#include "importer/storage/sqlite/connection.hpp"
#include "importer/parser/memory_parser.hpp"
#include "common/ansi_colors.hpp" 
#include <iostream>
#include <iomanip>
#include <memory>

namespace {
void print_report(const ImportStats& stats, const std::string& title) {
    double total_time = stats.parsing_duration_s + stats.db_insertion_duration_s;
    std::cout << "\n--- " << title << " Report ---" << std::endl;

    if (!stats.db_open_success) {
        std::cerr << RED_COLOR << "[Fatal] DB Error: " << (stats.error_message.empty() ? "Unknown" : stats.error_message) << RESET_COLOR << std::endl;
        return;
    }

    if (!stats.transaction_success) {
        std::cerr << RED_COLOR << "[Fatal] Transaction Failed: " << stats.error_message << RESET_COLOR << std::endl;
        return;
    }

    if (stats.failed_files.empty()) {
        std::cout << GREEN_COLOR << "[Success] Processed " << stats.successful_files << " items." << RESET_COLOR << std::endl;
    } else {
        std::cout << YELLOW_COLOR << "[Partial] Success: " << stats.successful_files << ", Failed: " << stats.failed_files.size() << RESET_COLOR << std::endl;
        for (const auto& f : stats.failed_files) std::cerr << "  Failed: " << f << std::endl;
    }

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Timing: Parse=" << stats.parsing_duration_s << "s, Insert=" << stats.db_insertion_duration_s << "s, Total=" << total_time << "s\n";
}
}

// ---------------------------------------------------------
// Facade Implementation (Composition Root)
// ---------------------------------------------------------

void handle_process_memory_data(const std::string& db_name, const std::map<std::string, std::vector<DailyLog>>& data) {
    std::cout << "Task: Memory Import..." << std::endl;
    
    // 1. 创建组件 (Wiring Dependencies)
    auto connection = std::make_shared<Connection>(db_name);
    auto repository = std::make_shared<Repository>(connection);
    auto parser = std::make_shared<MemoryParser>();
    
    // 2. 注入 Service
    ImportService service(repository, parser);
    
    // 3. 执行业务
    ImportStats stats = service.import_from_memory(data);
    
    print_report(stats, "Memory Import");
}