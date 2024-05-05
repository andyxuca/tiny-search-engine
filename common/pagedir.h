/*
* pagedir.h     Andy Xu     April 26, 2023
*
* Description: Header file for pagedir
*/

#include <stdbool.h>
#include "webpage.h"

/*
* pagedir_init() - initializes a page directory
* 
* Caller provides a valid pageDirectory name.
* pagedir_init() creates a .crawler file in the directory and 
* opens the file for writing. It closes the file when the writing
* is complete.
*/
bool pagedir_init(const char* pageDirectory);


/*
* pagedir_save() - saves a webpage into a file
*
* Caller provides a valid webpage, a working pageDirectory, and a unique ID.
* pagedir_save() will construct a pathname for the page file in pageDirectory,
* open that file for writing, print the URL, depth, and contents of the webpage, 
* and then close the file.
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/*
* pagedir_validate() - verifies whether given directory is crawler-produced
*
* Caller provides a string pageDirectory.
* pagedir_validate() will check if the pageDirectory exists and if it contains
* a file called ".crawler", which indicates that it is a crawler-produced directory.
* True is returned if both cases are met, false otherwise.
*/
bool pagedir_validate(const char* pageDirectory);

/*
* pagedir_load() - loads a webpage based on a given a pageDirectory and docID
*
* Caller provides a string pageDirectory and int docID
* pagedir_load() will create a webpage based on the URL, depth, and HTML stored in 
* the file of the given directory with the docID.
*/
webpage_t* pagedir_load(const char* pageDirectory, const int docID);