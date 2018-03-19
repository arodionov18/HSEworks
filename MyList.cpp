#include <iostream>
#include <cstddef>
#include <utility>
#include <type_traits>

template<typename T>

class List {
public:
    class iterator;

    List();
    List(const List& other);
    ~List();

    List& operator =(const List& other);

    size_t size() const;

    void push_back(const T&);
    void push_front(const T&);
    void pop_back();
    void pop_front();

    iterator begin() const;
    iterator end() const;

    class Elem {
    public:
        Elem() : val(nullptr), l(this), r(this) {}
        Elem(T* val, Elem* l, Elem* r);
        ~Elem();

        friend class List;
    private:
        T* val;
        Elem* l;
        Elem* r;
    };

    class iterator {
    public:
        explicit iterator(Elem* ptr);

        iterator& operator ++();
        iterator operator ++(int);
        iterator& operator --();
        iterator operator --(int);

        bool operator ==(const iterator other) const;
        bool operator !=(const iterator other) const;

        T& operator *();

    private:
        Elem* ptr;
    };

private:
    Elem *last;
    size_t sz;
};

template<typename T>
List<T>::List() {
    last = new List<T>::Elem();
    sz = 0;
}

template<typename T>
List<T>::List(const List& other) {
    last = new List<T>::Elem();
    sz = 0;
    List<T>::iterator x = other.begin();
    while (x != other.end()) {
        this->push_back(*x);
        ++x;
    }
}

template<typename T>
List<T>::~List() {
    while (sz > 0) {
        this->pop_back();
    }
    delete last;
}

template<typename T>
void List<T>::push_back(const T& x) {
    Elem* tmp = new Elem(new T(x), last->l, last);
    last->l = tmp;
    tmp->l->r = tmp;
    ++sz;
}

template<typename T>
void List<T>::push_front(const T& x) {
    Elem* tmp = new Elem(new T(x), last, last->r);
    last->r = tmp;
    tmp->r->l = tmp;
    ++sz;
}

template<typename T>
void List<T>::pop_back() {
    Elem* tmp = last->l;
    tmp->l->r = last;
    last->l = tmp->l;
    delete tmp;
    --sz;
}

template<typename T>
void List<T>::pop_front() {
    Elem* tmp = last->r;
    last->r = tmp->r;
    tmp->r->l = last;
    delete tmp;
    --sz;
}

template<typename T>
List<T>& List<T>::operator =(const List<T>& other) {
    while (sz > 0) {
        this->pop_back();
    }
    List<T>::iterator x = other.begin();
    while (x != other.end()) {
        this->push_back(*x);
        ++x;
    }
}

template<typename T>
size_t List<T>::size() const {
    return sz;
}

template<typename T>
typename List<T>::iterator List<T>::begin() const {
    return iterator(last->r);
}

template<typename T>
typename List<T>::iterator List<T>::end() const {
    return iterator(last);
}

template<typename T>
List<T>::Elem::Elem(T* val, Elem* l, Elem* r) :
    val(val), l(l), r(r) {
    if (l != nullptr) {
        l->r = this;
    } else {
        l = this;
    }
    if (r != nullptr) {
        r->l = this;
    } else {
        r = this;
    }
}

template<typename T>
List<T>::Elem::~Elem() {
    l = nullptr;
    r = nullptr;
    delete val;
}

template<typename T>
List<T>::iterator::iterator(List<T>::Elem* ptr) : ptr(ptr) {
}

template<typename T>
typename List<T>::iterator& List<T>::iterator::operator ++() {
    ptr = ptr->r;
    return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator ++(int) {
    auto tmp = ptr;
    ptr = ptr->r;
    return iterator(tmp);
}

template<typename T>
typename List<T>::iterator& List<T>::iterator::operator --() {
    ptr = ptr->l;
    return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator --(int) {
    auto tmp = ptr;
    ptr = ptr->l;
    return iterator(tmp);
}

template<typename T>
T& List<T>::iterator::operator *() {
    return *(ptr->val);
}

template<typename T>
bool List<T>::iterator::operator ==(const typename List<T>::iterator other) const {
    return (ptr == other.ptr);
}

template<typename T>
bool List<T>::iterator::operator !=(const typename List<T>::iterator other) const {
    return (ptr != other.ptr);
}
