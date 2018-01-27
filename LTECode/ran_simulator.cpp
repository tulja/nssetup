#include "ran_simulator.h"
#include <fstream>


time_t g_start_time;
int g_threads_count;
int bearer_count = 0 ;
uint64_t g_req_dur;
uint64_t g_run_dur;
time_t start ;
int g_tot_regs;
uint64_t g_tot_regstime;
pthread_mutex_t g_mux;
vector<thread> g_umon_thread;
vector<thread> g_dmon_thread;
vector<thread> g_threads;
thread g_rtt_thread;
TrafficMonitor g_traf_mon;
bool scale_down = false;
const int nstars = 150;   // maximum number of stars to distribute
const int nstars1 = 70;   // maximum number of stars to distribute in Load 1
const int nstars2 = 100;   // maximum number of stars to distribute in Load 2 
const int nstars3 = 90;   // maximum number of stars to distribute in Load 3 
const int nstars4 = 80;   // maximum number of stars to distribute in Load 4 
const int nstars5 = 90;   // maximum number of stars to distribute in Load 5 
 const int TimePeriod = 140 ; 
int TimetoStart[nstars] = {0};
 int arrivalsAtTime[TimePeriod*5] = {0};

//const int TIME_INTERVAL  = 140; // Scale Up Scenario 
//const int TIME_INTERVAL  = 10; // Scale Down Scenario




void utraffic_monitor() {
	UdpClient sgw_s1_client;
	int elapsedtime;
	sgw_s1_client.set_client(g_trafmon_ip_addr);
	while (1) {
	
		/*time_t end = time(NULL);
	    elapsedtime = difftime(end, start);
	    if(elapsedtime == 95)
	    	g_traf_mon.update_uplink_info(ip_addr,3,"10.0.3.207", 7100);*/
		//if(scale_down)
			//g_traf_mon.update_uplink_info("172.16.1.6",3,"10.0.3.207", 7100);
	    g_traf_mon.handle_uplink_udata(sgw_s1_client);
	}
	
}
int schedule(){
	
   
    const int nrolls = 10000; // number of experiments

 
   int ueNum = 0 ; 
   
 
  int totalUEsArrivedinSystem = 0;



  for (int i = 0; i < 150; ++i)
  {
  	TimetoStart[i] = 150000;
  }
  
  /*
  std::default_random_engine generator;
  std::poisson_distribution<int> distribution(85.1);

  int p[TimePeriod*2]={};
  int i,j ;

  for ( i=0; i<nrolls; ++i) {
    int number = distribution(generator);
    if (number<TimePeriod) ++p[number];
  }

  //std::cout << "poisson_distribution (mean=70.1):" << std::endl;
  for ( i=0; i<TimePeriod; ++i){
  	//std::cout << i << "   " <<p[i]*nstars/nrolls <<endl;
  	  arrivalsAtTime[i] = p[i]*nstars/nrolls;
  	  totalUEsArrivedinSystem += arrivalsAtTime[i];
    //std::cout << i << ": " << std::string(p[i]*nstars/nrolls,'*') << std::endl;
    //  std::cout << i <<" "<<arrivalsAtTime[i] <<endl;
   
  }
  std::poisson_distribution<int> distribution2(160.1);
   for ( i=0; i<nrolls; ++i) {
    int number = distribution2(generator);                                                                                                                                                        
    if (number<TimePeriod*2) ++p[number];
  }
 //std::cout << "poisson_distribution (mean=180.1):" << std::endl;
  for (j = TimePeriod ; j < 2* TimePeriod ; j++  )
  {
  	    arrivalsAtTime[j] = p[j]*nstars/nrolls;
      //std::cout << j << ": " << std::string(p[j]*nstars/nrolls,'*') << std::endl;
  	      totalUEsArrivedinSystem += arrivalsAtTime[j];
  	   // std::cout << j << "  " <<arrivalsAtTime[j] <<endl;
   
  }

  

      for(int k=0;k < TimePeriod*2;k++)
    {
    	int numberUEatI = arrivalsAtTime[k] ;
    	for(int j = 0 ;j < numberUEatI ; j++)
    	{   
    		TimetoStart[ueNum] = k ; 
    		ueNum++;
    	} 
       
    }

   /* for(int k =0 ; k< 300;k++)
    {
    	cout<<"Time "<<k<<" "<<TimetoStart[k]<<endl;
    }*/

   std::default_random_engine generator;



  std::poisson_distribution<int> distribution(100.1);
  std::cout << "poisson_distribution (mean=70.1):  Load 1 " << std::endl;

  int p[TimePeriod]={};
  int d1 ; // Load1 Dist
  int d2; // Load2 Dist
  int d3; //Load3 Dist
  int d4; // Load4 Dist 
  int d5;//Load5 Dist



  //Load Distribution 1 
    
  
  for ( d1=0; d1<nrolls; ++d1) {
    int number = distribution(generator);
    if (number<TimePeriod) ++p[number];
  }

  
  for ( d1=0; d1<TimePeriod; ++d1){
  	//std::cout << i << "   " <<p[i]*nstars1/nrolls <<endl;
  	  arrivalsAtTime[d1] = p[d1]*nstars1/nrolls;
  	  totalUEsArrivedinSystem += arrivalsAtTime[d1];
    //std::cout << i << ": " << std::string(p[d1]*nstars1/nrolls,'*') << std::endl;
      std::cout << d1 <<" "<<arrivalsAtTime[d1] <<endl;
    //myfile1 << d1 <<" "<<p[d1]*nstars1/nrolls <<endl;
    
  }




   





      for(int k=0;k < TimePeriod;k++)
    {
    	int numberUEatI = arrivalsAtTime[k] ;
    	for(int j = 0 ;j < numberUEatI ; j++)
    	{   
    		TimetoStart[ueNum] = k ; 
    		ueNum++;
    	} 
       
    }

    /*for(int k =0 ; k< 300;k++)
    {
    	cout<<"Time "<<k<<" "<<TimetoStart[k]<<endl;
    }*/




	
}

