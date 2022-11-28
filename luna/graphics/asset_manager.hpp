#pragma once
#include "luna/graphics/impl/impl.hpp"
#include <string_view>
#include <string>
#include <memory>
namespace luna {
namespace gfx {
template<typename Type>
class Asset;
class AssetManager {
  public:
  AssetManager();
  ~AssetManager();
  auto loadImage(std::string_view name) -> Asset<gfx::Image>;
  private:
};

template<typename Type>
class Asset {
  public:
    Asset() = delete;

    Asset(std::string name, std::shared_ptr<Type> ptr) {
      this->m_name = name;
      this->m_data = ptr;
    }

    ~Asset() {
    }
    
    auto name() -> std::string {return this->m_name;}
    auto operator*() -> Type& {
      return *this->m_data;
    }

     auto operator*() const -> const Type& {
      return *this->m_data;
    }

  private:  
    std::shared_ptr<Type> m_data;
    std::string m_name;
};
}
}