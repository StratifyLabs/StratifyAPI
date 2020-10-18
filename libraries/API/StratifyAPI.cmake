


if(SOS_IS_LINK)
	set(CONFIG_LIST release debug coverage)
else()
	set(CONFIG_LIST release debug)
endif()

include(targets/StratifyOS)

sos_sdk_include_target(mbedtls "${CONFIG_LIST}")

sos_sdk_include_target(API "${CONFIG_LIST}")
sos_sdk_include_target(VarAPI "${CONFIG_LIST}")
sos_sdk_include_target(PrinterAPI "${CONFIG_LIST}")
sos_sdk_include_target(SysAPI "${CONFIG_LIST}")
sos_sdk_include_target(ChronoAPI "${CONFIG_LIST}")
sos_sdk_include_target(FsAPI "${CONFIG_LIST}")
sos_sdk_include_target(ThreadAPI "${CONFIG_LIST}")
sos_sdk_include_target(CryptoAPI "${CONFIG_LIST}")
sos_sdk_include_target(TestAPI "${CONFIG_LIST}")
sos_sdk_include_target(InetAPI "${CONFIG_LIST}")
