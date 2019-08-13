#ifndef SAPI_ARG_ARGUMENT_HPP
#define SAPI_ARG_ARGUMENT_HPP

#include <utility>
#include <pthread.h>
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

#include "../api/ArgumentObject.hpp"

namespace var {
class ConstString;
class Data;
class JsonValue;
}

namespace fs {
class File;
}

namespace sys {
class Thread;
}

namespace inet {
class SocketAddress;
}

namespace chrono {
class MicroTime;
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


#define ARG_DEFINE_ARGUMENT(name, type) \
	using name = Argument<type, struct name ## Parameter>

#define ARG_DEFINE_IMPLICIT_ARGUMENT(name, type, implicit_of) \
	using name = ImplicitArgument<type, struct name ## Parameter, implicit_of>


//fs
ARG_DEFINE_ARGUMENT(Location, int);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitLocation, const int &, Location);
ARG_DEFINE_ARGUMENT(PageSize,	u32);


ARG_DEFINE_ARGUMENT(Size, u32);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSize, u32, Size);

ARG_DEFINE_ARGUMENT(Capacity, u32);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitCapacity, u32, Capacity);

ARG_DEFINE_ARGUMENT(SourceBuffer, const void*);
ARG_DEFINE_ARGUMENT(DestinationBuffer,	void*);

ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceBuffer, const void*, SourceBuffer);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitDestinationBuffer,	void*, DestinationBuffer);

ARG_DEFINE_ARGUMENT(SourceData, const var::Data &);
ARG_DEFINE_ARGUMENT(DestinationData, var::Data &);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceData, const var::Data &, SourceData);


ARG_DEFINE_ARGUMENT(SourcePath, const var::ConstString &);
ARG_DEFINE_ARGUMENT(DestinationPath, const var::ConstString &);

ARG_DEFINE_ARGUMENT(DirectoryPath, const var::ConstString &);
ARG_DEFINE_ARGUMENT(FilePath, const var::ConstString &);
ARG_DEFINE_ARGUMENT(Path, const var::ConstString &);
ARG_DEFINE_ARGUMENT(SourceFilePath, const var::ConstString &);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceFilePath, const var::ConstString &, SourceFilePath);
ARG_DEFINE_ARGUMENT(DestinationFilePath, const var::ConstString &);

ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitFilePath, const var::ConstString &, FilePath);

ARG_DEFINE_ARGUMENT(SourceDirectoryPath, const var::ConstString &);
ARG_DEFINE_ARGUMENT(DestinationDirectoryPath, const var::ConstString &);

ARG_DEFINE_ARGUMENT(FileName, const var::ConstString &);
ARG_DEFINE_ARGUMENT(Name, const var::ConstString &);
ARG_DEFINE_ARGUMENT(SourceFileName, const var::ConstString &);
ARG_DEFINE_ARGUMENT(DestinationFileName, const var::ConstString &);
ARG_DEFINE_ARGUMENT(IsOverwrite, bool);



ARG_DEFINE_ARGUMENT(SourceFile, const fs::File&);
ARG_DEFINE_ARGUMENT(DestinationFile, fs::File&);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceFile, const fs::File&, SourceFile);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitDestinationFile,	fs::File&, DestinationFile);

//sys
ARG_DEFINE_ARGUMENT(Pid, pid_t);
ARG_DEFINE_ARGUMENT(ThreadId, pthread_t);
ARG_DEFINE_ARGUMENT(IsDetached, bool);
ARG_DEFINE_ARGUMENT(ThreadFunction, void * (*)(void *));
ARG_DEFINE_ARGUMENT(ThreadToJoin, const sys::Thread&);
ARG_DEFINE_ARGUMENT(ThreadFunctionArgument, void *);
ARG_DEFINE_ARGUMENT(ThreadStackSize, u32);

ARG_DEFINE_ARGUMENT(ThreadReturn, void **);
ARG_DEFINE_ARGUMENT(DelayInterval, const chrono::MicroTime &);

//var::Tokenizer
ARG_DEFINE_ARGUMENT(TokenEncodedString, const var::ConstString &);
ARG_DEFINE_ARGUMENT(TokenDelimeters, const var::ConstString &);
ARG_DEFINE_ARGUMENT(IgnoreTokensBetween, const var::ConstString &);
ARG_DEFINE_ARGUMENT(IsCountEmptyTokens, bool);
ARG_DEFINE_ARGUMENT(MaximumTokenCount,	u32);


//var::Data
ARG_DEFINE_ARGUMENT(IsReadOnly, bool);
ARG_DEFINE_ARGUMENT(IsResize, bool);
ARG_DEFINE_ARGUMENT(IsRecursive, bool);


//var::String
ARG_DEFINE_ARGUMENT(StringToAssign, const var::ConstString &);
ARG_DEFINE_ARGUMENT(StringToAppend, const var::ConstString &);
ARG_DEFINE_ARGUMENT(StringToCompare, const var::ConstString &);
ARG_DEFINE_ARGUMENT(StringToFind, const var::ConstString &);
ARG_DEFINE_ARGUMENT(StringToErase, const var::ConstString &);
ARG_DEFINE_ARGUMENT(StringToInsert, const var::ConstString &);
ARG_DEFINE_ARGUMENT(CharacterToFind, char);

ARG_DEFINE_ARGUMENT(Position, u32);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitPosition, u32, Position);
ARG_DEFINE_ARGUMENT(SubPosition, u32);
ARG_DEFINE_ARGUMENT(Length, u32);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitLength, u32, Length);
ARG_DEFINE_ARGUMENT(SubLength, u32);
ARG_DEFINE_ARGUMENT(MatchLength, u32);
ARG_DEFINE_ARGUMENT(Count, u32);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitCount, u32, Count);

