#include "bitmap.h"
#include <iostream>
#include <memory.h>
#include <assert.h>
#include <bitset>
#include <random>

void BitMap::set_btmp_len(int len) {
    this -> btmp_len_ = len / 8;
}

void BitMap::init() {
    bytes_ = new uint8_t[btmp_len_];
    memset(this -> bytes_, 0xff, this -> btmp_len_);
    bit_idx();
}

bool BitMap::check(uint32_t bit_idx) {
    assert(bit_idx < this -> btmp_len_ * 8);
    uint32_t byte_idx = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;
    return (this -> bytes_[byte_idx] & (BITMAP_MASK << (7 - bit_odd)));
}

void BitMap::set(uint32_t bit_idx, int8_t value) {
    assert((value == 0) || (value == 1));
    uint32_t byte_idx = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;
    if (value) {
        this -> bytes_[byte_idx] |= (BITMAP_MASK << (7 - bit_odd));
    } else {
         this -> bytes_[byte_idx] &= ~(BITMAP_MASK << (7 - bit_odd));
    }
}

int BitMap::size() {
     return (this -> btmp_len_);
 }

 int BitMap::bit_len() {
     int len = 0;
     for (int idx = 0; idx < (btmp_len_ * 8); ++idx) {
         if (check(idx) == 0) {
             len++;
         }
     }
     return len;
 }

 std::vector<int> BitMap::bit_idx() {
     idxs_.clear();
     for (int idx = 0; idx < (btmp_len_ * 8); ++idx) {
         if (check(idx) == 0) {
             idxs_.push_back(idx);
         }
     }

     return idxs_;
 }

void BitMap::set_idx(std::vector<int> new_idxs) {
    idxs_ = new_idxs;
}

std::vector<int>BitMap::get_idx() const {
    return idxs_;
}
 void BitMap::destory() {
     delete []bytes_;
     this -> bytes_ = nullptr;
 }

 void BitMap::show() {
     assert(bytes_ != nullptr);
     for (int i = 0; i < btmp_len_ ; ++i) {
         std::bitset<sizeof(uint8_t) * 8> bs(bytes_[i]);
         std::cout << "第" << i << "字节： " << bs << std::endl;
     }

     std::cout << "可用块数：" << bit_len() << std::endl;
     std::cout << "可用位置：";
     std::vector<int> idx = bit_idx();
     for (auto id : idx) {
         std::cout << id << " ";
     } 
     std::cout << std::endl;
 }

 void BitMap::random() {
    std::default_random_engine e;
    std::uniform_int_distribution<unsigned> u(0, (btmp_len_ * 8));
    for (size_t i = 0; i < 3; i++) {
        set(u(e),0);
    }
 }








