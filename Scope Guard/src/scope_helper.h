#ifndef SCOPE_HELPER_H
#define SCOPE_HELPER_H

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define AUTOGEN_NAME3(prefix, line) \
    AUTOGEN__ ## prefix ## __ ## line

#define AUTOGEN_NAME2(prefix, line) \
    AUTOGEN_NAME3(prefix, line)

#define AUTOGEN_NAME(prefix) \
    AUTOGEN_NAME2(prefix, __LINE__)

#endif
