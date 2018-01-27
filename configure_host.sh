sudo chmod 777 /etc/hosts
hostname=$(hostname)
sudo echo '127.0.1.1 '$hostname  >> /etc/hosts  
