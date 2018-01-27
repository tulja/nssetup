echo "Changing directory to /opt/openbaton/scripts/"
cd /opt/openbaton/scripts
source "ip_map.sh" 
echo "Changing sgw.cpp"
sudo sed -i "s/^\(string\ g_sgw_s11_ip_addr\s*=\s*\).*\$/\1"$g_sgw_ip_addr"/" /opt/openbaton/scripts/NFV_LTE_EPC-1.0/src/sgw.cpp
sudo sed -i "s/^\(string\ g_sgw_s1_ip_addr\s*=\s*\).*\$/\1"$g_sgw_ip_addr"/" /opt/openbaton/scripts/NFV_LTE_EPC-1.0/src/sgw.cpp
sudo sed -i "s/^\(string\ g_sgw_s5_ip_addr\s*=\s*\).*\$/\1"$g_sgw_ip_addr"/" /opt/openbaton/scripts/NFV_LTE_EPC-1.0/src/sgw.cpp

