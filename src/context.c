#include <lwcgl/context.h>

static int major = 2;
static int minor = 1;
static int profile = LWCGL_CONTEXT_ANY_PROFILE;
static int debug;

static void normalize_context_request(void)
{
#ifdef __APPLE__
    /*
     * Apple exposes the legacy OpenGL 2.1 compatibility context and core
     * contexts starting at 3.2, capped at 4.1. GLFW cannot create a 3.2+
     * compatibility context on macOS, and requesting 4.2/4.3 fails outright.
     * Normalize impossible requests to the closest usable Apple context so
     * Display.create() remains portable; feature users must query the actual
     * context version after creation.
     */
    const int newer_than_21 = major > 2 || (major == 2 && minor > 1);
    const int at_least_32 = major > 3 || (major == 3 && minor >= 2);

    if (profile == LWCGL_CONTEXT_COMPATIBILITY_PROFILE && newer_than_21) {
        major = 2;
        minor = 1;
        profile = LWCGL_CONTEXT_ANY_PROFILE;
        return;
    }

    if (at_least_32) {
        if (major > 4 || (major == 4 && minor > 1)) {
            major = 4;
            minor = 1;
        }
        profile = LWCGL_CONTEXT_CORE_PROFILE;
        return;
    }

    if (newer_than_21) {
        major = 2;
        minor = 1;
        profile = LWCGL_CONTEXT_ANY_PROFILE;
    }
#endif
}

void lwcglSetContextVersion(int M, int m)
{
    if (M < 1 || m < 0) {
        M = 2;
        m = 1;
    }
    major = M;
    minor = m;
    normalize_context_request();
}

void lwcglSetContextProfile(int p)
{
    profile = (p == LWCGL_CONTEXT_CORE_PROFILE || p == LWCGL_CONTEXT_COMPATIBILITY_PROFILE)
        ? p
        : LWCGL_CONTEXT_ANY_PROFILE;
    normalize_context_request();
}

void lwcglSetContextDebug(int v)
{
    debug = v ? 1 : 0;
}

int lwcglRequestedContextMajorVersion(void) { return major; }
int lwcglRequestedContextMinorVersion(void) { return minor; }
int lwcglRequestedContextProfile(void) { return profile; }
int lwcglRequestedContextDebug(void) { return debug; }
