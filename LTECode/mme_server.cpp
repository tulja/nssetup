#include "mme_server.h"
#include "sync.h"
#include <fstream>
using namespace std;
Mme g_mme;
int g_workers_count;
vector<SctpClient> hss_clients;
vector<UdpClient> sgw_s11_clients;
pthread_mutex_t g_mux;
int qci_number[5] = {0,0,0,0,0};
int UE2Slice[100][2] = {0};
uint64_t guti[100] = {0};
Packet pkt_session[100];
int ran_num2wrk_id[100];
int numberOfActiveInstances = 0 ;
int numberOfActiveInstancesdetach = 0 ;
string sgw1_ip,pgw1_ip;
const int SLICE_LIMIT = 25;
const int NUMBER_OF_INSTANCES = 5 ;


int Instance[NUMBER_OF_INSTANCES] = {0} ; 
int Temp_Instance[NUMBER_OF_INSTANCES] = {0} ;
string Instance_SGW[NUMBER_OF_INSTANCES] = {"10.0.3.240","10.0.3.240","10.0.3.206","10.0.3.240","10.0.3.206"};
string Instance_PGW[NUMBER_OF_INSTANCES] = {"10.0.3.112","10.0.3.112","10.0.3.134","10.0.3.112","10.0.3.134"};

int ran_num ; 
CLOCK::time_point mstart_time;


int minimum(int x,int y)
{
	if(x < y )
		return x; 
	else 
		return y;
}

int maximumOfTempArrayofInstances()
{
   int max = Temp_Instance[0];
   for (int i = 1; i < NUMBER_OF_INSTANCES; ++i)
   {
     if(max < Temp_Instance[i])
         max = Temp_Instance[i];
   }
   return max;
}

int indexOfmaxOftemp()
{
  int max = Temp_Instance[0];
  int index = 0 ;
   for (int i = 1; i < NUMBER_OF_INSTANCES; ++i)
   {
     if(max < Temp_Instance[i]){
         max = Temp_Instance[i];
         index = i ;
       }
   }
   return index;
}