ARG_DEFINE_ARGUMENT(Width, u32);
ARG_DEFINE_ARGUMENT(Height, u32);
ARG_DEFINE_ARGUMENT(XValue, s32);
ARG_DEFINE_ARGUMENT(YValue, s32);
ARG_DEFINE_ARGUMENT(BitsPerPixel, u16);
ARG_DEFINE_ARGUMENT(PlaneCount, u16);


//var::Json
ARG_DEFINE_ARGUMENT(JsonKey, const var::ConstString &);
ARG_DEFINE_ARGUMENT(JsonEncodedString, const var::ConstString&);
ARG_DEFINE_ARGUMENT(Base64EncodedString, const var::ConstString&);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitBase64EncodedString, const var::ConstString&, Base64EncodedString);

ARG_DEFINE_ARGUMENT(SourceJsonValue, const var::JsonValue&);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitSourceJsonValue, const var::JsonValue&, SourceJsonValue);
ARG_DEFINE_ARGUMENT(DestinationJsonValue, var::JsonValue&);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitDestinationJsonValue, var::JsonValue&, DestinationJsonValue);


#if defined __link
ARG_DEFINE_ARGUMENT(LinkDriver, link_transport_mdriver_t*);
ARG_DEFINE_ARGUMENT(LinkDirp, int);
ARG_DEFINE_ARGUMENT(IsCopyToDevice, bool);
#endif

ARG_DEFINE_ARGUMENT(BootloaderRetryCount, u32);
ARG_DEFINE_ARGUMENT(IsVerify, bool);
ARG_DEFINE_ARGUMENT(IsLegacy, bool);
ARG_DEFINE_ARGUMENT(FileDescriptor, int);
ARG_DEFINE_ARGUMENT(IoRequest, int);
ARG_DEFINE_ARGUMENT(IoArgument, void*);
ARG_DEFINE_ARGUMENT(IoConstArgument, const void*);
ARG_DEFINE_ARGUMENT(IoIntArgument, int);
ARG_DEFINE_ARGUMENT(NumberBase, int);

//hal
ARG_DEFINE_ARGUMENT(Port, u32);
ARG_DEFINE_ARGUMENT(PortNumber, u32);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitPortNumber, u32, PortNumber);
ARG_DEFINE_ARGUMENT(PinNumber, u32);
ARG_DEFINE_ARGUMENT(McuPin, const mcu_pin_t &);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitMcuPin, const mcu_pin_t &, McuPin);
ARG_DEFINE_ARGUMENT(IsMcuPinMask, bool);
ARG_DEFINE_ARGUMENT(IsPinActiveHigh, bool);
ARG_DEFINE_ARGUMENT(PinFunction, u32);

ARG_DEFINE_ARGUMENT(Channel, u32);
ARG_DEFINE_ARGUMENT(Events, u32);
ARG_DEFINE_ARGUMENT(InterruptPriority, s8);
ARG_DEFINE_ARGUMENT(SchedulerPriority, int);
ARG_DEFINE_ARGUMENT(Context, void*);

ARG_DEFINE_ARGUMENT(McuCallback, const mcu_callback_t &);
ARG_DEFINE_IMPLICIT_ARGUMENT(ImplicitMcuCallback, const mcu_callback_t &, McuCallback);

ARG_DEFINE_ARGUMENT(SignalNumber, int);
ARG_DEFINE_ARGUMENT(SignalValueInteger, int);
ARG_DEFINE_ARGUMENT(SignalValuePointer, void*);
ARG_DEFINE_ARGUMENT(SignalCode, int);
ARG_DEFINE_ARGUMENT(IsPersistent, bool);
ARG_DEFINE_ARGUMENT(KernelRequestApi, u32);
ARG_DEFINE_ARGUMENT(KernelRequest, int);
ARG_DEFINE_ARGUMENT(FontPointSize, u8);
ARG_DEFINE_ARGUMENT(FontStyle, u8);
ARG_DEFINE_ARGUMENT(FontName, const var::ConstString&);
ARG_DEFINE_ARGUMENT(IsExactMatch, bool);

ARG_DEFINE_ARGUMENT(SourceSocketAddress, const inet::SocketAddress&);
ARG_DEFINE_ARGUMENT(DestinationSocketAddress, inet::SocketAddress&);
ARG_DEFINE_ARGUMENT(ListenBacklogCount, int);
ARG_DEFINE_ARGUMENT(HttpStringToPost, const var::ConstString&);
ARG_DEFINE_ARGUMENT(UrlEncodedString, const var::ConstString&);

ARG_DEFINE_ARGUMENT(VersionEncodedString, const var::ConstString&);
ARG_DEFINE_ARGUMENT(GitHash, const var::ConstString&);

ARG_DEFINE_ARGUMENT(RedColorComponent, u8);
ARG_DEFINE_ARGUMENT(GreenColorComponent, u8);
ARG_DEFINE_ARGUMENT(BlueColorComponent, u8);
ARG_DEFINE_ARGUMENT(HueColorComponent, u8);
ARG_DEFINE_ARGUMENT(ValueColorComponent, u8);
ARG_DEFINE_ARGUMENT(SaturationColorComponent, u8);
ARG_DEFINE_ARGUMENT(AlphaColorComponent, u8);
ARG_DEFINE_ARGUMENT(BrightnessColorComponent, u8);

}


#endif // SAPI_ARG_ARGUMENT_HPP
