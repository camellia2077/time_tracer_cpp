// importer/data_importer.cpp
#include "importer/data_importer.hpp"
#include "common/ansi_colors.hpp"
#include "importer/import_service.hpp"
#include "importer/parser/memory_parser.hpp"
#include "importer/storage/repository.hpp"
#include "importer/storage/sqlite/connection.hpp"
#include <iomanip>
#include <iostream>
#include <memory>

namespace {
void print_report(const ImportStats &stats, const std::string &title) {
  double total_time = stats.parsing_duration_s + stats.db_insertion_duration_s;
  std::cout << "\n--- " << title << " Report ---" << std::endl;

  if (!stats.db_open_success) {
    std::cerr << kRedColor << "[Fatal] DB Error: "
              << (stats.error_message.empty() ? "Unknown" : stats.error_message)
              << kResetColor << std::endl;
    return;
  }

  if (!stats.transaction_success) {
    std::cerr << kRedColor
              << "[Fatal] Transaction Failed: " << stats.error_message
              << kResetColor << std::endl;
    return;
  }

  if (stats.failed_files.empty()) {
    std::cout << kGreenColor << "[Success] Processed " << stats.successful_files
              << " items." << kResetColor << std::endl;
  } else {
    std::cout << kYellowColor << "[Partial] Success: " << stats.successful_files
              << ", Failed: " << stats.failed_files.size() << kResetColor
              << std::endl;
    for (const auto &f : stats.failed_files)
      std::cerr << "  Failed: " << f << std::endl;
  }

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Timing: Parse=" << stats.parsing_duration_s
            << "s, Insert=" << stats.db_insertion_duration_s
            << "s, Total=" << total_time << "s\n";
}
} // namespace

// ---------------------------------------------------------
// Facade Implementation (Composition Root)
// ---------------------------------------------------------

void handle_process_memory_data(
    const std::string &db_name,
    const std::map<std::string, std::vector<DailyLog>> &data) {
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
