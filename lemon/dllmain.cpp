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

		if (auto icall = il2cpp::resolve_icall<UnityEngine::GameObject* (*)(il2cpp::structs::String)>(
				"UnityEngine.GameObject::Find(System.String)")) {
			auto local = icall(il2cpp::structs::String(L"LocalPlayer"));
			if (local) {
				printf("icall: %s = 0x%llX\n",
					local->get_scripting_game_object()->m_MonoReference.m_Object.m_Target->klass->name,
					reinterpret_cast<uintptr_t>(local));
			}
		}

		auto metadata_registration = il2cpp::structs::MetadataRegistration::get();

		auto local_player = UnityEngine::GameObject::Find(il2cpp::structs::String(L"LocalPlayer"));

		if (local_player) {
			printf("%s: 0x%llX\n",
				local_player->get_scripting_game_object()->m_MonoReference.m_Object.m_Target->klass->name,
				reinterpret_cast<std::uintptr_t>(local_player));

			auto base_player = local_player->get_scripting_game_object()->find_component_by_type<BasePlayer*>();

			if (base_player) {
				printf("%s: 0x%llX\n", base_player->klass->name, reinterpret_cast<std::uintptr_t>(base_player->klass));

				auto base_player_type = base_player->klass->get_type();

				if (base_player_type) {
					printf("%s type: %s @ 0x%llX\n", base_player->klass->name,
						magic_enum::enum_name(base_player_type->type).data(),
						reinterpret_cast<std::uintptr_t>(base_player_type));

					auto base_player_type_definition = base_player_type->get_definition();

					if (base_player_type_definition) {
						printf("%s token: %u\n", base_player->klass->name, base_player_type_definition->token);

						auto namespace_index = base_player_type_definition->namespaceIndex;
						auto name_index = base_player_type_definition->nameIndex;

						auto bp_namespace = metadata_handle->GetStringFromIndex(namespace_index);
						auto bp_name = metadata_handle->GetStringFromIndex(name_index);

						printf("index: %i::%i\n", namespace_index, name_index);
						printf("string: %s::%s\n", bp_namespace, bp_name);

						printf(
							"size: %u, static fields size: %u, method count: %u, "
							"field count: %u\n",
							base_player->klass->instance_size, base_player->klass->thread_static_fields_size,
							base_player->klass->method_count, base_player->klass->field_count);

						if (auto type_def = base_player->klass->typeMetadataHandle) {
							printf("\n------ FIELDS ------\n");

							auto fields_end = type_def->field_count;
							for (auto index = 0; index < fields_end; index++) {
								auto field_def =
									metadata_handle->GetFieldDefinitionFromIndex(type_def->fieldStart + index);

								auto type = metadata_registration->GetIl2CppTypeFromIndex(field_def->typeIndex);
								auto name = metadata_handle->GetStringFromIndex(field_def->nameIndex);
								auto offset = metadata_registration->GetFieldOffsetFromIndex(
									metadata_handle, base_player->klass, index);

								if (auto type_class = il2cpp::class_from_type(type)) {
									printf("[%i] %s.%s %s.%s::%s => 0x%X\n", index, type_class->namespaze,
										type_class->name, bp_namespace, bp_name, name, offset);
								}
								else if (auto def = type->get_definition()) {
									auto full_name = def->get_full_name(metadata_handle);
									printf("[%i] %s.%s %s.%s::%s => 0x%X\n", index, full_name.first, full_name.second,
										bp_namespace, bp_name, name, offset);
								}
								else {
									printf("[%i] UNK_TYPE %s.%s::%s => 0x%X\n", index, bp_namespace, bp_name, name,
										offset);
								}
							}

							printf("\n------ METHODS ------\n");

							auto method_def = type_def->method_count;
							for (auto index = 0; index < method_def; index++) {
								auto method_def =
									metadata_handle->GetMethodDefinitionFromIndex(type_def->methodStart + index);
								auto param_count = method_def->parameterCount;

								std::string typed_string;

								struct param_t {
									const il2cpp::structs::Type* type;
									const char* name;
								};

								auto params = std::vector<param_t> {};
								for (auto i = 0; i < param_count; i++) {
									auto param = metadata_handle->GetParameterDefinitionFromIndex(
										method_def->parameterStart + i);
									auto param_type = metadata_registration->GetIl2CppTypeFromIndex(param->typeIndex);
									auto param_name = metadata_handle->GetStringFromIndex(param->nameIndex);

									params.push_back(param_t {param_type, param_name});
								}

								auto ret_type = metadata_registration->GetIl2CppTypeFromIndex(method_def->returnType);
								auto name = metadata_handle->GetStringFromIndex(method_def->nameIndex);

								if (auto type_class = il2cpp::class_from_type(ret_type)) {
									printf("[%i] %s.%s %s.%s::%s(", index, type_class->namespaze, type_class->name,
										bp_namespace, bp_name, name);

									for (auto i = 0; i < param_count; i++) {
										auto param = params.at(i);

										if (param_count > 0) {
											if (auto param_type_class = il2cpp::class_from_type(param.type)) {
												if (param_count == 1 || i + 1 == param_count) {
													typed_string = std::format("{}{}.{} {}", typed_string,
														param_type_class->namespaze, param_type_class->name,
														param.name);
												}
												else {
													typed_string = std::format("{}{}.{} {}, ", typed_string,
														param_type_class->namespaze, param_type_class->name,
														param.name);
												}
											}
											else if (auto param_type_def = param.type->get_definition()) {
												auto full_name = param_type_def->get_full_name(metadata_handle);

												if (param_count == 1 || i + 1 == param_count) {
													typed_string = std::format("{}{}.{} {}", typed_string,
														full_name.first, full_name.second, param.name);
												}
												else {
													typed_string = std::format("{}{}.{} {}, ", typed_string,
														full_name.first, full_name.second, param.name);
												}
											}
											else {
												if (param_count == 1 || i + 1 == param_count) {
													typed_string =
														std::format("{}UNK_TYPE {}", typed_string, param.name);
												}
												else {
													typed_string =
														std::format("{}UNK_TYPE {}, ", typed_string, param.name);
												}
											}
										}
									}

									if (auto method_ptr =
											il2cpp::get_method_pointer(base_player->klass->image, method_def->token)) {
										auto base_addr =
											reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
										printf("%s) => 0x%llX\n", typed_string.c_str(),
											reinterpret_cast<std::uintptr_t>(method_ptr) - base_addr);
									}
									else {
										printf("%s) => ???\n", typed_string.c_str());
									}
								}
								else if (auto type_type_def = ret_type->get_definition()) {
									auto full_name = type_type_def->get_full_name(metadata_handle);

									printf("[%i] %s.%s %s.%s::%s(", index, full_name.first, full_name.second,
										bp_namespace, bp_name, name);

									for (auto i = 0; i < param_count; i++) {
										auto param = params.at(i);

										if (param_count > 0) {
											if (auto param_type_class = il2cpp::class_from_type(param.type)) {
												if (param_count == 1 || i + 1 == param_count) {
													typed_string = std::format("{}{}.{} {}", typed_string,
														param_type_class->namespaze, param_type_class->name,
														param.name);
												}
												else {
													typed_string = std::format("{}{}.{} {}, ", typed_string,
														param_type_class->namespaze, param_type_class->name,
														param.name);
												}
											}
											else if (auto param_type_def = param.type->get_definition()) {
												auto full_name = param_type_def->get_full_name(metadata_handle);

												if (param_count == 1 || i + 1 == param_count) {
													typed_string = std::format("{}{}.{} {}", typed_string,
														full_name.first, full_name.second, param.name);
												}
												else {
													typed_string = std::format("{}{}.{} {}, ", typed_string,
														full_name.first, full_name.second, param.name);
												}
											}
											else {
												if (param_count == 1 || i + 1 == param_count) {
													typed_string =
														std::format("{}UNK_TYPE {}", typed_string, param.name);
												}
												else {
													typed_string =
														std::format("{}UNK_TYPE {}, ", typed_string, param.name);
												}
											}
										}
									}

									if (auto method_ptr =
											il2cpp::get_method_pointer(base_player->klass->image, method_def->token)) {
										auto base_addr =
											reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
										printf("%s) => 0x%llX\n", typed_string.c_str(),
											reinterpret_cast<std::uintptr_t>(method_ptr) - base_addr);
									}
									else {
										printf("%s) => ???\n", typed_string.c_str());
									}
								}
								else {
									printf("[%i] UNK_TYPE %s.%s::%s(", index, bp_namespace, bp_name, name);

									for (auto i = 0; i < param_count; i++) {
										auto param = params.at(i);

										if (param_count > 0) {
											if (auto param_type_class = il2cpp::class_from_type(param.type)) {
												if (param_count == 1 || i + 1 == param_count) {
													typed_string = std::format("{}{}.{} {}", typed_string,
														param_type_class->namespaze, param_type_class->name,
														param.name);
												}
												else {
													typed_string = std::format("{}{}.{} {}, ", typed_string,
														param_type_class->namespaze, param_type_class->name,
														param.name);
												}
											}
											else if (auto param_type_def = param.type->get_definition()) {
												auto full_name = param_type_def->get_full_name(metadata_handle);

												if (param_count == 1 || i + 1 == param_count) {
													typed_string = std::format("{}{}.{} {}", typed_string,
														full_name.first, full_name.second, param.name);
												}
												else {
													typed_string = std::format("{}{}.{} {}, ", typed_string,
														full_name.first, full_name.second, param.name);
												}
											}
											else {
												if (param_count == 1 || i + 1 == param_count) {
													typed_string =
														std::format("{}UNK_TYPE {}", typed_string, param.name);
												}
												else {
													typed_string =
														std::format("{}UNK_TYPE {}, ", typed_string, param.name);
												}
											}
										}
									}

									if (auto method_ptr =
											il2cpp::get_method_pointer(base_player->klass->image, method_def->token)) {
										auto base_addr =
											reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
										printf("%s) => 0x%llX\n", typed_string.c_str(),
											reinterpret_cast<std::uintptr_t>(method_ptr) - base_addr);
									}
									else {
										printf("%s) => ???\n", typed_string.c_str());
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
		fclose(stdout);
		FreeConsole();
	}

	return true;
}