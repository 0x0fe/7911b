.section .sys.version, "ax"

.global lib_echocloud_version

lib_echocloud_version:

#ifdef CONFIG_RELEASE_ENABLE

#else

.asciz "ECHOCLOUD-*modified*--@20220916-$d1adac0"
#endif
