#ifndef BASIC_BITMAP_H_
#define BASIC_BITMAP_H_

#pragma once;

#include <stdint.h>
#include <vector>

#define BITMAP_MASK 1

class BitMap {
private:
    uint32_t btmp_len_;
    uint8_t *bytes_;
    std::vector<int> idxs_;
public:
    void set_btmp_len(int len);
    void random();
    BitMap() = default;
    void init();
    bool check(uint32_t bit_idx);
    void set(uint32_t bit_idx, int8_t value);
    int size();
    int bit_len();
    std::vector<int> bit_idx();
    void set_idx(std::vector<int> new_idxs);
    std::vector<int>get_idx() const;
    void destory();
    void show();
    ~BitMap() {destory();}
};

#endif // BASIC_BITMAP_H_