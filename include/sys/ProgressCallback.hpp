#ifndef PROGRESSCALLBACK_HPP
#define PROGRESSCALLBACK_HPP

#include "../api/SysObject.hpp"


namespace sys {

class ProgressCallback : public api::SysInfoObject {
public:
	typedef bool (*callback_t)(void*, int, int);

	ProgressCallback();
	ProgressCallback(callback_t callback, void * context = 0);


	void set_callback(bool (*callback)(void*,int,int)){
		m_update = callback;
	}

	void set_context(void * context){ m_context = context; }

	bool update(int value, int total) const;

private:
	bool (*m_update)(void*, int, int);
	void * m_context;

};


}

#endif // PROGRESSCALLBACK_HPP
