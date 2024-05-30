#pragma once

#include "../utils/utils.hpp"

namespace il2cpp::structs {
	enum TypeEnum {
		IL2CPP_TYPE_END = 0x00, /* End of List */
		IL2CPP_TYPE_VOID = 0x01,
		IL2CPP_TYPE_BOOLEAN = 0x02,
		IL2CPP_TYPE_CHAR = 0x03,
		IL2CPP_TYPE_I1 = 0x04,
		IL2CPP_TYPE_U1 = 0x05,
		IL2CPP_TYPE_I2 = 0x06,
		IL2CPP_TYPE_U2 = 0x07,
		IL2CPP_TYPE_I4 = 0x08,
		IL2CPP_TYPE_U4 = 0x09,
		IL2CPP_TYPE_I8 = 0x0a,
		IL2CPP_TYPE_U8 = 0x0b,
		IL2CPP_TYPE_R4 = 0x0c,
		IL2CPP_TYPE_R8 = 0x0d,
		IL2CPP_TYPE_STRING = 0x0e,
		IL2CPP_TYPE_PTR = 0x0f,         /* arg: <type> token */
		IL2CPP_TYPE_BYREF = 0x10,       /* arg: <type> token */
		IL2CPP_TYPE_VALUETYPE = 0x11,   /* arg: <type> token */
		IL2CPP_TYPE_CLASS = 0x12,       /* arg: <type> token */
		IL2CPP_TYPE_VAR = 0x13,         /* Generic parameter in a generic type definition, represented as
		                                   number (compressed unsigned integer) number */
		IL2CPP_TYPE_ARRAY = 0x14,       /* type, rank, boundsCount, bound1, loCount, lo1 */
		IL2CPP_TYPE_GENERICINST = 0x15, /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
		IL2CPP_TYPE_TYPEDBYREF = 0x16,
		IL2CPP_TYPE_I = 0x18,
		IL2CPP_TYPE_U = 0x19,
		IL2CPP_TYPE_FNPTR = 0x1b, /* arg: full method signature */
		IL2CPP_TYPE_OBJECT = 0x1c,
		IL2CPP_TYPE_SZARRAY = 0x1d,   /* 0-based one-dim-array */
		IL2CPP_TYPE_MVAR = 0x1e,      /* Generic parameter in a generic method definition, represented as
		                                 number (compressed unsigned integer)  */
		IL2CPP_TYPE_CMOD_REQD = 0x1f, /* arg: typedef or typeref token */
		IL2CPP_TYPE_CMOD_OPT = 0x20,  /* optional arg: typedef or typref token */
		IL2CPP_TYPE_INTERNAL = 0x21,  /* CLR internal type */

		IL2CPP_TYPE_MODIFIER = 0x40, /* Or with the following types */
		IL2CPP_TYPE_SENTINEL = 0x41, /* Sentinel for varargs method signature */
		IL2CPP_TYPE_PINNED = 0x45,   /* Local var that points to pinned object */

		IL2CPP_TYPE_ENUM = 0x55,             /* an enumeration */
		IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xff /* an index into IL2CPP type metadata table */
	};

	struct Assembly;
	struct Domain;
	struct Image;

	struct Class;
	typedef Class VTable;

	struct Type;

	struct MethodInfo;

	struct GenericClass;

	struct Object {
		union {
			Class* klass;
			VTable* vtable;
		};
		void* monitor;
	};

#define IL2CPP_ARRAY_MAX_INDEX ((int32_t)0x7fffffff)
#define IL2CPP_ARRAY_MAX_SIZE  ((uint32_t)0xffffffff)

	struct ArrayBounds {
		uintptr_t length;
		int32_t lower_bound;
	};

	struct Array : public Object {
		ArrayBounds* bounds;
		uintptr_t max_length;
	};

	struct ArraySize : public Array {
		alignas(8) void* vector[65565];
	};

	struct String {
		Object object;
		int32_t length;
		wchar_t chars[128 + 1];

		String(const wchar_t* str) {
			this->length = min((int)wcslen(str), 128);
			for (size_t i = 0; i < this->length; i++)
				this->chars[i] = str[i];

			this->chars[this->length] = 0;
		}
	};

	struct MarshalByRefObject {
		Object obj;
		Object* identity;
	};

	struct AppDomain {
		MarshalByRefObject mbr;
		Domain* data;
	};