int is_scheduled(int ran_num)
{ 
	time_t end = time(NULL);
	int elapsedtime = difftime(end, start);
	//TRACE(cout<<"elapsedtime is "<<elapsedtime;)
	if(TimetoStart[ran_num] == elapsedtime) {
         return 1 ; 
	}else 
	{
		return 0;
	}

}


void dtraffic_monitor() {
        while (1) {
        	    //TRACE(cout<<"dtraffic_monitor::"<<"Inside the While loop"<<endl;)
                g_traf_mon.handle_downlink_udata();
        }
}

void ping(){
	string cmd;
	
	cmd = "ping -I 172.16.1.3 172.16.0.2 -c 60 | grep \"^rtt\" >> ping.txt";
	cout << cmd << endl;
	system(cmd.c_str());
}

void transferUdpData(){
	string cmd;

	cmd = "iperf3 -u -B  172.16.1.3  -c   172.16.0.2  -p 55003  -t 10 | tee  -a sample_udp.log";
	cout << cmd << endl;
	system(cmd.c_str());
}
void simulate(int arg) {
	CLOCK::time_point mstart_time;
	CLOCK::time_point mstop_time;
	MICROSECONDS mtime_diff_us;		
	Ran ran;
	int status;
	int ran_num;
	bool ok;
	int qci = 2;
	bool time_exceeded;
   
	ran_num = arg;
	time_exceeded = false;
	ran.init(ran_num);
	ran.conn_mme();
	time_t end ;
	int elapsedtime =0;
	int ueNum = 0 ;
    

	while (1) {
       
		if(is_scheduled(ran_num)){
		// Run duration check
		g_utils.time_check(g_start_time, g_req_dur, time_exceeded);
		if (time_exceeded) {
			break;
		}
        
        TRACE(cout << "ransimulator_simulate:" << "ran_num is " << ran_num << endl;)

		// Start time
		mstart_time = CLOCK::now();	

		// Initial attach
		ran.initial_attach();


		// Authentication
		ok = ran.authenticate();
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " autn failure" << endl;)
			return;
		}

		// Set security
		ok = ran.set_security();
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " security setup failure" << endl;)
			return;
		}

		// Set eps session
		ok = ran.set_eps_session(g_traf_mon);
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " eps session setup failure" << endl;)
			return;
		}

		// TRACE(cout << "ransimulator_simulate:" << "ran_num is "<< ran_num <<"sgw_s1_ip_addr "<< sgw_s1_ip_addr << endl;)
       
		/*
		// To find RTT
		if (ran_num == 0) {
			g_rtt_thread = thread(ping);
			g_rtt_thread.detach();		
		}
		*/ 

		/* Data transfer */
		bearer_count++;		
       //if(ran_num != 3 && ran_num != 4 && ran_num != 6 && ran_num != 7)
		/*
        if(ran_num == 2 )
        ran.transfer_data(g_req_dur);	
        if(ran_num == 0)
        	ran.transfer_data(g_req_dur);
		
		
        if(ran_num == 3 || ran_num == 1){
    	  // Set eps session again for second time for second kind of traffic 
        qci = 1 ;
        ran.add_dedicated_bearer(qci);
        TRACE(cout << "ransimulator_simulate:" << " ran number " << ran_num << endl;)
        TRACE(cout << "ransimulator_simulate:" << " another bearer is being setup for ran_num" <<ran_num << endl;)
		ok = ran.set_eps_session(g_traf_mon);
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " eps session setup failure" << endl;)
			return;
		}
		//ran.set_data_plane(1);
		ran.transfer_data(g_req_dur);

        TRACE(cout << "ransimulator_simulate:" << " data transfer done on the Dedicated Bearer " << endl;)
        }
        
         if(ran_num == 4 || ran_num == 9){
    	  // Set eps session again for second time for second kind of traffic 
        // 	qci = 2 ;
        ran.add_dedicated_bearer(2);
        TRACE(cout << "ransimulator_simulate:" << " ran number " << ran_num << endl;)
       TRACE(cout << "ransimulator_simulate:" << " another bearer is being setup for ran_num" <<ran_num << endl;)
		ok = ran.set_eps_session(g_traf_mon);
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " eps session setup failure" << endl;)
			return;
		}
		ran.transfer_data(g_req_dur);
        TRACE(cout << "ransimulator_simulate:" << " data transfer done on the Dedicated Bearer " << endl;)
        }
        
         if(ran_num == 6 || ran_num == 5 || ran_num == 8 || ran_num == 7){
    	  // Set eps session again for second time for second kind of traffic
    	//  qci = 3 ; 
        ran.add_dedicated_bearer(3);
        TRACE(cout << "ransimulator_simulate:" << " ran number " << ran_num << endl;)
      TRACE(cout << "ransimulator_simulate:" << " another bearer is being setup for ran_num" <<ran_num << endl;)
		ok = ran.set_eps_session(g_traf_mon);
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " eps session setup failure" << endl;)
			return;
		}
		ran.transfer_data(g_req_dur);
        TRACE(cout << "ransimulator_simulate:" << " data transfer done on the Dedicated Bearer " << endl;)
        }
        */
         if(ran_num >=0 && ran_num < 450){
    	  // Set eps session again for second time for second kind of traffic 
         	qci = 1 ;
        ran.add_dedicated_bearer(1,ran_num);
        TRACE(cout << "ransimulator_simulate:" << " ran number " << ran_num << endl;)
        TRACE(cout << "ransimulator_simulate:" << " another bearer is being setup for ran_num" <<ran_num << endl;)
		ok = ran.set_eps_session(g_traf_mon);
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " eps session setup failure" << endl;)
			return;
		}
		//ip_addr = ran.ran_ctx.ip_addr;
		//s1_uteid_ul = ran.ran_ctx.s1_uteid_ul;
		//ran.transfer_data(g_req_dur);
		ran.scale_down(g_traf_mon,ran_num);
		//ran.transfer_data(ran_num);
        TRACE(cout << "ransimulator_simulate:" << " data transfer done on the Dedicated Bearer " << endl;)
        }
        
        
		/*// Detach
		ok = ran.detach(ran_num);
		if (!ok) {
			TRACE(cout << "ransimulator_simulate:" << " detach failure" << endl;)
			return;
		}else{
			//print_results(ran_num);
		}*/

		// Stop time
		mstop_time = CLOCK::now();
		
		// Response time
		mtime_diff_us = std::chrono::duration_cast<MICROSECONDS>(mstop_time - mstart_time);

		/* Updating performance metrics */
		g_sync.mlock(g_mux);
		g_tot_regs++;
		g_tot_regstime += mtime_diff_us.count();		
		g_sync.munlock(g_mux);			
	}
}}

