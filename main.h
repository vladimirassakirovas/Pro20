#ifndef MAIN_H_
#define MAIN_H_
//-----------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//-----------------------





//// ---------------------------
//typedef struct
//{
//
//	unsigned int len;		// Item lenght
//	char data[1536];		// Item data
//}db_item_ptr;
//
//typedef struct
//{
//	unsigned int cnt;
//	db_item_ptr items[10];
//}database_ptr;
//// -------------------------------




// ---------------------------
typedef struct
{
	unsigned int len;		// Item lenght
	unsigned char data[768];		// Item data
}db_item_ptr;

typedef struct
{
	unsigned int cnt;
	db_item_ptr items[10];
}database_ptr;
// -------------------------------





char input_data[1536];
database_ptr  db;

unsigned int cnt_n1_io;
unsigned int cnt_n2_io;
unsigned int cnt_n4_io;
unsigned int cnt_n8_io;

void input_handler(char *);
void command_handler(char *);
void data_handler(char *);
void data_parser (void *, unsigned int);
unsigned short swap_uint16( unsigned short );
unsigned long swap_uint32( unsigned long  );
unsigned long long swap_uint64( unsigned long long  );
void show_data(unsigned int);

const char bad_command_message[52] = "Invalid command! chose from .exit .free .show .info\0";
//-----------------------
#endif /* MAIN_H */
