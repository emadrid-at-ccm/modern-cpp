/// Uses type erasure to hold a pointer to an instance of an arbitrary type
class Any {
    struct CommonInterface {
        virtual ~CommonInterface() {}
        virtual CommonInterface *clone() { return new CommonInterface; }
        virtual uint16_t size() const { return 0; }
    };

    template<typename T> struct GenericHolder: CommonInterface {
        T value;
        GenericHolder() = default;
        GenericHolder(const T &modelInstance): value(modelInstance) {}

        CommonInterface *clone() { return new GenericHolder<T>(value); }
    };

    CommonInterface *instancePointer;

public:
    Any(): instancePointer(0) {}
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
