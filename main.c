#include "main.h"
/********************************/



int main()
{
	while(1)
	{
		/* input data */
		input_handler(input_data);

	}
return 0;
}/********************************/










/* read input string */
void input_handler(char *str)
{
	fflush( stdout );
	scanf(" %s", str);

	if(str[0] == '.')
	{
		command_handler(str);
	}
	else
	{
		data_handler(str);
	}
}/********************************/









/* check and execute command */
void command_handler(char *str)
{
	if(str[5] == '\0')
	{
		switch(*(unsigned int*)(str+1))
		{
			case 0x74697865: // 0x74697865 - .exit
				fflush( stdout );
				printf("%s\n","Chosen .exit");
				fflush( stdout );
				printf("%s\n","Bye");
				exit(0);
				break;
			case 0x65657266: // 0x65657266 - .free
				fflush( stdout );
				printf("%s\n","Chosen .free");
				db.cnt = 0;
				fflush( stdout );
				printf("%s\n","Database is empty");
				break;
			case 0x776F6873: // 0x776F6873 - .show
				fflush( stdout );
				printf("%s\n","Chosen .show");

				if(db.cnt == 0)
				{
					fflush( stdout );
					printf("%s\n","Database is empty");
				}
				for(unsigned int k = 0; k < db.cnt; k++)
				{
					show_data(k);
				}
				break;
			case 0x6F666E69: // 0x6F666E69 - .info
				fflush( stdout );
				printf("%s\n","Chosen .info");

				for(unsigned int i = 0; i < db.cnt; i++)
				{
					show_data(i);
					data_parser (db.items[i].data, i);
				}

				break;
			default:
				fflush( stdout );
				printf("%s\n",bad_command_message);
		}
	}
	else
	{
		fflush( stdout );
		printf("%s\n",bad_command_message);
	}
}/********************************/











/* change data from ascii to hex and store in db */
void data_handler(char *str)
{
	db.items[db.cnt].len = strlen(str);
	for(unsigned int i =  0, j = 0; i < db.items[db.cnt].len; i+=2, j++)
	{
		db.items[db.cnt].data[j] =  (str[i+1]<='9') ? (str[i+1]-'0') : (str[i+1]-'A'+10) ;
		db.items[db.cnt].data[j] +=  ((str[i]<='9') ? (str[i]-'0') : (str[i]-'A'+10) )*  0x10;
	}
	db.cnt++;
}/********************************/