	struct AppDomainSetup {
		Object object;
		String* application_base;
		String* application_name;
		String* cache_path;
		String* configuration_file;
		String* dynamic_base;
		String* license_file;
		String* private_bin_path;
		String* private_bin_path_probe;
		String* shadow_copy_directories;
		String* shadow_copy_files;
		uint8_t publisher_policy;
		uint8_t path_changed;
		int loader_optimization;
		uint8_t disallow_binding_redirects;
		uint8_t disallow_code_downloads;
		Object* activation_arguments; /* it is System.Object in 1.x, ActivationArguments in 2.0 */
		Object* domain_initializer;
		Object* application_trust; /* it is System.Object in 1.x, ApplicationTrust in 2.0 */
		Array* domain_initializer_args;
		uint8_t disallow_appbase_probe;
		Array* configuration_bytes;
		Array* serialized_non_primitives;
	};

	struct AppContext {
		Object obj;
		int32_t domain_id;
		int32_t context_id;
		void* static_data;
	};

	struct Domain {
		AppDomain* domain;
		AppDomainSetup* setup;
		AppContext* default_context;
		Object* ephemeron_tombstone;
		const char* friendly_name;
		uint32_t domain_id;

		volatile int threadpool_jobs;
		void* agent_info;

		static Domain* get() {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			return *reinterpret_cast<Domain**>(module_base + 0x3CF1D60);  // S_domain
		}
	};

	struct TypeDefinition;

	struct FieldDefinition {
		int32_t nameIndex;
		int32_t typeIndex;
		uint32_t token;
	};

	struct MethodDefinition {
		int32_t nameIndex;
		int32_t declaringType;
		int32_t returnType;
		int32_t parameterStart;
		int32_t genericContainerIndex;
		uint32_t token;
		uint16_t flags;
		uint16_t iflags;
		uint16_t slot;
		uint16_t parameterCount;

		static MethodInfo* GetMethodInfoFromMethodDefinitionIndex(int32_t index) {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			auto ret = *reinterpret_cast<MethodInfo***>(module_base + 0x3CF1D90);  // s_MethodInfoDefinitionTable
			return ret[index];
		}
	};

	struct ParameterDefinition {
		int32_t nameIndex;
		uint32_t token;
		int32_t typeIndex;
	};

	struct GlobalMetadata {
		static void* get() {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			return *reinterpret_cast<void**>(module_base + 0x3CF1FA8);  // s_GlobalMetadata
		}
	};

#pragma pack(push, p1, 4)
	struct GlobalMetadataHeader {
		int32_t sanity;
		int32_t version;
		int32_t stringLiteralOffset;  // string data for managed code
		int32_t stringLiteralSize;
		int32_t stringLiteralDataOffset;
		int32_t stringLiteralDataSize;
		int32_t stringOffset;  // string data for metadata
		int32_t stringSize;
		int32_t eventsOffset;  // Il2CppEventDefinition
		int32_t eventsSize;
		int32_t propertiesOffset;  // Il2CppPropertyDefinition
		int32_t propertiesSize;
		int32_t methodsOffset;  // Il2CppMethodDefinition
		int32_t methodsSize;
		int32_t parameterDefaultValuesOffset;  // Il2CppParameterDefaultValue
		int32_t parameterDefaultValuesSize;
		int32_t fieldDefaultValuesOffset;  // Il2CppFieldDefaultValue
		int32_t fieldDefaultValuesSize;
		int32_t fieldAndParameterDefaultValueDataOffset;  // uint8_t
		int32_t fieldAndParameterDefaultValueDataSize;
		int32_t fieldMarshaledSizesOffset;  // Il2CppFieldMarshaledSize
		int32_t fieldMarshaledSizesSize;
		int32_t parametersOffset;  // Il2CppParameterDefinition
		int32_t parametersSize;
		int32_t fieldsOffset;  // Il2CppFieldDefinition
		int32_t fieldsSize;
		int32_t genericParametersOffset;  // Il2CppGenericParameter
		int32_t genericParametersSize;
		int32_t genericParameterConstraintsOffset;  // TypeIndex
		int32_t genericParameterConstraintsSize;
		int32_t genericContainersOffset;  // Il2CppGenericContainer
		int32_t genericContainersSize;
		int32_t nestedTypesOffset;  // TypeDefinitionIndex
		int32_t nestedTypesSize;
		int32_t interfacesOffset;  // TypeIndex
		int32_t interfacesSize;
		int32_t vtableMethodsOffset;  // EncodedMethodIndex
		int32_t vtableMethodsSize;
		int32_t interfaceOffsetsOffset;  // Il2CppInterfaceOffsetPair
		int32_t interfaceOffsetsSize;
		int32_t typeDefinitionsOffset;  // Il2CppTypeDefinition
		int32_t typeDefinitionsSize;
		int32_t imagesOffset;  // Il2CppImageDefinition
		int32_t imagesSize;
		int32_t assembliesOffset;  // Il2CppAssemblyDefinition
		int32_t assembliesSize;
		int32_t fieldRefsOffset;  // Il2CppFieldRef
		int32_t fieldRefsSize;
		int32_t referencedAssembliesOffset;  // int32_t
		int32_t referencedAssembliesSize;
		int32_t attributeDataOffset;
		int32_t attributeDataSize;
		int32_t attributeDataRangeOffset;
		int32_t attributeDataRangeSize;
		int32_t unresolvedVirtualCallParameterTypesOffset;  // TypeIndex
		int32_t unresolvedVirtualCallParameterTypesSize;
		int32_t unresolvedVirtualCallParameterRangesOffset;  // Il2CppMetadataRange
		int32_t unresolvedVirtualCallParameterRangesSize;
		int32_t windowsRuntimeTypeNamesOffset;  // Il2CppWindowsRuntimeTypeNamePair
		int32_t windowsRuntimeTypeNamesSize;
		int32_t windowsRuntimeStringsOffset;  // const char*
		int32_t windowsRuntimeStringsSize;
		int32_t exportedTypeDefinitionsOffset;  // TypeDefinitionIndex
		int32_t exportedTypeDefinitionsSize;

