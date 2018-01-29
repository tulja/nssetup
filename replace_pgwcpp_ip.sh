echo "Changing directory to /opt/openbaton/scripts/"
cd /opt/openbaton/scripts
source "ip_map.sh" 
echo "Changing pgw.cpp"

sudo sed -i "s/^\(string\ g_sgw_s5_ip_addr\s*=\s*\).*\$/\1"$g_sgw_ip_addr"/" /opt/openbaton/scripts/LTECode/pgw.cpp
sudo sed -i "s/^\(string\ g_pgw_s5_ip_addr\s*=\s*\).*\$/\1"$g_pgw_ip_addr"/" /opt/openbaton/scripts/LTECode/pgw.cpp
sudo sed -i "s/^\(string\ g_pgw_sgi_ip_addr\s*=\s*\).*\$/\1"$g_pgw_ip_addr"/" /opt/openbaton/scripts/LTECode/pgw.cpp
sudo sed -i "s/^\(string\ g_sink_ip_addr\s*=\s*\).*\$/\1"$g_sink_ip_addr"/" /opt/openbaton/scripts/LTECode/pgw.cpp
