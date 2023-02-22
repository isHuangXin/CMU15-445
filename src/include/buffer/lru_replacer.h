//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.h
//
// Identification: src/include/buffer/lru_replacer.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex>  // NOLINT 包含std::mutex、std::scoped_lock
#include <vector>

#include "buffer/replacer.h"
#include "common/config.h"
#include "unordered_map"

namespace bustub {

/**
 * LRUReplacer implements the lru replacement policy, which approximates the Least Recently Used policy.
 */
class LRUReplacer : public Replacer {
 public:
  /**
   * Create a new LRUReplacer.
   * @param num_pages the maximum number of pages the LRUReplacer will be required to store
   */

  // explicit关键字只能用来修饰类内部的构造函数声明，作用于单个参数的构造函数；
  // 被修饰的构造函数的类，不能发生相应的隐式类型转换。
  explicit LRUReplacer(size_t num_pages);

  /**
   * Destroys the LRUReplacer.
   */
  ~LRUReplacer() override;

  bool Victim(frame_id_t *frame_id) override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  size_t Size() override;

 private:
  // TODO(student): implement me!
  std::mutex mut;                                                           // 信号量
  std::list<frame_id_t> LRUlist;                                            // 存放frame_id_t的双向链表
  std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator> LRUhash;  // 哈希表 frame_id_t->链表位置
  size_t max_size;   
};

}  // namespace bustub
