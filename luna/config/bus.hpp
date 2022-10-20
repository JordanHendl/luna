#pragma once
#include <any>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <map>
namespace luna {
namespace cfg {
enum class EventFlags {
  None,
  Require,
  Defer
};

// Object for subscribing and publishing data.
class Bus {
 public:
  inline Bus();
  inline ~Bus();
  template <typename Type>
  inline auto subscribe(std::string_view name, void (*)(const Type&), EventFlags flags = EventFlags::None) -> void;
  template <typename Type>
  inline auto subscribe(std::string_view name, void (*)(Type), EventFlags flags = EventFlags::None) -> void;
  template <typename Object, typename Type>
  inline auto subscribe(std::string_view name, Object* ref,
                        void (Object::*raw_func)(const Type&), EventFlags flags = EventFlags::None) -> void;
  template <typename Object, typename Type>
  inline auto subscribe(std::string_view name, Object* ref,
                        void (Object::*raw_func)(Type), EventFlags flags = EventFlags::None) -> void;
  template <typename Object, typename Type>
  inline auto enroll(std::string_view name, Object* ref,
                      Type (Object::*raw_func)() const) -> void;
  template <typename Object, typename Type>
  inline auto enroll(std::string_view name, Object* ref,
                      const Type& (Object::*raw_func)() const) -> void;
  template <typename Type>
  inline auto publish(std::string_view name, const Type& val) -> void;

  template <typename Type>
  inline auto publish_by_val(std::string_view name, Type val) -> void;

  inline auto publish() -> void;
  inline auto num_subscriptions() -> size_t {
    auto& subs = this->get_subscriptions();
    return subs.size();
  }

  inline auto num_subscriptions(std::string_view key) -> size_t {
    auto& subs = this->get_subscriptions();
    auto iter = subs.find(std::string(key));
    if(iter != subs.end()) return iter->second.size();
    return 0;
  }

  inline auto ready() const -> bool { 
    auto rdy = true;
    for(auto& map : this->m_required) {
      for(auto& signal : map.second) {
        if(!signal.second.first) rdy = false;
      }
    }
    return rdy;
  }

  inline auto reset() -> void {
    for(auto& map : this->m_required) {
      for(auto& signal : map.second) {
        signal.second.first = false;
      }
    }
  }
  static auto shutdown() -> void;
  static auto initialized() -> bool;
  class Subscriber {
    public:
      virtual ~Subscriber() {}
      virtual auto receive(const std::any& val) -> void = 0;

      virtual auto signal() -> void {
        for(auto* ref : this->m_required_refs){
          *ref = true;
        }
      }

      virtual auto add_required(bool* ref) -> void {
        this->m_required_refs.push_back(ref);
      }

      virtual auto remove_required(bool* ref) -> void {
        for(auto index = 0u; index < this->m_required_refs.size(); index++) {
          if(this->m_required_refs[index] == ref) {
            this->m_required_refs.erase(this->m_required_refs.begin() + index);
          }
        }
      }
    protected:
      std::vector<bool*> m_required_refs;
  };

  template <typename Type>
  class FunctionSubscriber : public Subscriber {
   public:
    FunctionSubscriber(std::function<void(Type)> func_ref) {
      this->m_func = func_ref;
    }
    auto receive(const std::any& val) -> void override {
      this->m_func(std::any_cast<Type>(val));
      this->signal();
    }

   private:
    std::function<void(Type)> m_func;
  };

  template <typename Type>
  class FunctionReferenceSubscriber : public Subscriber {
   public:
    FunctionReferenceSubscriber(std::function<void(const Type&)> func_ref) {
      this->m_func = func_ref;
    }
    auto receive(const std::any& val) -> void override {
      this->m_func(*std::any_cast<const Type*>(val));
      this->signal();
    }

   private:
    std::function<void(const Type&)> m_func;
  };

  template <typename Object, typename Type>
  class MemberFunctionReferenceSubscriber : public Subscriber {
   public:
    MemberFunctionReferenceSubscriber(
        Object* ref, std::function<void(Object*, const Type&)> func_ref) {
      this->m_ref = ref;
      this->m_func = func_ref;
    }
    auto receive(const std::any& val) -> void override {
      this->m_func(this->m_ref, *std::any_cast<const Type*>(val));
      this->signal();
    }

   private:
    Object* m_ref;
    std::function<void(Object*, const Type&)> m_func;
  };

  template <typename Object, typename Type>
  class MemberFunctionSubscriber : public Subscriber {
   public:
    MemberFunctionSubscriber(Object* ref,
                             std::function<void(Object*, Type)> func_ref) {
      this->m_ref = ref;
      this->m_func = func_ref;
    }
    virtual ~MemberFunctionSubscriber(){};
    auto receive(const std::any& val) -> void override {
      this->m_func(this->m_ref, std::any_cast<Type>(val));
      this->signal();
    }

   private:
    Object* m_ref;
    std::function<void(Object*, Type)> m_func;
  };
  class Publisher {
   public:
    virtual ~Publisher(){};
    virtual auto enroll() -> std::any = 0;
  };

