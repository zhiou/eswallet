//
//  bytestream.hpp
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>

#include "coder.hpp"
#include "cipher.hpp"
#include "hasher.hpp"

namespace excelsecu {
//    class coder;
    class padding_handler;
    class hasher;
    class random_generator;
    
    typedef unsigned char ebyte;
    typedef std::vector<ebyte> ebyte_vec;
    
    class bytestream {
    public:
        bytestream():m_mem(new ebyte_vec()) {}
        
        bytestream(size_t size, bool random = false) // 初始化指定大小，默认全0，否则填充随机数
        {
            m_mem = std::unique_ptr<ebyte_vec>(new ebyte_vec(size));
            if (!random) return;
        
            std::default_random_engine source;
            std::uniform_int_distribution<ebyte> dist(0, 0xFF);
            auto r = std::bind(dist, source);
            std::generate(std::begin(*m_mem), std::end(*m_mem), std::ref(r));
        }
        
        bytestream(const void* hex, size_t len)
        {
            m_mem = std::unique_ptr<ebyte_vec>(new ebyte_vec(len));
            for (int i = 0; i < len; ++i) {
                (*m_mem)[i] = ((const unsigned char*)hex)[i];
            }
        }
        
        bytestream(const std::string &hex_str) {
            std::unique_ptr<ebyte_vec> result(new ebyte_vec());
            for (size_t i = 0; i < hex_str.length(); i += 2)
            {
                std::string byte = hex_str.substr(i, 2);
                unsigned char chr = (unsigned char)(int)strtol(byte.c_str(), NULL, 16);
                result->push_back(chr);
            }
            m_mem = std::move(result);
        }
        
        bytestream(const ebyte_vec &mem) {
            m_mem = std::unique_ptr<ebyte_vec>(new ebyte_vec());
            for (auto& b: mem) {
                m_mem->emplace_back(b);
            }
        }
        
    public:

    public:
        
        void append(uint8_t ut) {
             m_mem->emplace_back(ut);
        }
        
        void append(uint16_t ut) {
            m_mem->emplace_back((ut >> 8) & 0xFF);
            m_mem->emplace_back(ut & 0xFF);
        }
        
        void append(uint32_t ut) {
            m_mem->emplace_back((ut >> 24) & 0xFF);
            m_mem->emplace_back((ut >> 16) & 0xFF);
            m_mem->emplace_back((ut >> 8) & 0xFF);
            m_mem->emplace_back(ut & 0xFF);
        }
        
        void append(uint64_t ut) {
            m_mem->emplace_back((ut >> 56) & 0xFF);
            m_mem->emplace_back((ut >> 48) & 0xFF);
            m_mem->emplace_back((ut >> 40) & 0xFF);
            m_mem->emplace_back((ut >> 32) & 0xFF);
            m_mem->emplace_back((ut >> 24) & 0xFF);
            m_mem->emplace_back((ut >> 16) & 0xFF);
            m_mem->emplace_back((ut >> 8) & 0xFF);
            m_mem->emplace_back(ut & 0xFF);
        }
        
        uint32_t readUInt32(size_t index) const { 
            if (index >= length() - 4) { throw std::out_of_range("bytestream split out of range"); }
            return (((uint32_t)(*m_mem)[index]) << 24)
            | (((uint32_t)(*m_mem)[index + 1]) << 16)
            | (((uint32_t)(*m_mem)[index + 2]) << 8)
            | (((uint32_t)(*m_mem)[index + 3]) << 0);
        }
        
        bytestream split(size_t start, size_t length) const {
            if (start + length > m_mem->size()) { throw std::out_of_range("bytestream split out of range"); }
            return bytestream(ebyte_vec(m_mem->begin() + start, m_mem->begin() + start + length));
        }
    public:
        //MARK: copy constructor & assignment operator
        bytestream(const bytestream& other) {
            m_mem = std::unique_ptr<ebyte_vec>(new ebyte_vec());
            try {
                for (ebyte& b: other.mem()) {
                    m_mem->emplace_back(b);
                }
            }catch (std::exception &e) {
                
            }
        }
        
        bytestream(bytestream && other) {
            m_mem = std::move(other.m_mem);
        }
        
        bytestream& operator=(const bytestream& other) {
            if (&other == this) return *this;
            this->m_mem = std::make_unique<ebyte_vec>(*(other.m_mem));
            return *this;
        }
        
        bytestream& operator=(bytestream&& other) {
            if (&other == this) return *this;
            
            this->m_mem = std::move(other.m_mem);
            return *this;
        }
        
