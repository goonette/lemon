#include "unity/unity.hpp"

class BasePlayer : public UnityEngine::Component {
public:

};

bool __stdcall DllMain(HINSTANCE dll, DWORD reason, void* reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(dll);

        AllocConsole();
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

        auto local_player = UnityEngine::GameObject::Find(il2cpp::structs::String(L"LocalPlayer"));

        if (local_player) {
            printf(
                "%s: 0x%llX\n",
                local_player->get_scripting_game_object()->m_MonoReference.m_Object.m_Target->klass->name,
                reinterpret_cast<std::uintptr_t>(local_player));

            auto base_player = local_player->get_scripting_game_object()->find_component_by_type<BasePlayer*>();

            if (base_player) {
                printf("%s: 0x%llX\n", base_player->klass->name, reinterpret_cast<std::uintptr_t>(base_player));

                auto base_player_type = base_player->klass->get_type();

                if (base_player_type) {
                    printf(
                        "%s type: %s @ 0x%llX\n",
                        base_player->klass->name,
                        magic_enum::enum_name(base_player_type->type).data(),
                        reinterpret_cast<std::uintptr_t>(base_player_type));

                    auto base_player_type_definition = base_player_type->get_definition();

                    if (base_player_type_definition) {
                        printf("%s token: %u\n", base_player->klass->name, base_player_type_definition->token);
                    }
                }

                printf(
                    "size: %u, static fields size: %u, method count: %u, field count: %u\n",
                    base_player->klass->instance_size,
                    base_player->klass->static_fields_size,
                    base_player->klass->method_count,
                    base_player->klass->field_count);

                auto fields = base_player->klass->get_fields();
                if (fields.size() > 0) {
                    for (auto i = 0; i < fields.size(); i++) {
                        auto field = fields.at(i);
                        printf("%s => 0x%X\n", field.name, field.offset);
                    }
                }
            }
        }

    } else if (reason == DLL_PROCESS_DETACH) {
        fclose(stdout);
        FreeConsole();
    }

    return true;
}