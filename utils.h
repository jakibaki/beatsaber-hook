long getRealOffset(long offset);



#define MAKE_HOOK(name, addr, retval, ...) \
long addr_ ## name = (long) addr; \
retval (*name)(__VA_ARGS__) = NULL; \
retval hook_ ## name(__VA_ARGS__) 

#define INSTALL_HOOK(name) \
registerInlineHook((uint32_t)getRealOffset(addr_ ## name), (uint32_t)hook_ ## name, (uint32_t **)&name);\
inlineHook((uint32_t)getRealOffset(addr_ ## name));\