  template <typename Object, typename Type, bool Referenced>
  class MemberFunctionPublisher : public Publisher {
   public:
    MemberFunctionPublisher(Object* ref,
                            std::function<Type(Object*)> func_ref) {
      this->m_ref = ref;
      this->m_func = func_ref;
    }

    virtual ~MemberFunctionPublisher(){};
    auto enroll() -> std::any override {
      if constexpr (Referenced) {
        const auto& value = this->m_func(this->m_ref);
        return std::any(value);
      } else {
        auto value = this->m_func(this->m_ref);
        return std::any(value);
      }
    }

   private:
    Object* m_ref;
    std::function<Type(Object*)> m_func;
  };

  template <typename Object, typename Type, bool Referenced>
  class MemberFunctionReferencedPublisher : public Publisher {
   public:
    MemberFunctionReferencedPublisher(Object* ref,
                            std::function<const Type&(Object*)> func_ref) {
      this->m_ref = ref;
      this->m_func = func_ref;
    }

    virtual ~MemberFunctionReferencedPublisher(){};
    auto enroll() -> std::any override {
      const auto& value = this->m_func(this->m_ref);
      return std::any(&value);
    }

   private:
    Object* m_ref;
    std::function<const Type&(Object*)> m_func;
  };

 private:
  // Global map of typeid : subscribers.
  using TypeMap = std::multimap<size_t, std::shared_ptr<Subscriber>>;

  // Global map of keys : map of type type id : subscribers.
  using KeyMap = std::unordered_map<std::string, TypeMap>;
  static auto get_subscriptions() -> KeyMap&;

  // Local storage of typeid : Iterator to the type map. This is so we can remove from the global maps on deconstructor.
  using LocalIterMap = std::multimap<size_t, TypeMap::iterator>;

  /** Same as LocalIterMap, but for required entries to store a flag. Flags MUST be stored locally, as they are unique to each bus, but...
   * this means that on a publish, each event bus can access the underlying flags of each other event bus. So, to accomplish this, we store
   * flags locally in this map, and then add references to them to the matching subscriber (see @Subscriber), so that the subscriber can handle 
   * pushing those values when they get signalled, and each bus can reset it manually (see @Bus::reset). When the bus deconstructs, it manually removes
   * the 'required' references from the specified subscribtions.
   */
  using LocalRequiredIterMap = std::multimap<size_t, std::pair<bool, TypeMap::iterator>>;

  /** Local storage of keys : map of (typeid : global map iterator). Used so object can know what to destroy when deconstructing.
   */
  using LocalKeyMap = std::unordered_map<std::string, LocalIterMap>;

  /** Same as above, but for required keys.
   */
  using LocalRequiredKeyMap = std::unordered_map<std::string, LocalRequiredIterMap>;
  
  LocalKeyMap m_map;
  LocalRequiredKeyMap m_required;

  /** Map of typeid : Publishers.
   */
  using PublisherTypeMap =
      std::unordered_map<size_t, std::shared_ptr<Publisher>>;

