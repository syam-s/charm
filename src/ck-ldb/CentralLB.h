/*****************************************************************************
 * $Source$
 * $Author$
 * $Date$
 * $Revision$
 *****************************************************************************/

/**
 * \addtogroup CkLdb
*/
/*@{*/

#ifndef CENTRALLB_H
#define CENTRALLB_H

#include "BaseLB.h"
#include "CentralLB.decl.h"
#include "LBSimulation.h"

extern CkGroupID loadbalancer;

#define PER_MESSAGE_SEND_OVERHEAD   35e-6
#define PER_BYTE_SEND_OVERHEAD      8.5e-9
#define PER_MESSAGE_RECV_OVERHEAD   0.0
#define PER_BYTE_RECV_OVERHEAD      0.0

void CreateCentralLB();
void set_avail_vector(char * bitmap);

class CLBStatsMsg;
class LBSimulation;

/// for backward compatibility
typedef LBMigrateMsg  CLBMigrateMsg;

class CentralLB : public CBase_CentralLB
{
public:
  CentralLB();
  ~CentralLB();
  CentralLB(CkMigrateMessage *m):CBase_CentralLB(m) {}

  int useDefCtor(void){ return 1; }
  static void staticAtSync(void*);
  void AtSync(void); // Everything is at the PE barrier
  void ProcessAtSync(void); // Receive a message from AtSync to avoid
                            // making projections output look funny

  void ReceiveStats(CLBStatsMsg *); 		// Receive stats on PE 0
  void ResumeClients(void);                     // Resuming clients needs
	                                        // to be resumed via message
  void ReceiveMigration(LBMigrateMsg *); 	// Receive migration data

  // manuall start load balancing
  inline void StartLB() { thisProxy.ProcessAtSync(); }
  static void staticStartLB(void* data);

  // Migrated-element callback
  static void staticMigrated(void* me, LDObjHandle h);
  void Migrated(LDObjHandle h);

  void MigrationDone(int balancing);  // Call when migration is complete
  int step() { return mystep; };

  void set_avail_vector(char *new_vector);

   struct LDOId {
     LDObjid oid;
     LDOMid mid;
   };

  struct ProcStats {  // per processor data
    double total_walltime;
    double total_cputime;
    double idletime;
    double bg_walltime;
    double bg_cputime;
    int pe_speed;
    double utilization;
    CmiBool available;
    int   n_objs;
    ProcStats(): total_walltime(0.0), total_cputime(0.0), idletime(0.0),
	   	 bg_walltime(0.0), bg_cputime(0.0), pe_speed(1),
		 utilization(1.0), available(1), n_objs(0) {}
  };

  struct LDStats {  // Passed to Strategy
    struct ProcStats  *procs;
    int count;
    
    int n_objs;
    LDObjData* objData;
    int  *from_proc, *to_proc;
    int n_comm;
    LDCommData* commData;

    LDOId *transTable;
    int *objHash;

    LDStats(): n_objs(0), n_comm(0) { objData = NULL; commData = NULL; 
		from_proc = NULL; to_proc = NULL;
		transTable = NULL; objHash = NULL; }
      // build hash table
    void makeCommHash();
    void deleteCommHash();
    int getHash(LDObjid oid, LDOMid mid);
    void clear() {
      n_objs = n_comm = 0;
      delete [] objData;
      delete [] commData;
      delete [] from_proc;
      delete [] to_proc;
      deleteCommHash();
    }
    void pup(PUP::er &p);
    int useMem();
  };

   LBMigrateMsg* callStrategy(LDStats* stats,int count){
	return Strategy(stats,count);
   };

  int cur_ld_balancer;
  char *avail_vector;
  /* for Node 0 */
  int new_ld_balancer;

  void readStatsMsgs(const char* filename);
  void writeStatsMsgs(const char* filename);

protected:
  virtual CmiBool QueryBalanceNow(int) { return CmiTrue; };  
  virtual CmiBool QueryDumpData() { return CmiFalse; };  
  virtual LBMigrateMsg* Strategy(LDStats* stats,int count);
  virtual void work(LDStats* stats,int count);
  virtual LBMigrateMsg * createMigrateMsg(LDStats* stats,int count);

  void simulation();
  void FindSimResults(LDStats* stats, int count, LBMigrateMsg* msg, LBSimulation* simResults);
#if 0
  void RemoveNonMigratable(LDStats* statsDataList, int count);
#endif
  void buildStats();

private:  

  int mystep;
  int myspeed;
  int stats_msg_count;
  CLBStatsMsg **statsMsgsList;
  LDStats *statsData;
  int migrates_completed;
  int migrates_expected;
  double start_lb_time;

public:
  int useMem();
};

class CLBStatsMsg : public CMessage_CLBStatsMsg {
public:
  int from_pe;
  int serial;
  int pe_speed;
  double total_walltime;
  double total_cputime;
  double idletime;
  double bg_walltime;
  double bg_cputime;
  int n_objs;
  LDObjData *objData;
  int n_comm;
  LDCommData *commData;

  char * avail_vector;
  int next_lb;
}; 

#endif /* CENTRALLB_H */

/*@}*/


