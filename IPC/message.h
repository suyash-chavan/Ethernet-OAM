#define MCF_MA_DATA_indication              1
#define MCF_MA_DATA_request                 2
#define CTL_OAMI_indication                 3
#define CTL_OAMI_request                    4
#define LBF_OAMI_request                    5
#define OAMPDU_indication                   6
#define OAMPDU_request                      7

struct _message{

	long int mtype;

	unsigned char data[1600];

};
