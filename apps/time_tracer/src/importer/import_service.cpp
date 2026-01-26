// importer/import_service.cpp
#include "importer/import_service.hpp"
#include "importer/parser/memory_parser.hpp"
#include "importer/storage/repository.hpp"
#include <chrono>

// [修改] 构造函数保存注入的依赖
ImportService::ImportService(std::shared_ptr<Repository> repository, 
                             std::shared_ptr<MemoryParser> parser)
    : repository_(std::move(repository)), 
      parser_(std::move(parser)) {}

ImportStats ImportService::import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map) {
    ImportStats stats;
    for(const auto& p : data_map) stats.total_files += p.second.size();
    stats.successful_files = stats.total_files;
    
    if (data_map.empty()) return stats;

    auto start = std::chrono::high_resolution_clock::now();

    // 1. 转换 (使用注入的 Parser)
    ParsedData all_data = parser_->parse(data_map);

    auto end_parsing = std::chrono::high_resolution_clock::now();
    stats.parsing_duration_s = std::chrono::duration<double>(end_parsing - start).count();

    // 2. 入库 (使用注入的 Repository)
    if (repository_->is_db_open()) {
        stats.db_open_success = true;
        try {
            repository_->import_data(all_data.days, all_data.records);
            stats.transaction_success = true;
        } catch (const std::exception& e) {
            stats.transaction_success = false;
            stats.error_message = e.what();
        }
    } else {
        stats.db_open_success = false;
        stats.error_message = "Cannot open database.";
    }

    auto end_total = std::chrono::high_resolution_clock::now();
    stats.db_insertion_duration_s = std::chrono::duration<double>(end_total - end_parsing).count();

    return stats;
}