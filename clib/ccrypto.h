/***********************************************************************************************
    created:         2019-03-06
    
    author:            chensong
                    
    purpose:        ccrypto
************************************************************************************************/
#ifndef _C_CRYPTO_H_
#define _C_CRYPTO_H_


#include "crc4.h"

namespace chen {

	class ccrypto
	{
	public:
		ccrypto() {}
		virtual ~ccrypto() {}

		bool set_encrypt_key(const void* encrypt_data, int encrypt_len);
		bool set_decrypt_key(const void* decrypt_data, int decrypt_len);
		void encrypt(const void* data_in, void* data_out, int len_in);
		void decrypt(const void* data_in, void* data_out, int len_in);
    private:
        ccrypto(const ccrypto&);
        ccrypto& operator=(const ccrypto&);
	private:
		cRC4				m_encrypt_key;
		cRC4				m_decrypt_key;
	};

} // namespace chen

#endif //_C_CRYPTO_H_
