#!/bin/bash
echo "Change the Directory to /opt/openbaton/scripts"
cd /opt/openbaton/scripts/
echo "Running  the Server Configure at Client" 
echo "Server IP is " 
echo $server_private
echo "Server Hostname is "
echo $server_hostname 

#$server = ""
#ip="10.0.0.2"
#g_ran_ip_addr="\"192.168.0.83\";"
server=$(echo $server_hostname | cut -d- -f1 )
echo "g_"$server"_ip_addr=\"\\\""$server_private"\\\";\"" >> ip_map.sh



