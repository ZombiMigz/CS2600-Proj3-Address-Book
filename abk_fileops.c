#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

void parseLine(AddressBook *address_book, FILE *fp, char *line);

Status load_file(AddressBook *address_book)
{
	// opens file
	FILE *fp = fopen(DEFAULT_FILE, "r");

	// init address book
	ContactInfo(contactInfo)[100] = {{0}};
	address_book->list = &contactInfo[0];
	address_book->fp = fp;
	address_book->count = 0;

	/*
	 * Check for file existance
	 */
	if (fp != NULL)
	{
		// printf("Found file\n");
		/*
		 * Do the neccessary step to open the file
		 * Do error handling
		 */

		char line[999];
		while (fgets(line, sizeof(line), fp))
		{
			// printf("line: |%s|", line);
			parseLine(address_book, fp, line);
		}
	}
	else
	{
		// printf("No file\n");
	}

	return e_success;
}

void parseLine(AddressBook *address_book, FILE *fp, char *line)
{
	ContactInfo info;

	char *tok = strtok(line, ",");
	info.si_no = atoi(tok);
	strcpy(info.name[0], strtok(NULL, ","));
	strcpy(info.phone_numbers[0], strtok(NULL, ","));
	strcpy(info.phone_numbers[1], strtok(NULL, ","));
	strcpy(info.phone_numbers[2], strtok(NULL, ","));
	strcpy(info.phone_numbers[3], strtok(NULL, ","));
	strcpy(info.phone_numbers[4], strtok(NULL, ","));
	strcpy(info.email_addresses[0], strtok(NULL, ","));
	strcpy(info.email_addresses[1], strtok(NULL, ","));
	strcpy(info.email_addresses[2], strtok(NULL, ","));
	strcpy(info.email_addresses[3], strtok(NULL, ","));
	strcpy(info.email_addresses[4], strtok(NULL, ","));

	address_book->list[address_book->count++] = info;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	/*
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */

	fclose(address_book->fp);

	return e_success;
}
