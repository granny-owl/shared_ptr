#include <algorithm>
#include <memory>
#include <utility>


template <class T>
class shared {
    T* ptr = nullptr;
    int* count = nullptr;

public:
    shared() {}

    shared(T* p) : ptr(p), count(new int(1)) {} //ctor

    shared(const shared<T>& p) { //copy ctor
        *this = p;
    }

    shared(shared<T>&& p) { //move ctor
        *this = std::move(p);
    }

    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr ;}
    T* operator->() { return ptr; }
    const T* operator->() const { return ptr; }
    T* get() { return ptr; }
    const T* get() const { return ptr; }
    int use_count() const { return (count)? *count : 0; }

    shared<T>& operator= (shared<T>&& p) {
        if(p == *this) return *this;
        ptr = p.ptr;
        count = p.count;
        p.ptr = nullptr;
        p.count = nullptr;
        return *this;
    }

    shared<T>& operator= (const shared<T>& p) {
        if(p == *this) return *this;
        ptr = p.ptr;
        count = p.count;
        ++(*count);
        return *this;
    }

    ~shared() {
        if(count) {
            --(*count);
            if(*count == 0) {
                delete ptr;
                delete count;
            }
        }   
    }

    bool operator== (const shared<T> &rhs) const {
        return rhs.ptr == this->ptr;
    }
};


template <class T, class ...Args>
shared<T> make_shared(Args... args) {
    return shared<T>(new T(args...));
}