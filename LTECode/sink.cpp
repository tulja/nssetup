#include "sink.h"
#include <string>
#include <map>


//std::string pgw_sgi_ip_addr[10];// = "10.0.3.79";
string g_pgw_sgi_ip_addr ; 
string g_sink_ip_addr = "10.0.3.92";
string incoming_pgw_ip ="";
int g_pgw_sgi_port = 8100;
int g_sink_port = 8500;
int conn_count = 0 ; 

/*
void TrafficMonitor::handle_uplink_udata() {
	struct sockaddr_in src_sock_addr;
	Packet pkt;
	int qci = 0 ;
    
	// TRACE(cout << "sink_handleuplinkudata:" << " Data pending from pgw" << endl;)
	server.rcv(src_sock_addr, pkt);
    //pkt.extract_item(qci);
    //TRACE(cout<<"qci extracted is "<<qci<<endl;)
	pkt.extract_gtp_hdr();
		switch(pkt.gtp_hdr.msg_type) {
			// Create session 
			case 1:
			tun.snd(pkt);
			 break;
			case 6:
			pkt.extract_item(qci);
			switch(qci)
			{
				case 1 :
				       g_pgw_sgi_ip_addr = "10.0.3.77"; 
				       break;
				case 2 :
				       break;
				case 3 :
				       break;
				case 4 :
				       break;
				              
			}
			break;
		}





	

    



}
*/





void TrafficMonitor::handle_uplink_udata() {
	char srcPGW[11];
	struct sockaddr_in src_sock_addr;
	string ip_addr;
	Packet pkt;
    
    int qci = 0;
    int i ;

	// TRACE(cout << "sink_handleuplinkudata:" << " Data pending from pgw" << endl;)
	server.rcv(src_sock_addr, pkt);
	struct sockaddr_in* ipv4add = (struct sockaddr_in*)&src_sock_addr;
	struct in_addr ipAddr = ipv4add->sin_addr;
	inet_ntop(AF_INET,&ipAddr,srcPGW,11);
	//pkt.extract_gtp_hdr();
    //TRACE(cout<<"TrafficMonitor::handle_uplink_udata() :: pkt.gtp_hdr.msg_type "<<pkt.gtp_hdr.msg_type<<endl;)
     ip_addr = g_nw.get_src_ip_addr(pkt);
  /*  for ( i = 0; i < 3; ++i)
    {
    	pgw_sgi_ip_addr[i] = "10.0.3.79";
    }
    for ( i = 3; i < 6; ++i)
    {
    	pgw_sgi_ip_addr[i] = "10.0.3.69";
    }
    for ( i = 6; i < 10; ++i)
    {
    	pgw_sgi_ip_addr[i] = "10.0.3.19";
    }*/
    TRACE(cout<<"Src PGW ip of packet is "<<srcPGW<<endl;)
    incoming_pgw_ip = srcPGW;
   /* if(ip_addr == "0.0.0.0")
    {
    	//pkt.extract_gtp_hdr();
    	pkt.extract_item(qci);
    	//if (qci == 1)
    	//////	 pgw_sgi_ip_addr = "10.0.3.77";

        TRACE(cout<<"TrafficMonitor::handle_uplink_udata() :: pkt.qci "<<qci<<endl;)
    }
    else  */
	tun.snd(pkt);
}






void TrafficMonitor::handle_downlink_udata() {
	UdpClient pgw_sgi_client;
	Packet pkt;
	 string ip_addr;
  
	// TRACE(cout << "sink_handledownlinkudata:" << " Data pending at tun" << endl;)
	tun.rcv(pkt);
	 ip_addr = g_nw.get_dst_ip_addr(pkt);
	 conn_count++;
	 /*
	 if(ip_addr == "172.16.1.6" || ip_addr == "172.16.1.4")
	 	g_pgw_sgi_ip_addr = "10.0.3.77";
	 else if (ip_addr == "172.16.1.7" || ip_addr == "172.16.1.12")
	 	g_pgw_sgi_ip_addr = "10.0.3.65";
	 else if (ip_addr == "172.16.1.9" || ip_addr == "172.16.1.8" || ip_addr == "172.16.1.11" || ip_addr == "172.16.1.10"  )
	 	g_pgw_sgi_ip_addr = "10.0.3.19";
	 else if (ip_addr == "172.16.1.14" )
	 	g_pgw_sgi_ip_addr = "10.0.3.11";
	 else
	 	g_pgw_sgi_ip_addr = "10.0.3.79";
	 	*/
	 /*TRACE(cout<<"TrafficMonitor::handle_downlink_udata() :: conn_count = "<<conn_count<<endl;) 
	if(conn_count < 20){
		 g_pgw_sgi_ip_addr = "10.0.3.112";

	} else if (conn_count >= 21 && conn_count < 40){
		 g_pgw_sgi_ip_addr = "10.0.3.134";

	}else{*/
         g_pgw_sgi_ip_addr = incoming_pgw_ip;
	//}
   
	//for (int i = 0; i < 10; ++i)
	//{
		pgw_sgi_client.conn(g_sink_ip_addr, g_pgw_sgi_ip_addr, g_pgw_sgi_port);
		pgw_sgi_client.snd(pkt);
	//}
	
	
}
