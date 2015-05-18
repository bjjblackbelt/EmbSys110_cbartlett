#include "IUart.h"
#include <string.h>

//!< Create stub to test DUart interface
class StubDUart : public IUart
{
  public:
    StubDUart()
        : IUart(),
        m_buffer(),
        m_index(0)
    {
        ResetBuffer();
    }

    virtual void Init() {;}

    void ResetBuffer()
    {
        memset(&m_buffer[0], '\0', sizeof(m_buffer));
        m_index = 0;
    }

    static const uint8_t MAX_STRING_LENGTH = 127;
    char m_buffer[MAX_STRING_LENGTH + 1];
    uint8_t m_index;

  protected:
    virtual uint8_t GetC() { return 0;}

    virtual void PutC(uint8_t ch)
    {
        if (m_index < MAX_STRING_LENGTH)
        {
            m_buffer[m_index++] = ch;
        }
    }
};


