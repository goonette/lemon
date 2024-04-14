#pragma once

#include "../il2cpp/il2cpp.hpp"

namespace UnityEngine {
    class Vector2 {
    public:
        float x, y;

        Vector2()
            : x(0.f)
            , y(0.f) {}
        explicit Vector2(float x, float y)
            : x(x)
            , y(y) {}
    };

    class Vector3 {
    public:
        float x, y, z;

        Vector3()
            : x(0.f)
            , y(0.f)
            , z(0.f) {}
        explicit Vector3(float x, float y, float z)
            : x(x)
            , y(y)
            , z(z) {}
    };

    class Vector4 {
    public:
        float x, y, z, w;

        Vector4()
            : x(0.f)
            , y(0.f)
            , z(0.f)
            , w(0.f) {}
        explicit Vector4(float x, float y, float z, float w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) {}
    };

    class Object : public il2cpp::structs::Object {
    public:
        std::uintptr_t m_cachedPtr;
    };

    class Component : public Object {
    public:
        il2cpp::structs::Component* get_scripting_component() { return reinterpret_cast<il2cpp::structs::Component*>(m_cachedPtr); }
    };

    class GameObject : public Object {
    public:
        static GameObject* Find(il2cpp::structs::String name) {
            static const auto find = il2cpp::structs::Assembly::get_image_by_name("UnityEngine.CoreModule")
                                         ->get_class_by_name("UnityEngine", "GameObject")
                                         ->get_method_from_name(__func__, 1)
                                         ->get_function_pointer<GameObject* (*)(il2cpp::structs::String)>();
            return find(name);
        }

        il2cpp::structs::GameObject* get_scripting_game_object() { return reinterpret_cast<il2cpp::structs::GameObject*>(m_cachedPtr); }
    };
}    // namespace UnityEngine