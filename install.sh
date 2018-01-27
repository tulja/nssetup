sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y python-dev
sudo apt-get install snmpd
sudo apt-get install -y python-pip
sudo pip install --upgrade setuptools
sudo pip install netifaces
sudo apt-get install -y libssl-dev
sudo apt-get install -y iperf screen
echo "Moving to Scripts folder "
cd /opt/openbaton/scripts/
cd NFV_LTE_EPC-1.0/scripts/v1.0
chmod 777 install.sh
./install.sh

