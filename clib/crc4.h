/***********************************************************************************************
    created:         2019-03-06
    
    author:            chensong
                    
    purpose:        crc4
************************************************************************************************/
#ifndef _C_RC4_H_
#define _C_RC4_H_

namespace chen
{
    class cRC4 
    {
	public:
		cRC4();

        void set_key(int len, const unsigned char* data);
        
        void process(int len, const unsigned char* in_data, unsigned char* out_data);
		
    private:    
        unsigned char m_data[256];
        int m_x;
        int m_y;
	};
}

#endif //_C_RC4_H_
