Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"
  config.vm.synced_folder ".", "/vagrant", rsync__exclude: [
    "*.sublime-*",".DS_Store",".git/",".gitignore"]
  config.vm.provision "shell",
    path: "provision/clean-apt"
  config.vm.provision "shell",
    path: "provision/disable-release-upgrader"
  config.vm.provision "shell",
    path: "provision/set-utf8-locale"
  config.vm.provision "shell",
    path: "provision/development-setup-as-root"
  config.vm.provision "shell",
    path: "provision/development-setup-as-user",
    privileged: false
  # Avoid is-not-a-tty messages.
  config.ssh.shell = "bash -c 'BASH_ENV=/etc/profile exec bash'"
  config.vm.provider :virtualbox do |vb|
    # Enable the VM's virtual USB controller & enable the virtual USB 2.0 controller.
    # (You need to install VirtualBox Extension Pack for this to work)
    vb.customize ["modifyvm", :id, "--usb", "on", "--usbehci", "on"]
  end
end