        unsigned char &operator[](int i) {
            return m_mem->at(i);
        }
        
        const ebyte& operator[](int i) const {
            return m_mem->at(i);
        }
    public:
        //MARK: operators
        bytestream& operator+=(const bytestream& other) {
            for (auto &ele: other.mem()) {
                m_mem->emplace_back(ele);
            }
            return *this;
        }
        
        bytestream operator+(const bytestream& other) {
            bytestream result;
            result += *this;
            result += other;
            return result;
        }
        
        bytestream operator~() // ~x, 求逆
        {
            ebyte_vec vec(m_mem->size());
            for (int i = 0; i < m_mem->size(); ++i) {
                vec[i] = ~(*m_mem)[i];
            }
            return bytestream(vec);
        }
        
        bytestream& operator^=(const bytestream& other)
        {
            size_t shorter = std::min(m_mem->size(), other.mem().size());
            for (int i = 0; i < shorter; ++i) {
                (*m_mem)[i] ^= other.mem()[i];
            }
            return *this;
        }
        
        bytestream operator^(const bytestream& other)
        {
            bytestream result(*this);
            result ^= other;
            return result;
        }
        
        bool operator==(const bytestream &other) const {
            return this == &other || *(this->m_mem) == other.mem();
        }
        
        bool operator==(const std::string &hex_str) const {
            return hex_str == this->hex_str();
        }
        
        bool operator!=(const bytestream &other) const {
            return !(*this == other);
        }
        
        bool operator!=(const std::string &hex_str) const {
            return hex_str != this->hex_str();
        }
    public:
        //MARK: getters
        const ebyte* bytes() const { return m_mem->data(); }
        
        size_t length() const { return m_mem->size(); }
        
        ebyte_vec& mem() const { return *m_mem; }
        
        std::string utf8_str() const {
            ebyte_vec utf8_bytes(*m_mem);
            if (utf8_bytes.empty() || utf8_bytes.back() != 0)
            {
                utf8_bytes.push_back(0);
            }
            return std::string((char*)utf8_bytes.data());
        }
        
        std::string hex_str() const {
            std::stringstream ss;
            for (auto b: *m_mem) {
                ss  << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)b;
            }
            return ss.str();
        }
    public:
        //MARK: encoding & decoding
//        std::string encode(const coder& acoder){
//            return acoder.encode(*this);
//        }
//        static bytestream decode(const coder& acoder, const std::string &serialized_string)
//        {
//            return acoder.decode(serialized_string);
//        }
//    public:
//        //MARK: padding & unpadding
//        bytestream& padding(const padding_handler&) {
//            return padding_handler
//        }
//        bytestream& unpadding(const padding_handler&);
//    public:
//        //MARK: hash
//        bytestream hash(const hasher& ahasher) {
//            return ahasher.hash(*this);
//        }
    public:
        bytestream tail(size_t aNum) const {
            if (length() < aNum) { throw std::invalid_argument("bytestream out of bounds"); }
            return this->split(length() - aNum, aNum);
        }
        
        bytestream front(size_t aNum) const {
            if (length() < aNum) { throw std::invalid_argument("bytestream out of bounds"); }
            return this->split(0, aNum);
        }
        
        bytestream drop_tail(size_t aNum) {
            if (length() < aNum) { throw std::invalid_argument("bytestream out of bounds"); }
            return this->split(0, length() - aNum);
        }
        
        bytestream drop_front(size_t aNum) {
            if (length() < aNum) { throw std::invalid_argument("bytestream out of bounds"); }
            return this->split(aNum, length() - aNum);
        }

        bool startWith(const std::string &prefix) const {
            return this->hex_str().substr(0, prefix.length()) == prefix;
        }
        
        bytestream& padding_zero(size_t to_size) {
            if (to_size < m_mem->size()) { throw std::invalid_argument("padding size less than current size"); }
            auto padding_len = to_size - m_mem->size();
            for (int i = 0; i < padding_len; ++i)
            {
                m_mem->emplace_back(0x00);
            }
            return *this;
        }

    public:
        //MARK: functional
        template<class T>
        T map(std::function<T(const bytestream&)> atransformer) {
            return atransformer(*this);
        }
    public:
        friend std::ostream & operator<<(std::ostream &out, bytestream &obj);
    private:
        std::unique_ptr<ebyte_vec> m_mem;
    };
    
    std::ostream & operator<<(std::ostream &out, bytestream &obj)
    {
        for (auto b: *(obj.m_mem)) {
            out  << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)b;
        }
        out << std::endl;
        return out;
    }

}
