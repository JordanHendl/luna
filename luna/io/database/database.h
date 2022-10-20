#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace luna {
inline namespace v1 {
/**
 * @brief Object to describe a database for the engine.
 * Database structure must contain a top-level database.json, the rest of the
 * structure does not matter. This object uses the top-level JSON file to
 * determine the access to the rest of the structure.
 */
class Database {
 public:
  struct ModelInfo {
    std::string model_path;
    std::vector<std::string> albedos;
    std::vector<std::string> normals;
    std::vector<std::string> roughness;
  };

  explicit Database(std::string_view base_path);
  ~Database();
  auto audio(std::string_view key) -> std::optional<std::string_view>;
  auto image(std::string_view key) -> std::optional<std::string_view>;
  auto model(std::string_view key) -> std::optional<ModelInfo>;

 private:
  struct DatabaseData;
  std::unique_ptr<DatabaseData> data;
};
}  // namespace v1
}  // namespace luna