  /** Map of keys : maps of (typeid : Publisher)
   */
  using PublisherMap = std::unordered_map<std::string, PublisherTypeMap>;
  PublisherMap m_publishers;
};

Bus::Bus() {}
Bus::~Bus() {
  if(!this->initialized()) return;

  auto& subs = this->get_subscriptions();
  for (auto& iter : this->m_map) {
    auto& key = iter.first;
    auto global_map = subs.find(key);
    if(global_map == subs.end()) return;
    for (auto& value : iter.second) {
      global_map->second.erase(value.second);
    }
  }

  for (auto& iter : this->m_required) {
    auto& key = iter.first;
    auto global_map = subs.find(key);
    if(global_map == subs.end()) return;
    for (auto& value : iter.second) {
      value.second.second->second->remove_required(&value.second.first);
      global_map->second.erase(value.second.second);
    }
  }
  this->m_publishers.clear();
}

template <typename Type>
auto Bus::subscribe(std::string_view name, void (*raw_func)(const Type&), EventFlags flags)
    -> void {
  using Subscriber = Bus::FunctionReferenceSubscriber<Type>;
  auto func = std::function<void(const Type&)>(raw_func);
  auto& sub = this->get_subscriptions();
  auto& key_map = sub[name.begin()];  // Create if doesn't exist.
  auto iter = key_map.insert(
      {typeid(const Type&).hash_code(), std::make_shared<Subscriber>(func)});

  if(flags == EventFlags::Require) {
    auto req_iter = this->m_required[name.begin()].insert({typeid(const Type&).hash_code(), {false, iter}});
    auto* ref = &req_iter->second.first;
    iter->second->add_required(ref);
  } else this->m_map[name.begin()].insert({typeid(Type).hash_code(), iter});
}

template <typename Type>
auto Bus::subscribe(std::string_view name, void (*raw_func)(Type), EventFlags flags) -> void {
  using Subscriber = Bus::FunctionSubscriber<Type>;
  auto func = std::function<void(Type)>(raw_func);
  auto& sub = this->get_subscriptions();
  auto& key_map = sub[name.begin()];  // Create if doesn't exist.
  auto iter = key_map.insert(
      {typeid(Type).hash_code(), std::make_shared<Subscriber>(func)});
  if(flags == EventFlags::Require) {
    auto req_iter = this->m_required[name.begin()].insert({typeid(Type).hash_code(), {false, iter}});
    auto* ref = &req_iter->second.first;
    iter->second->add_required(ref);
  } else this->m_map[name.begin()].insert({typeid(Type).hash_code(), iter});
}

template <typename Object, typename Type>
auto Bus::subscribe(std::string_view name, Object* ref,
                    void (Object::*raw_func)(const Type&), EventFlags flags) -> void {
  using Subscriber = Bus::MemberFunctionReferenceSubscriber<Object, Type>;
  auto func = std::function<void(Object*, const Type&)>(raw_func);
  auto& sub = this->get_subscriptions();
  auto& key_map = sub[name.begin()];  // Create if doesn't exist.
  auto iter = key_map.insert(
      {typeid(const Type&).hash_code(), std::make_shared<Subscriber>(ref, func)});
  if(flags == EventFlags::Require) {
    auto req_iter = this->m_required[name.begin()].insert({typeid(const Type&).hash_code(), {false, iter}});
    auto* ref = &req_iter->second.first;
    iter->second->add_required(ref);
  } else this->m_map[name.begin()].insert({typeid(Type).hash_code(), iter});
}

template <typename Object, typename Type>
auto Bus::subscribe(std::string_view name, Object* ref,
                    void (Object::*raw_func)(Type), EventFlags flags) -> void {
  using Sub = Bus::MemberFunctionSubscriber<Object, Type>;
  auto func = std::function<void(Object*, Type)>(raw_func);
  auto& sub = this->get_subscriptions();
  auto& key_map = sub[name.begin()];  // Create if doesn't exist.
  auto iter = key_map.insert(
      {typeid(Type).hash_code(), std::make_shared<Sub>(ref, func)});
  if(flags == EventFlags::Require) {
    auto req_iter = this->m_required[name.begin()].insert({typeid(Type).hash_code(), {false, iter}});
    auto* ref = &req_iter->second.first;
    iter->second->add_required(ref);
  } else this->m_map[name.begin()].insert({typeid(Type).hash_code(), iter});
}

template <typename Object, typename Type>
auto Bus::enroll(std::string_view name, Object* ref,
                  Type (Object::*raw_func)() const) -> void {
  using Pub = Bus::MemberFunctionPublisher<Object, Type, false>;
  auto func = std::function<Type(Object*)>(raw_func);
  auto& pub = this->m_publishers;
  auto& key_map = pub[name.begin()];  // Create if doesn't exist.
  key_map.insert({typeid(Type).hash_code(), std::make_shared<Pub>(ref, func)});
}

template <typename Object, typename Type>
auto Bus::enroll(std::string_view name, Object* ref,
                  const Type& (Object::*raw_func)() const) -> void {
  using Pub = Bus::MemberFunctionReferencedPublisher<Object, Type, true>;
  auto func = std::function<const Type&(Object*)>(raw_func);
  auto& pub = this->m_publishers;
  auto& key_map = pub[name.begin()];  // Create if doesn't exist.
  key_map.insert({typeid(const Type&).hash_code(), std::make_shared<Pub>(ref, func)});
}

template <typename Type>
auto Bus::publish(std::string_view name, const Type& val) -> void {
  auto& sub_map = this->get_subscriptions();
  auto key_iter = sub_map.find(name.begin());
  if (key_iter != sub_map.end()) {
    auto& key_map = key_iter->second;
    auto id = static_cast<size_t>(typeid(const Type&).hash_code());
    auto range = key_map.equal_range(id);
    for (auto& iter = range.first; iter != range.second; ++iter) {
      iter->second->receive(std::any(&val));
    }
  }
}

template <typename Type>
auto Bus::publish_by_val(std::string_view name, Type val) -> void {
  auto& sub_map = this->get_subscriptions();
  auto key_iter = sub_map.find(name.begin());
  if (key_iter != sub_map.end()) {
    auto& key_map = key_iter->second;
    auto id = static_cast<size_t>(typeid(Type).hash_code());
    auto range = key_map.equal_range(id);
    for (auto& iter = range.first; iter != range.second; ++iter) {
      iter->second->receive(std::any(val));
    }
  }
}

auto Bus::publish() -> void {
  auto& sub_map = this->get_subscriptions();
  for (auto& pub : this->m_publishers) {
    auto& type_map = pub.second;
    auto& key = pub.first;
    auto sub_iter = sub_map.find(key);
    if (sub_iter != sub_map.end()) {
      auto& sub_type_map = sub_iter->second;
      for (auto& type : type_map) {
        auto& type_id = type.first;
        auto range = sub_type_map.equal_range(type_id);
        for (auto& iter = range.first; iter != range.second; ++iter) {
          const auto value = type.second->enroll();
          iter->second->receive(value);
        }
      }
    }
  }
}
}
}