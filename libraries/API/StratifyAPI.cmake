

if(SOS_IS_ARM)
	include(targets/StratifyAPI_arm)
else()
	include(targets/StratifyAPI_link)
endif()
