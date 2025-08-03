#pragma once

template<typename T>
struct GlobalInstance {
    static T* instance;
};

template<typename T>
T* GlobalInstance<T>::instance = nullptr;
