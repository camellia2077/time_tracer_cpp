// importer/storage/repository.hpp
#ifndef IMPORTER_STORAGE_REPOSITORY_HPP_
#define IMPORTER_STORAGE_REPOSITORY_HPP_

#include <string>
#include <vector>
#include <memory>
#include "importer/model/time_sheet_data.hpp"

// 引用新的组件头文件
#include "importer/storage/sqlite/writer.hpp"
#include "importer/storage/sqlite/connection.hpp"
#include "importer/storage/sqlite/statement.hpp"

class Repository {
public:
    // [修改] 注入 Connection，解耦数据库文件的打开逻辑
    explicit Repository(std::shared_ptr<Connection> connection);
    ~Repository() = default;

    bool is_db_open() const;

    void import_data(
        const std::vector<DayData>& days,
        const std::vector<TimeRecordInternal>& records
    );

private:
    std::shared_ptr<Connection> connection_manager_;
    std::unique_ptr<Statement> statement_manager_; 
    std::unique_ptr<Writer> data_inserter_;        
};

#endif // IMPORTER_STORAGE_REPOSITORY_HPP_