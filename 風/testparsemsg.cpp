#include "parsemsg.h"

int main() {
	//char* testInput = "35=1`1=1300018`76=8450`117=V0001`55=1260`38=1000`44=20`54=1`80024=20240112141234559`81001=0`81008=I`20000=192.168.103.90`50001=06`50003= `50002=110101`60005=N`20002=";
	char* testInput = "35=999`50008=1`1=1300018`37=10001`38=1`40=2`44=15`50=8450`54=B`55=6016`56=O`59=0`60=20240112134012617`10000=1`10001=0`50001=06`9487=Y`9488=1`50002=110101`50003= `50004=N`50005=N`50009=0`60001=N`60002=N`60003=N`60004=`60005=N`";
    // 建立存儲結果的陣列
    char tags[MAX_TAGS][MAX_TAG_LENGTH];
    char values[MAX_TAGS][MAX_VALUE_LENGTH];
    int numTags = 0;
    parsemsg(testInput,tags,values,numTags);

    for (int i = 0; i < numTags; ++i) {
        std::cout << "TAG: " << tags[i] << " VALUE: " << values[i] << std::endl;
    }
    return 0;
}
