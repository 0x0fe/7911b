.section .sys.version, "ax"

.global lib_uip_version

lib_uip_version:

#ifdef CONFIG_RELEASE_ENABLE

#else

.asciz "UIP-*modified*--@20220916-$d1adac0"
#endif
