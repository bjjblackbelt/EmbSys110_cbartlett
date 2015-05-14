#include <TestHarness.h>

#include <string.h>
#include "DUartIF.h"
#include "StubDUart.h"

//!< Begin tests
TEST_GROUP(DUartIF)
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

TEST(DUartIF, Initialize)
{
    CHECK(m_pUart != NULL);
}

TEST(DUartIF, PrintsCorrectString)
{
    char msg[] = "Hello World.";
    m_pUart->PrintStr(&msg[0]);
    CHECK(strcmp(&msg[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintingNullHasNoEffect)
{
    m_pUart->PrintStr(NULL);
    CHECK(m_pUart->m_buffer[0] == '\0');
}

TEST(DUartIF, PrintsASingleCharacterSuccessfully)
{
    char msg[] = "?";
    m_pUart->PrintStr(&msg[0]);
    CHECK(strcmp(&msg[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintsSingleNumberInHex)
{
    uint32_t numberIn = 8;
    char charOut[] = "0x8";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintsZeroInHex)
{
    uint32_t numberIn = 0;
    char charOut[] = "0x0";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, DoesNotPrintLeadingZerosInHex)
{
    uint32_t numberIn = 0x00000008;
    char charOut[] = "0x8";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintsEveryNibbleInHex)
{
    uint32_t numberIn = 0xFFFFFFFF;
    char charOut[] = "0xFFFFFFFF";
    m_pUart->PrintHex(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintsSingleNumberInDecimal)
{
    uint32_t numberIn = 8;
    char charOut[] = "8";
    m_pUart->PrintUInt(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintsZeroInDecimalUInt)
{
    uint32_t numberIn = 0;
    char charOut[] = "0";
    m_pUart->PrintUInt(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}

TEST(DUartIF, PrintsMultipleDigitNumberInDecimalUInt)
{
    uint32_t numberIn = 123456;
    char charOut[] = "123456";
    m_pUart->PrintUInt(numberIn);
    CHECK(strcmp(&charOut[0], &m_pUart->m_buffer[0]) == 0);
}
