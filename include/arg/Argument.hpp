#ifndef SAPI_ARG_ARGUMENT_HPP
#define SAPI_ARG_ARGUMENT_HPP

#include <utility>
#include <pthread.h>
#include <signal.h>

#if !defined __link || defined __macosx
#include <unistd.h>
#include <sched.h>
#endif

#if defined __win32
typedef int pid_t;
#endif

#if defined __link
#include <sos/link.h>
#endif

#if defined __link
typedef void (*signal_function_callback_t)(int);
#if defined __win32
typedef void (*signal_action_callback_t)(int, void*, void*);
#define SIGNAL_SIGINFO_FLAG 0
#else
typedef void (*signal_action_callback_t)(int, siginfo_t*, void*);
typedef signal_function_callback_t _sig_func_ptr;
#define SIGNAL_SIGINFO_FLAG SA_SIGINFO
#endif
#endif


#if defined __StratifyOS__
typedef void (*signal_function_callback_t)(int);
typedef void (*signal_action_callback_t)(int, siginfo_t*, void*);
#define SIGNAL_SIGINFO_FLAG (1<<SA_SIGINFO)
#endif

#if defined __win32
#define sapi_signal_posix_kill raise
typedef u32 sigset_t;
struct sigaction {
	signal_function_callback_t sa_handler;
	signal_action_callback_t sa_sigaction;
	u32 sa_flags;
	u32 sa_mask;
};
union sigval {
	int sival_int;
	void * sival_ptr;
};
#else
#define sapi_signal_posix_kill kill
#endif

#include "../api/ArgumentObject.hpp"

namespace var {
class ConstString;
class DataReference;
class JsonValue;
}

namespace fs {
class File;
}

namespace sys {
class Thread;
class Printer;
}

namespace inet {
class SocketAddress;
}

namespace chrono {
class Seconds;
class Milliseconds;
class Microseconds;
class Nanoseconds;
}

namespace arg {

template <typename T, typename Parameter>
class Argument {
public:
	explicit Argument(T const& value) : m_value(value) {}

	template<typename T_ = T>
	explicit Argument(T&& value,
							typename std::enable_if<!std::is_reference<T_>{},
							std::nullptr_t>::type = nullptr)
		: m_value(std::move(value)) {}

	T& argument() { return m_value; }
	T const& argument() const {return m_value; }
private:
	T m_value;
};


template <typename T, typename Parameter, typename A>
class ImplicitArgument {
public:
	ImplicitArgument(T const& value) : m_value(value) {}
	ImplicitArgument(A const& value) : m_value(value.argument()) {}

	template<typename T_ = T>
	ImplicitArgument(T&& value,
						  typename std::enable_if<!std::is_reference<T_>{},
						  std::nullptr_t>::type = nullptr)
		: m_value(std::move(value)) {}

