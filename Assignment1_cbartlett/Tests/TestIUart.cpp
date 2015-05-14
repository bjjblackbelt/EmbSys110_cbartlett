#include <TestHarness.h>

#include <string.h>
#include "IUart.h"
#include "StubDUart.h"

//!< Begin tests
TEST_GROUP(IUart)
{
    StubDUart* m_pUart;

    void setup()
    {
        m_pUart = new StubDUart;
    }

    void teardown()
    {
        delete m_pUart;
    }
};

TEST(IUart, Initialize)
{
    CHECK(m_pUart != NULL);
}

TEST(IUart, PrintsCorrectString)
{
    char msg[] = "Hello World.";
    m_pUart->PrintStr(&msg[0]);
    CHECK(strcmp(&msg[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintingNullHasNoEffect)
{
    m_pUart->PrintStr(NULL);
    CHECK(m_pUart->m_buffer[0] == '\0');
}

TEST(IUart, PrintsASingleCharacterSuccessfully)
{
    char msg[] = "?";
    m_pUart->PrintStr(&msg[0]);
    CHECK(strcmp(&msg[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintsSingleNumberInHex)
{
    uint32_t numberIn = 8;
    char charOut[] = "0x8";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintsZeroInHex)
{
    uint32_t numberIn = 0;
    char charOut[] = "0x0";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, DoesNotPrintLeadingZerosInHex)
{
    uint32_t numberIn = 0x00000008;
    char charOut[] = "0x8";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintsEveryNibbleInHex)
{
    uint32_t numberIn = 0xFFFFFFFF;
    char charOut[] = "0xFFFFFFFF";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintsSingleNumberInDecimal)
{
    uint32_t numberIn = 8;
    char charOut[] = "8";
    m_pUart->PrintUInt(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintsZeroInDecimalUInt)
{
    uint32_t numberIn = 0;
    char charOut[] = "0";
    m_pUart->PrintUInt(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(IUart, PrintsMultipleDigitNumberInDecimalUInt)
{
    uint32_t numberIn = 123456;
    char charOut[] = "123456";
    m_pUart->PrintUInt(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}
