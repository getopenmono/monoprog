# Notes

## Windows Virtual Machine

```
  config.vm.define "windows", autostart: false do |windows|
    windows.vm.box = "opentable/win-7-professional-i386-nocm"
    #windows.vm.communicator = "winrm"
    windows.vm.provider :virtualbox do |vb|
      # Enable the VM's virtual USB controller & enable the virtual USB 2.0 controller.
      # (You need to install VirtualBox Extension Pack for this to work)
      vb.customize ["modifyvm", :id, "--usb", "on", "--usbehci", "on"]
  end
```

Video mem: 27MB

Install: chrome

C:\vagrant is the mout point.

VS 2010 Express (x86) DVD:
https://msdn.microsoft.com/subscriptions/json/GetDownloadRequest?brand=MSDN&locale=en-us&fileId=41808&activexDisabled=true&akamaiDL=false

----

To use VMs & USB: VirtualBox Devices USB: add Mono as device, then manually put in bootloader, and add device (PSoC) again.
