// importer/storage/repository.cpp
#include "importer/storage/repository.hpp"
#include <iostream>

// [修改] 构造函数只负责组装组件
Repository::Repository(std::shared_ptr<Connection> connection) 
    : connection_manager_(std::move(connection)) 
{
    if (is_db_open()) { 
        sqlite3* db = connection_manager_->get_db();

        // 1. 初始化 Statements
        statement_manager_ = std::make_unique<Statement>(db); 

        // 2. 初始化 Resolver (作为 Writer 的依赖)
        auto project_resolver = std::make_unique<ProjectResolver>(
            db, 
            statement_manager_->get_insert_project_stmt()
        );

        // 3. 初始化 Writer (注入 Resolver)
        data_inserter_ = std::make_unique<Writer>( 
            db, 
            statement_manager_->get_insert_day_stmt(), 
            statement_manager_->get_insert_record_stmt(), 
            std::move(project_resolver)
        );
    }
}

bool Repository::is_db_open() const {
    return connection_manager_ && connection_manager_->get_db(); 
}

void Repository::import_data(
    const std::vector<DayData>& days,
    const std::vector<TimeRecordInternal>& records)
{
    if (!is_db_open()) {
        std::cerr << "Database is not open. Cannot import data." << std::endl;
        return;
    }

    if (!connection_manager_->begin_transaction()) { 
        return; 
    }

    try {
        data_inserter_->insert_days(days); 
        data_inserter_->insert_records(records); 

        if (!connection_manager_->commit_transaction()) { 
            throw std::runtime_error("Failed to commit transaction.");
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred during data import: " << e.what() << std::endl;
        connection_manager_->rollback_transaction(); 
    }
}