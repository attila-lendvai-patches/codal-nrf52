#ifndef NRF52_SERIAL_H
#define NRF52_SERIAL_H

#include "Pin.h"
#include "nrf.h"
#include "CodalComponent.h"
#include "CodalConfig.h"
#include "Serial.h"
#include "hal/nrf_uart.h"

namespace codal
{
    /**
     * The serial port on UART0, run without EasyDMA.
     *
     * With DMA the received bytes can only be counted by RXDRDY, a flag
     * that rises once however many bytes arrived while the interrupt
     * waited, so a late interrupt left bytes behind until more came.
     * Without DMA each byte is read from RXD, and RXDRDY rises again for
     * the next one in the six-byte FIFO. UART0 is the only UARTE instance
     * that can run this way.
     */
    class NRF52Serial : public Serial
    {
        volatile bool is_tx_in_progress_;

        NRF_UART_Type *p_uart_;
        static void _irqHandler(void *self);

        protected:
        virtual int enableInterrupt(SerialInterruptType t) override;
        virtual int disableInterrupt(SerialInterruptType t) override;
        virtual int configurePins(Pin& tx, Pin& rx) override;

        public:

        /**
         * Constructor
         *
         * @param tx the pin instance to use for transmission
         *
         * @param rx the pin instance to use for reception
         *
         * @param device NRF_UARTE0, the only one that will do; NULL means it too
         *
         **/
        NRF52Serial(Pin& tx, Pin& rx, NRF_UARTE_Type* device = NULL);

        virtual int putc(char) override;
        virtual int getc() override;
        virtual int setBaudrate(uint32_t baudrate) override;

        /**
          * Puts the component in (or out of) sleep (low power) mode.
          */
        virtual int setSleep(bool doSleep) override;

        ~NRF52Serial();
    };
}

#endif