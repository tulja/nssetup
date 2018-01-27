#!/bin/bash
echo "Move to Scripts directory"
cd /opt/openbaton/scripts
ip=$(ifconfig ens3 | grep "inet addr" | cut -d ':' -f 2 | cut -d ' ' -f 1)
hostname=$(hostname)
server=$(echo $hostname | cut -d- -f1 )
echo "g_"$server"_ip_addr=\"\\\""$ip"\\\";\"" > ip_map.sh 
