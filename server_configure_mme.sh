echo "Change the Directory to /opt/openbaton/scripts"
cd /opt/openbaton/scripts/
echo "Running  the Server Configure at Client" 
echo "Server IP is " 
echo $server_private
echo "Server Hostname is "
echo $server_hostname 

s1="sgw1"
s2="pgw1"
if [ "$server_hostname" == "$s1" ]; then
	echo "g_"$server_hostname"_ip_addr=\"\\\""$server_private"\\\";\"" >> sgw1.txt
fi

if [ "$server_hostname" == "$s2" ]; then
	echo "g_"$server_hostname"_ip_addr=\"\\\""$server_private"\\\";\"" >> pgw1.txt
fi



#if server hostname == sgw1 write server_private to sgw1.txt 


#if server hostname == pgw1 Write server_private to sgw1.txt 
