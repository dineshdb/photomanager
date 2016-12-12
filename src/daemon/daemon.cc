#include <iostream>
#include <string>

#include <sys/types.h>
#include <unistd.h>



using namespace std;
/**
 * Daemon process that executes in the background.
 * Returns immediately forking a child daemon process. The parent process ceases to exist
 * while the child process runs as daemon process for serving its purpose.
 * The functions of this daemon is to 
 *		- watch for given folder
 *		- respond to changes by updating database
 *
 */
int main() {
   pid_t pID = fork();
   if (pID == 0) {
    } else if (pID < 0) {
        cerr << "Failed to fork" << endl; // Error while initializing, exit the parent.
        exit(1);
    } else {
      exit(0);
    }
	sleep(5);
	cout << "elasdf" << endl;
    return 0;
}
