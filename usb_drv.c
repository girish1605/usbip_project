#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/usb.h>

#define VENDOR_ID 0x0781
#define PRODUCT_ID 0x5567

struct drv_priv {
	struct usb_device *udev;
	struct usb_interface *interface;
	unsigned char *bulk_in_buffer;
	size_t bulk_in_size;
	__u8 bulk_in_endpointAddr;
	__u8 bulk_out_endpointAddr;
	int error;
	struct kref kref;
};

static struct usb_device_id usb_id_table[] = {
	{USB_DEVICE(VENDOR_ID, PRODUCT_ID)},
	{}	//NULL : Terminator (Required)
}

static int usb_drv_probe(struct usb_interface *interface, struct usb_device_id *id) {
	struct drv_priv *dev;
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;
	size_t buffer_size;
	int i;

	dev = kzalloc(sizeof(drv_priv), GPL_KERNEL);
	if(!dev) {
		dev_err(&interface->dev, "Out of Memory\n");
		goto error;	
	}

	dev->udev = interface_to_usbdev(interface);
	dev->interface = interface;
}

static struct usb_driver usb_drv = {
	.name = "My SanDisk USB Stick",
	.id_table = usb_id_table,	
	.probe = usb_drv_probe,
	.disconnect = usb_drv_disconnect,
}

static int __init usb_drv_init(void) {
	int retval;
	pr_info();
	retval = usb_register(&usb_drv);
	return retval;
}

static __exit usb_drv_exit(void) {
	usb_deregister(&usb_drv);	
}

module_init(usb_drv_init);
module_exit(usb_drv_exit);

MODULE_AUTHOR("Girish Bhangale <girish.1605@gmail.com>");
MODULE_DESCRIPTION("USB Basic Driver");
MODULE_LICENSE("GPL");
