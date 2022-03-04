#include<vector>
#include<queue>

const int BUCKETSIZE = 3;
const int bucket_capicity[BUCKETSIZE] = {8,5,3};
const int bucket_init_state[BUCKETSIZE] = {8,0,0};
const int bucket_final_state[BUCKETSIZE] = {4,4,0};
typedef struct Action{
    int weight;
    int from;
    int to;
}ACTION;
class bucketState{
    int bucket[BUCKETSIZE];
    ACTION actionToThis;
};


