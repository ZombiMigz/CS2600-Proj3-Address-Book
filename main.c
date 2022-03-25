#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#include "address_book.h"
#include "abk_fileops.h"
#include "abk_menus.h"

int main(void)
{
	AddressBook address_book;
	Status ret;

	/* Load the file from .csv file if any */
	ret = load_file(&address_book);

	if (ret == e_success)
	{
		/* Show all the available menu */
		ret = menu(&address_book);

		if (ret == e_success)
		{
			/* Save the entries */
			save_prompt(&address_book);
		}
	}

	return 0;
}
