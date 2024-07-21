# Lemon 🍋

A proof of concept importless way to modify Il2Cpp Unity games.

> [!WARNING]
> This code is messy.

## How to get static addresses:

### S_domain:
- Open `GameAssembly.dll` in IDA.
- Find the `il2cpp_domain_get` export.
- Follow control flow until you find a function like this:
- ![domain](/imgs/domain.png)
- Get the address of that static variable.

### s_Assemblies
- Open `GameAssembly.dll` in IDA.
- Find the `il2cpp_domain_get_assemblies` export.
- Follow the control flow until you find a function like this:
- ![assemblies](/imgs/assemblies.png)
- Get the address of that static variable.

### s_TypeInfoDefinitionTable
- Open `GameAssembly.dll` in IDA.
- Search for the string `global-metadata.dat`.
- Open the function that contains said string.
- Find the static variable with this math:
- ![typeinfo](/imgs/typeinfo.png)
- Get the address of that static variable.

### s_GlobalMetadata
- Open `GameAssembly.dll` in IDA.
- Search for the string `global-metadata.dat`.
- Open the function that contains said string.
- Find the first static variable.
- ![metadata](/imgs/metadata.png)
- Get the address of that static variable.

### s_GlobalMetadataHeader
- Open `GameAssembly.dll` in IDA.
- Search for the string `global-metadata.dat`.
- Open the function that contains said string.
- Find the second static variable.
- ![metadata_header](/imgs/metadata_header.png)
- Get the address of that static variable.

### s_Il2CppMetadataRegistration
- Open `GameAssembly.dll` in IDA.
- Search for the string `global-metadata.dat`.
- Open the function that contains said string.
- Find the static variable with this math.
- ![metadata_registration](/imgs/metadata_registration.png)
- Get the address of that static variable.

### s_MethodInfoDefinitionTable
- Open `GameAssembly.dll` in IDA.
- Search for the string `global-metadata.dat`.
- Open the function that contains said string.
- Find the static variable with this math.
- ![method_info](/imgs/method_info.png)
- Get the address of that static variable.

### il2cpp_defaults
- Open `GameAssembly.dll` in IDA.
- Search for the string `__Generated`.
- Open the function that contains said string.
- Find the static variable that is used in all these calls.
- ![defaults](/imgs/defaults.png)
- Get the address of that static variable.

### s_InternalCalls
- Open `GameAssembly.dll` in IDA.
- Find the `il2cpp_resolve_icall` export.
- Follow control flow until you find a function like this:
- ![icalls](/imgs/icalls.png)
- Get the address of that static variable.