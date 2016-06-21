Vagrant.configure(2) do |config|
  config.vm.define "debian", primary: true do |debian|
    debian.vm.box = "ubuntu/trusty64"
    debian.vm.synced_folder "src", "/home/vagrant/src", rsync__exclude: [
      ".DS_Store", "bin"]
    debian.vm.provision "shell",
      path: "provision/clean-apt"
    debian.vm.provision "shell",
      path: "provision/disable-release-upgrader"
    debian.vm.provision "shell",
      path: "provision/set-utf8-locale"
    debian.vm.provision "shell",
      path: "provision/development-setup-as-root"
    debian.vm.provision "shell",
      path: "provision/development-setup-as-user",
      privileged: false
    # Avoid is-not-a-tty messages.
    debian.ssh.shell = "bash -c 'BASH_ENV=/etc/profile exec bash'"
    debian.vm.provider :virtualbox do |vb|
      # Enable the VM's virtual USB controller & enable the virtual USB 2.0 controller.
      # (You need to install VirtualBox Extension Pack for this to work)
      vb.customize ["modifyvm", :id, "--usb", "on", "--usbehci", "on"]
    end
  end
  config.vm.define "windows", autostart: false do |windows|
    windows.vm.box = "opentable/win-7-professional-i386-nocm"
    #windows.vm.communicator = "winrm"
  end
end
