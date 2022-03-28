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

//Saving prompt for address book
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

// Main menu with different options displayed
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
			add_contacts(address_book);
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
			return e_exit;
			break;
		}
	} while (option != e_exit);

	return e_success;
}

// Allowing user to add contact
Status add_contacts(AddressBook *address_book)
{
	// create blank ContactInfo
	ContactInfo info;
	info.si_no = address_book->list[address_book->count - 1].si_no + 1;
	strcpy(info.name[0], "");
	for (int i = 0; i < 5; i++)
	{
		strcpy(info.phone_numbers[i], "");
		strcpy(info.email_addresses[i], "");
	}

	int option;
	char input[100];

	do
	{
		// print contact info
		printf("\n");
		menu_header("Add Contact:");
		printf("\n0. Back       : ");
		printf("\n1. Name       : ");
		printf(info.name[0]);
		printf("\n2. Phone No 1 : ");
		printf(info.phone_numbers[0]);
		for (int i = 1; i < 5; i++)
		{
			if (!strcmp(info.phone_numbers[i], ""))
				continue;
			printf("\n   Phone No %d : %s", i - 1, info.phone_numbers[i]);
		}
		printf("\n3. Email ID 1 : ");
		printf(info.email_addresses[0]);
		for (int i = 1; i < 5; i++)
		{
			// printf("** |%s|\n", info.email_addresses[i]);
			if (!strcmp(info.email_addresses[i], ""))
				continue;
			printf("\n   Email ID %d : %s", i - 1, info.email_addresses[i]);
		}

		// edit contact info
		printf("\n\nPlease select an option: ");
		option = get_option(NUM);
        // gives different options of what to change
		switch (option)
		{
		case 1:
			printf("Enter the name: ");
			scanf("%[^\n]%*c", input);
			strcpy(info.name[0], input);
			break;
		case 2:
			printf("Enter Phone Number index to be changed [Max 5]: ");
			do
			{
				option = get_option(NUM);
			} while ((option > 5 || option <= 0));
			printf("Enter Phone Number %d [enter 0 to delete entry]: ", option);
			scanf("%[^\n]%*c", input);
			if (!strcmp(input, "0"))
			{
				strcpy(info.phone_numbers[option + 1], "");
				break;
			}
			strcpy(info.phone_numbers[option + 1], input);
			break;
		case 3:
			printf("Enter Email Address index to be changed [Max 5]: ");
			do
			{
				option = get_option(NUM);
			} while ((option > 5 || option <= 0));
			printf("Enter Email Address %d [enter 0 to delete entry]: ", option);
			scanf("%[^\n]%*c", input);
			printf("Comparing: |%s| and |%s|\n", input, "0");
			if (!strcmp(input, "0"))
			{
				strcpy(info.email_addresses[option + 1], "");
				break;
			}
			strcpy(info.email_addresses[option + 1], input);
			break;
		case 0:
			printf("\n");
			break;
		default:
			printf("Please select a valid option (0-3)\n");
			break;
		}
	} while (option != 0);

	address_book->list[address_book->count++] = info;
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

// asks user for input and searches for contact, sets index to the index of the search result (sets to -1 if not found) and returns the number of results
int search(AddressBook *address_book, int (*index)[100])
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
	// printf("search parameter: %s\n", input);

	int count = 0;
	switch (option)
	{
	case (1):
		for (int i = 0; i < address_book->count; i++)
		{
			// printf("Comparing |%s| and |%s|\n", address_book->list[i].name[0], input);
			if (!strcmp(address_book->list[i].name[0], input))
			{
				*index[count++] = i;
			}
		}
		break;
	case (2):
		for (int i = 0; i < address_book->count; i++)
		{
			for (int j = 0; j < 5; j++)
				if (!strcmp(address_book->list[i].phone_numbers[j], input))
				{
					*index[count++] = i;
				}
		}
		break;
	case (3):
		for (int i = 0; i < address_book->count; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				// printf("Comparing |%s| and |%s|\n", address_book->list[i].email_addresses[j], input);
				if (!strcmp(address_book->list[i].email_addresses[j], input))
				{
					*index[count++] = i;
				}
			}
		}
		break;
	case (4):
		for (int i = 0; i < address_book->count; i++)
		{
			// printf("id: %d\n", address_book->list[i].si_no);
			if (address_book->list[i].si_no == atoi(input))
			{
				*index[count++] = i;
			}
		}
		break;
	}

	// for (int i = 0; i < count; i++)
	// {
	// 	printf("Result: %d\n", *index[i]);
	// }
	if (count == 0)
		return -1;
	return count;
}

// searching a contact
Status search_contact(AddressBook *address_book)
{
	// get index of display
	int ind[100];
	int count = search(address_book, &ind);

	// check if found

	int option;
	do
	{
		menu_header("Search Results:");
		// printf("ind: %d\n", ind);
		if (count != -1)
		{
			contact_header();
			for (int i = 0; i < count; i++)
				print_contact(&address_book->list[ind[i]]);
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

// prompts the user to select a contact, returns -1 if the user cancels, else returns the index of the contact to delete
int select(AddressBook *address_book, int (*index)[100], int count)
{
	if (count == 0)
		return -1;
	int page = 0;
	int option;
	do
	{
		menu_header("Select Contact:");
		contact_header();
		for (int i = page * WINDOW_SIZE; i < page * WINDOW_SIZE + count; i++)
		{
			// printf("index: %d\n", *index[i]);
			print_contact(&address_book->list[*index[i]]);
		}
		printf("Press: [s] = Select | [q] = Cancel:");
		option = get_option(CHAR);
		if (option == 'q')
			return -1;
	} while (option != 's');

	while (1)
	{
		printf("Enter a Serial No. of a contact: ");
		option = get_option(NUM);
		for (int i = 0; i < address_book->count; i++)
		{
			if (address_book->list[i].si_no == option)
			{
				return i;
			}
		}
		printf("Invalid Serial Number.");
	}
}

// allows to edit contact
Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

// remove contacts
Status delete (AddressBook *address_book, int *ind)
{
	// if index is the last entry decrementing count will suffice
	if (*ind != address_book->count - 1)
	{
		// shifts the array leftward starting from the deleted contact
		for (int i = *ind; i < address_book->count - 1; i++)
		{
			address_book->list[i] = address_book->list[i + 1];
		}
	}

	address_book->count--;
	return e_success;
}

// allows user to delete contacts
Status delete_contact(AddressBook *address_book)
{
	// get index to remove
	int results[100];
	int count = search(address_book, &results);

	int ind = select(address_book, &results, count);

	int option;
	do
	{
		menu_header("Contact Deletion Results: ");
		if (ind != -1)
		{
			contact_header();
			print_contact(&address_book->list[ind]);
			delete (address_book, &ind);
			printf("Contact successfully deleted\n");
		}
		else
		{
			printf("No contact matches your search parameter\n");
		}
		printf("Press [q] = back | [r] = delete another: ");
		option = get_option(CHAR);
	} while (option != 'q' && option != 'r');
	if (option == 'r')
		delete_contact(address_book);
	return e_success;
}