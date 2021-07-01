#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

enum _boolean {FALSE, TRUE};

enum _reception_status {receiveOK,}; // Here values are not with us

enum _local_link_status {FAIL, OK};

enum _local_oam_enable {DISABLE, ENABLE};

enum _local_oam_mode {PASSIVE, ACTIVE};

enum _local_mux_action {mux_FWD, mux_DISCARD};

enum _local_par_action {par_DISCARD, par_LB, par_FWD};

enum _local_pdu {RX_INFO, LF_INFO, INFO, ANY};

enum _pdu_req {NONE, CRITICAL, NORMAL};

struct _shared_memory
{
  enum _boolean BEGIN;
  
  enum _boolean local_critical_event;
  
  enum _boolean local_dying_gasp;
  
  enum _boolean local_lost_link_timer_done;
  
  enum _boolean local_satisfied;
  
  enum _boolean local_stable;
  
  enum _boolean remote_stable;
  
  enum _boolean remote_state_valid;
  
  enum _boolean local_unidirectional;

  enum _reception_status reception_status; // Here values are not with us

  enum _local_link_status local_link_status;

  enum _local_oam_enable local_oam_enable;

  enum _local_oam_mode local_oam_mode;

  enum _local_par_action local_par_action;

  enum _local_mux_action local_mux_action;

  enum _local_pdu local_pdu;

  enum _pdu_req pdu_req;
  
  unsigned char source_address[6];
  
  unsigned int parser_postbox_size;
  
  unsigned int multiplexer_postbox_size;
  
  unsigned int oam_client_postbox_size;
  
  unsigned int mac_client_postbox_size;
  
  unsigned int control_postbox_size;
};


  enum _boolean remote_loopback_support;
  
  enum _boolean link_event_support;
  
  enum _boolean variable_retrieval_support;

  unsigned char source_address[6];

  unsigned int local_oam_version;

  unsigned int local_tlv_revision;

  