		static GlobalMetadataHeader* get() {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			return *reinterpret_cast<GlobalMetadataHeader**>(module_base + 0x3CF1FB0);  // s_GlobalMetadataHeader
		}

		int32_t GetIndexForTypeDefinition(const Class* klass);

		template <typename T>
		T MetadataOffset(const void* metadata, size_t sectionOffset, size_t itemIndex) {
			return reinterpret_cast<T>(reinterpret_cast<uint8_t*>(const_cast<void*>(metadata)) + sectionOffset)
				+ itemIndex;
		}

		const char* GetStringFromIndex(int32_t index) {
			if (index > this->stringSize)
				return {};

			return MetadataOffset<const char*>(GlobalMetadata::get(), this->stringOffset, index);
		}

		const FieldDefinition* GetFieldDefinitionFromIndex(int32_t index) {
			if (index > this->fieldsSize)
				return {};

			return MetadataOffset<FieldDefinition*>(GlobalMetadata::get(), this->fieldsOffset, index);
		}

		const MethodDefinition* GetMethodDefinitionFromIndex(int32_t index) {
			if (index > this->methodsSize)
				return {};

			return MetadataOffset<MethodDefinition*>(GlobalMetadata::get(), this->methodsOffset, index);
		}

		const ParameterDefinition* GetParameterDefinitionFromIndex(int32_t index) {
			if (index > this->parametersSize)
				return {};

			return MetadataOffset<ParameterDefinition*>(GlobalMetadata::get(), this->parametersOffset, index);
		}
	};
#pragma pack(pop, p1)

	struct TypeDefinition {
		int32_t nameIndex;
		int32_t namespaceIndex;
		int32_t byvalTypeIn;
		int32_t declaringTypeIndex;
		int32_t parentIndex;
		int32_t elementTypeIndex;
		int32_t genericContainerIndex;

		uint32_t flags;

		int32_t fieldStart;
		int32_t methodStart;
		int32_t eventStart;
		int32_t propertyStart;
		int32_t nestedTypesStart;
		int32_t interfacesStart;
		int32_t vtableStart;
		int32_t interfaceOffsetsStart;

		uint16_t method_count;
		uint16_t property_count;
		uint16_t field_count;
		uint16_t event_count;
		uint16_t nested_type_count;
		uint16_t vtable_count;
		uint16_t interfaces_count;
		uint16_t interface_offsets_count;

		// bitfield to portably encode boolean values as single bits
		// 01 - valuetype;
		// 02 - enumtype;
		// 03 - has_finalize;
		// 04 - has_cctor;
		// 05 - is_blittable;
		// 06 - is_import_or_windows_runtime;
		// 07-10 - One of nine possible PackingSize values (0, 1, 2, 4, 8, 16, 32, 64, or 128)
		// 11 - PackingSize is default
		// 12 - ClassSize is default
		// 13-16 - One of nine possible PackingSize values (0, 1, 2, 4, 8, 16, 32, 64, or 128) - the
		// specified packing size (even for explicit layouts)
		uint32_t bitfield;
		uint32_t token;

