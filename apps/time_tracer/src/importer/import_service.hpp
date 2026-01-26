// importer/import_service.hpp
#ifndef IMPORTER_IMPORT_SERVICE_HPP_
#define IMPORTER_IMPORT_SERVICE_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "importer/model/import_stats.hpp"
#include "core/domain/model/daily_log.hpp"

// 前向声明
class Repository;
class MemoryParser;

class ImportService {
public:
    // [修改] 依赖注入: 接收 Repository 和 Parser
    ImportService(std::shared_ptr<Repository> repository, 
                  std::shared_ptr<MemoryParser> parser);

    ImportStats import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map);

private:
    std::shared_ptr<Repository> repository_;
    std::shared_ptr<MemoryParser> parser_;
};

#endif // IMPORTER_IMPORT_SERVICE_HPP_