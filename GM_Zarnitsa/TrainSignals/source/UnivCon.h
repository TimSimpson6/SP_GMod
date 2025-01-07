// Base class for Universal CAN controller
#pragma once
#include "params.h"
#include <Windows.h>
#include <memory>

class CUnivCon
{
public:
    enum ErrorCode
    {
        // OpenCOMPort()
        E_SUCCESS           = 0x00,
        E_GETCOMMSTATE      = 0x01,
        E_SETCOMMSTATE      = 0x02,
        E_SETCOMMMASK       = 0x03,
        E_GETCOMMTIMEOUTS   = 0x04,
        E_SETCOMMTIMEOUTS   = 0x05,
        E_SETUPCOMM         = 0x06,
        E_CONNECTFAIL       = 0x10,
        E_WRONGPORT         = 0x11,
        E_HANDLEERROR       = 0x12,

        // Setup()
        E_BADCONFIG         = 0x20,
        E_NOCONFIGPTR       = 0x21,
        E_SETUPFAIL         = 0x22,
    };

    enum ConfigState {
        NotUsed = 0,
        Input,
        Output,
        InputADC
    };

    struct SevenDecSignals
    {
        int port[3]{};
    };

    struct Configuration {
        int nControllers = 0; // ���������� ������������

        std::unique_ptr<int[]> arrPins; // ������ ������������ �����
        std::unique_ptr<int[]> arrArrows; // ���������� ���������� �������� �� ����������
        std::unique_ptr<SevenDecSignals[]> arr7SegDec; // ������ ������������ 7-�� ����������� ���������� �������� ��� ����� (����� ���������� �514��2)
        std::unique_ptr<int[]> arrTextDisplaySize; // ������ ������������ ��������� ��������.

        std::unique_ptr<int[]> arrADCPerController; // ���������� ADC �� ����������

        int nInputBytes = -1; // ���������� �������� ����
        int nOutputBytes = -1; // ���������� ��������� ����
        int nUARTBytes = -1; // ���������� ��������� UART ����
    } m_Config;

    struct TextDisplaySignals
    {
        int on = 0;    // ������� �����
        int ledOn = 0; // �������� ���������.
        char text[50]{};
    };

    struct Signals {
        int nPins = 0; // ������ ������� ������������ �����
        int nADC = 0; // ���������� ���
        int nArrows = 0; // ���������� ���������� ��������
        int n7SegDec = 0; // ������ ������� �������� 7SegDec
        int nTextDisplays = 0; // ���������� ��������� ��������

        std::unique_ptr<int[]> arrInput;
        std::unique_ptr<int[]> arrOutput;
        std::unique_ptr<int[]> arrADC;
        std::unique_ptr<int[]> arrArrow;
        std::unique_ptr<int[]> arr7SegDec;
        std::unique_ptr<TextDisplaySignals[]> arrTextDisplay;
    };

    virtual ErrorCode Start(int port);
    virtual ErrorCode Setup() = 0;
    virtual void Stop() = 0;
    

    virtual void ReadSignals(Signals& signals) = 0;
    virtual void WriteSignals(Signals& signals) = 0;
    virtual void WriteUARTData(Signals& signals) = 0;

    virtual bool IsConnected();
    virtual int GetPortNumber();

    static byte ConvertIntTo7DecSegByte(int number);
protected:
    HANDLE m_hPort = INVALID_HANDLE_VALUE;
    bool m_Connected = false;
    int m_PortNumber = -1;

    struct Data {
        int nInputBytes = 0;
        int nOutputBytes = 0;
        int nUARTBytes = 0;

        std::unique_ptr<byte[]> arrInputBytes; // ������ ��� ������ ������
        std::unique_ptr<byte[]> arrOutputBytes; // ������ ��� �������� ������
        std::unique_ptr<byte[]> arrUARTBytes; // ������ ��� �������� UART ������
    } m_Data;

    virtual ErrorCode OpenCOMPort(int port) = 0;
    virtual ErrorCode WriteConfiguration(const byte* configData, int size) = 0;
    virtual void DestroyHandle();
};