		std::pair<const char*, const char*> get_full_name(GlobalMetadataHeader* metadata) {
			auto name_space = metadata->GetStringFromIndex(this->namespaceIndex);
			auto name = metadata->GetStringFromIndex(this->nameIndex);

			return {name_space, name};
		}
	};

	struct ArrayType {
		const Type* etype;
		uint8_t rank;
		uint8_t numsizes;
		uint8_t numlobounds;
		int* sizes;
		int* lobounds;
	};

	struct ReflectionType {
		Object object;
		const Type* type;
	};

	struct Type {
		union {
			void* dummy;
			int32_t __klassIndex;            /* for VALUETYPE and CLASS at startup */
			void* typeHandle;                /* for VALUETYPE and CLASS at runtime */
			const Type* type;                /* for PTR and SZARRAY */
			ArrayType* array;                /* for ARRAY */
			int32_t __genericParameterIndex; /* for VAR and MVAR at startup */
			void* genericParameterHandle;    /* for VAR and MVAR at runtime */
			GenericClass* generic_class;     /* for GENERICINST */
		} data;

		unsigned int attrs : 16;
		TypeEnum type : 8;
		unsigned int num_mods : 5;
		unsigned int byref : 1;
		unsigned int pinned : 1;
		unsigned int valuetype : 1;

		TypeDefinition* get_definition() const {
			if (this->type == IL2CPP_TYPE_CLASS || this->type == IL2CPP_TYPE_VALUETYPE) {
				return reinterpret_cast<TypeDefinition*>(this->data.typeHandle);
			}

			return {};
		}
	};

	struct GenericInst {
		uint32_t type_argc;
		const Type** type_argv;
	};

	struct GenericContext {
		/* The instantiation corresponding to the class generic parameters */
		const GenericInst* class_inst;
		/* The instantiation corresponding to the method generic parameters */
		const GenericInst* method_inst;
	};

	struct GenericClass {
		const Type* type;       /* the generic type definition */
		GenericContext context; /* a context that contains the type instantiation doesn't contain
		                           any method instantiation */
		Class* cached_class;    /* if present, the Class corresponding to the instantiation.  */
	};

	struct GenericMethodIndices {
		int32_t methodIndex;
		int32_t invokerIndex;
		int32_t adjustorThunkIndex;
	};

	struct GenericMethodFunctionsDefinitions {
		int32_t genericMethodIndex;
		GenericMethodIndices indices;
	};

	struct MethodSpec {
		int32_t methodDefinitionIndex;
		int32_t classIndexIndex;
		int32_t methodIndexIndex;
	};

	struct TypeDefinitionSizes {
		uint32_t instance_size;
		int32_t native_size;
		uint32_t static_fields_size;
		uint32_t thread_static_fields_size;
	};

	struct MetadataRegistration {
		int32_t genericClassesCount;
		GenericClass* const* genericClasses;
		int32_t genericInstsCount;
		const GenericInst* const* genericInsts;
		int32_t genericMethodTableCount;
		const GenericMethodFunctionsDefinitions* genericMethodTable;
		int32_t typesCount;
		const Type* const* types;
		int32_t methodSpecsCount;
		const MethodSpec* methodSpecs;

		int32_t fieldOffsetsCount;
		const int32_t** fieldOffsets;

		int32_t typeDefinitionsSizesCount;
		const TypeDefinitionSizes** typeDefinitionsSizes;
		const size_t metadataUsagesCount;
		void** const* metadataUsages;

		static MetadataRegistration* get() {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			return *reinterpret_cast<MetadataRegistration**>(module_base + 0x3CF1FC0);  // s_Il2CppMetadataRegistration
		}

		const Type* GetIl2CppTypeFromIndex(int32_t index) {
			if (index == -1)
				return {};

			if (index > this->typesCount)
				return {};

			return this->types[index];
		}

		int32_t GetFieldOffsetFromIndex(GlobalMetadataHeader* metadata, structs::Class* klass, int32_t index) {
			uint32_t type_index = metadata->GetIndexForTypeDefinition(klass);
			int32_t offset = this->fieldOffsets[type_index][index];

			if (offset < 0)
				return -1;

			return offset;
		}
	};

