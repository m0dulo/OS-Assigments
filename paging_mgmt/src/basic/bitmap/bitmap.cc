#include "bitmap.h"
#include <iostream>
#include <memory.h>
#include <assert.h>
#include <bitset>

void BitMap::init(int len) {
    this -> btmp_len_ = len;
    bytes_ = new uint8_t;
    memset(this -> bytes_, 0xff, this -> btmp_len_);
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
     std::vector<int> idxs;
     idxs.clear();
     for (int idx = 0; idx < (btmp_len_ * 8); ++idx) {
         if (check(idx) == 0) {
             idxs.push_back(idx);
         }
     }

     return idxs;
 }

 void BitMap::destory() {
     delete bytes_;
 }

 void BitMap::show() {
     for (int i = 0; i < btmp_len_ ; ++i) {
         std::bitset<sizeof(uint8_t) * 8> bs(bytes_[i]);
         std::cout << bs << std::endl;
     }
 }








