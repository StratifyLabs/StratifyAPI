

macro(stratify_api_include_arch ARCH)
	include(targets/API_release_${ARCH})
	include(targets/VarAPI_release_${ARCH})
	include(targets/PrinterAPI_release_${ARCH})
	include(targets/ChronoAPI_release_${ARCH})
	include(targets/FsAPI_release_${ARCH})
	include(targets/ThreadAPI_release_${ARCH})
	include(targets/CryptoAPI_release_${ARCH})
	include(targets/SysAPI_release_${ARCH})
	include(targets/HalAPI_release_${ARCH})
	include(targets/TestAPI_release_${ARCH})
	include(targets/InetAPI_release_${ARCH})
endmacro()

set(ARCH_LIST v7m v7em v7em_f4sh v7em_f5sh v7em_f5dh)

foreach(ARCH ARCH_LIST)
stratify_api_include_arch(${ARCH})
endforeach(ARCH)