/* parsing data */
void data_parser (void *st_ptr, unsigned int a)
{
		/* IO nr */
		cnt_n1_io = db.items[a].data[36];
		cnt_n2_io = db.items[a].data[36 + cnt_n1_io*2 + 1];
		cnt_n4_io = db.items[a].data[37 + cnt_n1_io*2 + cnt_n2_io*3 + 1];
		cnt_n8_io = db.items[a].data[38 + cnt_n1_io*2 + cnt_n2_io*3 + cnt_n4_io*5 + 1];


	typedef struct
	{
		unsigned char n_io_id[1];
		unsigned char n_io_value[1];
	} n1_io_ptr;


	typedef struct
	{
		unsigned char n_io_id[1];
		unsigned char n_io_value[2];
	} n2_io_ptr;


	typedef struct
	{
		unsigned char n_io_id[1];
		unsigned char n_io_value[4];
	} n4_io_ptr;


	typedef struct
	{
		unsigned char n_io_id[1];
		unsigned char n_io_value[8];
	} n8_io_ptr;


	typedef struct
	{
		unsigned char event_io_id[1];
		unsigned char N_of_total_id[1];
		unsigned char N1_of_one_byte_io[1];
		n1_io_ptr n1_io[cnt_n1_io];
		unsigned char N2_of_two_byte_io[1];
		n2_io_ptr n2_io[cnt_n2_io];
		unsigned char N4_of_four_byte_io[1];
		n4_io_ptr n4_io[cnt_n4_io];
		unsigned char N8_of_eight_byte_io[1];
		n8_io_ptr n8_io[cnt_n8_io];
	} io_element_ptr;


	typedef struct
	{
		unsigned char longitude[4];
		unsigned char latitude[4];
		unsigned char altitude[2];
		unsigned char angle[2];
		unsigned char satellites[1];
		unsigned char speed[2];
	} gps_element_ptr;


	typedef struct
	{
		unsigned char timestamp[8];
		unsigned char priority[1];
		gps_element_ptr gps_element[1];
		io_element_ptr io_element[1];
	} avl_data_ptr;


	typedef struct
	{
		unsigned char preamble[4];
		unsigned char data_field_lenght[4];
		unsigned char codec_id[1];
		unsigned char nr_of_data_1[1];
		avl_data_ptr avl_data[1];
		unsigned char nr_of_data_2[1];
		unsigned char crc_16[4];
	} stdn;




	fflush( stdout );
	printf("Name\t\t\t\tSize\t\t\tValue\t\t\tHex Value\n");

	printf("Preamble\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->preamble));
	printf("%lu\t\t\t", swap_uint32(*(unsigned long*)((stdn*)st_ptr)->preamble));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->preamble[i]), i < sizeof(((stdn*)st_ptr)->preamble)-1; i++, printf("-"));

	printf("\nData Field Lenght\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->data_field_lenght));
	printf("%lu\t\t\t", swap_uint32(*(unsigned long*)((stdn*)st_ptr)->data_field_lenght));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->data_field_lenght[i]), i < sizeof(((stdn*)st_ptr)->data_field_lenght)-1 ; i++, printf("-"));

	printf("\nCodec ID\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->codec_id));
	printf("%d\t\t\t", *((stdn*)st_ptr)->codec_id);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->codec_id[i]), i < sizeof(((stdn*)st_ptr)->codec_id)-1; i++, printf("-"));

	printf("\nNr of Data 1\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->nr_of_data_1));
	printf("%d\t\t\t", *((stdn*)st_ptr)->nr_of_data_1);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->nr_of_data_1[i]), i < sizeof(((stdn*)st_ptr)->nr_of_data_1)-1; i++, printf("-"));


	printf("\nAVL Data\t\t\t%d", sizeof(((stdn*)st_ptr)->avl_data));

	printf("\n Timestamp\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->timestamp));
	printf("0\t\t\t");
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->timestamp[i]), i < sizeof(((stdn*)st_ptr)->avl_data->timestamp)-1; i++, printf("-"));

	printf("\n Priority\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->priority));
	printf("%d\t\t\t", *((stdn*)st_ptr)->avl_data->priority);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->priority[i]), i < sizeof(((stdn*)st_ptr)->avl_data->priority)-1; i++, printf("-"));


	printf("\n GPS Element\t\t\t%d", sizeof(((stdn*)st_ptr)->avl_data->gps_element));

	printf("\n  Longitude\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->gps_element->longitude));
	printf("%lu\t\t\t", swap_uint32(*(unsigned long*)((stdn*)st_ptr)->avl_data->gps_element->longitude));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->gps_element->longitude[i]), i < sizeof(((stdn*)st_ptr)->avl_data->gps_element->longitude)-1; i++, printf("-"));

	printf("\n  Latitude\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->gps_element->latitude));
	printf("%lu\t\t\t", swap_uint32(*(unsigned long*)((stdn*)st_ptr)->avl_data->gps_element->latitude));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->gps_element->latitude[i]), i < sizeof(((stdn*)st_ptr)->avl_data->gps_element->latitude)-1; i++, printf("-"));

	printf("\n  Altitude\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->gps_element->altitude));
	printf("%u\t\t\t", swap_uint16(*(unsigned short*)((stdn*)st_ptr)->avl_data->gps_element->altitude));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->gps_element->altitude[i]), i < sizeof(((stdn*)st_ptr)->avl_data->gps_element->altitude)-1; i++, printf("-"));

	printf("\n  Angle\t\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->gps_element->angle));
	printf("%u\t\t\t", swap_uint16(*(unsigned short*)((stdn*)st_ptr)->avl_data->gps_element->angle));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->gps_element->angle[i]), i < sizeof(((stdn*)st_ptr)->avl_data->gps_element->angle)-1; i++, printf("-"));

	printf("\n  Satellites\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->gps_element->satellites));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->gps_element->satellites);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->gps_element->satellites[i]), i < sizeof(((stdn*)st_ptr)->avl_data->gps_element->satellites)-1; i++, printf("-"));

	printf("\n  Speed\t\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->gps_element->speed));
	printf("%u\t\t\t", swap_uint16(*(unsigned short*)((stdn*)st_ptr)->avl_data->gps_element->speed));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->gps_element->speed[i]), i < sizeof(((stdn*)st_ptr)->avl_data->gps_element->speed)-1; i++, printf("-"));


	printf("\n IO Element\t\t\t%d", sizeof(((stdn*)st_ptr)->avl_data->gps_element));

	printf("\n  Event IO id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->event_io_id));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->event_io_id);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->event_io_id[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->event_io_id)-1; i++, printf("-"));

	printf("\n  N of total id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->N_of_total_id));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->N_of_total_id);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->N_of_total_id[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->N_of_total_id)-1; i++, printf("-"));

	printf("\n  N1 of one byte io\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->N1_of_one_byte_io));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->N1_of_one_byte_io);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->N1_of_one_byte_io[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->N1_of_one_byte_io)-1; i++, printf("-"));



	for (unsigned int j = 0; j < *((stdn*)st_ptr)->avl_data->io_element->N1_of_one_byte_io; j++)
	{

		printf("\n   N1 IO id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_id));
		printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_id);
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_id[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_id)-1; i++, printf("-"));


		printf("\n   N1 value id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_value));
		printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_value);
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_value[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n1_io[j].n_io_value)-1; i++, printf("-"));

	}

	printf("\n  N2 of two byte io\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->N2_of_two_byte_io));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->N2_of_two_byte_io);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->N2_of_two_byte_io[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->N2_of_two_byte_io)-1; i++, printf("-"));

	for (unsigned int j = 0; j < *((stdn*)st_ptr)->avl_data->io_element->N2_of_two_byte_io; j++)
	{
		printf("\n   N2 IO id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_id));
		printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_id);
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_id[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_id)-1; i++, printf("-"));

		printf("\n   N2 value id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_value));
		printf("%u\t\t\t", swap_uint16(*(unsigned short*)((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_value));
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_value[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_value)-1; i++, printf("-"));
	}


	printf("\n  N4 of four byte io\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->N4_of_four_byte_io));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->N4_of_four_byte_io);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->N4_of_four_byte_io[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->N4_of_four_byte_io)-1; i++, printf("-"));

	for (unsigned int j = 0; j < *((stdn*)st_ptr)->avl_data->io_element->N4_of_four_byte_io; j++)
	{
		printf("\n   N4 IO id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_id));
		printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_id);
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_id[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_id)-1; i++, printf("-"));

		printf("\n   N4 value id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n2_io[j].n_io_value));
		printf("%lu\t\t\t", swap_uint32(*(unsigned long*)((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_value));
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_value[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n4_io[j].n_io_value)-1; i++, printf("-"));
	}

	printf("\n  N8 of eight byte io\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->N8_of_eight_byte_io));
	printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->N8_of_eight_byte_io);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->N8_of_eight_byte_io[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->N8_of_eight_byte_io)-1; i++, printf("-"));


	for (unsigned int j = 0; j < *((stdn*)st_ptr)->avl_data->io_element->N8_of_eight_byte_io; j++)
	{
		printf("\n   N8 IO id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_id));
		printf("%u\t\t\t", *((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_id);
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_id[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_id)-1; i++, printf("-"));

		printf("\n   N8 value id\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_value));
		printf("%lu\t\t\t", swap_uint64(*(unsigned long long*)((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_value));
		for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_value[i]), i < sizeof(((stdn*)st_ptr)->avl_data->io_element->n8_io[j].n_io_value)-1; i++, printf("-"));
	}

	printf("\nNr of Data 2\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->nr_of_data_2));
	printf("%d\t\t\t", *((stdn*)st_ptr)->nr_of_data_2);
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->nr_of_data_2[i]), i < sizeof(((stdn*)st_ptr)->nr_of_data_2)-1; i++, printf("-"));

	printf("\nCRC-16\t\t\t\t%d\t\t\t", sizeof(((stdn*)st_ptr)->crc_16));
	printf("%lu\t\t\t", swap_uint32(*(unsigned long*)((stdn*)st_ptr)->crc_16));
	for(unsigned int i = 0; printf("%02X",((stdn*)st_ptr)->crc_16[i]), i < sizeof(((stdn*)st_ptr)->crc_16)-1; i++, printf("-"));

	printf("\n ------------------------------------------------------------------------------------------------ \n");
}/********************************/






/* change data from hex to ascii and show all data in database */
void show_data( unsigned int cnt)
{
	char str0[1536];

	for(unsigned int i = 0, j = 0; j < db.items[cnt].len; i++, j+=2)
	{
		if(((db.items[cnt].data[i] >> 4) & 0x0f ) <= 0x9)
		{
			str0[j] = ((db.items[cnt].data[i] >> 4) & 0x0f ) + '0';
		}
		else if(((db.items[cnt].data[i] >> 4) & 0x0f ) > 0x9)
		{
			str0[j] = ((db.items[cnt].data[i] >> 4) & 0x0f ) - 10 + 'A';
		}


		if(((db.items[cnt].data[i] ) & 0x0f ) <= 0x9)
		{
			str0[j+1] = (db.items[cnt].data[i]  & 0x0f ) + '0';
		}
		else if(((db.items[cnt].data[i] ) & 0x0f ) > 0x9)
		{
			str0[j+1] = (db.items[cnt].data[i]  & 0x0f ) - 10 + 'A';
		}
	}
	str0[db.items[cnt].len] = 0;
	fflush( stdout );
	printf("%s\n",str0);

}










//! Byte swap unsigned short
unsigned short swap_uint16( unsigned short val )
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap unsigned long
unsigned long swap_uint32( unsigned long val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

//! Byte swap unsigned long long
unsigned long long swap_uint64( unsigned long long val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

