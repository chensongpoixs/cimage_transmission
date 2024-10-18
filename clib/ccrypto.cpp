/***********************************************************************************************
    created:         2019-03-06
    
    author:            chensong
                    
    purpose:        ccrypto
************************************************************************************************/
#include "ccrypto.h"

namespace chen {

	bool ccrypto::set_encrypt_key(const void* encrypt_data, int encrypt_len)
	{
		if (!encrypt_data || 0 == encrypt_len)
		{
			return false;
		}
		m_encrypt_key.set_key(encrypt_len, (const unsigned char*)encrypt_data);
		return true;
	}

	bool ccrypto::set_decrypt_key(const void* decrypt_data, int decrypt_len)
	{
		if (!decrypt_data || 0 == decrypt_len)
		{
			return false;
		}

		m_decrypt_key.set_key(decrypt_len, (const unsigned char*)decrypt_data);
		return true;
	}

	void ccrypto::encrypt(const void* data_in, void* data_out, int len_in)
	{
		if (!data_in || !data_out || 0 == len_in)
		{
			return;
		}

		m_encrypt_key.process(len_in, (const unsigned char*)data_in, (unsigned char*)data_out);
	}

	void ccrypto::decrypt(const void* data_in, void* data_out, int len_in)
	{
		if (!data_in || !data_out || 0 == len_in)
		{
			return;
		}

		m_decrypt_key.process(len_in, (const unsigned char*)data_in, (unsigned char*)data_out);
	}

} // namespace chen
