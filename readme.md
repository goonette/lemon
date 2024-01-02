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
- Find the variable with this math:
- ![typeinfo](/imgs/typeinfo.png)
- Get the address of that static variable.
