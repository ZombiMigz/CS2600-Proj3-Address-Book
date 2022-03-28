#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

void parseLine(AddressBook *address_book, FILE *fp, char *line);

// saving and storing information for the addressbook
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
	fclose(fp);
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
	strcpy(info.email_addresses[4], strtok(NULL, "\n"));
	if (!strcmp(info.name[0], "0"))
	{
		strcpy(info.name[0], "");
	}
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(info.phone_numbers[i], "0"))
		{
			strcpy(info.phone_numbers[i], "");
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(info.email_addresses[i], "0"))
		{
			strcpy(info.email_addresses[i], "");
		}
	}

	address_book->list[address_book->count++] = info;
}

// saving to file
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

	char save[32 * 4 * 1000];
	strcpy(save, "");
	for (int i = 0; i < address_book->count; i++)
	{
		ContactInfo info = address_book->list[i];
		char num[20];
		sprintf(num, "%d", info.si_no);
		strcat(save, num);
		strcat(save, ",");
		strcat(save, info.name[0]);
		if (!strcmp(info.name[0], ""))
			strcat(save, "0");
		strcat(save, ",");
		for (int j = 0; j < 5; j++)
		{
			strcat(save, info.phone_numbers[j]);
			if (!strcmp(info.phone_numbers[j], ""))
				strcat(save, "0");
			strcat(save, ",");
		}
		for (int j = 0; j < 5; j++)
		{
			strcat(save, info.email_addresses[j]);
			if (!strcmp(info.email_addresses[j], ""))
				strcat(save, "0");
			if (j != 4)
				strcat(save, ",");
		}
		strcat(save, "\n");
	}

	// printf("save: |%s|", save);
	fprintf(address_book->fp, save);

	fclose(address_book->fp);

	return e_success;
}