	struct FieldInfo {
		const char* name;
		const Type* type;
		Class* parent;
		int32_t offset;
		uint32_t token;
	};

	typedef void (*MethodPointer)();
	typedef void (*InvokerMethod)(MethodPointer, const MethodInfo*, void*, void**, void*);

	struct MethodInfo {
		MethodPointer methodPointer;
		MethodPointer virtualMethodPointer;
		InvokerMethod invoker_method;
		const char* name;
		Class* klass;
		const Type* return_type;
		const Type** parameters;

		union {
			const void* rgctx_data;
			void* methodMetadataHandle;
		};

		union {
			const void* genericMethod;
			void* genericContainerHandle;
		};

		uint32_t token;
		uint16_t flags;
		uint16_t iflags;
		uint16_t slot;
		uint8_t parameters_count;
		uint8_t is_generic : 1;
		uint8_t is_inflated : 1;
		uint8_t wrapper_type : 1;
		uint8_t has_full_generic_sharing_signature : 1;
		uint8_t indirect_call_via_invokers : 1;

		template <typename def>
		def get_function_pointer() const {
			return reinterpret_cast<def>(this->methodPointer);
		}
	};

	struct PropertyInfo {
		Class* parent;
		const char* name;
		const MethodInfo* get;
		const MethodInfo* set;
		uint32_t attrs;
		uint32_t token;
	};

	struct EventInfo {
		const char* name;
		const Type* eventType;
		Class* parent;
		const MethodInfo* add;
		const MethodInfo* remove;
		const MethodInfo* raise;
		uint32_t token;
	};

	struct RuntimeInterfaceOffsetPair {
		Class* interfaceType;
		int32_t offset;
	};

	struct VirtualInvokeData {
		MethodPointer methodPtr;
		const MethodInfo* method;
	};

	struct Class {
		const Image* image;
		void* gc_desc;
		const char* name;
		const char* namespaze;
		Type byval_arg;
		Type this_arg;
		Class* element_class;
		Class* castClass;
		Class* declaringType;
		Class* parent;
		GenericClass* generic_class;
		TypeDefinition* typeMetadataHandle;
		const void* interopData;
		Class* klass;

		FieldInfo* fields;
		const EventInfo* events;
		const PropertyInfo* properties;
		const MethodInfo** methods;
		Class** nestedTypes;
		Class** implementedInterfaces;
		RuntimeInterfaceOffsetPair* interfaceOffsets;
		void* static_fields;
		void* rgctx_data;
		Class** typeHierarchy;

		void* unity_user_data;

		uint32_t initializationExceptionGCHandle;

		uint32_t cctor_started;
		uint32_t cctor_finished_or_no_cctor;
		alignas(8) size_t cctor_thread;

		void* genericContainerHandle;
		uint32_t instance_size;
		uint32_t actualSize;
		uint32_t element_size;
		int32_t native_size;
		uint32_t static_fields_size;
		uint32_t thread_static_fields_size;
		int32_t thread_static_fields_offset;
		uint32_t flags;
		uint32_t token;

		uint16_t method_count;
		uint16_t property_count;
		uint16_t field_count;
		uint16_t event_count;
		uint16_t nested_type_count;
		uint16_t vtable_count;
		uint16_t interfaces_count;
		uint16_t interface_offsets_count;

		uint8_t typeHierarchyDepth;
		uint8_t genericRecursionDepth;
		uint8_t rank;
		uint8_t minimumAlignment;
		uint8_t naturalAligment;
		uint8_t packingSize;

		uint8_t initialized_and_no_error : 1;
		uint8_t initialized : 1;
		uint8_t enumtype : 1;
		uint8_t nullabletype : 1;
		uint8_t is_generic : 1;
		uint8_t has_references : 1;
		uint8_t init_pending : 1;
		uint8_t size_init_pending : 1;
		uint8_t size_inited : 1;
		uint8_t has_finalize : 1;
		uint8_t has_cctor : 1;
		uint8_t is_blittable : 1;
		uint8_t is_import_or_windows_runtime : 1;
		uint8_t is_vtable_initialized : 1;
		uint8_t is_byref_like : 1;
		VirtualInvokeData vtable[65565];

		struct FieldInfoArrayContainer {
			FieldInfo arr[65565];
		};

