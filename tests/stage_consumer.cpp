#include <lwcgl/glmodern.h>
#include <lwcgl/lwcgl.h>

#include <cstdint>

int main()
{
    if (Display.abiVersion != LWCGL_ABI_VERSION) return 1;

    const std::uintptr_t tls_addresses =
        reinterpret_cast<std::uintptr_t>(&GL15) ^
        reinterpret_cast<std::uintptr_t>(&GL20) ^
        reinterpret_cast<std::uintptr_t>(&GL30) ^
        reinterpret_cast<std::uintptr_t>(&GL42) ^
        reinterpret_cast<std::uintptr_t>(&GL43) ^
        reinterpret_cast<std::uintptr_t>(&GLModern);

    return tls_addresses == 0 ? 2 : 0;
}
