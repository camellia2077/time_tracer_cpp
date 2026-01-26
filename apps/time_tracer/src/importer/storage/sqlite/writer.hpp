// importer/storage/sqlite/writer.hpp
#ifndef IMPORTER_STORAGE_SQLITE_WRITER_HPP_
#define IMPORTER_STORAGE_SQLITE_WRITER_HPP_

#include <vector>
#include <sqlite3.h>
#include <memory>
#include "importer/model/time_sheet_data.hpp"
#include "importer/storage/sqlite/project_resolver.hpp" 

class Writer {
public:
    // [修改] 注入 ProjectResolver，移除 stmt_insert_project
    explicit Writer(sqlite3* db, 
                    sqlite3_stmt* stmt_day, 
                    sqlite3_stmt* stmt_record, 
                    std::unique_ptr<ProjectResolver> project_resolver);
    
    ~Writer();

    void insert_days(const std::vector<DayData>& days);
    void insert_records(const std::vector<TimeRecordInternal>& records);

private:
    sqlite3* db_;
    sqlite3_stmt* stmt_insert_day_;
    sqlite3_stmt* stmt_insert_record_;
    
    // [修改] 持有注入的 Resolver
    std::unique_ptr<ProjectResolver> project_resolver_;
};

#endif // IMPORTER_STORAGE_SQLITE_WRITER_HPP_