		FieldInfo get_field_from_name(const std::string name) {
			for (auto klass = this; klass; klass = klass->parent) {
				auto arr = reinterpret_cast<FieldInfoArrayContainer*>(klass->fields);

				for (auto i = 0u; i < klass->field_count; i++) {
					auto field = arr->arr[i];
					if (!field.token || field.token == -1)
						return {};

					if (strcmp(field.name, name.c_str()) == 0)
						return field;
				}
			}

			return {};
		}

		std::vector<FieldInfo> get_fields() {
			std::vector<FieldInfo> ret {};

			for (auto klass = this; klass; klass = klass->parent) {
				auto arr = reinterpret_cast<FieldInfoArrayContainer*>(klass->fields);

				for (auto i = 0u; i < klass->field_count; i++) {
					auto field = arr->arr[i];
					if (!field.token || field.token == -1)
						continue;

					ret.push_back(field);
				}
			}

			std::ranges::sort(ret, [](const FieldInfo& a, const FieldInfo& b) { return a.offset < b.offset; });

			return ret;
		}

		const MethodInfo* get_method_from_name(const std::string name, std::int32_t parameter_count) {
			for (auto klass = this; klass; klass = klass->parent) {
				for (auto i = 0u; i < klass->method_count; i++) {
					auto method = klass->methods[i];
					if (!method || method->token == -1)
						return nullptr;

					if (strcmp(method->name, name.c_str()) == 0 && method->parameters_count == parameter_count)
						return method;
				}
			}

			return nullptr;
		}

		std::vector<const MethodInfo*> get_methods() {
			std::vector<const MethodInfo*> ret {};

			for (auto klass = this; klass; klass = klass->parent) {
				for (auto i = 0u; i < klass->method_count; i++) {
					auto method = klass->methods[i];
					if (!method || method->token == -1)
						continue;

					auto name = method->name;

					printf("found method: %s\n", method->name);

					ret.push_back(method);
				}
			}

			return ret;
		}

		Type* get_type() { return &this->byval_arg; }
	};

	struct AssemblyName {
		const char* name;
		const char* culture;
		const uint8_t* public_key;
		uint32_t hash_alg;
		int32_t hash_len;
		uint32_t flags;
		int32_t major;
		int32_t minor;
		int32_t build;
		int32_t revision;
		uint8_t public_key_token[8];
	};

	struct ImageGlobalMetadata {
		int32_t typeStart;
		int32_t exportedTypeStart;
		int32_t customAttributeStart;
		int32_t entryPointIndex;
		const Image* image;
	};

	struct CodeGenModule {
		const char* moduleName;
		const uint32_t methodPointerCount;
		const MethodPointer* methodPointers;
		const uint32_t adjustorThunkCount;
		const void* adjustorThunks;
		const int32_t* invokerIndices;
		const uint32_t reversePInvokeWrapperCount;
		const void* reversePInvokeWrapperIndices;
		const uint32_t rgctxRangesCount;
		const void* rgctxRanges;
		const uint32_t rgctxsCount;
		const void* rgctxs;
		const void* debuggerMetadata;
		const MethodPointer moduleInitializer;
		int32_t* staticConstructorTypeIndices;
		const MetadataRegistration* metadataRegistration;  // Per-assembly mode only
		const void* codeRegistaration;                     // Per-assembly mode only
	};

	struct Image {
		const char* name;
		const char* nameNoExt;
		Assembly* assembly;

		uint32_t typeCount;
		uint32_t exportedTypeCount;
		uint32_t customAttributeCount;

		char pad[4];

		ImageGlobalMetadata* metadataHandle;
		mutable void* nameToClassHashTable;
		const CodeGenModule* codeGenModule;

		uint32_t token;
		uint8_t dynamic;

		Class* get_class_by_name(const std::string namespace_name, const std::string class_name) {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			auto type_info_def_table =
				*reinterpret_cast<Class***>(module_base + 0x3CF1DA8);  // s_TypeInfoDefinitionTable

			for (auto i = 0u; i < this->typeCount; i++) {
				auto type_def_idx = this->metadataHandle->typeStart + (int)i;

				auto type = type_info_def_table[type_def_idx];
				if (!type)
					return nullptr;

				if (strcmp(type->namespaze, namespace_name.c_str()) == 0 && strcmp(type->name, class_name.c_str()) == 0)
					return type;
			}

			return nullptr;
		}
	};

	struct Assembly {
		Image* image;
		uint32_t token;
		int32_t referencedAssemblyStart;
		int32_t referencedAssemblyCount;
		AssemblyName name;