	T& argument() { return m_value; }
	T const& argument() const {return m_value; }
private:
	T m_value;
};

#define ARG_DEFINE_IMPLICIT_ARGUMENT(name, type, implicit_of) \
	using name = ImplicitArgument<type, struct name ## Tag, implicit_of>

typedef Argument< const var::ConstString &, struct HttpStringToPostTag > HttpStringToPost;
typedef Argument< const var::ConstString &, struct UrlEncodedStringTag > UrlEncodedString;
typedef Argument< const var::ConstString &, struct VersionEncodedStringTag > VersionEncodedString;
typedef Argument< const var::ConstString &, struct GitHashTag > GitHash;
typedef Argument< const var::ConstString &, struct SourcePathTag > SourcePath;
typedef Argument< const var::ConstString &, struct DestinationPathTag > DestinationPath;
typedef Argument< const var::ConstString &, struct DirectoryPathTag > DirectoryPath;
typedef Argument< const var::ConstString &, struct FilePathTag > FilePath;
typedef Argument< const var::ConstString &, struct PathTag > Path;
typedef Argument< const var::ConstString &, struct SourceFilePathTag > SourceFilePath;
typedef Argument< const var::ConstString &, struct DestinationFilePathTag > DestinationFilePath;
typedef Argument< const var::ConstString &, struct SourceDirectoryPathTag > SourceDirectoryPath;
typedef Argument< const var::ConstString &, struct DestinationDirectoryPathTag > DestinationDirectoryPath;
typedef Argument< const var::ConstString &, struct FileNameTag > FileName;
typedef Argument< const var::ConstString &, struct NameTag > Name;
typedef Argument< const var::ConstString &, struct SourceFileNameTag > SourceFileName;
typedef Argument< const var::ConstString &, struct DestinationFileNameTag > DestinationFileName;
typedef Argument< const var::ConstString &, struct TokenEncodedStringTag > TokenEncodedString;
typedef Argument< const var::ConstString &, struct TokenDelimetersTag > TokenDelimeters;
typedef Argument< const var::ConstString &, struct IgnoreTokensBetweenTag > IgnoreTokensBetween;
typedef Argument< const var::ConstString &, struct StringToAssignTag > StringToAssign;
typedef Argument< const var::ConstString &, struct StringToAppendTag > StringToAppend;
typedef Argument< const var::ConstString &, struct StringToCompareTag > StringToCompare;
typedef Argument< const var::ConstString &, struct StringToFindTag > StringToFind;
typedef Argument< const var::ConstString &, struct StringToEraseTag > StringToErase;
typedef Argument< const var::ConstString &, struct StringToInsertTag > StringToInsert;
typedef Argument< const var::ConstString &, struct JsonKeyTag > JsonKey;
typedef Argument< const var::ConstString &, struct JsonEncodedStringTag > JsonEncodedString;
typedef Argument< const var::ConstString &, struct Base64EncodedStringTag > Base64EncodedString;
typedef Argument< const var::ConstString &, struct FontNameTag > FontName;
typedef Argument< const var::ConstString &, struct OptionNameTag > OptionName;
typedef Argument< const var::ConstString &, struct OptionDescriptionTag > OptionDescription;
typedef Argument< const var::ConstString &, struct NodeToFetchTag > NodeToFetch;
typedef Argument< const var::ConstString &, struct ServiceToFetchTag > ServiceToFetch;

typedef Argument< char, struct CharacterToFindTag > CharacterToFind;
typedef Argument< char, struct CharacterToAssignTag > CharacterToAssign;

typedef Argument< int, struct LocationTag > Location;
typedef Argument< int, struct GroupIdTag > GroupId;
typedef Argument< int, struct OwnerIdTag > OwnerId;
typedef Argument< int, struct FileDescriptorTag > FileDescriptor;
typedef Argument< int, struct IoRequestTag > IoRequest;
typedef Argument< int, struct IoIntArgumentTag > IoIntArgument;
typedef Argument< int, struct NumberBaseTag > NumberBase;
typedef Argument< int, struct SchedulerPriorityTag > SchedulerPriority;
typedef Argument< int, struct SignalNumberTag > SignalNumber;
typedef Argument< int, struct SignalValueIntegerTag > SignalValueInteger;
typedef Argument< int, struct SignalCodeTag > SignalCode;
typedef Argument< int, struct KernelRequestTag > KernelRequest;
typedef Argument< int, struct ListenBacklogCountTag > ListenBacklogCount;
typedef Argument< int, struct CurrentProgressTag > CurrentProgress;
typedef Argument< int, struct TotalProgressTag > TotalProgress;
typedef Argument< int, struct SignalFlagsTag> SignalFlags;

typedef Argument< u32, struct PageSizeTag > PageSize;
typedef Argument< u32, struct SizeTag > Size;
typedef Argument< u32, struct CapacityTag > Capacity;
typedef Argument< u32, struct HardwareIdTag > HardwareId;
typedef Argument< u32, struct ThreadStackSizeTag > ThreadStackSize;
typedef Argument< u32, struct MaximumTokenCountTag > MaximumTokenCount;
typedef Argument< u32, struct PositionTag > Position;
typedef Argument< u32, struct SubPositionTag > SubPosition;
typedef Argument< u32, struct LengthTag > Length;
typedef Argument< u32, struct SubLengthTag > SubLength;
typedef Argument< u32, struct MatchLengthTag > MatchLength;
typedef Argument< u32, struct CountTag > Count;
typedef Argument< u32, struct WidthTag > Width;
typedef Argument< u32, struct HeightTag > Height;
typedef Argument< u32, struct BootloaderRetryCountTag > BootloaderRetryCount;
typedef Argument< u32, struct RetryCountTag > RetryCount;
typedef Argument< u32, struct PortTag > Port;
typedef Argument< u32, struct PortNumberTag > PortNumber;
typedef Argument< u32, struct PinNumberTag > PinNumber;
typedef Argument< u32, struct PinFunctionTag > PinFunction;
typedef Argument< u32, struct ChannelTag > Channel;
typedef Argument< u32, struct ValueTag > Value;
typedef Argument< u32, struct EventsTag > Events;
typedef Argument< u32, struct KernelRequestApiTag > KernelRequestApi;


typedef Argument< s32, struct XValueTag > XValue;
typedef Argument< s32, struct YValueTag > YValue;
typedef Argument< u16, struct BitsPerPixelTag > BitsPerPixel;
typedef Argument< u16, struct PlaneCountTag > PlaneCount;

typedef Argument< const void*, struct SourceBufferTag > SourceBuffer;
typedef Argument< const void*, struct ReadOnlyBufferTag > ReadOnlyBuffer;
typedef Argument< const void*, struct IoConstArgumentTag > IoConstArgument;

typedef Argument< void*, struct DestinationBufferTag > DestinationBuffer;
typedef Argument< void*, struct ReadWriteBufferTag > ReadWriteBuffer;
typedef Argument< void*, struct IoArgumentTag > IoArgument;
typedef Argument< void*, struct ContextTag > Context;
typedef Argument< void*, struct ThreadFunctionArgumentTag > ThreadFunctionArgument;
typedef Argument< void*, struct SignalValuePointerTag > SignalValuePointer;


typedef Argument< const var::DataReference &, struct SourceDataTag > SourceData;
typedef Argument< var::DataReference &, struct DestinationDataTag > DestinationData;

typedef Argument< bool, struct IsOverwriteTag > IsOverwrite;
typedef Argument< bool, struct IsDetachedTag > IsDetached;
typedef Argument< bool, struct IsCountEmptyTokensTag > IsCountEmptyTokens;
typedef Argument< bool, struct IsReadOnlyTag > IsReadOnly;
typedef Argument< bool, struct IsResizeTag > IsResize;
typedef Argument< bool, struct IsRecursiveTag > IsRecursive;
typedef Argument< bool, struct IsVerifyTag > IsVerify;
typedef Argument< bool, struct IsLegacyTag > IsLegacy;
typedef Argument< bool, struct IsMcuPinMaskTag > IsMcuPinMask;
typedef Argument< bool, struct IsPinActiveHighTag > IsPinActiveHigh;
typedef Argument< bool, struct IsPersistentTag > IsPersistent;
typedef Argument< bool, struct IsExactMatchTag > IsExactMatch;

typedef Argument< const fs::File&, struct SourceFileTag > SourceFile;
typedef Argument<	fs::File&, struct DestinationFileTag > DestinationFile;

typedef Argument< pid_t, struct PidTag > Pid;
typedef Argument< pthread_t, struct ThreadIdTag > ThreadId;
typedef Argument< const sys::Thread&, struct ThreadToJoinTag > ThreadToJoin;
typedef Argument< signal_function_callback_t, struct SignalFunctionTag > SignalFunction;
typedef Argument< signal_action_callback_t, struct SignalActionFunctionTag > SignalActionFunction;
typedef Argument< sigset_t, struct SignalMaskTag > SignalMask;
typedef Argument< void * (*)(void *), struct ThreadFunctionTag > ThreadFunction;
typedef Argument< void **, struct ThreadReturnTag > ThreadReturn;

typedef Argument< const chrono::Microseconds &, struct DelayIntervalTag > DelayInterval;
typedef Argument< const chrono::Microseconds &, struct ButtonHeldThresholdTag > ButtonHeldThreshold;
typedef Argument< const chrono::Microseconds &, struct ButtonActuatedThresholdTag > ButtonActuatedThreshold;
typedef Argument< const chrono::Microseconds &, struct ActiveDurationTag > ActiveDuration;
typedef Argument< const chrono::Microseconds &, struct InActiveDurationTag > InActiveDuration;
typedef Argument< const chrono::Microseconds &, struct PeriodTag > Period;
typedef Argument< const chrono::Microseconds &, struct DurationTag > Duration;



//var::Json

typedef Argument< const var::JsonValue&, struct SourceJsonValueTag > SourceJsonValue;
typedef Argument< var::JsonValue&, struct DestinationJsonValueTag > DestinationJsonValue;


typedef Argument< sys::Printer &, struct ProgressPrinterTag > ProgressPrinter;

typedef Argument< const chrono::Microseconds &, struct RetryDelayTag > RetryDelay;

typedef Argument< const mcu_pin_t &, struct McuPinTag > McuPin;
typedef Argument< s8, struct InterruptPriorityTag > InterruptPriority;
typedef Argument< const mcu_callback_t &, struct McuCallbackTag > McuCallback;
typedef Argument< u8, struct FontPointSizeTag > FontPointSize;
typedef Argument< u8, struct FontStyleTag > FontStyle;
typedef Argument< const inet::SocketAddress&, struct SourceSocketAddressTag > SourceSocketAddress;
typedef Argument< inet::SocketAddress&, struct DestinationSocketAddressTag > DestinationSocketAddress;

typedef Argument< u8, struct RedColorComponentTag > RedColorComponent;
typedef Argument< u8, struct GreenColorComponentTag > GreenColorComponent;
typedef Argument< u8, struct BlueColorComponentTag > BlueColorComponent;
typedef Argument< u8, struct HueColorComponentTag > HueColorComponent;
typedef Argument< u8, struct ValueColorComponentTag > ValueColorComponent;
typedef Argument< u8, struct SaturationColorComponentTag > SaturationColorComponent;
typedef Argument< u8, struct AlphaColorComponentTag > AlphaColorComponent;
typedef Argument< u8, struct BrightnessColorComponentTag > BrightnessColorComponent;


#if defined __link
typedef Argument< link_transport_mdriver_t*, struct LinkDriverTag > LinkDriver;
typedef Argument< link_transport_mdriver_t*, struct SourceLinkDriverTag > SourceLinkDriver;
typedef Argument< link_transport_mdriver_t*, struct DestinationLinkDriverTag > DestinationLinkDriver;
typedef Argument< int, struct LinkDirpTag > LinkDirp;
typedef Argument< bool , struct IsCopyToDeviceTag > IsCopyToDevice;
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitLinkDriver, link_transport_mdriver_t*, LinkDriver);

#endif

#if defined __link
typedef Argument< int, struct DeviceThreadIdTag > DeviceThreadId;
#else
typedef ThreadId DeviceThreadId;
#endif

/*! \cond */
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceFilePath, const var::ConstString &, SourceFilePath);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitFilePath, const var::ConstString &, FilePath);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitBase64EncodedString, const var::ConstString&, Base64EncodedString);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSize, u32, Size);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitLocation, const int &, Location);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitValue, const int &, Value);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitCapacity, u32, Capacity);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceBuffer, const void*, SourceBuffer);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitDestinationBuffer,	void*, DestinationBuffer);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceData, const var::DataReference &, SourceData);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceFile, const fs::File&, SourceFile);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitDestinationFile,	fs::File&, DestinationFile);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitLength, u32, Length);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitCount, u32, Count);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceJsonValue, const var::JsonValue&, SourceJsonValue);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitDestinationJsonValue, var::JsonValue&, DestinationJsonValue);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitPortNumber, u32, PortNumber);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitMcuPin, const mcu_pin_t &, McuPin);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitPosition, u32, Position);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitMcuCallback, const mcu_callback_t &, McuCallback);
/*! \endcond */


}


#endif // SAPI_ARG_ARGUMENT_HPP
