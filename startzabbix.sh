cd /opt/openbaton/scripts/
sudo cp zabbix_agentd.conf /etc/zabbix/
sudo systemctl start zabbix-agent.service 
#cat /etc/zabbix/zabbix_agentd.conf
echo "started zabbix-agent.service"
