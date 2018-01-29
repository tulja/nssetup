echo "Changing directory to /opt/openbaton/scripts/"
cd /opt/openbaton/scripts
source "ip_map.sh" 
echo "Changing ran.cpp"

sudo sed -i "s/^\(string\ g_ran_ip_addr\s*=\s*\).*\$/\1"$g_ran_ip_addr"/" /opt/openbaton/scripts/LTECode/ran.cpp
sudo sed -i "s/^\(string\ g_trafmon_ip_addr\s*=\s*\).*\$/\1"$g_ran_ip_addr"/" /opt/openbaton/scripts/LTECode/ran.cpp
sudo sed -i "s/^\(string\ g_mme_ip_addr\s*=\s*\).*\$/\1"$g_mme_ip_addr"/" /opt/openbaton/scripts/LTECode/ran.cpp
