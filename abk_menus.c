#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "abk_fileops.h"
// #include "abk_log.h"
#include "abk_menus.h"
#include "address_book.h"

int get_option(int type)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */

	/* Fill the code to add above functionality */

	if (type == NUM)
	{
		int in;
		scanf("%d", &in);

		// clears input buffer
		while ((getchar()) != '\n')
			;
		return in;
	}

	if (type == CHAR)
	{
		char in;
		scanf("%c", &in);
		while ((getchar()) != '\n')
			;
		return in;
	}

	return 0;
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();
		printf("%s", "\rEnter 'N' to Ignore and 'Y' to Save: ");
		option = get_option(CHAR);

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

// prints out a ContactInfo struct
void print_contact(ContactInfo *info)
{
	printf(":%-5d:%-32s:%-32s:%-32s:\n", info->si_no, info->name[0], info->phone_numbers[0], info->email_addresses[0]);
	printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info->phone_numbers[1], info->email_addresses[1]);
	printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info->phone_numbers[2], info->email_addresses[2]);
	printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info->phone_numbers[3], info->email_addresses[3]);
	printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info->phone_numbers[4], info->email_addresses[4]);

	printf("==============================================================================================================\n");
}

void contact_header()
{
	printf("\n==============================================================================================================\n");
	printf(":%-5s:%-32s:%-32s:%-32s:\n", "S.No", "Name", "Phone No.", "Email ID");
	printf("==============================================================================================================\n");
}
// lists a page of contacts (5 at a time based on WINDOW_SIZE)
Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/*
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */
	contact_header();

	for (int i = *index * WINDOW_SIZE; i < address_book->count && i < *index * WINDOW_SIZE + 5; i++)
	{
		ContactInfo info = address_book->list[i];
		print_contact(&info);
	}
	printf("%s: ", msg);
	return e_success;
}

// prints a formatted header a screen/frame of the application
void menu_header(const char *str)
{
	fflush(stdout);

	// system("clear");

	printf("#######  Address Book  #######\n");
	if (*str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

// prints out options for main menu
void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		// prints main menu
		main_menu();

		// asks user for which menu option
		option = get_option(NUM);

		// restricts user to add contact if contactlist is empty
		if ((address_book->count == 0) && (option != e_add_contact))
		{
			printf("%s", "No entries found!!. Would you like to add? Use Add Contacts");
			get_option(NONE);
			continue;
		}

		// different functionality depending on user input
		switch (option)
		{
		case e_add_contact:
			/* Add your implementation to call add_contacts function here */
			break;
		case e_search_contact:
			search_contact(address_book);
			break;
		case e_edit_contact:
			edit_contact(address_book);
			break;
		case e_delete_contact:
			delete_contact(address_book);
			break;
		case e_list_contacts:
			/* Add your implementation to call list_contacts function here */
			char option;
			int ind = 0;

			// calculate the number of pages (depends on WINDOW_SIZE which is currently 5, meaning 5 contacts per page)
			int pageCount = address_book->count / WINDOW_SIZE;
			if (address_book->count % WINDOW_SIZE != 0)
				pageCount++;

			// keep printing out contacts, change index if user selects "previous" or "next" options, "q" or returns to menu
			do
			{
				list_contacts(address_book, "Contact List", &ind, "Press [a] = prev | [d] = next | [q] = menu", e_list);
				option = (char)get_option(CHAR);
				if (option == 'a' && ind > 0)
					ind--;
				if (option == 'd' && ind < pageCount - 1)
					ind++;
			} while (option != 'q');
			break;

		case e_save:
			// saves data to address_book.csv
			save_file(address_book);
			break;
		case e_exit:
			break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
}

void print_search_options()
{
	menu_header("Search Contact By:\n");
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No.\n");
	printf("3. Email Address\n");
	printf("4. Serial No\n");
	printf("\n");
	printf("Please select an option: ");
}

// asks user for input and searches for contact, sets index to the index of the search result (sets to -1 if not found)
Status search(AddressBook *address_book, int *index)
{
	int option;
	// choose what to search by
	do
	{
		print_search_options();
		option = get_option(NUM);
	} while (!(option >= 0 && option <= 4));
	// printf("option: %d\n", option);
	// set search type based on input (return if 0)
	char type[20];
	switch (option)
	{
	case (1):
		strcpy(type, "Name");
		break;
	case (2):
		strcpy(type, "Phone No.");
		break;
	case (3):
		strcpy(type, "Email Address");
		break;
	case (4):
		strcpy(type, "Serial No.");
		break;
	default:
		return e_back;
	}

	printf("Please enter the %s you want to search by: ", type);
	char input[100];

	scanf("%[^\n]%*c", input);
	// clears input buffer (handles "\n" input)
	while ((getchar()) != '\n')
		;
	printf("search parameter: %s\n", input);

	switch (option)
	{

	case (1):
		for (int i = 0; i < address_book->count; i++)
		{
			// printf("Comparing |%s| and |%s|\n", address_book->list[i].name[0], input);
			if (!strcmp(address_book->list[i].name[0], input))
			{
				*index = i;
				return e_success;
			}
		}
		*index = -1;
		return e_fail;
		break;
	case (2):
		for (int i = 0; i < address_book->count; i++)
		{
			for (int j = 0; j < 5; j++)
				if (!strcmp(address_book->list[i].phone_numbers[j], input))
				{
					*index = i;
					return e_success;
				}
		}
		*index = -1;
		return e_fail;
		break;
	case (3):
		for (int i = 0; i < address_book->count; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				printf("Comparing |%s| and |%s|\n", address_book->list[i].email_addresses[j], input);
				if (!strcmp(address_book->list[i].email_addresses[j], input))
				{
					*index = i;
					return e_success;
				}
			}
		}
		*index = -1;
		return e_fail;
		break;
	case (4):
		for (int i = 0; i < address_book->count; i++)
		{
			// printf("id: %d\n", address_book->list[i].si_no);
			if (address_book->list[i].si_no == atoi(input))
			{
				*index = i;
				return e_success;
			}
		}
		*index = -1;
		return e_fail;
		break;
	}

	//
}

Status search_contact(AddressBook *address_book)
{
	// get index of display
	int ind;
	search(address_book, &ind);

	// check if found

	int option;
	do
	{
		menu_header("Search Results:");
		printf("ind: %d\n", ind);
		if (ind != -1)
		{
			contact_header();
			print_contact(&address_book->list[ind]);
		}
		else
		{
			printf("No contact matches your search parameter\n");
		}

		printf("Press [q] = back | [r] = search again: ");
		option = get_option(CHAR);
	} while (option != 'q' && option != 'r');
	if (option == 'r')
		search_contact(address_book);

	return e_success;
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
