#include "unity/unity.hpp"

class BasePlayer : public UnityEngine::Component {
public:
};

bool __stdcall DllMain(HINSTANCE dll, DWORD reason, void* reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(dll);

        AllocConsole();
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

        auto metadata_handle = il2cpp::structs::GlobalMetadataHeader::get();
        if (metadata_handle->sanity != 0xFAB11BAF) {
            printf("metadata sanity fail\n");
        }

        auto metadata_registration = il2cpp::structs::MetadataRegistration::get();

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

                        auto namespace_index = base_player_type_definition->namespaceIndex;
                        auto name_index = base_player_type_definition->nameIndex;

                        auto name_space = metadata_handle->GetStringFromIndex(namespace_index);
                        auto name = metadata_handle->GetStringFromIndex(name_index);

                        printf("index: %i::%i\n", namespace_index, name_index);
                        printf("string: %s::%s\n", name_space, name);
                    }
                }

                printf(
                    "size: %u, static fields size: %u, method count: %u, field count: %u\n\n",
                    base_player->klass->instance_size,
                    base_player->klass->static_fields_size,
                    base_player->klass->method_count,
                    base_player->klass->field_count);

                auto end = base_player->klass->field_count;
                for (auto index = 0; index < end; index++) {
                    auto type_def = base_player->klass->typeMetadataHandle;
                    auto field_def = metadata_handle->GetFieldDefinitionFromIndex(type_def->fieldStart + index);

                    auto type = metadata_registration->GetIl2CppTypeFromIndex(field_def->typeIndex);
                    auto name = metadata_handle->GetStringFromIndex(field_def->nameIndex);
                    auto offset = metadata_registration->GetFieldOffsetFromIndex(metadata_handle, base_player->klass, index);
                    auto token = field_def->token;

                    if (auto type_class = il2cpp::class_from_type(type)) {
                        printf("[%i] %s::%s %s => 0x%X, %u\n", index, type_class->namespaze, type_class->name, name, offset, token);
                    } else if (auto def = type->get_definition()) {
                        auto full_name = def->get_full_name(metadata_handle);
                        printf("[%i] %s::%s %s => 0x%X, %u\n", index, full_name.first, full_name.second, name, offset, token);
                    } else {
                        printf("[%i] UNK_TYPE %s => 0x%X, %u\n", index, name, offset, token);
                    }
                }

                /*auto fields = base_player->klass->get_fields();
                if (fields.size() > 0) {
                    for (auto i = 0; i < fields.size(); i++) {
                        auto field = fields.at(i);

                        if (auto type_class = il2cpp::class_from_type(field.type)) {
                            printf("[%i] %s => 0x%X | %s::%s\n", i, field.name, field.offset, type_class->namespaze, type_class->name);
                        } else if (auto def = field.type->get_definition()) {
                            auto full_name = def->get_full_name(metadata_handle);
                            printf("[%i] %s => 0x%X | %s::%s\n", i, field.name, field.offset, full_name.first, full_name.second);
                        } else {
                            printf("[%i] %s => 0x%X | UNK_TYPE\n", i, field.name, field.offset);
                        }
                    }
                }*/
            }
        }

    } else if (reason == DLL_PROCESS_DETACH) {
        fclose(stdout);
        FreeConsole();
    }

    return true;
}