


if(SOS_IS_LINK)
	set(STRATIFYAPI_CONFIG_LIST release debug coverage)
else()
	set(STRATIFYAPI_CONFIG_LIST release debug)
endif()

include(targets/StratifyOS)

sos_sdk_include_target(mbedtls "${STRATIFYAPI_CONFIG_LIST}")

sos_sdk_include_target(API "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(VarAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(PrinterAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(SysAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(ChronoAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(FsAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(ThreadAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(CryptoAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(TestAPI "${STRATIFYAPI_CONFIG_LIST}")
sos_sdk_include_target(InetAPI "${STRATIFYAPI_CONFIG_LIST}")
