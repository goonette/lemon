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
        IL2CPP_TYPE_PTR = 0x0f,       /* arg: <type> token */
        IL2CPP_TYPE_BYREF = 0x10,     /* arg: <type> token */
        IL2CPP_TYPE_VALUETYPE = 0x11, /* arg: <type> token */
        IL2CPP_TYPE_CLASS = 0x12,     /* arg: <type> token */
        IL2CPP_TYPE_VAR = 0x13,   /* Generic parameter in a generic type definition, represented as number (compressed
                                     unsigned integer) number */
        IL2CPP_TYPE_ARRAY = 0x14, /* type, rank, boundsCount, bound1, loCount, lo1 */
        IL2CPP_TYPE_GENERICINST = 0x15, /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
        IL2CPP_TYPE_TYPEDBYREF = 0x16,
        IL2CPP_TYPE_I = 0x18,
        IL2CPP_TYPE_U = 0x19,
        IL2CPP_TYPE_FNPTR = 0x1b, /* arg: full method signature */
        IL2CPP_TYPE_OBJECT = 0x1c,
        IL2CPP_TYPE_SZARRAY = 0x1d, /* 0-based one-dim-array */
        IL2CPP_TYPE_MVAR = 0x1e, /* Generic parameter in a generic method definition, represented as number (compressed
                                    unsigned integer)  */
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

    struct Object {
        union {
            Class* klass;
            VTable* vtable;
        };
        void* monitor;
    };

#define IL2CPP_ARRAY_MAX_INDEX ((int32_t)0x7fffffff)
#define IL2CPP_ARRAY_MAX_SIZE ((uint32_t)0xffffffff)

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
            return *reinterpret_cast<Domain**>(module_base + 0x35DE238);    // S_domain
        }
    };

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
        // 13-16 - One of nine possible PackingSize values (0, 1, 2, 4, 8, 16, 32, 64, or 128) - the specified packing
        // size (even for explicit layouts)
        uint32_t bitfield;
        uint32_t token;
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
            void* generic_class;             /* for GENERICINST */
        } data;

        unsigned int attrs    : 16;
        TypeEnum type         : 8;
        unsigned int num_mods : 5;
        unsigned int byref    : 1;
        unsigned int pinned   : 1;
        unsigned int valuetype: 1;

        TypeDefinition* get_definition() const {
            if (this->type == IL2CPP_TYPE_CLASS || this->type == IL2CPP_TYPE_VALUETYPE) {
                return reinterpret_cast<TypeDefinition*>(this->data.typeHandle);
            }

            return {};
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
        uint8_t is_generic                        : 1;
        uint8_t is_inflated                       : 1;
        uint8_t wrapper_type                      : 1;
        uint8_t has_full_generic_sharing_signature: 1;
        uint8_t indirect_call_via_invokers        : 1;

        template<typename def>
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
        void* generic_class;
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

        uint8_t initialized_and_no_error    : 1;
        uint8_t initialized                 : 1;
        uint8_t enumtype                    : 1;
        uint8_t nullabletype                : 1;
        uint8_t is_generic                  : 1;
        uint8_t has_references              : 1;
        uint8_t init_pending                : 1;
        uint8_t size_init_pending           : 1;
        uint8_t size_inited                 : 1;
        uint8_t has_finalize                : 1;
        uint8_t has_cctor                   : 1;
        uint8_t is_blittable                : 1;
        uint8_t is_import_or_windows_runtime: 1;
        uint8_t is_vtable_initialized       : 1;
        uint8_t is_byref_like               : 1;
        VirtualInvokeData vtable[65565];

        struct FieldInfoArrayContainer {
            FieldInfo arr[65565];
        };

        FieldInfo get_field_from_name(const std::string name) const {
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

        std::vector<FieldInfo> get_fields() const {
            std::vector<FieldInfo> ret{};

            for (auto klass = this; klass; klass = klass->parent) {
                auto arr = reinterpret_cast<FieldInfoArrayContainer*>(klass->fields);

                for (auto i = 0u; i < klass->field_count; i++) {
                    auto field = arr->arr[i];
                    if (!field.token || field.token == -1)
                        continue;

                    ret.push_back(field);
                }
            }

            return ret;
        }

        const MethodInfo* get_method_from_name(const std::string name, std::int32_t parameter_count) const {
            for (auto klass = this; klass; klass = klass->parent) {
                for (auto i = 0u; i < klass->method_count; i++) {
                    auto method = klass->methods[i];
                    if (!method)
                        return nullptr;

                    if (strcmp(method->name, name.c_str()) == 0 && method->parameters_count == parameter_count)
                        return method;
                }
            }

            return nullptr;
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
        void* codeGenModule;
        uint32_t token;
        uint8_t dynamic;

        Class* get_class_by_name(const std::string namespace_name, const std::string class_name) const {
            auto module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
            auto type_info_def_table =
                *reinterpret_cast<Class***>(module_base + 0x35DE230);    // s_TypeInfoDefinitionTable

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
            auto all_assemblies = reinterpret_cast<std::vector<Assembly*>*>(module_base + 0x35DDF50);    // s_Assemblies

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
        uint32_t m_MemLabelIdentifier: 12;
        uint32_t m_TemporaryFlags    : 1;
        uint32_t m_HideFlags         : 7;
        uint32_t m_IsPersistent      : 1;
        uint32_t m_CachedTypeIndex   : 11;
        void* m_EventIndex;
        ScriptingGCHandle m_MonoReference;
    };

    template<typename T>
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

        template<typename T>
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
        ComponentPair arr[65565];    // dynamic_array->m_capacity
    };

    template<typename T>
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

        template<typename T>
            requires std::is_pointer_v<T>
        T get_typed_target() {
            return reinterpret_cast<T>(this->m_MonoReference.m_Object.m_Target);
        }

        template<typename T>
        T find_component_by_class_name(std::string name = utils::split_typename<std::remove_pointer_t<T>>().second) {
            for (auto i = 0; i < this->m_Components.m_size; i++) {
                auto component = this->m_Components.m_data->arr[i].component;

                auto component_name = component->m_MonoReference.m_Object.m_Target->klass->name;

                if (strcmp(component_name, name.c_str()) == 0)
                    return component->get_typed_target<T>();
            }

            return nullptr;
        }

        template<typename T>
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

}    // namespace il2cpp::structs