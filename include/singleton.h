#pragma once

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }    

    Singleton(const T&) = delete;
    void operator=(const T&) = delete;
    Singleton(T&&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};

