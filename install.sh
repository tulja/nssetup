sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y python-dev
sudo apt-get install snmpd
sudo apt-get install -y python-pip
sudo pip install --upgrade setuptools
sudo pip install netifaces
sudo apt-get install -y libssl-dev
sudo apt-get install -y iperf screen
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y install openvpn
sudo apt-get -y install libsctp-dev
sudo apt-get -y install openssl
sudo add-apt-repository "ppa:patrickdk/general-lucid" -y
sudo apt-get -y update
sudo apt-get -y install iperf3
sudo apt-get -y install htop
sudo apt-get -y update
sudo apt-get -y upgrade
sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password password secret'
sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password secret'
sudo apt-get -y install mysql-server
sudo apt-get -y install libmysqlclient-dev