void check_usage(int argc) {
	if (argc < 3) {
		TRACE(cout << "Usage: ./<ran_simulator_exec> THREADS_COUNT DURATION" << endl;)
		g_utils.handle_type1_error(-1, "Invalid usage error: ransimulator_checkusage");
	}
}

void init(char *argv[]) {
	g_start_time = time(0);
	g_threads_count = atoi(argv[1]);
	g_req_dur = atoi(argv[2]);
	g_tot_regs = 0;
	g_tot_regstime = 0;
	g_sync.mux_init(g_mux);	
	g_umon_thread.resize(NUM_MONITORS);
	g_dmon_thread.resize(NUM_MONITORS);
	g_threads.resize(g_threads_count);
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	int i;

	/* Tun */
	g_traf_mon.tun.set_itf("tun1", "172.16.0.1/24");
	g_traf_mon.tun.conn("tun1");

	/* Traffic monitor server */
	TRACE(cout << "Traffic monitor server started" << endl;)
	g_traf_mon.server.run(g_trafmon_ip_addr, g_trafmon_port);	

	// Uplink traffic monitor
	for (i = 0; i < NUM_MONITORS; i++) {
		g_umon_thread[i] = thread(utraffic_monitor);
		g_umon_thread[i].detach();		
	}

	// Downlink traffic monitor
	for (i = 0; i < NUM_MONITORS; i++) {
		g_dmon_thread[i] = thread(dtraffic_monitor);
		g_dmon_thread[i].detach();			
	}
	
	// Simulator threads
	for (i = 0; i < g_threads_count; i++) {
		g_threads[i] = thread(simulate, i);
	}	
	for (i = 0; i < g_threads_count; i++) {
		if (g_threads[i].joinable()) {
			g_threads[i].join();
		}
	}	
	//print_results();
}

void print_results(int ran_num) {
	g_run_dur = difftime(time(0), g_start_time);
	TRACE(cout << "ran number is "<<ran_num <<endl;)
	TRACE(cout << "Requested duration has ended. Finishing the program." << endl;)
	TRACE(cout << "Total number of registrations is " << g_tot_regs << endl;)
	TRACE(cout << "Total time for registrations is " << g_tot_regstime * 1e-6 << " seconds" << endl;)
	TRACE(cout << "Total run duration is " << g_run_dur << " seconds" << endl;)
	TRACE(cout << "Latency is " << ((double)g_tot_regstime/g_tot_regs) * 1e-6 << " seconds" << endl;)
	TRACE(cout << "Throughput is " << ((double)g_tot_regs/g_run_dur) << endl;)	
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	start = time(NULL);
	schedule();
	run();
	cout <<"Printing The Results";
	//print_results();
	return 0;
}
