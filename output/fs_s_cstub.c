#include "cidl_gen.h"

struct track_block {
	int tid;
	struct track_block *next, *prev;
};
struct track_block tracking_block_list[MAX_NUM_SPDS];

    /* this is just a fake main function for testing. Remove it later  */
int main()
{
	return 0;
}
