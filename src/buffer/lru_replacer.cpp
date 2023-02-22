//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {
    max_size = num_pages;
}

LRUReplacer::~LRUReplacer() = default; // 析构函数几乎不用改变

/*
LRUReplacer的大小与缓冲池相同，因为它包含BufferPoolManager中所有帧的占位符。
LRUReplacer被初始化为没有frame。在LRUReplacer中只会考虑新取消固定的frame。
实现课程中讨论的LRU策略，实现以下方法：
1. Victim(T*): Replacer跟踪与所有元素相比最近最少访问的对象并删除，将其页号存储在输出参数中并返回True，为空则返回False。
2. Pin(T)：在将page固定到BufferPoolManager中的frame后，应调用此方法。它应该从LRUReplacer中删除包含固定page的frame。
3. Unpin(T)：当page的pin_count变为0时应调用此方法。此方法应将包含未固定page的frame添加到LRUReplacer。
4. Size()：此方法返回当前在LRUReplacer中的frame数量。
实现细节由您决定。您可以使用内置的STL容器。您可以假设不会耗尽内存，但必须确保操作是线程安全的。
*/

/**
 * 使用LRU策略删除一个victim frame，这个函数能得到frame_id
 * @param[out] frame_id id of frame that was removed, nullptr if no victim was found
 * @return true if a victim frame was found, false otherwise
 */
bool LRUReplacer::Victim(frame_id_t *frame_id) {
    // C++17 std::scoped_lock
    // 它能够避免死锁发生，其构造函数能够自动进行上锁操作，析构函数会对互斥量进行解锁操作，保证线程安全。
    std::scoped_lock lock{mut};
    if(LRUlist.empty()) {
        frame_id = nullptr;
        return false;
    }

    // list<int>a，那么a.back()取出的是int类型
    *frame_id = LRUlist.back(); // 取出最后一个给frame_id（对传入的参数进行修改）
    LRUhash.erase(*frame_id);   // 哈希表中删除其映射关系
    LRUlist.pop_back();         // 链表中删除最后一个
    return true;                // 以上均要加*，才能改变函数外调用时传入的参数
}

/**
 * 固定一个frame, 表明它不应该成为victim（即在replacer中移除该frame_id）
 * @param frame_id the id of the frame to pin
 */
void LRUReplacer::Pin(frame_id_t frame_id) {
    std::scoped_lock lock{mut};
    // 哈希表中找不到该frame_id
    if (!LRUhash.count(frame_id)) {
        return;
    }
    auto iter = LRUhash[frame_id];
    LRUlist.erase(iter);
    LRUhash.erase(frame_id);
}

/**
 * 取消固定一个frame, 表明它可以成为victim（即将该frame_id添加到replacer）
 * @param frame_id the id of the frame to unpin
 */
void LRUReplacer::Unpin(frame_id_t frame_id) {
    std::scoped_lock lock{mut};
    // 哈希表中已有该frame_id，直接退出，避免重复添加到replacer
    if (LRUhash.count(frame_id)) {
        return;
    }
    // 已达最大容量，无法添加到replacer
    if (LRUlist.size() == max_size) {
        return;
    }
    // 正常添加到replacer
    LRUlist.push_front(frame_id);  // 注意是添加到首部还是尾部呢？
    // 首部是最近被使用，尾部是最久未被使用
    LRUhash.emplace(frame_id, LRUlist.begin());
}

    /** @return replacer中能够victim的数量 */
    size_t LRUReplacer::Size() {
        return LRUlist.size(); 
    } 
}  // namespace bustub
