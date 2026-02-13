#include "windows/elements.hpp"

namespace danmaku
{
    // 静态成员定义及初始化
    UINT_PTR element::currentElementID = 0;
    std::unordered_map<UINT, element *> element::s_idMap;
    std::vector<UINT> element::s_freeIds;
    // 互斥锁（我知道我们这里用不上但我想试试）
    // （毕竟这个代码未来或许能用上）
    std::mutex element::s_mutex;

    // ---------- 静态辅助函数 ----------
    UINT element::allocateID()
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (!s_freeIds.empty())
        {
            UINT id = s_freeIds.back();
            s_freeIds.pop_back();
            return id;
        }
        // 无空闲ID，使用递增计数器（从1开始）
        return ++currentElementID;
    }

    void element::releaseID(UINT id, element *obj)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_idMap.find(id); // 迭代器
        // 确保要释放的ID确实对应正确的对象（防止错误调用）
        // 否则可能重复释放或无效ID
        if (it != s_idMap.end() && it->second == obj)
        {
            s_idMap.erase(it);       // 从对照表中移除
            s_freeIds.push_back(id); // 加入空闲池以供重用
        } // else{debug::logOutput("[erro]");}
    }

    // // ---------- 移动构造函数 ----------
    // element::element(element &&other) noexcept
    //     : elementID(other.elementID) // 窃取原对象的ID
    // {
    //     if (this != &other)
    //     {
    //         // 原对象不再拥有该ID，将其置为0（无效）
    //         other.elementID = 0;

    //         // 更新全局对照表：将ID对应的指针指向新对象
    //         std::lock_guard<std::mutex> lock(s_mutex);
    //         s_idMap[elementID] = this;
    //     }
    // }

    // // ---------- 移动赋值运算符 ----------
    // element &element::operator=(element &&other) noexcept
    // {
    //     if (this != &other)
    //     {
    //         // 释放当前对象持有的ID（如果有）
    //         if (elementID != 0)
    //             releaseID(elementID, this);

    //         // 转移ID
    //         elementID = other.elementID;
    //         other.elementID = 0;

    //         // 更新对照表
    //         std::lock_guard<std::mutex> lock(s_mutex);
    //         s_idMap[elementID] = this;
    //     }
    //     return *this;
    // }

    // // ---------- 析构函数 ----------
    // element::~element()
    // {
    //     if (elementID != 0)
    //         releaseID(elementID, this);
    // }

    // ---------- 全局查找函数 ----------
    element &searchID(UINT id)
    {
        std::lock_guard<std::mutex> lock(element::s_mutex);
        auto it = element::s_idMap.find(id);
        if (it != element::s_idMap.end())
            return *(it->second);
        else
            throw std::runtime_error("element::searchID: invalid ID");
    }
}