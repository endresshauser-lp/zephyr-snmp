zephyr_include_directories(include)

if (CONFIG_LIB_SNMP)
zephyr_library()
zephyr_library_sources(
  src/pbuf.c
  src/snmp_asn1.c
  src/snmp_asn1.h
  src/snmp_core.c
  src/snmp_core_priv.h
  src/snmp_msg.c
  src/snmp_msg.h
  src/snmp_pbuf_stream.c
  src/snmp_pbuf_stream.h
  src/snmp_scalar.c
  src/snmp_table.c
  src/snmp_traps.c
  src/snmp_zephyr.c
)

endif (CONFIG_LIB_SNMP)
