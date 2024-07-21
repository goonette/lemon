#pragma once

#include "structs.hpp"

namespace il2cpp {
	structs::Class* class_from_type_enum(structs::TypeEnum type) {
		auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
		auto defaults = reinterpret_cast<structs::Defaults*>(module_base + 0x573DD50);  // il2cpp_defaults

#define RETURN_DEFAULT_TYPE(field_name) \
	do {                                \
		return defaults->field_name;    \
	} while (false)

		switch (type) {
		case structs::IL2CPP_TYPE_OBJECT:
			RETURN_DEFAULT_TYPE(object_class);
		case structs::IL2CPP_TYPE_VOID:
			RETURN_DEFAULT_TYPE(void_class);
		case structs::IL2CPP_TYPE_BOOLEAN:
			RETURN_DEFAULT_TYPE(boolean_class);
		case structs::IL2CPP_TYPE_CHAR:
			RETURN_DEFAULT_TYPE(char_class);
		case structs::IL2CPP_TYPE_I1:
			RETURN_DEFAULT_TYPE(sbyte_class);
		case structs::IL2CPP_TYPE_U1:
			RETURN_DEFAULT_TYPE(byte_class);
		case structs::IL2CPP_TYPE_I2:
			RETURN_DEFAULT_TYPE(int16_class);
		case structs::IL2CPP_TYPE_U2:
			RETURN_DEFAULT_TYPE(uint16_class);
		case structs::IL2CPP_TYPE_I4:
			RETURN_DEFAULT_TYPE(int32_class);
		case structs::IL2CPP_TYPE_U4:
			RETURN_DEFAULT_TYPE(uint32_class);
		case structs::IL2CPP_TYPE_I:
			RETURN_DEFAULT_TYPE(int_class);
		case structs::IL2CPP_TYPE_U:
			RETURN_DEFAULT_TYPE(uint_class);
		case structs::IL2CPP_TYPE_I8:
			RETURN_DEFAULT_TYPE(int64_class);
		case structs::IL2CPP_TYPE_U8:
			RETURN_DEFAULT_TYPE(uint64_class);
		case structs::IL2CPP_TYPE_R4:
			RETURN_DEFAULT_TYPE(single_class);
		case structs::IL2CPP_TYPE_R8:
			RETURN_DEFAULT_TYPE(double_class);
		case structs::IL2CPP_TYPE_FNPTR:
			RETURN_DEFAULT_TYPE(int_class);
		case structs::IL2CPP_TYPE_STRING:
			RETURN_DEFAULT_TYPE(string_class);
		case structs::IL2CPP_TYPE_TYPEDBYREF:
			RETURN_DEFAULT_TYPE(typed_reference_class);
		case structs::IL2CPP_TYPE_IL2CPP_TYPE_INDEX:
			RETURN_DEFAULT_TYPE(systemtype_class);
		default:
			break;
		}

		return nullptr;

#undef RETURN_DEFAULT_TYPE
	}

	structs::Class* class_from_type(const structs::Type* type) {
		auto default_class = class_from_type_enum(type->type);
		if (default_class != nullptr)
			return default_class;

		// most of these involve recreating Il2CppReaderWriterLockedHashMap/Il2CppHashMap which seems really annoying :3
		switch (type->type) {
		case structs::IL2CPP_TYPE_ARRAY: {
			auto element_class = class_from_type(type->data.array->etype);
			// return Class::GetBoundedArrayClass(elementClass, type->data.array->rank, true);
			return nullptr;
		}
		case structs::IL2CPP_TYPE_SZARRAY: {
			auto element_class = class_from_type(type->data.type);
			// return Class::GetArrayClass(elementClass, 1);
			return nullptr;
		}
		case structs::IL2CPP_TYPE_PTR: {
			// return Class::GetPtrClass(type->data.type);
			return nullptr;
		}
		case structs::IL2CPP_TYPE_GENERICINST: {
			auto cached = type->data.generic_class->cached_class;
			if (cached)
				return cached;
			return nullptr;
		}
		case structs::IL2CPP_TYPE_VAR: {
			// return Class::FromGenericParameter(type->data.genericParameterHandle);
			return nullptr;
		}
		case structs::IL2CPP_TYPE_MVAR: {
			// return Class::FromGenericParameter(type->data.genericParameterHandle);
			return nullptr;
		}
		default:
			break;
		}

		return nullptr;
	}

	typedef std::map<std::string, structs::MethodPointer> ICallMap;

	template <typename def>
	static def resolve_icall(const char* name) {
		auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
		auto map = *reinterpret_cast<ICallMap*>(module_base + 0x573E770);  // s_InternalCalls

		// Try to find the whole name first, then search using just type::method if parameters were passed
		// ex: First, System.Foo::Bar(System.Int32)
		// Then, System.Foo::Bar

		ICallMap::iterator res = map.find(name);

		if (res != map.end())
			return reinterpret_cast<def>(res->second);

		std::string short_name(name);
		size_t index = short_name.find('(');

		if (index != std::string::npos) {
			short_name = short_name.substr(0, index);
			res = map.find(short_name);

			if (res != map.end())
				return reinterpret_cast<def>(res->second);
		}

		return def {};
	}

	structs::MethodPointer get_method_pointer(const structs::Image* image, uint32_t token) {
		uint32_t row_id = token & 0x00FFFFFF;

		return image->codeGenModule->methodPointers[row_id - 1];
	}
}  // namespace il2cpp