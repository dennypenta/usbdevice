#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <libusb.h>

#include <stdexcept>
#include <string>


using namespace std;


class UsbDevice {
private:
    int r;
    libusb_context* ctx;
    libusb_device_handle* dev_handle;
public:
    UsbDevice(uint16_t vendor_id, uint16_t product_id){
        this->r = libusb_init(&this->ctx);
        this->valid_returned("Init error " + to_string(r));
        this->dev_handle = libusb_open_device_with_vid_pid(this->ctx, vendor_id, product_id);
    };

    ~UsbDevice(){
        this->remove();
    }

    void remove(){
        libusb_close(this->dev_handle);
        libusb_exit(this->ctx);
    }

    void write(unsigned char* data){
        int actual_length;
        this->r = libusb_bulk_transfer(this->dev_handle,
                                       LIBUSB_ENDPOINT_IN,
                                       data,
                                       sizeof(data),
                                       &actual_length,
                                       0);
        this->valid_returned("Transfer data has been closed with error");

    }

    void valid_returned(string msg) {
        if (this->r != 0) {
            throw runtime_error(msg);
        }
    }

};


class GeneratorDevice : public UsbDevice{
    void write_freq(string frq) {

    }

    void write_vol(string vol) {

    }

    void write_power_on() {

    }

    void write_power_off() {

    }
};


#endif
