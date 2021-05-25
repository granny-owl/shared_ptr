#include <algorithm>
#include <memory>
#include <utility>

template <class T>
class shared {
    T* ptr = nullptr;
    int* count = new int(0);

public:
    shared() { }

    shared(T* p) : ptr(p) { //ctor
        (*count)++;
    } 

    shared(const shared<T>& p) : ptr(p.ptr), count(p.count) { //copy ctor
        ++(*count);
    }

    shared(shared<T>&& p) : ptr(p.ptr), count(p.count) { //move ctor
        p.ptr = nullptr;
        p.count = nullptr;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
    int use_count() const { return *count; }

    bool operator==(const shared<T> rhs) {
        return this->ptr == rhs.ptr;
    }

    bool operator!=(const shared<T> rhs) {
        return !(*this == rhs);
    }

    shared<T>& operator=(shared<T>&& p) {
        if (*this != p) {
            ptr = p.ptr;
        }
        return *this;
    }

    ~shared() {
        if (count != nullptr && *count >= 1) {
            --(*count);
        }
        if (count != nullptr && *count == 0) delete count;    
        if(count == nullptr) delete ptr;

    }
};


template <class T>
shared<T> make_shared(T& obj) {
    T* ptr = &obj;
    return shared<T>(ptr); //count = 1, use ctor
}