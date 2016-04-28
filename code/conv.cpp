#include <typeindex>

/// Uses type erasure to hold a pointer to an instance of an arbitrary type
struct Any {
    struct CommonInterface {
        virtual ~CommonInterface() {}
        virtual CommonInterface *clone() const { return nullptr; }
        virtual std::type_index tyi() const = 0;
    };

    template<typename T> struct GenericHolder: CommonInterface {
        T value;
        GenericHolder() = default;
        GenericHolder(const T &modelInstance): value(modelInstance) {}

        CommonInterface *clone() const override { return new GenericHolder<T>(value); }
        std::type_index tyi() const override { return std::type_index(typeid(GenericHolder)); }
    };

    CommonInterface *instancePointer = nullptr;

public:
    Any() = default;

    Any(const Any &model): instancePointer(0) {
        if(model.instancePointer) {
            instancePointer = model.instancePointer->clone();
        }
    }

    ~Any() { delete instancePointer; }

    Any &operator=(const Any &model) {
        if(this == &model) { return *this; }
        delete instancePointer;
        instancePointer = 0;
        if(model.instancePointer) {
            instancePointer = model.instancePointer->clone();
        }
        return *this;
    }

    Any &operator=(Any &&model) {
        delete instancePointer;
        instancePointer = nullptr;
        instancePointer = model.instancePointer;
        return *this;
    }

    template<typename T> void set(const T &value) {
        auto substitute = new GenericHolder<T>(value);
        delete instancePointer;
        instancePointer = substitute;
    }

    template<typename T> T &as() {
        GenericHolder<T> &desiredType =
            dynamic_cast<GenericHolder<T> &>(*instancePointer);
        return desiredType.value;
    }

    template<typename T> const T &as() const {
        return const_cast<Any *>(this)->as<T>();
    }

    template<typename T> T *ptr(T *&p) {
        GenericHolder<T> *desiredPtr =
            dynamic_cast<GenericHolder<T> *>(instancePointer);
        return p = desiredPtr ? &desiredPtr->value : 0;
    }

    template<typename T> const T *ptr(const T *&p) const {
        return const_cast<Any *>(this)->ptr<T>(const_cast<T *&>(p));
    }
};

#include <type_traits>
#include <unordered_map>

std::unordered_map<std::type_index, std::unordered_map<std::type_index, void (*)(Any &target, const Any &source)>> the_map;

template<typename...> struct Initializer;
template<> struct Initializer<> { static void doyathing() {} };

template<typename Target, typename... Sources> struct PairConvertibility { static void doyathing() {} };
template<typename Head, typename... Rest> struct Initializer<Head, Rest...> {
    static void doyathing() {
        PairConvertibility<Head, Rest...>::doyathing();
        Initializer<Rest...>::doyathing();
    }
};

void seeme(std::type_index t, std::type_index d);

template<typename Target, typename Source>
typename std::enable_if<!std::is_assignable<Target &, Source>::value, void>::type
setConversion() {}

template<typename Target, typename Source>
typename std::enable_if<std::is_assignable<Target &, Source>::value, void>::type
setConversion() {
    auto
        targetNdx = std::type_index(typeid(Any::GenericHolder<Target>)),
        sourceNdx = std::type_index(typeid(Any::GenericHolder<Source>));
    the_map[targetNdx][sourceNdx] =
        [](Any &target, const Any &source)
        { static_cast<Any::GenericHolder<Target> *>(target.instancePointer)->value = source.as<Source>(); };
        //{ target.set<Target>(source.as<Source>()); };
}


template<typename T>
struct PairConvertibility<T> {
    static_assert(std::is_assignable<T &, T>::value, "");
    static void doyathing() {
        setConversion<T, T>();
    }
};

template<typename T, typename H, typename... Rest>
struct PairConvertibility<T, H, Rest...> {
    static void doyathing() {
        setConversion<T, H>();
        setConversion<H, T>();
        PairConvertibility<T, Rest...>::doyathing();
    }
};

#include <string>

void qq() {
    setConversion<std::string, int>();
    setConversion<std::string, char>();
    setConversion<int, double>();
}

void initialize() { Initializer<int, double, float, std::string, char>::doyathing(); }

bool convertible(const Any &a, const Any &b) {
    auto ndx = a.instancePointer->tyi();
    auto fr = the_map.find(ndx);
    if(the_map.end() == fr) { return false; }
    auto fr2 = fr->second.find(b.instancePointer->tyi());
    if(fr->second.end() == fr2) { return false; }
    return true;
}

template<typename T> void convert_any(T &t, const Any &source) {
    Any result;
    result.set<T>(T());
    if(!convertible(result, source)) { return; }
    the_map[result.instancePointer->tyi()][source.instancePointer->tyi()](result, source);
    t = result.as<T>();
}


#include<iostream>

int main(int argc, const char *argv[]) {
    initialize();
    Any str;
    str.set(std::string("hello"));
    Any cha;
    cha.set(char('a'));
    Any iger;
    iger.set(44);

    std::cout << convertible(str, cha) << std::endl;
    std::cout << convertible(cha, str) << std::endl;
    std::cout << convertible(iger, cha) << std::endl;
    std::cout << convertible(str, iger) << std::endl;

    Any doub;
    doub.set<double>(77.888);
    Any flv;
    flv.set<float>(333);
    std::cout << "double <- float " << convertible(flv, doub) << std::endl;
    float asF;
    convert_any(asF, doub);
    std::cout << asF << std::endl;
    int val = 9;
    convert_any(val, doub);
    {
        std::string str;
        str = 55;
        std::cout << str << std::endl;
    }
    std::cout << val << std::endl;
    return 0;
}

static_assert(std::is_assignable<std::string &, char>::value, "");

