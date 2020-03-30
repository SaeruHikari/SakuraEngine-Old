#pragma once
#ifdef _MSC_VER
#pragma warning (disable:4624)
#endif

#include "T2String.h"
#include <assert.h>
#include "../CoreMinimal/CoreMinimal.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>

class sstream
{
public:
	sstream() : _cap(0), _size(0), _p(0) {}
	explicit sstream(size_t capacity)
		: _cap(capacity), _size(0), _p((char*)malloc(capacity)) {
	}
	~sstream() {
		if (_p) free(_p);
	}
	sstream(const sstream&) = delete;
	void operator=(const sstream&) = delete;

    sstream(sstream&& fs) noexcept
        : _cap(fs._cap), _size(fs._size), _p(fs._p) {
        fs._p = 0;
    }

    const char* data() const {
        return _p;
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    size_t capacity() const {
        return _cap;
    }

    char& back() const {
        return _p[_size - 1];
    }

    char& front() const {
        return _p[0];
    }

    char& operator[](size_t i) const {
        return _p[i];
    }

    const char* c_str() const {
        ((sstream*)this)->reserve(_size + 1);
        if (_p[_size] != '\0') _p[_size] = '\0';
        return _p;
    }

    Sakura::sstring str() const {
        return Sakura::sstring(_p, _size);
    }

    void clear() {
        _size = 0;
    }

    void resize(size_t n) {
        this->reserve(n);
        _size = n;
    }

    void reserve(size_t n) {
        if (_cap < n) {
            _p = (char*)realloc(_p, n);
            assert(_p);
            _cap = n;
        }
    }

    void swap(sstream& fs) noexcept {
        std::swap(fs._cap, _cap);
        std::swap(fs._size, _size);
        std::swap(fs._p, _p);
    }

    void swap(sstream&& fs) noexcept {
        fs.swap(*this);
    }

    sstream& append(char c) {
        this->_Ensure(1);
        _p[_size++] = c;
        return *this;
    }

    sstream& append(const void* p, size_t n) {
        this->_Ensure(n);
        memcpy(_p + _size, p, n);
        _size += n;
        return *this;
    }

    sstream& append(const char* s) {
        return this->append(s, strlen(s));
    }

    sstream& append(const std::string& s) {
        return this->append(s.data(), s.size());
    }

    sstream& append(const Sakura::sstring& s) {
        return this->append(s.data(), s.size());
    }

    sstream& append(const sstream& s) {
        return this->append(s.data(), s.size());
    }

    sstream& append(char c, size_t n) {
        this->_Ensure(n);
        memset(_p + _size, c, n);
        _size += n;
        return *this;
    }

    sstream& append(size_t n, char c) {
        return this->append(c, n);
    }

    template <typename T>
    sstream& append(const T& v) {
        return this->append(&v, sizeof(T));
    }

    sstream& operator<<(bool v) {
        if (v) return this->append("true", 4);
        return this->append("false", 5);
    }

    sstream& operator<<(char v) {
        return this->append(v);
    }

    sstream& operator<<(unsigned char v) {
        this->_Ensure(4);
        _size += Sakura::u32toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(short v) {
        this->_Ensure(8);
        _size += Sakura::i32toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(unsigned short v) {
        this->_Ensure(8);
        _size += Sakura::u32toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(int v) {
        this->_Ensure(12);
        _size += Sakura::i32toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(unsigned int v) {
        this->_Ensure(12);
        _size += Sakura::u32toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(long v) {
        this->_Ensure(24);
        _size += Sakura::i64toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(unsigned long v) {
        this->_Ensure(24);
        _size += Sakura::u64toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(long long v) {
        this->_Ensure(24);
        _size += Sakura::i64toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(unsigned long long v) {
        this->_Ensure(24);
        _size += Sakura::u64toa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(const char* v) {
        return this->append(v, strlen(v));
    }

    sstream& operator<<(const std::string& v) {
        return this->append(v.data(), v.size());
    }


    sstream& operator<<(const sstream& fs) {
        return this->append(fs.data(), fs.size());
    }

    sstream& operator<<(const void* v) {
        this->_Ensure(20);
        _size += Sakura::u64toh((Sakura::uint64)v, _p + _size);
        return *this;
    }

    sstream& operator<<(float v) {
        this->_Ensure(24);
        _size += Sakura::dtoa(v, _p + _size);
        return *this;
    }

    sstream& operator<<(double v) {
        this->_Ensure(24);
        _size += Sakura::dtoa(v, _p + _size);
        return *this;
    }

private:
    void _Ensure(size_t n) {
        if (_cap < _size + n) this->reserve((_cap * 3 >> 1) + n);
    }

    // enable more efficient control of the underlying buffer
    friend class magicstream;
private:
	size_t _cap;
	size_t _size;
	char* _p;
};

