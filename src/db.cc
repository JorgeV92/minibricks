#include "minibricks/db.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace minibricks {

namespace {

std::string ReadFile(const std::string& path) {
  std::ifstream input(path);
  if (!input) {
    throw SqliteError("failed to open file: " + path);
  }
  std::ostringstream buffer;
  buffer << input.rdbuf();
  return buffer.str();
}

}  // namespace

void CheckSqlite(int rc, sqlite3* db, const std::string& context) {
  if (rc == SQLITE_OK || rc == SQLITE_DONE || rc == SQLITE_ROW) {
    return;
  }
  const char* message = db != nullptr ? sqlite3_errmsg(db) : "unknown sqlite error";
  throw SqliteError(context + ": " + message);
}

Database::Database() = default;

Database::~Database() { Close(); }

Database::Database(Database&& other) noexcept : db_(other.db_) {
  other.db_ = nullptr;
}

Database& Database::operator=(Database&& other) noexcept {
  if (this != &other) {
    Close();
    db_ = other.db_;
    other.db_ = nullptr;
  }
  return *this;
}

void Database::Open(const std::string& path) {
  Close();
  int rc = sqlite3_open(path.c_str(), &db_);
  if (rc != SQLITE_OK) {
    std::string message = ErrorMessage();
    Close();
    throw SqliteError("failed to open database: " + message);
  }
}

void Database::Close() {
  if (db_ != nullptr) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

bool Database::IsOpen() const { return db_ != nullptr; }

void Database::Execute(const std::string& sql) const {
  char* error = nullptr;
  int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &error);
  if (rc != SQLITE_OK) {
    std::string message = error != nullptr ? error : "sqlite exec failed";
    sqlite3_free(error);
    throw SqliteError(message);
  }
}

void Database::ExecuteFile(const std::string& path) const { Execute(ReadFile(path)); }

void Database::WithTransaction(const std::function<void()>& fn) const {
  Execute("BEGIN IMMEDIATE;");
  try {
    fn();
    Execute("COMMIT;");
  } catch (...) {
    Execute("ROLLBACK;");
    throw;
  }
}

std::string Database::ErrorMessage() const {
  return db_ != nullptr ? sqlite3_errmsg(db_) : "database is not open";
}

Statement::Statement(sqlite3* db, const std::string& sql) {
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt_, nullptr);
  CheckSqlite(rc, db, "prepare statement failed");
}

Statement::~Statement() {
  if (stmt_ != nullptr) {
    sqlite3_finalize(stmt_);
  }
}

Statement::Statement(Statement&& other) noexcept : stmt_(other.stmt_) {
  other.stmt_ = nullptr;
}

Statement& Statement::operator=(Statement&& other) noexcept {
  if (this != &other) {
    if (stmt_ != nullptr) {
      sqlite3_finalize(stmt_);
    }
    stmt_ = other.stmt_;
    other.stmt_ = nullptr;
  }
  return *this;
}

void Statement::BindInt(int index, int value) const {
  CheckSqlite(sqlite3_bind_int(stmt_, index, value), sqlite3_db_handle(stmt_),
              "bind int failed");
}

void Statement::BindInt64(int index, int64_t value) const {
  CheckSqlite(sqlite3_bind_int64(stmt_, index, value), sqlite3_db_handle(stmt_),
              "bind int64 failed");
}

void Statement::BindDouble(int index, double value) const {
  CheckSqlite(sqlite3_bind_double(stmt_, index, value), sqlite3_db_handle(stmt_),
              "bind double failed");
}

void Statement::BindText(int index, const std::string& value) const {
  CheckSqlite(sqlite3_bind_text(stmt_, index, value.c_str(), -1, SQLITE_TRANSIENT),
              sqlite3_db_handle(stmt_), "bind text failed");
}

void Statement::BindNull(int index) const {
  CheckSqlite(sqlite3_bind_null(stmt_, index), sqlite3_db_handle(stmt_),
              "bind null failed");
}

bool Statement::Step() {
  int rc = sqlite3_step(stmt_);
  if (rc == SQLITE_ROW) {
    return true;
  }
  if (rc == SQLITE_DONE) {
    return false;
  }
  CheckSqlite(rc, sqlite3_db_handle(stmt_), "step failed");
  return false;
}

void Statement::Reset() const {
  CheckSqlite(sqlite3_reset(stmt_), sqlite3_db_handle(stmt_), "reset failed");
}

void Statement::ClearBindings() const {
  CheckSqlite(sqlite3_clear_bindings(stmt_), sqlite3_db_handle(stmt_),
              "clear bindings failed");
}

int Statement::ColumnInt(int index) const { return sqlite3_column_int(stmt_, index); }

int64_t Statement::ColumnInt64(int index) const {
  return sqlite3_column_int64(stmt_, index);
}

double Statement::ColumnDouble(int index) const {
  return sqlite3_column_double(stmt_, index);
}

std::string Statement::ColumnText(int index) const {
  const unsigned char* text = sqlite3_column_text(stmt_, index);
  return text == nullptr ? "" : reinterpret_cast<const char*>(text);
}

}  // namespace minibricks
