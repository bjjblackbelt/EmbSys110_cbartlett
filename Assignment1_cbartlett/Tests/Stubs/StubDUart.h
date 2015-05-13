#include "DUartIF.h"
#include <string.h>

//!< Create stub to test DUart interface
class StubDUart : public DUartIF
{
  public:
    StubDUart()
        : DUartIF(),
        m_buffer(),
        m_index(0)
    {
        ResetBuffer();
    }

    void ResetBuffer()
    {
        memset(&m_buffer[0], '\0', sizeof(m_buffer));
        m_index = 0;
    }

    static const uint8_t MAX_STRING_LENGTH = 127;
    char m_buffer[MAX_STRING_LENGTH + 1];
    uint8_t m_index;

  protected:
    virtual void PutC(uint8_t ch)
    {
        if (m_index < MAX_STRING_LENGTH)
        {
            m_buffer[m_index++] = ch;
        }
    }
};


