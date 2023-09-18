#pragma once


#include <memory>
#include <string>
#include <unordered_map>


template<class tag, class resource>
class resource_manager {
public:
    using ptr = std::weak_ptr<resource>;
    using storage_type = std::shared_ptr<resource>;


    virtual ptr Get(const tag& key) = 0;
    virtual bool Exists(const tag& key) const = 0;
    virtual void Remove(const tag& key) = 0;


    ptr operator[](const tag& key)
    {
        return Get(key);
    }

protected:
    resource_manager() = default;
    virtual ~resource_manager() = default;

private:
    resource_manager(const resource_manager&) = delete;
    resource_manager(resource_manager&&) = delete;
};


template<class resource>
class resource_manager<std::string, resource> {
public:
    using ptr = std::weak_ptr<resource>;
    using storage_type = std::shared_ptr<resource>;


    resource_manager() = default;
    virtual ~resource_manager() = default;


    const std::unordered_map<std::string, storage_type>& Get() const { return storage; }


    virtual ptr Get(const std::string& name)
    {
        auto it = storage.find(name);
        if (it == storage.end()) {
            return storage.insert({name, storage_type(new resource())}).first->second;
        }
        return ptr(it->second);
    }


    ptr operator[](const std::string& key)
    {
        return Get(key);
    }


    virtual void Remove(const std::string& key)
    {
        auto it = storage.find(key);
        if (it != storage.end()) {
            storage.erase(it);
        }
    }


    virtual bool Exists(const std::string &name) const
    {
        return storage.find(name) != storage.end();
    }

protected:
    std::unordered_map<std::string, storage_type> storage;

private:
    resource_manager(const resource_manager&) = delete;
    resource_manager(resource_manager&&) = delete;
};


template<class resource>
using resource_manager_named = resource_manager<std::string, resource>;
