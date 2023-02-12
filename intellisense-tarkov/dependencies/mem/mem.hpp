#pragma once

#include <Windows.h>

#include <vector>

#include "../lazy_importer/lazy_importer.hpp"

namespace mem
{
	int cstrcmp(const char* a, const char* b);

	int cstrlen(const char* str);

    char* cstrcat(char* dest, const char* source);

    inline size_t cstrcpy(char* dst, const char* src, size_t dstsize)
    {
        size_t	i;

        i = 0;
        if (!dst || !src)
            return (0);
        if (dstsize == 0)
            return (cstrlen(src));
        while ((i < dstsize - 1) && src[i])
        {
            dst[i] = src[i];
            i++;
        }
        dst[i] = '\0';
        return (cstrlen(src));
    }

    inline void* cmemcpy(void* dst, const void* src, size_t n)
    {
        size_t	i;

        i = 0;
        if (!src && !dst)
            return (NULL);
        while (i < n)
        {
            ((char*)dst)[i] = ((char*)src)[i];
            i++;
        }
        return (dst);
    }

    inline void* cmemmove(void* dst, const void* src, size_t len)
    {
        if (dst > src)
        {
            while (len > 0)
            {
                ((char*)dst)[len - 1] = ((char*)src)[len - 1];
                len--;
            }
        }
        else
            cmemcpy(dst, src, len);
        return (dst);
    }

    template<typename... t>
    void cprintf(const char* const fmt, t... args)
    {
        char addr[55];
		wsprintf(addr, fmt, args...);

		OutputDebugString(addr);
    }

    template <class T>
    struct custom_allocator_t
    {
        typedef T value_type;

        custom_allocator_t() = default;
        template <class U> constexpr custom_allocator_t(const custom_allocator_t <U>&) noexcept {}

        T* allocate(size_t n) {
            if (auto p = static_cast<T*>(LI_FN(malloc)( n * sizeof(T)))) {
                return p;
            }

            return nullptr;
        }

        void deallocate(T* p, size_t n) noexcept {
            LI_FN(free)(p);
        }
    };

    template <class T, class U>
    bool operator==(const custom_allocator_t <T>&, const custom_allocator_t <U>&) { return true; }
    template <class T, class U>
    bool operator!=(const custom_allocator_t <T>&, const custom_allocator_t <U>&) { return false; }

    template<typename t>
    using vector = std::vector<t, custom_allocator_t<t>>;

    inline std::uintptr_t hook(const std::uintptr_t HookAddress, const std::uintptr_t CaveAddress)
    {
        constexpr std::uint8_t shellcode[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };

        auto clone = reinterpret_cast<std::uintptr_t>(LI_FN(VirtualAlloc)(nullptr, 15, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)); //CHANGE THIS TO EXECUTABLE MEM IN MODULE

        mem::cmemcpy((void*)clone, (void*)HookAddress, 15);

        for (int x = 0; x < sizeof(shellcode); x++)
            *reinterpret_cast<std::uint8_t*>(clone + 15 + x) = shellcode[x];

        *reinterpret_cast<std::uintptr_t*>(clone + 15 + 6) = HookAddress + 15;

        DWORD old_protect;

        LI_FN(VirtualProtect)(reinterpret_cast<void*>(HookAddress), 14, 0x40, &old_protect);

        for (int x = 0; x < sizeof(shellcode); x++)
            *reinterpret_cast<std::uint8_t*>(HookAddress + x) = shellcode[x];

        *reinterpret_cast<std::uintptr_t*>(HookAddress + 6) = CaveAddress;

        LI_FN(VirtualProtect)(reinterpret_cast<void*>(HookAddress), 14, old_protect, &old_protect);

        return clone;
    }
}
