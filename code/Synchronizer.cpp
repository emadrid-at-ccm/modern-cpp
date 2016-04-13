#include <mutex>
#include <utility>

template<typename R> class Synchronizer {
    struct Bracket {
        Bracket(const Bracket &) = delete;
        Bracket(Bracket &&) = default;

        Bracket(Synchronizer &s): s(s) { s.synchronizer.lock(); }
        ~Bracket() { s.synchronizer.unlock(); }

        R &resource() { return s.resource; }

    private:
        Synchronizer &s;
    };

public:
    template<typename... Pack> Synchronizer(Pack &&... pack):
        resource(std::forward<Pack>(pack)...)
    {}

    Bracket bracket() { return Bracket(*this); }

private:
    R resource;
    std::mutex synchronizer;
};




#include <sstream>

struct SynchronizedIO: std::ostringstream {
    Synchronizer<std::ostream &> &output;

    SynchronizedIO(
        Synchronizer<std::ostream &> &output
    ): output(output) {}

    struct Barrier {};
    constexpr static Barrier barrier{};

    template<typename T> SynchronizedIO &operator<<(const T &v) {
        std::ostringstream &thy = *this;
        thy << v;
        return *this;
    }

    SynchronizedIO &operator<<(Barrier) {
        output.bracket().resource() << str() << std::endl;
            // note: std::endl flushes the stream
        str("");
        clear();
        return *this;
    }
};





void logSynchronized(Synchronizer<std::ostream &> &sio) {
    sio.bracket().resource() << "Hello World, this is 5: " << 5 << std::endl;
}
