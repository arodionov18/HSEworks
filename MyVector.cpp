#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

template<typename T>
struct VectorBase {
    size_t capacity;
    T* data;

    VectorBase() {
        data = nullptr;
        capacity = 0;
    }

    VectorBase(size_t n) {
        data = static_cast<T*>(operator new(n * sizeof(T)));
        capacity = n;
    }

    void swap(VectorBase& other) {
        std::swap(data, other.data);
        std::swap(capacity, other.capacity);
    }

    ~VectorBase() {
        operator delete(data);
    }
};

template<typename T>
class Vector {
private:
    VectorBase<T> base;
    size_t sz;

    void reallocate(size_t n) {
        VectorBase<T> tmp(n);
        size_t i = 0;
        try {
            for (i = 0; i < sz; ++i) {
                new (tmp.data + i) T(base.data[i]);
            }
        } catch(...) {
            for (int j = 0; j != i; ++i) {
                base.data[j].~T();
            }
            throw;
        }
        base.swap(tmp);
        for (size_t j = 0; j < sz; ++j) {
            tmp.data[j].~T();
        }
    }

public:
    Vector() {
        sz = 0;
    }

    Vector(size_t n, T& x = T()) : base(n) {
        size_t i;
        try {
            for (i = 0; i < n; ++i) {
                new (base.data + i) T(x);
            }
        } catch(...) {
            for (int j = 0; j != i; ++i) {
                base.data[j].~T();
            }
            throw;
        }
        sz = n;
    }

    void swap(Vector<T>& other) {
        base.swap(other.base);
        std::swap(sz, other.sz);
    }

    void reserve(size_t n) {
        if (n > base.capacity) {
            reallocate(n);
        }
    }

    void resize(size_t n) {
        if (n < sz) {
            for (size_t i = n; i < sz; ++i) {
                base.data[i].~T();
            }
            sz = n;
            return;
        } else if (n > sz) {
            if (n > base.capacity) {
                VectorBase<T> tmp(std::max(n, base.capacity));
                size_t i = 0;
                try {
                    for (i = 0; i < sz; ++i) {
                        new (tmp.data + i) T(base.data[i]);
                    }
                } catch(...) {
                    for (int j = 0; j != i; ++i) {
                        tmp.data[j].~T();
                    }
                    throw;
                }
                try {
                    for (i = sz; i < n; ++i) {
                        new (tmp.data + i) T();
                    }
                } catch(...) {
                    for (int j = 0; j != i; ++i) {
                        tmp.data[j].~T();
                    }
                    throw;
                }
                base.swap(tmp);
                for (int j = 0; j < sz; ++j) {
                    tmp.data[j].~T();
                }
            } else {
                size_t i;
                try {
                    for (i = sz; i < n; ++i) {
                        new (base.data + i) T();
                    }
                } catch(...) {
                    for (int j = 0; j != i; ++i) {
                        base.data[j].~T();
                    }
                    throw;
                }
            }
            sz = n;
            return;
        }
    }

    size_t capacity() const {
        return base.capacity;
    }

    size_t size() const {
        return sz;
    }

    Vector(const Vector& other) {
        for (size_t i = 0; i < sz; ++i) {
            base.data[i].~T();
        }
        sz = 0;
        reserve(other.sz);
        size_t i;
        try {
            for (i = 0; i < other.sz; ++i) {
                new (base.data + i) T(other[i]);
            }
        } catch(...) {
            for (size_t j = 0; j < i; ++j) {
                base.data[j].~T();
            }
        }
        sz = other.sz;
    }

    Vector<T>& operator =(const Vector<T>& other) {
        Vector<T> tmp(other);
        swap(tmp);
        return (*this);
    }

    void push_back(const T& x) {
        if (sz >= base.capacity) {
            if (base.capacity == 0) {
                reallocate(1);
            } else {
                reallocate(2 * base.capacity);
            }
        }
        try {
            new (base.data + sz) T(x);
        } catch(...) {
            throw;
        }
        ++sz;
    }

    void push_back(T&& x) {
        if (sz >= base.capacity) {
            if (base.capacity == 0) {
                reallocate(1);
            } else {
                reallocate(2 * base.capacity);
            }
        }
        try {
            new (base.data + sz) T(std::move(x));
        } catch(...) {
            throw;
        }
        ++sz;
    }

    void pop_back() {
        base.data[sz - 1].~T();
        --sz;
    }

    T& operator[](const int i) {
        return base.data[i];
    }

    T operator[](const int i) const {
        return base.data[i];
    }

    T* begin() {
        return base.data;
    }

    const T* begin() const {
        return base.data;
    }

    T* end() {
        return (base.data + sz);
    }

    const T* end() const {
        return base.data[sz];
    }

    void clear() {
        resize(0);
    }

    ~Vector<T>() {
        for (size_t i = 0; i < sz; ++i) {
            base.data[i].~T();
        }
    }
};
