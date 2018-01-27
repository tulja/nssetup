#!/bin/sh
cd /opt/openbaton/scripts/
sudo python send_ip.py 192.168.136.88 8081
sudo python zabbix_resolve_ip.py 10.0.0.3 8050
