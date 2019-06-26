#ifndef CRYPTOBJECT_HPP
#define CRYPTOBJECT_HPP

#include <sos/crypt_api.h>

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

typedef Api<crypt_hash_api_t, CRYPT_SHA256_API_REQUEST> Sha256Api;
typedef Api<crypt_hash_api_t, CRYPT_SHA512_API_REQUEST> Sha512Api;

class CryptObject {
public:
	static Sha256Api & sha256_api(){ return m_sha256_api; }
	static Sha512Api & sha512_api(){ return m_sha512_api; }
private:
	static Sha256Api m_sha256_api;
	static Sha512Api m_sha512_api;
};

class CryptInfoObject : public virtual InfoObject, public CryptObject {

};

class CryptWorkObject : public virtual WorkObject, public CryptObject {

};

}

#endif // CRYPTOBJECT_HPP