		static Image* get_image_by_name(std::string name) {
			auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
			auto all_assemblies = reinterpret_cast<std::vector<Assembly*>*>(module_base + 0x3CF2C28);  // s_Assemblies

			for (auto i = 0; i < all_assemblies->size(); i++) {
				auto assembly = all_assemblies->at(i);
				if (!assembly)
					return nullptr;

				auto image = assembly->image;
				if (!image)
					return nullptr;

				if (strcmp(image->name, name.c_str()) == 0)
					return image;
				else if (strcmp(image->nameNoExt, name.c_str()) == 0)
					return image;
			}

			return nullptr;
		}
	};

	struct Defaults {
		Image* corlib;
		Image* corlib_gen;
		Class* object_class;
		Class* byte_class;
		Class* void_class;
		Class* boolean_class;
		Class* sbyte_class;
		Class* int16_class;
		Class* uint16_class;
		Class* int32_class;
		Class* uint32_class;
		Class* int_class;
		Class* uint_class;
		Class* int64_class;
		Class* uint64_class;
		Class* single_class;
		Class* double_class;
		Class* char_class;
		Class* string_class;
		Class* enum_class;
		Class* array_class;
		Class* delegate_class;
		Class* multicastdelegate_class;
		Class* asyncresult_class;
		Class* manualresetevent_class;
		Class* typehandle_class;
		Class* fieldhandle_class;
		Class* methodhandle_class;
		Class* systemtype_class;
		Class* monotype_class;
		Class* exception_class;
		Class* threadabortexception_class;
		Class* thread_class;
		Class* internal_thread_class;
		/*Class *transparent_proxy_class;
		Class *real_proxy_class;
		Class *mono_method_message_class;*/
		Class* appdomain_class;
		Class* appdomain_setup_class;
		Class* member_info_class;
		Class* field_info_class;
		Class* method_info_class;
		Class* property_info_class;
		Class* event_info_class;
		Class* stringbuilder_class;
		/*Class *math_class;*/
		Class* stack_frame_class;
		Class* stack_trace_class;
		Class* marshal_class;
		/*Class *iserializeable_class;
		Class *serializationinfo_class;
		Class *streamingcontext_class;*/
		Class* typed_reference_class;
		/*Class *argumenthandle_class;*/
		Class* marshalbyrefobject_class;
		/*Class *monitor_class;
		Class *iremotingtypeinfo_class;
		Class *runtimesecurityframe_class;
		Class *executioncontext_class;
		Class *internals_visible_class;*/
		Class* generic_ilist_class;
		Class* generic_icollection_class;
		Class* generic_ienumerable_class;
		Class* generic_ireadonlylist_class;
		Class* generic_ireadonlycollection_class;
		Class* runtimetype_class;
		Class* generic_nullable_class;
		/*Class *variant_class;
		Class *com_object_class;*/
		Class* il2cpp_com_object_class;
		/*Class *com_interop_proxy_class;
		Class *iunknown_class;
		Class *idispatch_class;
		Class *safehandle_class;
		Class *handleref_class;*/
		Class* attribute_class;
		Class* customattribute_data_class;
		Class* customattribute_typed_argument_class;
		Class* customattribute_named_argument_class;
		// Class *critical_finalizer_object;
		Class* version;
		Class* culture_info;
		Class* async_call_class;
		Class* assembly_class;
		Class* assembly_name_class;
		Class* parameter_info_class;
		Class* module_class;
		Class* system_exception_class;
		Class* argument_exception_class;
		Class* wait_handle_class;
		Class* safe_handle_class;
		Class* sort_key_class;
		Class* dbnull_class;
		Class* error_wrapper_class;
		Class* missing_class;
		Class* value_type_class;

		// Stuff used by the mono code
		Class* threadpool_wait_callback_class;
		MethodInfo* threadpool_perform_wait_callback_method;
		Class* mono_method_message_class;

		// Windows.Foundation.IReference`1<T>
		Class* ireference_class;
		// Windows.Foundation.IReferenceArray`1<T>
		Class* ireferencearray_class;
		// Windows.Foundation.Collections.IKeyValuePair`2<K, V>
		Class* ikey_value_pair_class;
		// System.Collections.Generic.KeyValuePair`2<K, V>
		Class* key_value_pair_class;
		// Windows.Foundation.Uri
		Class* windows_foundation_uri_class;
		// Windows.Foundation.IUriRuntimeClass
		Class* windows_foundation_iuri_runtime_class_class;
		// System.Uri
		Class* system_uri_class;
		// System.Guid
		Class* system_guid_class;