void check_usage(int argc) {
	if (argc < 2) {
		TRACE(cout << "Usage: ./<mme_server_exec> THREADS_COUNT" << endl;)
		g_utils.handle_type1_error(-1, "Invalid usage error: mmeserver_checkusage");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
	hss_clients.resize(g_workers_count);
	sgw_s11_clients.resize(g_workers_count);
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	int i;
	
	TRACE(cout << "MME server started" << endl;)
	 g_sync.mux_init(g_mux);
	
	for (i = 0; i < g_workers_count; i++) {
		hss_clients[i].conn(g_hss_ip_addr, g_hss_port);	
		TRACE(cout << "mme_server:inside run method  g_sgw_s11_ip_addr is  " << g_sgw_s11_ip_addr << endl;)
		sgw_s11_clients[i].conn(g_mme_ip_addr, g_sgw_s11_ip_addr, g_sgw_s11_port);
	}
	
	g_mme.server.run(g_mme_ip_addr, g_mme_port, g_workers_count, handle_ue);
}

int handle_ue(int conn_fd, int worker_id) {
	bool res;
  bool ue20 = false;
	Packet pkt;
    int qci,ue_no;
     ofstream myfile[NUMBER_OF_INSTANCES];
     ofstream instanceVStimefile,sysLoadFile;
     int systemLoadDetach,systemLoadAttach;
    std::fstream sgw1("/home/ubuntu/sgw1.txt", std::ios_base::in);    
    sgw1 >> sgw1_ip;
    std::fstream pgw1("/home/ubuntu/pgw1.txt", std::ios_base::in); 
    pgw1 >> pgw1_ip;
   MICROSECONDS mtime_diff_us;		

	CLOCK::time_point mstop_time;
     double elapsedTime;
    /* myfile1.open("Instance1.txt",std::ios_base::app);
      myfile2.open("Instance2.txt",std::ios_base::app);
       myfile3.open("Instance3.txt",std::ios_base::app);
        myfile4.open("Instance4.txt",std::ios_base::app);
        myfile5.open("Instance5.txt",std::ios_base::app);*/
     for (int i = 0; i < NUMBER_OF_INSTANCES; ++i)
     {
     	myfile[i].open("Instance"+to_string(i)+".txt",std::ios_base::app);
     }

       instanceVStimefile.open("InstancesVStime.txt",std::ios_base::app);
       sysLoadFile.open("systemLoad.txt",std::ios_base::app);
       
	g_mme.server.rcv(conn_fd, pkt);
        TRACE(cout << "mmeserver_handleue:" << "Packet received from RAN" << endl;)

	if (pkt.len <= 0) {
		TRACE(cout << "mmeserver_handleue:" << " Connection closed" << endl;)
		return 0;
	}
	pkt.extract_s1ap_hdr();
	if (pkt.s1ap_hdr.mme_s1ap_ue_id == 0) {
		switch (pkt.s1ap_hdr.msg_type) {
			/* Initial Attach request */
			case 1: 
				TRACE(cout << "mmeserver_handleue:" << " case 1: initial attach" << endl;)
				g_mme.handle_initial_attach(conn_fd, pkt, hss_clients[worker_id]);
				break;
                        
                        /*add dedicated bearer*/
                        case 6:
                            TRACE(cout << "mmeserver_handleue:" << " case 6: dedicated bearer request" << endl;)
                           pkt.extract_item(qci);
                           TRACE(cout << "mmeserver_handleue:" << " qci extracted is "<<qci << endl;)
                           switch(qci){
                           	case 1:
                           	       TRACE(cout << "mmeserver_handleue:" << "Data Traffic will be redirected to Slice no "<<qci << endl;)
                           	       ///SPGW SLICE 1 
                           	   //string sgw1_ip;
                                   //sgw1 >> sgw1_ip;
                                   //string pgw1_ip;
                                   //pgw1 >> pgw1;   
                                   g_sgw_s1_ip_addr = sgw1_ip;  
                                   g_sgw_s11_ip_addr = sgw1_ip;
                                   g_sgw_s5_ip_addr = sgw1_ip;
                                   g_pgw_s5_ip_addr = pgw1_ip; 
                           	       break;
                           	case 2:
                           	       TRACE(cout << "mmeserver_handleue:" << "Data Traffic will be redirected to Slice no "<<qci << endl;)
                           	       ///SPGW SLICE 2
                                   g_sgw_s1_ip_addr = "10.0.3.81";  
                                   g_sgw_s11_ip_addr = "10.0.3.81";
                                   g_sgw_s5_ip_addr = "10.0.3.81";
                                   g_pgw_s5_ip_addr = "10.0.3.65";
                           	       break;
                           	case 3: 
                           	       TRACE(cout << "mmeserver_handleue:" << "Data Traffic will be redirected to Slice no "<<qci << endl;)
                           	       ///SPGW SLICE 3
                                   g_sgw_s1_ip_addr = "10.0.3.97";  
                                   g_sgw_s11_ip_addr = "10.0.3.97";
                                   g_sgw_s5_ip_addr = "10.0.3.97";
                                   g_pgw_s5_ip_addr = "10.0.3.19";
                                   break;
                            case 4:
                                   TRACE(cout << "mmeserver_handleue:" << "Data Traffic will be redirected to Slice no "<<qci << endl;)
                                    ///SPGW SLICE 4
                                   g_sgw_s1_ip_addr = "10.0.3.188";  
                                   g_sgw_s11_ip_addr = "10.0.3.188";
                                   g_sgw_s5_ip_addr = "10.0.3.188";
                                   g_pgw_s5_ip_addr = "10.0.3.11";       
                           	       break;       

                           }
                            //system("whiptail --textbox dedicated_bearer 12 80");
                            
                            TRACE(cout << "mme_server_handle_ue ::  g_sgw_s11_ip_addr is  " << g_sgw_s11_ip_addr << endl;)
                            //sgw_s11_clients[worker_id].clo();
                            sgw_s11_clients[worker_id].reconn(g_mme_ip_addr,g_sgw_s11_ip_addr,g_sgw_s11_port);
                            
                            g_mme.handle_create_session(conn_fd, pkt, sgw_s11_clients[worker_id]);


                            break;

			/* For error handling */
			default:
				TRACE(cout << "mmeserver_handleue:" << " default case: new" << endl;)
				break;
		}		
	}
	else if (pkt.s1ap_hdr.mme_s1ap_ue_id > 0) {
		switch (pkt.s1ap_hdr.msg_type) {
			/* Authentication response */
			case 2: 
				TRACE(cout << "mmeserver_handleue:" << " case 2: authentication response" << endl;)
				res = g_mme.handle_autn(conn_fd, pkt);
				if (res) {
					g_mme.handle_security_mode_cmd(conn_fd, pkt);
				}
				break;

			/* Security Mode Complete */
			case 3: 
				TRACE(cout << "mmeserver_handleue:" << " case 3: security mode complete" << endl;)
				res = g_mme.handle_security_mode_complete(conn_fd, pkt);
        // guti[worker_id]=g_mme.get_guti(pkt);
        
				if (res) {
					// g_mme.handle_location_update(pkt, hss_clients[worker_id]);
          pkt_session[worker_id] = pkt;
					g_mme.handle_create_session(conn_fd, pkt, sgw_s11_clients[worker_id]);
				}
				break;

			/* Attach Complete */
			case 4: 
				TRACE(cout << "mmeserver_handleue:" << " case 4: attach complete" << endl;)
				g_mme.handle_attach_complete(pkt);
				g_mme.handle_modify_bearer(pkt, sgw_s11_clients[worker_id]);
				break;

			/* Detach request */
			case 5: 
			 g_sync.mlock(g_mux);
        pkt.extract_item(ran_num);
         (qci_number[UE2Slice[ran_num][0]])--;
         switch(UE2Slice[ran_num][1]){
              case 0 : 
                    (Instance[0])--; 
                    break;
              case 1 : 
                    (Instance[1])--; 
                    break;
                    
              case 2 : 
                    (Instance[2])--; 
                    break;  
              case 3 :
                    (Instance[3])--;    
                    break;
              case 4:
                    (Instance[4])--;
                    break; 
              case 5:
                    (Instance[5])--;
                    break;
              case 6:
                    (Instance[6])--;
                    break;
              case 7:
                    (Instance[7])--;
                    break;
              case 8:
                    (Instance[8])--;
                    break;
              case 9:
                    (Instance[9])--;
                    break;
              case 10:
                    (Instance[10])--;
                    break;
              case 11:
                    (Instance[11])--;
                    break;
                                                   
         }
         g_sync.munlock(g_mux);

          /*TRACE(cout<<"case-5 detach"<<"Value of  Instance[0] is "<<Instance[0]<<endl;)
          TRACE(cout<<"case-5 detach"<<"Value of  Instance[1] is "<<Instance[1]<<endl;)
          TRACE(cout<<"case-5 detach"<<"Value of  Instance[2] is "<<Instance[2]<<endl;)
          TRACE(cout<<"case-5 detach"<<"Value of  Instance[3] is "<<Instance[3]<<endl;)
          TRACE(cout<<"case-5 detach"<<"Value of  Instance[4] is "<<Instance[4]<<endl;)*/
         for (int i = 0; i < NUMBER_OF_INSTANCES; ++i)
         {
         	TRACE(cout<<"case-5 detach"<<"Value of  Instance["<<i<<"] is "<<Instance[i]<<endl;)
         }

          systemLoadDetach = 0 ;
 
          for (int i = 0; i < NUMBER_OF_INSTANCES; ++i)
          {
          	 systemLoadDetach += Instance[i];
          }
       
           numberOfActiveInstancesdetach = 0 ;
           for(int i =0 ;i < NUMBER_OF_INSTANCES ;i++)
           {
                if(Instance[i] > 0)
                      {
                         numberOfActiveInstancesdetach++;
                      }

           }



          mstop_time = CLOCK::now();
          mtime_diff_us = std::chrono::duration_cast<MICROSECONDS>(mstop_time - mstart_time);
       
          for (int i = 0; i < NUMBER_OF_INSTANCES; ++i)
          {
          
             myfile[i] << mtime_diff_us.count()/1000000.0 <<"  "<<Instance[i]<<endl;

          }

          sysLoadFile << mtime_diff_us.count()/1000000.0 << "  " << systemLoadDetach <<endl;
             instanceVStimefile << mtime_diff_us.count()/1000000.0 << "  " << numberOfActiveInstancesdetach<<endl;
          
          TRACE(cout<<"case-5 detach ::elapsedTime is "<<mtime_diff_us.count()/1000000.0;)
        
          
				TRACE(cout << "mmeserver_handleue:" << " case 5: detach request" << endl;)
				g_mme.handle_detach(conn_fd, pkt, sgw_s11_clients[worker_id]);
				break;
                      /*add dedicated bearer*/
             case 6:
                           TRACE(cout << "mmeserver_handleue:" << " case 6: dedicated bearer request" << endl;)
                           pkt.extract_item(qci);
                           pkt.extract_item(ran_num);
                           TRACE(cout << "mmeserver_handleue:" << " qci extracted is "<<qci << endl;)
                           switch(qci){
                           	case 1:
                           	       TRACE(cout << "mmeserver_handleue:" << "Data Traffic will be redirected to Slice no "<<qci << endl;)
                           	    
                           	       g_sync.mlock(g_mux);
                                 
                                   g_sgw_s1_ip_addr = sgw1_ip;  
                                   g_sgw_s11_ip_addr = sgw1_ip;
                                   g_sgw_s5_ip_addr = sgw1_ip;
                                   g_pgw_s5_ip_addr = pgw1_ip; 
                                  
                         
                                   g_sync.munlock(g_mux);
                                   mstop_time = CLOCK::now();
                                   mtime_diff_us = std::chrono::duration_cast<MICROSECONDS>(mstop_time - mstart_time);     
                           	       break;
                      

                           }
                            //system("whiptail --textbox dedicated_bearer 12 80");
                            
                            TRACE(cout << "mme_server_handle_ue ::  g_sgw_s11_ip_addr is  " << g_sgw_s11_ip_addr << endl;)
                            //sgw_s11_clients[worker_id].clo();
                            sgw_s11_clients[worker_id].reconn(g_mme_ip_addr,g_sgw_s11_ip_addr,g_sgw_s11_port);
                            ran_num2wrk_id[ran_num] = worker_id;
                            g_mme.handle_create_session(conn_fd, pkt, sgw_s11_clients[worker_id]);
                             
                             // For Testing the MME to RAN Slice Change Message (Worked Fine!!)
                            /*sleep(20);
                            
                            g_sgw_s1_ip_addr = "10.0.3.206";  
                            g_sgw_s11_ip_addr = "10.0.3.206";
                            g_sgw_s5_ip_addr = "10.0.3.206";
                            g_pgw_s5_ip_addr = "10.0.3.134"; 
                             
                            sgw_s11_clients[worker_id].reconn(g_mme_ip_addr,g_sgw_s11_ip_addr,g_sgw_s11_port);
                            g_mme.handle_create_session(conn_fd, pkt, sgw_s11_clients[worker_id]);*/
                            
                            break;
			/* For error handling */	
			default:
				TRACE(cout << "mmeserver_handleue:" << " default case: attached" << endl;)
				break;
		}				
	}		

	return 1;

}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
    mstart_time = CLOCK::now();	
	run();
	return 0;
}
