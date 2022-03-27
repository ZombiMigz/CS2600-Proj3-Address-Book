#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "abk_fileops.h"
// #include "abk_log.h"
#include "abk_menus.h"
#include "address_book.h"

int get_option(int type, char *msg)
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

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

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

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/*
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */
	printf("\n==============================================================================================================\n");
	printf(":%-5s:%-32s:%-32s:%-32s:\n", "S.No", "Name", "Phone No.", "Email ID");
	printf("==============================================================================================================\n");

	for (int i = *index * WINDOW_SIZE; i < address_book->count && i < *index * WINDOW_SIZE + 5; i++)
	{
		ContactInfo info = address_book->list[i];
		printf(":%-5d:%-32s:%-32s:%-32s:\n", info.si_no, info.name[0], info.phone_numbers[0], info.email_addresses[0]);
		printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info.phone_numbers[1], info.email_addresses[1]);
		printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info.phone_numbers[2], info.email_addresses[2]);
		printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info.phone_numbers[3], info.email_addresses[3]);
		printf(":%-5s:%-32s:%-32s:%-32s:\n", "", "", info.phone_numbers[4], info.email_addresses[4]);

		printf("==============================================================================================================\n");
	}
	printf("%s: ", msg);
	return e_success;
}

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
		main_menu();

		option = get_option(NUM, NULL);

		if ((address_book->count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

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

			int pageCount = address_book->count / WINDOW_SIZE;
			if (address_book->count % WINDOW_SIZE != 0)
				pageCount++;

			do
			{
				list_contacts(address_book, "Contact List", &ind, "Press [a] = prev | [d] = next | [q] = menu", e_list);
				option = (char)get_option(CHAR, NULL);
				if (option == 'a' && ind > 0)
					ind--;
				if (option == 'd' && ind < pageCount - 1)
					ind++;
			} while (option != 'q');
			break;

		case e_save:
			save_file(address_book);
			break;
		case e_exit:
			return e_exit;
			break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	ContactInfo info;
	int option;
	char input[100];
	int phones = 0;
	int emails = 0;

	do {
		printf("\n");
		menu_header("Add Contact:");
		printf("\n0. Back       : ");
		printf("\n1. Name       : ");
		printf(info.name[0]);
		printf("\n2. Phone No 1 : ");
		printf(info.phone_numbers[0]);
		for (int i = 1; i + 1 <= phones; i++) {
			printf("\n   Phone No %d", i + 1);
			printf(" : ");
			printf(info.phone_numbers[i]);
		}
		printf("\n3. Email ID 1 : ");
		printf(info.email_addresses[0]);
		for (int i = 1; i + 1 <= emails; i++) {
			printf("\n   Email ID %d", i + 1);
			printf(" : ");
			printf(info.email_addresses[i]);
		}

		printf("\n\nPlease select an option: ");
		option = get_option(NUM, NULL);
		switch (option) {
			case 1:
				printf("Enter the name: ");
				scanf("%s", input);
				strcpy(info.name[0], input);
				break;
			case 2:
				if (phones < 5) {
					printf("Enter Phone Number %d", phones + 1);
					printf(": ");
					if (phones > 0) printf("[Please renter the same option of alternate Phone Number]: ");
					scanf("%s", input);
					strcpy(info.phone_numbers[phones], input);
					phones++;
				} else  {
					printf("Maximum Phone Numbers reached\n");
				}
				break;
			case 3:
				if (emails < 5) {
					printf("Enter Email ID %d", emails + 1);
					printf(": ");
					if (emails > 0) printf("[Please renter the same option of alternate Email ID]: ");
					scanf("%s", input);
					strcpy(info.email_addresses[emails], input);
					emails++;
				} else  {
					printf("Maximum Email IDs reached\n");
				}
				break;
			case 0:
				printf("\n");
				return e_success;
				break;
			default:
				printf("Please select a valid option (0-3)\n");
				break;
		}
	} while (option != 0);
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
