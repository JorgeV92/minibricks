#ifndef MINIBRICKS_DB_H_
#define MINIBRICKS_DB_H_

#include <sqlite3.h>

#include <functional>
#include <stdexcept>
#include <string>

namespace minibricks {

class SqliteError : public std::runtime_error {
 public:
  explicit SqliteError(const std::string& message) : std::runtime_error(message) {}
};

class Statement;

class Database {
 public:
  Database();
  ~Database();

  Database(const Database&) = delete;
  Database& operator=(const Database&) = delete;

  Database(Database&& other) noexcept;
  Database& operator=(Database&& other) noexcept;

  void Open(const std::string& path);
  void Close();
  bool IsOpen() const;

  void Execute(const std::string& sql) const;
  void ExecuteFile(const std::string& path) const;
  void WithTransaction(const std::function<void()>& fn) const;

  sqlite3* Handle() const { return db_; }
  std::string ErrorMessage() const;

 private:
  sqlite3* db_ = nullptr;
};

class Statement {
 public:
  Statement(sqlite3* db, const std::string& sql);
  ~Statement();

  Statement(const Statement&) = delete;
  Statement& operator=(const Statement&) = delete;

  Statement(Statement&& other) noexcept;
  Statement& operator=(Statement&& other) noexcept;

  void BindInt(int index, int value) const;
  void BindInt64(int index, int64_t value) const;
  void BindDouble(int index, double value) const;
  void BindText(int index, const std::string& value) const;
  void BindNull(int index) const;

  bool Step();
  void Reset() const;
  void ClearBindings() const;

  int ColumnInt(int index) const;
  int64_t ColumnInt64(int index) const;
  double ColumnDouble(int index) const;
  std::string ColumnText(int index) const;

 private:
  sqlite3_stmt* stmt_ = nullptr;
};

void CheckSqlite(int rc, sqlite3* db, const std::string& context);

}  // namespace minibricks

#endif  // MINIBRICKS_DB_H_
