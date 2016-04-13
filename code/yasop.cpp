#include <utility> // for forward

namespace acddeejj {

template<
    typename T
> constexpr T *deconstify(const T *ptr) { return const_cast<T *>(ptr); }

template<template<typename> class S, typename T> struct resembles_pointer {
    S<T> *downcast() { return static_cast<S<T> *>(this); }
          T &operator*() { return *downcast()->get(); }
    const T &operator*() const { return **deconstify(this); }
          T *operator->() { return downcast()->get(); }
    const T *operator->() const { return deconstify(this)->operator->(); }
};

template<typename T> struct yasop: resembles_pointer<yasop, T> {
    using ReferenceCount = unsigned;

    struct Box {
        ReferenceCount refcount = 1;
        union Holder {
            T value;
            char raw[sizeof(T)];

            template<typename... Pack> Holder(Pack &&... pack):
                value(std::forward<Pack>(pack)...)
            {}
            ~Holder() { reinterpret_cast<T *>(raw)->~T(); }
        } holder;

        template<typename... Pack> Box(Pack &&...pack):
            holder(std::forward<Pack>(pack)...)
        {}
    };

    Box *box = nullptr;

    yasop() {}
    yasop(const yasop &model): box(model.box) { incBox(); }

    ~yasop() { decBox(); }

    T *get() { return reinterpret_cast<T *>(&box->holder.raw); }

    operator bool() const { return box; }

    yasop &operator=(const yasop &model) {
        if(this != &model) {
            release();
            box = model.box;
            incBox();
        }
        return *this;
    }

    template<
        typename Other
    > yasop &operator=(const yasop<Other> &model) {
        auto casted = reinterpret_cast<const yasop *>(&model);
        *this = *casted;
        if(false) {
            // idiom to perform template properties check:
            // a false branch that does the checks
            auto error_not_compatible_types =
                static_cast<const T *>(&model.box->holder.value);

            union ContainsOtherWithoutConstructing {
                char whatever;
                yasop<T>::Box tBox;
                typename yasop<Other>::Box oBox;
                ContainsOtherWithoutConstructing(): whatever() {}
                ~ContainsOtherWithoutConstructing() {}
            };
            static ContainsOtherWithoutConstructing global;
            static_assert(
                &global.tBox.holder.value == &global.oBox.holder.value,
                "T and base T of Other have different alignment in the boxes"
            );
        }
        return *this;
    }

    void release() {
        decBox();
        box = nullptr;
    }

    void incBox() { if(box) { ++box->refcount; } }
    void decBox() {
        if(!box or --box->refcount) { return; }
        delete box;
    }
};

template<
    typename T, typename... Pack
> yasop<T> make_yasop(Pack &&... pack) {
    auto box = new typename yasop<T>::Box(std::forward<Pack>(pack)...);
    yasop<T> rv;
    rv.box = box;
    return rv;
}

template<
    typename Derived, typename Base
> yasop<Derived> dynamic_smart_ptr_cast(const yasop<Base> &arg) {
    auto sub = arg.box ? dynamic_cast<Derived *>(arg.box->cast()) : nullptr;
    yasop<Derived> rv;
    if(sub) {
        rv.box = arg.box;
        ++rv.box->refcount;
    }
    return rv;
}

}

// proves yasop is as big as a naked pointer
static_assert(sizeof(acddeejj::yasop<int>) == sizeof(void *), "");

// shows the overhead is the counter aligned
static_assert(sizeof(typename acddeejj::yasop<long>::Box) == 2*sizeof(long), "");
static_assert(sizeof(typename acddeejj::yasop<int>::Box) == 2*sizeof(int), "");
static_assert(sizeof(typename acddeejj::yasop<short>::Box) == 2*sizeof(int), "");

// The assembler shows not much is going on
acddeejj::yasop<int> example() {
    return acddeejj::make_yasop<int>(77);
}

struct Base1 { int v; };
struct Base2 { int v; };
struct Derived: Base1, Base2 { int v; };

using Base =
#ifdef SHOW_ERROR1
    double
#else
    #ifdef SHOW_ERROR2
        Base2
    #else
        Base1
    #endif
#endif
;

acddeejj::yasop<Base> convert(acddeejj::yasop<Derived> &c) {
    acddeejj::yasop<Base> rv;
    rv = c;
    return rv;
}

#ifdef SHOW_ERROR3
    struct Base3 {
        long v;
    };
    struct Derived2: Base1, Base3 {};

    acddeejj::yasop<Base> convert(acddeejj::yasop<Derived2> &c) {
        acddeejj::yasop<Base> rv;
        rv = c;
        return rv;
    }
#endif


#ifdef BIG_EXAMPLE

// A substantial example: yasop of a derived class
struct Tree {
    virtual ~Tree();

    acddeejj::yasop<Tree> root;
    int info;
    virtual void identity() const;
};

// Tree has a virtual pointer
static_assert(sizeof(Tree) == 3*sizeof(long), "");

// ugly inlining of recursion
void consume(acddeejj::yasop<Tree> &node) {
    if(node and !node->info) {
        node.release();
    }
}

#include <iostream>

Tree::~Tree() { std::cout << "Bye from Tree " << (void *)this << std::endl; }
void Tree::identity() const { std::cout << "I am a Tree" << std::endl; }

struct Other: Tree {
    Other() { std::cout << "Other Created at " << (void *)this << std::endl; }

    ~Other() { std::cout << "Bye from Other " << (void *)this << std::endl; }

    void identity() const override {
        std::cout << "I am an Other" << std::endl;
    }
};

void doSomething(Tree &tree);

int main(int argc, const char *argv[]) {
    acddeejj::yasop<Tree> root;
    { auto preroot = acddeejj::make_yasop<Other>();
        root = preroot;
    }
    std::cout << "All fine" << std::endl;
    root->identity();
    doSomething(*root);
    return 0;
}

#endif
