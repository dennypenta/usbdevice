#ifndef DEV_H
#define DEV_H

#include <libusb.h>

#include <string.h>
#include <stdio.h>

using namespace std;


int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                      libusb_hotplug_event event, void *user_data) {
  int count;
  static libusb_device_handle *handle = NULL;
  struct libusb_device_descriptor desc;
  int rc;
  (void)libusb_get_device_descriptor(dev, &desc);
  if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {
    rc = libusb_open(dev, &handle);
    if (LIBUSB_SUCCESS != rc) {
      cout<<"Could not open USB device\n"<<endl;
    }
  } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
    if (handle) {
      libusb_close(handle);
      handle = NULL;
    }
  } else {
    cout<<"Unhandled event "<<event<<endl;
  }
  count++;

  return 0;
}

void printdev(libusb_device *dev) {
    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        cout<<"failed to get device descriptor"<<endl;
        return;
    }
    cout<<"Number of possible configurations: "<<(int)desc.bNumConfigurations<<"  ";
    cout<<"Device Class: "<<(int)desc.bDeviceClass<<"  ";
    cout<<"VendorID: "<<desc.idVendor<<"  ";
    cout<<"ProductID: "<<desc.idProduct<<endl;
    libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);
    cout<<"Interfaces: "<<(int)config->bNumInterfaces<<" ||| ";
    const libusb_interface *inter;
    const libusb_interface_descriptor *interdesc;
    const libusb_endpoint_descriptor *epdesc;
    for(int i=0; i<(int)config->bNumInterfaces; i++) {
        inter = &config->interface[i];
        cout<<"Number of alternate settings: "<<inter->num_altsetting<<" | ";
        for(int j=0; j<inter->num_altsetting; j++) {
            interdesc = &inter->altsetting[j];
            cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
            cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<<" | ";
            for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
                epdesc = &interdesc->endpoint[k];
                cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
                cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<" | ";
            }
        }
    }
    cout<<endl<<endl<<endl;
    libusb_free_config_descriptor(config);
}

const struct libusb_endpoint_descriptor* active_config(struct libusb_device *dev,struct libusb_device_handle *handle)
{
    struct libusb_device_handle *hDevice_req;
    struct libusb_config_descriptor *config;
    struct libusb_endpoint_descriptor *endpoint;
    int altsetting_index,interface_index=0,ret_active;
    int i,ret_print;

    hDevice_req = handle;

    ret_active = libusb_get_active_config_descriptor(dev,&config);

    for(interface_index=0;interface_index<config->bNumInterfaces;interface_index++)
    {
        const struct libusb_interface *iface = &config->interface[interface_index];
        for(altsetting_index=0;altsetting_index<iface->num_altsetting;altsetting_index++)
        {
            const struct libusb_interface_descriptor *altsetting = &iface->altsetting[altsetting_index];

            int endpoint_index;
            for(endpoint_index=0;endpoint_index<altsetting->bNumEndpoints;endpoint_index++)
            {
                return &altsetting->endpoint[endpoint_index];
//                struct libusb_endpoint_desriptor *ep = &altsetting->endpoint[endpoint_index];
//                endpoint = ep;
//                endpoint = &altsetting->endpoint[endpoint_index];
//                alt_interface = altsetting->bAlternateSetting;
//                interface_number = altsetting->bInterfaceNumber;
            }

            printf("\nEndPoint Descriptors: ");
            printf("\n\tSize of EndPoint Descriptor : %d",endpoint->bLength);
            printf("\n\tType of Descriptor : %d",endpoint->bDescriptorType);
            printf("\n\tEndpoint Address : 0x0%x",endpoint->bEndpointAddress);
            printf("\n\tMaximum Packet Size: %x",endpoint->wMaxPacketSize);
            printf("\n\tAttributes applied to Endpoint: %d",endpoint->bmAttributes);
            printf("\n\tInterval for Polling for data Tranfer : %d\n",endpoint->bInterval);
        }
    }
    libusb_free_config_descriptor(NULL);
    return endpoint;
}

int print_configuration(struct libusb_device_handle *hDevice,struct libusb_config_descriptor *config)
{
    unsigned char *data;
    int index;

    data = (unsigned char *)malloc(512);
    memset(data,0,512);

    index = config->iConfiguration;

    libusb_get_string_descriptor_ascii(hDevice,index,data,512);

    printf("\nInterface Descriptors: ");
    printf("\n\tNumber of Interfaces : %d",config->bNumInterfaces);
    printf("\n\tLength : %d",config->bLength);
    printf("\n\tDesc_Type : %d",config->bDescriptorType);
    printf("\n\tConfig_index : %d",config->iConfiguration);
    printf("\n\tTotal length : %lu",config->wTotalLength);
    printf("\n\tConfiguration Value  : %d",config->bConfigurationValue);
    printf("\n\tConfiguration Attributes : %d",config->bmAttributes);
    printf("\n\tMaxPower(mA) : %d\n",config->MaxPower);

    free(data);
    data = NULL;
    return 0;
}

#endif // DEV_H