		Class* sbyte_shared_enum;
		Class* int16_shared_enum;
		Class* int32_shared_enum;
		Class* int64_shared_enum;

		Class* byte_shared_enum;
		Class* uint16_shared_enum;
		Class* uint32_shared_enum;
		Class* uint64_shared_enum;
		Class* il2cpp_fully_shared_type;
		Class* il2cpp_fully_shared_struct_type;
	};

	int32_t GlobalMetadataHeader::GetIndexForTypeDefinition(const Class* klass) {
		const TypeDefinition* type_def = reinterpret_cast<const TypeDefinition*>(klass->typeMetadataHandle);

		const TypeDefinition* type_defs =
			(const TypeDefinition*)((const char*)GlobalMetadata::get() + this->typeDefinitionsOffset);

		ptrdiff_t index = type_def - type_defs;

		return static_cast<int32_t>(index);
	}

	struct ScriptingObjectPtr {
		Object* m_Target;
	};

	struct ScriptingGCHandle {
		uint64_t m_Handle;
		int32_t m_Weakness;
		char pad[4];
		ScriptingObjectPtr m_Object;
	};

	struct ScriptingObject {
		void* __vftable;
		int m_InstanceID;
		uint32_t m_MemLabelIdentifier : 12;
		uint32_t m_TemporaryFlags : 1;
		uint32_t m_HideFlags : 7;
		uint32_t m_IsPersistent : 1;
		uint32_t m_CachedTypeIndex : 11;
		void* m_EventIndex;
		ScriptingGCHandle m_MonoReference;
	};

	template <typename T>
		requires std::is_pointer_v<T>
	struct dynamic_array {
		T m_data;
		int32_t m_label;
		char pad[4];
		uint64_t m_size;
		uint64_t m_capacity;
	};

	struct GameObject;

	struct Component : ScriptingObject {
		GameObject* m_GameObject;

		template <typename T>
			requires std::is_pointer_v<T>
		T get_typed_target() {
			return reinterpret_cast<T>(this->m_MonoReference.m_Object.m_Target);
		}
	};

	struct ComponentPair {
		uint32_t typeIndex;
		char pad[4];
		Component* component;
	};

	class ComponentPairContainer {
	public:
		ComponentPair arr[65565];  // dynamic_array->m_capacity
	};

	template <typename T>
		requires std::is_pointer_v<T>
	struct ListNode {
		ListNode<T>* m_Prev;
		ListNode<T>* m_Next;
		T m_Data;
	};

	struct GameObject : ScriptingObject {
		dynamic_array<ComponentPairContainer*> m_Components;
		uint32_t m_Layer;
		uint16_t m_Tag;
		bool m_IsActive;
		char m_IsActiveCached;
		int32_t m_ActivationState;
		uint32_t m_SupportedMessages;
		const char* m_Name;
		ListNode<GameObject*> m_ActiveGONode;

		template <typename T>
			requires std::is_pointer_v<T>
		T get_typed_target() {
			return reinterpret_cast<T>(this->m_MonoReference.m_Object.m_Target);
		}

		template <typename T>
		T find_component_by_class_name(std::string name = utils::split_typename<std::remove_pointer_t<T>>().second) {
			for (auto i = 0; i < this->m_Components.m_size; i++) {
				auto component = this->m_Components.m_data->arr[i].component;

				auto component_name = component->m_MonoReference.m_Object.m_Target->klass->name;

				if (strcmp(component_name, name.c_str()) == 0)
					return component->get_typed_target<T>();
			}

			return nullptr;
		}

		template <typename T>
		T find_component_by_type() {
			auto [name_space, name] = utils::split_typename<std::remove_pointer_t<T>>();
			auto klass = Assembly::get_image_by_name("Assembly-CSharp")->get_class_by_name(name_space, name);

			for (auto i = 0; i < this->m_Components.m_size; i++) {
				auto component = this->m_Components.m_data->arr[i].component;

				auto component_type = component->m_MonoReference.m_Object.m_Target->klass->get_type();
				auto component_token = component_type->get_definition()->token;

				if (component_token == klass->get_type()->get_definition()->token)
					return component->get_typed_target<T>();
			}

			return nullptr;
		}
	};

}  // namespace il2cpp::structs