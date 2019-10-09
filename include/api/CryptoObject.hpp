#ifndef SAPI_API_CRYPTO_OBJECT_HPP_
#define SAPI_API_CRYPTO_OBJECT_HPP_

#include <sos/crypt_api.h>

#include "WorkObject.hpp"
#include "InfoObject.hpp"

/*! \cond */
#if defined __link
#include <mbedtls_api.h>
#if !defined CRYPT_SHA256_API_REQUEST
#define CRYPT_SHA256_API_REQUEST &mbedtls_crypt_sha256_api
#endif

#if !defined CRYPT_SHA512_API_REQUEST
#define CRYPT_SHA512_API_REQUEST &mbedtls_crypt_sha512_api
#endif

#if !defined CRYPT_AES_API_REQUEST
#define CRYPT_AES_API_REQUEST &mbedtls_crypt_aes_api
#endif

#if !defined CRYPT_RANDOM_API_REQUEST
#define CRYPT_RANDOM_API_REQUEST &mbedtls_crypt_random_api
#endif

#endif
/*! \endcond */


namespace api {

typedef Api<crypt_hash_api_t, CRYPT_SHA256_API_REQUEST> Sha256Api;
typedef Api<crypt_hash_api_t, CRYPT_SHA512_API_REQUEST> Sha512Api;
typedef Api<crypt_aes_api_t, CRYPT_AES_API_REQUEST> AesApi;
typedef Api<crypt_random_api_t, CRYPT_RANDOM_API_REQUEST> RandomApi;

class CryptoObject {
public:
	static Sha256Api & sha256_api(){ return m_sha256_api; }
	static Sha512Api & sha512_api(){ return m_sha512_api; }
	static AesApi & aes_api(){ return m_aes_api; }
	static RandomApi & random_api(){ return m_random_api; }
private:
	static Sha256Api m_sha256_api;
	static Sha512Api m_sha512_api;
	static AesApi m_aes_api;
	static RandomApi m_random_api;
};

class CryptoInfoObject : public virtual InfoObject, public CryptoObject {

};

class CryptoWorkObject : public virtual WorkObject, public CryptoObject {

};

}

#endif // SAPI_API_CRYPTO_OBJECT_HPP_
