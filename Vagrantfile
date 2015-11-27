Vagrant.configure(2) do |config|

  config.vm.define "ubuntu" do |ubuntu|
    ubuntu.vm.box = "ubuntu/trusty64"
    ubuntu.vm.synced_folder ".", "/vagrant", rsync__exclude: [
      "*.sublime-*",".DS_Store",".git/",".gitignore"]
    ubuntu.vm.provision "shell",
      path: "provision/clean-apt"
    ubuntu.vm.provision "shell",
      path: "provision/disable-release-upgrader"
    ubuntu.vm.provision "shell",
      path: "provision/set-utf8-locale"
    ubuntu.vm.provision "shell",
      path: "provision/development-setup-as-root"
    ubuntu.vm.provision "shell",
      path: "provision/development-setup-as-user",
      privileged: false
    # Avoid is-not-a-tty messages.
    ubuntu.ssh.shell = "bash -c 'BASH_ENV=/etc/profile exec bash'"
    ubuntu.vm.provider :virtualbox do |vb|
      # Enable the VM's virtual USB controller & enable the virtual USB 2.0 controller.
      # (You need to install VirtualBox Extension Pack for this to work)
      vb.customize ["modifyvm", :id, "--usb", "on", "--usbehci", "on"]
    end
  end

  config.vm.define "windows" do |windows|
    windows.vm.box = "ferventcoder/win7pro-x64-nocm-lite"
    windows.vm.guest = :windows
    windows.ssh.insert_key = false
    windows.vm.provider "virtualbox" do |v|
      # v.gui = true
    end
  end

  config.vm.define "osx" do |osx|
    osx.vm.box = "jhcook/osx-yosemite-10.10"
    osx.vm.guest = :darwin
    osx.vm.provider "virtualbox" do |v|
      # v.gui = true
    end
  end

end
