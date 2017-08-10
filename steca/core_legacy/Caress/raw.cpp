/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        raw.c

        Offline access to
        CARESS raw data files:

        - read complete (closed) data file
        - read incomplete data file (preliminary end of file: two 0 bytes)
          this access is allowed if and only if CARESS currently does not
          access this file, e.g. this access to an incomplete file may
          be used by an external CARESS process to read some data stored
          up to now.

        6 (Online: 7) basic access functions are available:

                open_data_file
                close_data_file
                re_read  (Online only)
                next_data_unit
                get_data_unit
                get_data_partition
                last_data_of_element


        - int32 open_data_file(file_name,mode)

                char *file_name           name of raw data file (RO)
                int32 *mode		  ignored (RO)
                                          used for compatability purpose to
                                          previous version

          return value: 0: OK, 1: not OK
          take care that file_name is terminated by "NULL" character

        - int32 close_data_file(void)

          return value: 0: OK, 1: not OK

        - void re_read(void)

          (Online only)
          prepare reading starting at that file position where previously
          a preliminary end of file was detected

        - int32 next_data_unit(element_number,element_type,element,
                                  node,data_type,data_number)

                int32 *element_number  	element sequence number (WO)
                int32 *element_type	0:command, 1:data (WO)
                char element[8]         name of actual data element (WO)
                char node[8]		name of actual final node (WO)
                int32 *data_type	1 or 9:int16, 2 or 10:int32,
                                        3 or 4 or 5:float, 16:string,
                                        0:empty (WO)
                int32 *data_number	number of items (WO)

          return value: 0: OK, 1: not OK, 2: preliminay end of file

          next_data_unit positions to the next data group (data of same type)
          and transfers data description parameters to caller.

        - int32 get_data_unit(buffer_address)

                void *buffer_address  address of user data buffer (WO)

          return value: 0: OK, 1: not OK, 2: preliminary end of file

          get_data_unit transfers the current data group (i.e. consecutive
          values of a node which are of the same type) to the caller but
          does no positioning.

        - int32 get_data_partition(buffer_address,section,starting_item,
                                  number_of_items,type_of_items)

                void *buffer_address	address of user data buffer (WO)
                int32 *section		1 or 2 (if single and multi detectors
                                        are enabled then section#1 contains
                                        si-data, section#2 mu-data) (RO)
                int32 *starting_item	number of item to start data
                                        transfer (RO)
                int32 *number_of_items	number of items to transfer (RO)
                int32 *type_of_items	type of items (RO)

          return value:	0: ok, 1: not_ok (wrong parameter)

          get some number of data items out of the current data unit.
          This function is especially useful to extract parts of bulk data,
          e.g. histograms from data file containig DAU data.

        - int32 last_data_of_element(void)

          return value: 0: false, 1: true

          tells if program has positioned after last data of an element.


        For more detailled information on CARESS raw data file structure
        look into the corresponding description.

        Notes:
        1. As PV~Wave LINKNLOAD access to shared library uses
           C (argn,argv) parameter passing, a special calling
           interface (suffix _pv for routine names) is also
           implemented.
           Please use these routines for IDL CALL_EXTERNAL running on
           UNIX, MS Windows or MAC, too.
        2. raw.c is portable and can be used on:
           ALPHA/OpenVMS, ALPHA/Digital UNIX,
           SUN/SUNOS or SOLARIS, MAC, INTEL/MS Windows, etc.
           Transfer CARESS raw data files to those systems
           using FTP, specifying type "binary".
        3. Some conditional compiling is needed to convert different
           number representations (little_ending/big_ending,
           vax_float/ieee_float):

           platform:		predefined symbol(s):		representation:
           ALPHA/OpenVMS	__alpha, __vms			le, vax_float
           ALPHA/Digital UNIX	__alpha, __unix__		le, ieee_float
           SUN/SUNOS or SOLARIS	sun				be, ieee_float
           MAC			not known, define symbol mac	be, ieee_float
           POWERPC		__powerpc__, __PPC, _BIG_ENDIAN	be, ieee_float
           INTEL		not known, define symbol i386	le, ieee_float

        4. Unfortunately PV-WAVE long integer (e.g. LONARR) on DIGITAL UNIX
           have 8 bytes which is treated appropriatedly.
           IDL long integer have 4 bytes on DIGITAL UNIX, for IDL please
           compile with DEFINE option IDL.

        To build and run a process (e.g.) print_raw.exe using raw:
        OpenVMS:
                link print_raw,raw
                run print_raw
        UNIX:
                cc print_raw.c raw.c -o print_raw.exe
                print_raw.exe
        MS WINDOWS, MAC:
                most common compilers use the project facility and will
                guide you

        file:		raw.c
        author:		K.H. Degenhardt, L. Rossa
        last_update: 	15-oct-2009

-----------------------------------------------------------------------
-----------------------------------------------------------------------*/

#pragma GCC diagnostic ignored "-Wstrict-aliasing"

#include "raw.h"

#if defined(_MSC_VER) && defined(WINVER)
static int fseeko(FILE* pFile, int64 llPos, int iWhence);
static int64 ftello(FILE* pFile);
#endif

#define MAX_NAME_LENGTH 32     /* max length of names		*/
#define NIL 0                  /* NIL data token		*/
#define END_OF_FILE -64        /* EOF token			*/
#define END_OF_FILE_DETECTED 2 /* end of file return value     */
#define PAREN -128             /* opening PARENTHESIS for data description */
#define THESIS -127            /* closing PARENTHESIS for data description */
#define EMPTY 0                /* no data			*/
#define COMMAND_STRUCT 1       /* command structure		*/
#define DATA_STRUCT 2          /* data structure		*/
#define DATA_LABEL 3           /* data label			*/
#define DATA_VALUES 4          /* data	values			*/
#define CMD_ELEMENT 0          /* command element		*/
#define DAT_ELEMENT 1          /* data element			*/
//#define OFFLINE 0              /* OFFLINE use			*/
#define ONLINE 1     /* CARESS ONLINE use		*/
#define MONITORING 2 /* MONITORING using incomplete file */
#define INT16_LE 1   /* int16 little endian */
#define INT16_BE 9   /* int16 big endian */
#define INT32_LE 2   /* int32 little endian */
#define INT32_BE 10  /* int32 big endian */
#define INT64_LE 13  /* int64 little endian */
#define INT64_BE 14  /* int64 big endian */
#define IEEE32_LE 3  /* IEEE float little endian */
#define IEEE32_BE 4  /* IEEE float big endian */
#define IEEE64_LE 11 /* IEEE double little endian */
#define IEEE64_BE 12 /* IEEE double big endian */
#define VAX_FLOAT 5  /* VAX float */

#define BLOCK_LENGTH ((int32)sizeof(pVars->m_buf))                            /* 512 */
#define MAX_BLOCKS ((int32)(sizeof(pVars->m_big_buf) / sizeof(pVars->m_buf))) /* 130 */

/*	functions and procedures	*/
static void data_description(struct raw_infovars* pVars);
static void complex_data_description(struct raw_infovars* pVars);
static void simple_data_description(struct raw_infovars* pVars);
static void count(struct raw_infovars* pVars);
static void data_type(struct raw_infovars* pVars);
static int32 data(struct raw_infovars* pVars);
static int32 next_token(struct raw_infovars* pVars);
static int32 refresh_token(struct raw_infovars* pVars);
static int32 get_data_block(struct raw_infovars* pVars, char d_type, int64 number, char** addr);
static int32 label_name(struct raw_infovars* pVars, char* str, int32 number);
static int32 local_next_data_unit(struct raw_infovars* pVars, int bSkip, int32* e_number,
                                  int32* e_type, modname_t element, modname_t node, int32* d_type,
                                  int64* number);
static int32 local_strncmp(register const char* szString1, register const char* szString2,
                           register int32 iMaxLen);
static void evaluate(struct raw_infovars* pVars, char c_type, int32 number, char* addr);
static void separate(struct raw_infovars* pVars, char* str, int32 num);
static void store_type(struct raw_infovars* pVars, char token);
static void next_descr(struct raw_infovars* pVars, int32 kind);
static void insert_descr(struct raw_infovars* pVars);
static void store_descr(struct raw_infovars* pVars, char* str);
static void delete_descr(struct raw_infovars* pVars);
static void vaxfloat_to_ieeefloat(char* in_rep, char* out_rep);
static void ieeefloat_to_vaxfloat(struct raw_infovars* pVars, char* in_rep, char* out_rep);

/*	persistent variables	*/
static struct raw_infovars* g_pRawInfoVars = NULL;

struct raw_infovars* allocate_raw_infovars(void)
{
    struct raw_infovars* pVars = (struct raw_infovars*)malloc(sizeof(struct raw_infovars));
    if (pVars != NULL) {
        int i;
        memset(pVars, 0, sizeof(*pVars));

        for (i = 0; i < (ARRAY_SIZE(pVars->m_blanks) - 1); i++)
            pVars->m_blanks[i] = ' ';
        pVars->m_blanks[ARRAY_SIZE(pVars->m_blanks) - 1] = '\0';

/* setup local number representation */
#if defined(BIG_ENDIAN)
        pVars->m_local_int16 = INT16_BE;
        pVars->m_local_int32 = INT32_BE;
        pVars->m_local_int64 = INT64_BE;
        pVars->m_local_ieee32 = IEEE32_BE;
        pVars->m_local_ieee64 = IEEE64_BE;
#endif

#if defined(__vms)
        pVars->m_local_int16 = INT16_LE;
        pVars->m_local_int32 = INT32_LE;
        pVars->m_local_int64 = INT64_LE;
        pVars->m_local_ieee32 = VAX_FLOAT;
        pVars->m_local_ieee64 = VAX_FLOAT;
#endif

#if defined(LITTLE_ENDIAN)
        pVars->m_local_int16 = INT16_LE;
        pVars->m_local_int32 = INT32_LE;
        pVars->m_local_int64 = INT64_LE;
        pVars->m_local_ieee32 = IEEE32_LE;
        pVars->m_local_ieee64 = IEEE64_LE;
#endif
    }
    return pVars;
}

void free_raw_infovars(struct raw_infovars* pVars)
{
    if (pVars->m_file_name_save != NULL)
        free(pVars->m_file_name_save);
    free(pVars);
}

/*---------------------------------------------------------------*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 open_data_file(const char * file_name, int32 *mode)

Open CARESS data file.

-----------------------------------------------------------------*/
int32 open_data_file(const char* file_name, int32* mode)
{
    (void)mode;
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    return open_data_file_r(g_pRawInfoVars, file_name);
}

int32 open_data_file_r(struct raw_infovars* pVars, const char* file_name)
{
    int32 istatus;

    istatus = OK;
    if (pVars->m_file_name_save != NULL)
        free(pVars->m_file_name_save);
    pVars->m_file_name_save = strdup(file_name);
#if defined(__vms)
    pVars->m_raw_data_file = fopen(file_name, "rb", "shr=put");
#else
    pVars->m_raw_data_file = fopen(file_name, "rb");
#endif
    if (pVars->m_raw_data_file == NULL)
        istatus = NOT_OK;

    if (istatus == OK) {
        pVars->m_flag = FALSE;
        pVars->m_first_block_read = FALSE;
        pVars->m_last_data = FALSE;
        pVars->m_descr_index = 0;
        memset(&pVars->m_detname[0], 0, sizeof(pVars->m_detname));
    }
    return istatus;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 genwave(int32 argc, char *argv[])

One simple function call for access to all PV~Wave functions.

-----------------------------------------------------------*/

int genwave(int argc, char* argv[])
{
    int sel = *(((int**)argv)[0]);
    argc--;
    argv++;
    switch (sel & 0xf) {
    case 0: {
        /* configuration for PV-Wave, which is not able to handle 'int64' in the 32 bit version */
        if (argc < 2) {
            fprintf(stderr, "invalid call to genwave (configuration)\n");
            break;
        }
        if (g_pRawInfoVars == NULL)
            g_pRawInfoVars = allocate_raw_infovars();
        /*
              const char* szPVWaveVersion=((char**)argv)[0];
              fprintf(stderr,"PV-Wave Version '%s'\n",szPVWaveVersion);
        */
        g_pRawInfoVars->m_bConvertInt64ToDouble = *(((int32**)argv)[1]);
        return OK;
    }
    case 1:
        return open_data_file_pv(argc, argv);
    case 2:
        return close_data_file_pv();
    case 3:
        return next_data_unit_pv(argc, argv);
    case 4:
        return get_data_unit_pv(argc, argv);
    }
    return NOT_OK;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 open_data_file_pv(int32 argc, char *argv[])

PV~Wave/IDL interface for open_data_file.

-----------------------------------------------------------*/

int32 open_data_file_pv(int32 argc, char* argv[])
{
    const char* file_name;
    int32 ret_val;

    if (argc < 1)
        fprintf(stderr, "open_data_file_pv illegal argc: %d\n", argc);

    file_name = ((char**)argv)[0];
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = TRUE;
#endif
    ret_val = open_data_file_r(g_pRawInfoVars, file_name);
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = FALSE;
#endif
    return ret_val;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 close_data_file(void)

Close data file.

----------------------------------------------------------------*/
int32 close_data_file(void)
{
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    return close_data_file_r(g_pRawInfoVars);
}

int32 close_data_file_r(struct raw_infovars* pVars)
{
    int32 istatus = 0;
    if (pVars->m_raw_data_file != NULL) {
        istatus = fclose(pVars->m_raw_data_file);
        pVars->m_raw_data_file = NULL;
    }
    return istatus;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 close_data_file_pv(void)

PV~WAVE/IDL interface for close_data_file.

--------------------------------------------------------------------*/

int32 close_data_file_pv(void)
{
    int32 ret_val;

    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = TRUE;
#endif
    ret_val = close_data_file_r(g_pRawInfoVars);
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = FALSE;
#endif
    return ret_val;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 next_data_unit(int32 *e_number, int32 *e_type, modname_t element, modname_t node,
                   int32 *d_type, int32 *number)

Position to next data unit.

---------------------------------------------------------------------*/

int32 next_data_unit(int32* e_number, int32* e_type, modname_t element, modname_t node,
                     int32* d_type, int32* number)
{
    int64 iNum = *number;
    int32 iResult;
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    iResult =
        local_next_data_unit(g_pRawInfoVars, FALSE, e_number, e_type, element, node, d_type, &iNum);
    *number = (int32)iNum;
    return iResult;
}

int32 next_data_unit_r(struct raw_infovars* pVars, int32* e_number, int32* e_type,
                       modname_t element, modname_t node, int32* d_type, int64* number)
{
    return local_next_data_unit(pVars, FALSE, e_number, e_type, element, node, d_type, number);
}

int32 local_next_data_unit(struct raw_infovars* pVars, int bSkip, int32* e_number, int32* e_type,
                           modname_t element, modname_t node, int32* d_type, int64* number)
{
    int32 fstatus;
    int32 istatus;

    istatus = OK;
    if (!pVars->m_first_block_read) {
        pVars->m_block_number = 1;
        fstatus = fread(pVars->m_buf, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
        if (fstatus < 1)
            istatus = END_OF_FILE_DETECTED;
        if (istatus == OK) {
            pVars->m_first_block_read = TRUE;
            pVars->m_index1 = -1;
            pVars->m_list_index = 0;
            pVars->m_prelim_eof_seen = FALSE;
            pVars->m_data_end = TRUE;
            pVars->m_element_number = 0;
            pVars->m_data_blocks = 0;
            istatus = next_token(pVars);
        } else
            istatus = END_OF_FILE_DETECTED;
    }

    if (istatus != OK)
        return istatus;

    if (pVars->m_prelim_eof_seen) {
        pVars->m_prelim_eof_seen = FALSE;
        refresh_token(pVars);
    }
    if (pVars->m_data_end) {
        strcpy(pVars->m_command, pVars->m_blanks);
        strcpy(pVars->m_key, pVars->m_blanks);
        if (pVars->m_token == NIL &&
            pVars->m_actual_key_index >= pVars->m_descr_list[pVars->m_actual_descr_index].key_num) {
            /* pVars->m_prelim_eof_seen = TRUE;*/
            pVars->m_flag = TRUE;
            istatus = OK;
        }
    }
    if (istatus != OK)
        return istatus;
    do {
        if (pVars->m_data_end) {
            pVars->m_datatype = 0;
            pVars->m_datanumber = 0;
            pVars->m_dataaddr = NULL;
            if (pVars->m_token == END_OF_FILE)
                istatus = END_OF_FILE_DETECTED;
            else {
                if (pVars->m_token == NIL) {
                    if (pVars->m_data_unit_type == DATA_LABEL) {
                        strcpy(pVars->m_command,
                               pVars->m_descr_list[pVars->m_actual_descr_index].key_name[1]);
                        if ((++pVars->m_actual_key_index) <=
                            pVars->m_descr_list[pVars->m_actual_descr_index].key_num)
                            strcpy(pVars->m_key, pVars->m_descr_list[pVars->m_actual_descr_index]
                                                     .key_name[pVars->m_actual_key_index]);
                        pVars->m_data_unit_type = DATA_VALUES;
                    } else if (pVars->m_data_unit_type == DATA_VALUES) {
                        if ((++pVars->m_actual_key_index) <=
                            pVars->m_descr_list[pVars->m_actual_descr_index].key_num) {
                            strcpy(pVars->m_command,
                                   pVars->m_descr_list[pVars->m_actual_descr_index].key_name[1]);
                            strcpy(pVars->m_key, pVars->m_descr_list[pVars->m_actual_descr_index]
                                                     .key_name[pVars->m_actual_key_index]);
                            pVars->m_data_unit_type = DATA_VALUES;
                        } else
                            pVars->m_data_unit_type = EMPTY;
                    } else
                        pVars->m_data_unit_type = EMPTY;
                    istatus = next_token(pVars);
                    if (pVars->m_flag && pVars->m_token == 0) {
                        pVars->m_prelim_eof_seen = TRUE;
                        if (istatus == OK)
                            istatus = NOT_OK;
                    }
                } else {
                    data_description(pVars);
                    pVars->m_data_end = FALSE;
                    pVars->m_actual_list_index = 0;
                    istatus = data(pVars);
                }
            }
        } else
            istatus = data(pVars);
    } while (istatus == OK && pVars->m_data_unit_type != DATA_VALUES);

    /* return info about element */
    *e_number = pVars->m_element_number;
    *e_type = pVars->m_elementtype;
    switch (pVars->m_datatype) {
    case INT16_LE:
    case INT16_BE:
        *d_type = INTEGERTYPE;
        break;
    case INT32_LE:
    case INT32_BE:
        *d_type = LONGINTEGER;
        break;
    case INT64_LE:
    case INT64_BE:
        *d_type = INT64TYPE;
        if (pVars->m_bConvertInt64ToDouble)
            *d_type = DOUBLETYPE;
        break;
    case VAX_FLOAT:
    case IEEE32_LE:
    case IEEE32_BE:
        *d_type = REALTYPE;
        break;
        /*case VAX_DOUBLE:*/ /* cannot handle VAXDOUBLE */
    case IEEE64_LE:
    case IEEE64_BE:
        *d_type = DOUBLETYPE;
        break;
    default:
        *d_type = pVars->m_datatype;
        break;
    }
    *number = pVars->m_datanumber;

    pVars->m_command[ARRAY_SIZE(pVars->m_command) - 1] = '\0';
    fstatus = strlen(pVars->m_command);
    while (fstatus > 8 && pVars->m_command[fstatus - 1] == ' ')
        --fstatus;
    memmove(&element[0], &pVars->m_command[0], fstatus);
    while (fstatus < 8)
        element[fstatus++] = ' ';
    element[fstatus] = '\0';

    pVars->m_key[ARRAY_SIZE(pVars->m_key) - 1] = '\0';
    fstatus = strlen(pVars->m_key);
    while (fstatus > 8 && pVars->m_key[fstatus - 1] == ' ')
        --fstatus;
    memmove(&node[0], &pVars->m_key[0], fstatus);
    while (fstatus < 8)
        node[fstatus++] = ' ';
    node[fstatus] = '\0';

    if (pVars->m_list_index > 3 && pVars->m_datatype == 16 /*CHARTYPE*/ &&
        !local_strncmp(element, "EXPTYPE", MAXNAMELENGTH)) {
        /* EXPTYPE: get name of "the" detector from 3rd EXPTYPE element */
        switch (pVars->m_actual_list_index) {
        case 1:
            memset(&pVars->m_detname[0], 0, sizeof(pVars->m_detname));
            break;
        case 3: {
            int64 iNum = pVars->m_datanumber;
            if (iNum >= ARRAY_SIZE(pVars->m_detname))
                iNum = ARRAY_SIZE(pVars->m_detname) - 1;
            if (iNum < 4)
                break;
            if (!local_strncmp(pVars->m_dataaddr, "ADET", 4) ||
                !local_strncmp(pVars->m_dataaddr, "LDET", 4)) {
                /* store only multi detector names */
                memmove(&pVars->m_detname[0], pVars->m_dataaddr, iNum);
                pVars->m_detname[iNum] = '\0';
            }
            break;
        }
        default:
            break;
        }
    }

    if (!local_strncmp(element, "MASTER1VALUES", MAXNAMELENGTH) ||
        !local_strncmp(element, "MASTER2VALUES", MAXNAMELENGTH) ||
        !local_strncmp(element, "SETVALUES", MAXNAMELENGTH) ||
        !local_strncmp(element, "PROTOCOLVALUES", MAXNAMELENGTH) ||
        !local_strncmp(element, "POLVALUES", MAXNAMELENGTH))
        element[8] = '\0';
    if (istatus != OK)
        return istatus;

    /* skip axes info (real value(s)) stored with histogram */
    if (!bSkip &&
        (!local_strncmp(element, "MASTER1V", 8) || !local_strncmp(element, "MASTER2V", 8))) {
        if (pVars->m_detname[0] != '\0') {
            /* we have a detector name from EXPTYPE */
            if (!local_strncmp(node, pVars->m_detname, ARRAY_SIZE(pVars->m_detname)))
                bSkip = TRUE;
        } else {
            /* we don't have a detector name, use defaults */
            if (!local_strncmp(node, "ADET    ", 8) || !local_strncmp(node, "LDET    ", 8) ||
                !local_strncmp(node, "DAU     ", 8))
                bSkip = TRUE;
        }
        if (bSkip)
            istatus =
                local_next_data_unit(pVars, bSkip, e_number, e_type, element, node, d_type, number);
    }
    pVars->m_last_data =
        (pVars->m_descr_list[pVars->m_actual_descr_index].key_num == pVars->m_actual_key_index &&
         pVars->m_data_end)
            ? TRUE
            : FALSE;
    return istatus;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 next_data_unit_pv(int32 argc, char *argv[])

PV~Wave/IDL interface for next_data_unit.

--------------------------------------------------------------*/

int32 next_data_unit_pv(int32 argc, char* argv[])
{
    int32* e_number;
    int32* e_type;
    modname_t element;
    char* str1;
    modname_t node;
    char* str2;
    int32* d_type;
    int32* number;
    int64 iNum;
    int32 ret_val;

    if (argc < 6)
        fprintf(stderr, "next_data_unit_pv illegal argc: %d\n", argc);

    e_number = ((int32**)argv)[0];
    e_type = ((int32**)argv)[1];
    str1 = ((char**)argv)[2];
    str2 = ((char**)argv)[3];
    d_type = ((int32**)argv)[4];
    number = ((int32**)argv)[5];
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = TRUE;
#endif
    iNum = *number;
    ret_val = next_data_unit_r(g_pRawInfoVars, e_number, e_type, element, node, d_type, &iNum);
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = FALSE;
#endif
    *number = (int32)iNum;
    strcpy(str1, element);
    strcpy(str2, node);
    return ret_val;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 last_data_of_element(void)

Pass last_data flag to caller.

----------------------------------------------------------------*/
int32 last_data_of_element(void)
{
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    return g_pRawInfoVars->m_last_data;
}

int32 last_data_of_element_r(struct raw_infovars* pVars) { return pVars->m_last_data; }

int32 last_data_of_element_pv(void)
{
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    return g_pRawInfoVars->m_last_data;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 get_data_unit(void *addr)

Get the current data unit.

-------------------------------------------------------------------*/
int32 get_data_unit(void* addr)
{
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    return get_data_unit_r(g_pRawInfoVars, addr);
}

int32 get_data_unit_r(struct raw_infovars* pVars, void* addr)
{
    int32 istatus;
    int32 f_status;
    char* char_poi_1, *char_poi_2;
    char* temp_addr;
    char* current_poi;
    int64 i, offset;
    int32 j;

    istatus = OK;
    if (pVars->m_data_blocks > 2) {
        offset = (pVars->m_saved_block_number - 1) * BLOCK_LENGTH;
        pVars->m_big_buf_poi = pVars->m_big_buf + 2 * BLOCK_LENGTH;
        current_poi = pVars->m_big_buf_poi;
        for (j = 0; j < pVars->m_data_blocks - 1; j++) {
            f_status = fseeko(pVars->m_raw_data_file, offset, SEEK_SET);
            if (f_status < 0)
                break;
            f_status = fread(current_poi, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
            if (f_status < 1)
                break;
            offset += BLOCK_LENGTH;
            current_poi += BLOCK_LENGTH;
        }
        if (f_status < 1)
            istatus = NOT_OK;
    }
    if (istatus == OK) {
        temp_addr = pVars->m_dataaddr;
        switch (pVars->m_datatype) {
        case CHARTYPE:
            memcpy(addr, temp_addr, pVars->m_datanumber);
            ((char*)addr)[pVars->m_datanumber] = '\0';
            break;
        case INT16_LE:
        case INT16_BE:
            if (pVars->m_local_int16 == pVars->m_datatype)
                memcpy(addr, temp_addr, 2 * pVars->m_datanumber);
            else {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < pVars->m_datanumber; ++i) {
                    *(char_poi_2 + 1) = *char_poi_1++;
                    *char_poi_2++ = *char_poi_1++;
                    char_poi_2++;
                }
            }
            break;
        case INT32_LE:
        case INT32_BE:
            if (pVars->m_local_int32 == pVars->m_datatype) {
#if NATIVE_DATAWIDTH == 64 && !defined(IDL)
                int32 i4;
                int64 i8;
                if (!pVars->m_pv_flag)
                    memcpy(addr, temp_addr, 4 * pVars->m_datanumber);
                else {
                    char_poi_1 = temp_addr;
                    char_poi_2 = (char*)addr;
                    for (i = 0; i < pVars->m_datanumber; ++i) {
                        memcpy(&i4, char_poi_1, 4);
                        i8 = i4;
                        memcpy(char_poi_2, &i8, 8);
                        char_poi_1 += 4;
                        char_poi_2 += 8;
                    }
                }
#else
                memcpy(addr, temp_addr, 4 * pVars->m_datanumber);
#endif /* NATIVE_DATAWIDTH==64 && IDL */
            } else {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < pVars->m_datanumber; ++i) {
#if defined(BIG_ENDIAN) && NATIVE_DATAWIDTH == 64 && !defined(IDL)
                    if (pVars->m_pv_flag) {
                        int32 dummy = 0;
                        memcpy(char_poi_2, &dummy, 4);
                        char_poi_2 += 4;
                    }
#endif /* BIG_ENDIAN && NATIVE_DATAWIDTH==64 && IDL */
                    *(char_poi_2 + 3) = *char_poi_1++;
                    *(char_poi_2 + 2) = *char_poi_1++;
                    *(char_poi_2 + 1) = *char_poi_1++;
                    *(char_poi_2) = *char_poi_1++;
                    char_poi_2 += 4;
#if defined(LITTLE_ENDIAN) && NATIVE_DATAWIDTH == 64 && !defined(IDL)
                    if (pVars->m_pv_flag) {
                        int32 dummy = 0;
                        memcpy(char_poi_2, &dummy, 4);
                        char_poi_2 += 4;
                    }
#endif /* LITTLE_ENDIAN && NATIVE_DATAWIDTH==64 && IDL */
                }
            }
            break;
        case INT64_LE:
        case INT64_BE:
            if (pVars->m_local_int64 == pVars->m_datatype)
                memcpy(addr, temp_addr, 8 * pVars->m_datanumber);
            else {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < pVars->m_datanumber; ++i) {
                    char_poi_2[7] = *char_poi_1++;
                    char_poi_2[6] = *char_poi_1++;
                    char_poi_2[5] = *char_poi_1++;
                    char_poi_2[4] = *char_poi_1++;
                    char_poi_2[3] = *char_poi_1++;
                    char_poi_2[2] = *char_poi_1++;
                    char_poi_2[1] = *char_poi_1++;
                    char_poi_2[0] = *char_poi_1++;
                    char_poi_2 += 8;
                }
            }
            if (pVars->m_bConvertInt64ToDouble) {
                volatile int64* i8 = (int64*)addr;
                volatile double* r8 = (double*)addr;
                for (i = 0; i < pVars->m_datanumber; ++i)
                    r8[i] = (double)i8[i];
            }
            break;
        case IEEE32_LE:
        case IEEE32_BE:
        case VAX_FLOAT:
            if (pVars->m_local_ieee32 == pVars->m_datatype)
                memcpy(addr, temp_addr, 4 * pVars->m_datanumber);
            else {
                if (pVars->m_local_ieee32 != VAX_FLOAT && pVars->m_datatype != VAX_FLOAT) {
                    char_poi_1 = temp_addr;
                    char_poi_2 = (char*)addr;
                    for (i = 0; i < pVars->m_datanumber; ++i) {
                        *(char_poi_2 + 3) = *char_poi_1++;
                        *(char_poi_2 + 2) = *char_poi_1++;
                        *(char_poi_2 + 1) = *char_poi_1++;
                        *(char_poi_2) = *char_poi_1++;
                        char_poi_2 += 4;
                    }
                } else {
                    char_poi_1 = temp_addr;
                    char_poi_2 = (char*)addr;
                    for (i = 0; i < pVars->m_datanumber; ++i) {
                        if (pVars->m_datatype == VAX_FLOAT)
                            vaxfloat_to_ieeefloat(char_poi_1, char_poi_2);
                        else
                            ieeefloat_to_vaxfloat(pVars, char_poi_1, char_poi_2);
                        char_poi_1 += 4;
                        char_poi_2 += 4;
                    }
                }
            }
            break;
        case IEEE64_LE:
        case IEEE64_BE:
            if (pVars->m_local_ieee64 == pVars->m_datatype)
                memcpy(addr, temp_addr, 8 * pVars->m_datanumber);
            else {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < pVars->m_datanumber; ++i) {
                    char_poi_2[7] = *char_poi_1++;
                    char_poi_2[6] = *char_poi_1++;
                    char_poi_2[5] = *char_poi_1++;
                    char_poi_2[4] = *char_poi_1++;
                    char_poi_2[3] = *char_poi_1++;
                    char_poi_2[2] = *char_poi_1++;
                    char_poi_2[1] = *char_poi_1++;
                    char_poi_2[0] = *char_poi_1++;
                    char_poi_2 += 8;
                }
            }
            break;
        default:
            break;
        }
    }
    return istatus;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 get_data_unit_pv(int32 argc, char *argv[])

PV~Wave/IDL interface for get_data_unit.

---------------------------------------------------------*/

int32 get_data_unit_pv(int32 argc, char* argv[])
{
    void* addr;
    int32 ret_val;

    if (argc < 1)
        fprintf(stderr, "get_data_unit_pv illegal argc: %d\n", argc);

    addr = ((void**)argv)[0];
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = TRUE;
#endif
    ret_val = get_data_unit_r(g_pRawInfoVars, addr);
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = FALSE;
#endif
    return ret_val;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 get_data_partition(void *addr, int32 *section, int32 *starting_item,
                       int32 *number_of_items, int32 *type_of_items)

Get a data partition.

--------------------------------------------------------------*/

int32 get_data_partition(void* addr, int32* section, int32* starting_item, int32* number_of_items,
                         int32* type_of_items)
{
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
    return get_data_partition_r(g_pRawInfoVars, addr, *section, *starting_item, *number_of_items,
                                *type_of_items);
}

int32 get_data_partition_r(struct raw_infovars* pVars, void* addr, int32 section,
                           int64 starting_item, int64 number_of_items, int32 type_of_items)
{
    int64 starting_byte_no;
    int64 item_length = 0;
    int64 item_length_1 = 0;
    char* char_poi_1, *char_poi_2;
    char* temp_addr;
    char* current_poi;
    int32 remaining_length;
    int64 i, j, k;
    int32 f_status = 0;
    int32 istatus;
    int32 bConvertInt64ToDouble = 0;

    istatus = OK;
    switch (type_of_items) {
    case CHARTYPE:
        item_length = 1;
        break;
    case INT16_LE:
    case INT16_BE:
        item_length = 2;
        break;
    case INT32_LE:
    case INT32_BE:
    case IEEE32_LE:
    case IEEE32_BE:
    case VAX_FLOAT:
        item_length = 4;
        break;
    case IEEE64_LE:
    case IEEE64_BE:
        if (pVars->m_bConvertInt64ToDouble) {
            switch (pVars->m_datatype) {
            case INT64_LE:
            case INT64_BE:
                bConvertInt64ToDouble = 1;
                break;
            }
        }
        item_length = 8;
        break;
    case INT64_LE:
    case INT64_BE:
        item_length = 8;
        break;
    }
    starting_byte_no = (pVars->m_saved_block_number - 3) * BLOCK_LENGTH + pVars->m_saved_index +
                       (starting_item - 1) * item_length;
    if (section == 2) {
        switch (pVars->m_datatype) {
        case CHARTYPE:
            item_length_1 = 1;
            break;
        case INT16_LE:
        case INT16_BE:
            item_length_1 = 2;
            break;
        case INT32_LE:
        case INT32_BE:
        case IEEE32_LE:
        case IEEE32_BE:
        case VAX_FLOAT:
            item_length_1 = 4;
            break;
        case INT64_LE:
        case INT64_BE:
        case IEEE64_LE:
        case IEEE64_BE:
            item_length_1 = 8;
            break;
        }
        starting_byte_no += pVars->m_datanumber * item_length_1;
    }
    k = number_of_items * item_length / BLOCK_LENGTH;
    remaining_length = (int32)(number_of_items * item_length - k * BLOCK_LENGTH);
    pVars->m_big_buf_poi = pVars->m_big_buf + 2 * BLOCK_LENGTH;
    current_poi = pVars->m_big_buf_poi;
    for (j = 0; j < k; ++j) {
        f_status = fseeko(pVars->m_raw_data_file, starting_byte_no, SEEK_SET);
        if (f_status < 0)
            break;
        f_status = fread(current_poi, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
        if (f_status < 1)
            break;
        starting_byte_no += BLOCK_LENGTH;
        current_poi += BLOCK_LENGTH;
    }
    if (f_status > 0 && remaining_length > 0) {
        f_status = fseeko(pVars->m_raw_data_file, starting_byte_no, SEEK_SET);
        if (f_status == 0)
            f_status = fread(current_poi, remaining_length, 1, pVars->m_raw_data_file);
    }
    if (f_status < 1)
        istatus = NOT_OK;
    if (istatus != OK)
        goto finish;
    temp_addr = pVars->m_big_buf_poi;
    switch (type_of_items) {
    case CHARTYPE:
        memcpy(addr, temp_addr, number_of_items);
        ((char*)addr)[number_of_items] = '\0';
        break;
    case INT16_LE:
    case INT16_BE:
        if (pVars->m_local_int16 == pVars->m_datatype)
            memcpy(addr, temp_addr, 2 * number_of_items);
        else {
            char_poi_1 = temp_addr;
            char_poi_2 = (char*)addr;
            for (i = 0; i < number_of_items; ++i) {
                *(char_poi_2 + 1) = *char_poi_1++;
                *char_poi_2++ = *char_poi_1++;
                char_poi_2++;
            }
        }
        break;
    case INT32_LE:
    case INT32_BE:
        if (pVars->m_local_int32 == pVars->m_datatype) {
#if NATIVE_DATAWIDTH == 64 && !defined(IDL)
            int32 i4;
            int64 i8;
            if (!pVars->m_pv_flag)
                memcpy(addr, temp_addr, 4 * number_of_items);
            else {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < number_of_items; ++i) {
                    memcpy(&i4, char_poi_1, 4);
                    i8 = i4;
                    memcpy(char_poi_2, &i8, 8);
                    char_poi_1 += 4;
                    char_poi_2 += 8;
                }
            }
#else
            memcpy(addr, temp_addr, 4 * number_of_items);
#endif /* NATIVE_DATAWIDTH==64 && IDL */
        } else {
            char_poi_1 = temp_addr;
            char_poi_2 = (char*)addr;
            for (i = 0; i < number_of_items; ++i) {
#if defined(BIG_ENDIAN) && NATIVE_DATAWIDTH == 64 && !defined(IDL)
                if (pVars->m_pv_flag) {
                    int32 dummy = 0;
                    memcpy(char_poi_2, &dummy, 4);
                    char_poi_2 += 4;
                }
#endif /* BIG_ENDIAN && NATIVE_DATAWIDTH==64 && IDL */
                *(char_poi_2 + 3) = *char_poi_1++;
                *(char_poi_2 + 2) = *char_poi_1++;
                *(char_poi_2 + 1) = *char_poi_1++;
                *(char_poi_2) = *char_poi_1++;
                char_poi_2 += 4;
#if defined(LITTLE_ENDIAN) && NATIVE_DATAWIDTH == 64 && !defined(IDL)
                if (pVars->m_pv_flag) {
                    int32 dummy = 0;
                    memcpy(char_poi_2, &dummy, 4);
                    char_poi_2 += 4;
                }
#endif /* LITTLE_ENDIAN && NATIVE_DATAWIDTH==64 && IDL */
            }
        }
        break;
    case INT64_LE:
    case INT64_BE:
        if (pVars->m_local_int64 == pVars->m_datatype)
            memcpy(addr, temp_addr, 8 * number_of_items);
        else {
            char_poi_1 = temp_addr;
            char_poi_2 = (char*)addr;
            for (i = 0; i < number_of_items; ++i) {
                char_poi_2[7] = *char_poi_1++;
                char_poi_2[6] = *char_poi_1++;
                char_poi_2[5] = *char_poi_1++;
                char_poi_2[4] = *char_poi_1++;
                char_poi_2[3] = *char_poi_1++;
                char_poi_2[2] = *char_poi_1++;
                char_poi_2[1] = *char_poi_1++;
                char_poi_2[0] = *char_poi_1++;
                char_poi_2 += 8;
            }
        }
        break;
    case IEEE32_LE:
    case IEEE32_BE:
    case VAX_FLOAT:
        if (pVars->m_local_ieee32 == pVars->m_datatype)
            memcpy(addr, temp_addr, 4 * number_of_items);
        else {
            if (pVars->m_local_ieee32 != VAX_FLOAT && pVars->m_datatype != VAX_FLOAT) {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < number_of_items; ++i) {
                    *(char_poi_2 + 3) = *char_poi_1++;
                    *(char_poi_2 + 2) = *char_poi_1++;
                    *(char_poi_2 + 1) = *char_poi_1++;
                    *(char_poi_2) = *char_poi_1++;
                    char_poi_2 += 4;
                }
            } else {
                char_poi_1 = temp_addr;
                char_poi_2 = (char*)addr;
                for (i = 0; i < number_of_items; ++i) {
                    if (pVars->m_datatype == VAX_FLOAT)
                        vaxfloat_to_ieeefloat(char_poi_1, char_poi_2);
                    else
                        ieeefloat_to_vaxfloat(pVars, char_poi_1, char_poi_2);
                    char_poi_1 += 4;
                    char_poi_2 += 4;
                }
            }
        }
        break;
    case IEEE64_LE:
    case IEEE64_BE:
        if (pVars->m_local_ieee64 == pVars->m_datatype)
            memcpy(addr, temp_addr, 8 * number_of_items);
        else {
            char_poi_1 = temp_addr;
            char_poi_2 = (char*)addr;
            for (i = 0; i < number_of_items; ++i) {
                char_poi_2[7] = *char_poi_1++;
                char_poi_2[6] = *char_poi_1++;
                char_poi_2[5] = *char_poi_1++;
                char_poi_2[4] = *char_poi_1++;
                char_poi_2[3] = *char_poi_1++;
                char_poi_2[2] = *char_poi_1++;
                char_poi_2[1] = *char_poi_1++;
                char_poi_2[0] = *char_poi_1++;
                char_poi_2 += 8;
            }
        }
        if (bConvertInt64ToDouble) {
            volatile int64* i8 = (int64*)addr;
            volatile double* r8 = (double*)addr;
            for (i = 0; i < number_of_items; ++i)
                r8[i] = (double)i8[i];
        }
        break;
    default:
        istatus = NOT_OK;
        break;
    }
finish:
    return istatus;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int32 get_data_partition_pv(int32 argc, char *argv[])

PV~WAVE/IDL interface for get_data_partition.

-------------------------------------------------------*/

int32 get_data_partition_pv(int32 argc, char* argv[])
{
    void* addr;
    int32* section;
    int32* starting_item;
    int32* number_of_items;
    int32* type_of_items;
    int32 ret_val;

    if (argc < 5)
        fprintf(stderr, "get_data_partition_pv illegal argc: %d\n", argc);

    addr = ((void**)argv)[0];
    section = ((int32**)argv)[1];
    starting_item = ((int32**)argv)[2];
    number_of_items = ((int32**)argv)[3];
    type_of_items = ((int32**)argv)[4];
    if (g_pRawInfoVars == NULL)
        g_pRawInfoVars = allocate_raw_infovars();
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = TRUE;
#endif
    ret_val = get_data_partition_r(g_pRawInfoVars, addr, *section, *starting_item, *number_of_items,
                                   *type_of_items);
#ifndef IDL
    g_pRawInfoVars->m_pv_flag = FALSE;
#endif
    return ret_val;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void vaxfloat_to_ieeefloat(char *in_rep,char *out_rep)
Parameters:
        in_rep:    VAX float value (IN)
        out_rep:   IEEE float value (OUT)

Convert a VAX floating number into a IEEE floating number.
little endian/big endian are handled, too.

------------------------------------------------------------*/

static void vaxfloat_to_ieeefloat(char* in_rep, char* out_rep)
{
#define VAX_SNG_BIAS 0x81
#define IEEE_SNG_BIAS 0x7f

#if defined(LITTLE_ENDIAN)

    /* IEEE single precision floating point on computers with little endian */
    struct ieee_single {
        uint32 mantissa : 23;
        uint32 exp : 8;
        uint32 sign : 1;
    };

    /* Vax single precision floating point on computers with little endian*/
    struct vax_single {
        uint32 mantissa1 : 7;
        uint32 exp : 8;
        uint32 sign : 1;
        uint32 mantissa2 : 16;
    };

    static struct sgl_limits {
        struct vax_single s;
        struct ieee_single ieee;
    } sgl_limits[2] = {
          {{0x7f, 0xff, 0x0, 0xffff}, /* Max Vax */
           {0x0, 0xff, 0x0}},         /* Max IEEE */
          {{0x0, 0x0, 0x0, 0x0},      /* Min Vax */
           {0x0, 0x0, 0x0}}           /* Min IEEE */
      };

#elif defined(BIG_ENDIAN)

    /* IEEE single precision floating point on computers with big endian */
    struct ieee_single {
        uint32 sign : 1;
        uint32 exp : 8;
        uint32 mantissa : 23;
    };

    /* Vax single precision floating point on computers with big endian*/
    struct vax_single {
        uint32 mantissa2 : 16;
        uint32 sign : 1;
        uint32 exp : 8;
        uint32 mantissa1 : 7;
    };

    static struct sgl_limits {
        struct vax_single s;
        struct ieee_single ieee;
    } sgl_limits[2] = {
          {{0xffff, 0x0, 0xff, 0x7f}, /* Max Vax */
           {0x0, 0xff, 0x0}},         /* Max IEEE */
          {{0x0, 0x0, 0x0, 0x0},      /* Min Vax */
           {0x0, 0x0, 0x0}}           /* Min IEEE */
      };

#endif

    struct ieee_single is;
    struct vax_single vs;
    struct sgl_limits* lim;
    int32 i;

#if defined(BIG_ENDIAN)

    /* swap bytes */
    char four_bytes[4], c;
    memcpy(four_bytes, (char*)in_rep, 4);
    c = four_bytes[3];
    four_bytes[3] = four_bytes[0];
    four_bytes[0] = c;
    c = four_bytes[2];
    four_bytes[2] = four_bytes[1];
    four_bytes[1] = c;
    vs = *((struct vax_single*)four_bytes);
#elif defined(LITTLE_ENDIAN)
    char four_bytes[4];
    memcpy(four_bytes, in_rep, 4);
    vs = *((struct vax_single*)four_bytes);
#endif

    for (i = 0, lim = sgl_limits; i < (int32)(sizeof(sgl_limits) / sizeof(struct sgl_limits));
         i++, lim++) {
        if ((vs.mantissa2 == lim->s.mantissa2) && (vs.exp == lim->s.exp) &&
            (vs.mantissa1 == lim->s.mantissa1)) {
            is = lim->ieee;
            goto shipit;
        }
    }
    is.exp = vs.exp - VAX_SNG_BIAS + IEEE_SNG_BIAS;
    is.mantissa = (vs.mantissa1 << 16) | vs.mantissa2;
shipit:
    is.sign = vs.sign;
    memcpy(out_rep, (char*)&is, 4);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void ieeefloat_to_vaxfloat(char *in_rep,char *out_rep)
Parameters:
        in_rep:    IEEE float value (IN)
        out_rep:   VAX float value (OUT)

Convert a IEEE floating number to a VAX floating number.
little endian/big endian is handled, too.

----------------------------------------------------------------*/

static void ieeefloat_to_vaxfloat(struct raw_infovars* pVars, char* in_rep, char* out_rep)
{
#define VAX_SNG_BIAS 0x81
#define IEEE_SNG_BIAS 0x7f

    /* IEEE single precision floating point on computers with little endian */
    struct ieee_single {
        uint32 mantissa : 23;
        uint32 exp : 8;
        uint32 sign : 1;
    };

    /* Vax single precision floating point on computers with little endian*/
    struct vax_single {
        uint32 mantissa1 : 7;
        uint32 exp : 8;
        uint32 sign : 1;
        uint32 mantissa2 : 16;
    };

    static struct sgl_limits {
        struct ieee_single ieee;
        struct vax_single s;
    } sgl_limits[2] = {
          {{0x0, 0xff, 0x0},           /* Max IEEE */
           {0x7f, 0xff, 0x0, 0xffff}}, /* Max Vax */
          {{0x0, 0x0, 0x0},            /* Min IEEE */
           {0x0, 0x0, 0x0, 0x0}}       /* Min Vax */
      };

    struct ieee_single is;
    struct vax_single vs;
    struct sgl_limits* lim;
    int32 i;

    if (pVars->m_datatype == IEEE32_BE) {
        /* swap bytes */
        char four_bytes[4], c;
        memcpy(four_bytes, (char*)in_rep, 4);
        c = four_bytes[3];
        four_bytes[3] = four_bytes[0];
        four_bytes[0] = c;
        c = four_bytes[2];
        four_bytes[2] = four_bytes[1];
        four_bytes[1] = c;
        is = *((struct ieee_single*)four_bytes);
    } else {
        char four_bytes[4];
        memcpy(four_bytes, in_rep, 4);
        is = *((struct ieee_single*)four_bytes);
    }

    for (i = 0, lim = sgl_limits; i < (int32)(sizeof(sgl_limits) / sizeof(struct sgl_limits));
         i++, lim++) {
        if ((is.mantissa == lim->ieee.mantissa) && (is.exp == lim->s.exp)) {
            vs = lim->s;
            goto shipit;
        }
    }
    vs.exp = is.exp - IEEE_SNG_BIAS + VAX_SNG_BIAS;
    vs.mantissa2 = is.mantissa;
    vs.mantissa1 = (is.mantissa >> 16);
shipit:
    vs.sign = is.sign;
    memcpy(out_rep, (char*)&vs, 4);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void data_description(void)

Look for simple or complex data and call appropriated function.

------------------------------------------------------------*/

static void data_description(struct raw_infovars* pVars)
{
    if (pVars->m_token != PAREN)
        simple_data_description(pVars);
    else
        complex_data_description(pVars);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void complex_data_description(void)

Handle complex data description.

------------------------------------------------------------*/

static void complex_data_description(struct raw_infovars* pVars)
{
    next_token(pVars);
    do {
        if (pVars->m_token == NIL)
            next_token(pVars);
        else if (pVars->m_token != PAREN)
            data_type(pVars);
        else
            complex_data_description(pVars);
    } while (pVars->m_token != THESIS);
    next_token(pVars);
    count(pVars);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++

static void simple_data_description(void)

Handle a simple data description.

------------------------------------------------------*/
static void simple_data_description(struct raw_infovars* pVars)
{
    data_type(pVars);
    count(pVars);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void count(void)

Store number of values.

------------------------------------------------------------*/

static void count(struct raw_infovars* pVars)
{
    int64 iCount = 0;
    if ((pVars->m_token & 0x80) == 0)
        iCount = pVars->m_token; /* byte length */
    else
        switch (pVars->m_token & 0x7F) {
        case INT16_LE: /* 16 bit length */
        case INT16_BE: {
            union {
                char c[2];
                int16 i;
            } x;
            if (pVars->m_local_int16 == (pVars->m_token & 0x7F)) {
                next_token(pVars);
                x.c[0] = pVars->m_token;
                next_token(pVars);
                x.c[1] = pVars->m_token;
            } else {
                next_token(pVars);
                x.c[1] = pVars->m_token;
                next_token(pVars);
                x.c[0] = pVars->m_token;
            }
            iCount = x.i;
            break;
        }
        case INT32_LE: /* 32 bit length */
        case INT32_BE: {
            union {
                char c[4];
                int32 i;
            } x;
            int i;
            if (pVars->m_local_int32 == (pVars->m_token & 0x7F)) {
                for (i = 0; i < 4; ++i) {
                    next_token(pVars);
                    x.c[i] = pVars->m_token;
                }
            } else {
                for (i = 3; i >= 0; --i) {
                    next_token(pVars);
                    x.c[i] = pVars->m_token;
                }
            }
            iCount = x.i;
            break;
        }
        case INT64_LE: /* 64 bit length */
        case INT64_BE: {
            union {
                char c[8];
                int64 i;
            } x;
            int i;
            if (pVars->m_local_int64 == (pVars->m_token & 0x7F)) {
                for (i = 0; i < 8; ++i) {
                    next_token(pVars);
                    x.c[i] = pVars->m_token;
                }
            } else {
                for (i = 7; i >= 0; --i) {
                    next_token(pVars);
                    x.c[i] = pVars->m_token;
                }
            }
            iCount = x.i;
            break;
        }
        }

    /* store count */
    if (pVars->m_list[pVars->m_list_index].number > 0)
        pVars->m_list[++pVars->m_list_index].d_type = 0;
    pVars->m_list[pVars->m_list_index].number = iCount;
    next_token(pVars);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void data_type(void)

Get a data type.

---------------------------------------------------------*/

static void data_type(struct raw_infovars* pVars)
{
    store_type(pVars, pVars->m_token);
    next_token(pVars);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++

static int32 data(void)
Return value: 0: OK, else failure.

Get a new data.

----------------------------------------------------*/

static int32 data(struct raw_infovars* pVars)
{
    char* addr = 0;
    int32 istatus;

    istatus = OK;
    if (pVars->m_actual_list_index < pVars->m_list_index) {
        if (pVars->m_list[++pVars->m_actual_list_index].d_type != EMPTY) {
            istatus = get_data_block(pVars, pVars->m_list[pVars->m_actual_list_index].d_type,
                                     pVars->m_list[pVars->m_actual_list_index].number, &addr);
            pVars->m_datatype = pVars->m_list[pVars->m_actual_list_index].d_type;
            pVars->m_datanumber = pVars->m_list[pVars->m_actual_list_index].number;
            pVars->m_dataaddr = addr;
            evaluate(pVars, pVars->m_list[pVars->m_actual_list_index].d_type,
                     (int32)pVars->m_list[pVars->m_actual_list_index].number, addr);
        }
    }
    if ((pVars->m_actual_list_index == pVars->m_list_index) ||
        (pVars->m_list[pVars->m_actual_list_index + 1].d_type == EMPTY)) {
        pVars->m_data_end = TRUE;
        pVars->m_list_index = 0;
    }
    return istatus;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int32 refresh_token(void)
Return value: 0: OK, else failure

Re-read data from data file.

----------------------------------------------------------*/

static int32 refresh_token(struct raw_infovars* pVars)
{
    int32 istatus;
    int32 f_status;

    istatus = OK;
    /*f_status =*/ fclose(pVars->m_raw_data_file);
    if (pVars->m_file_name_save == NULL)
        return NOT_OK;
#ifdef __vms
    pVars->m_raw_data_file = fopen(pVars->m_file_name_save, "rb", "ctx=rec", "shr=put");
#else
    pVars->m_raw_data_file = fopen(pVars->m_file_name_save, "rb");
#endif
    if (pVars->m_raw_data_file == NULL)
        return NOT_OK;
    /*f_status =*/ fseeko(pVars->m_raw_data_file, (pVars->m_block_number - 1) * BLOCK_LENGTH, SEEK_SET);
    f_status = fread(pVars->m_buf, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
    if (f_status < 1)
        istatus = END_OF_FILE_DETECTED;
    if (istatus == OK)
        istatus = next_token(pVars);
    return istatus;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int32 next_token(void)
Return value: 0: OK, else failure

Get the nect info particle.

-------------------------------------------------------*/

static int32 next_token(struct raw_infovars* pVars)
{
    int32 istatus;
    int32 f_status;

    istatus = OK;
    if (pVars->m_index1 < BLOCK_LENGTH - 1) {
        pVars->m_token = pVars->m_buf[++pVars->m_index1];
    } else {
        ++pVars->m_block_number;
        /*f_status = */
            fseeko(pVars->m_raw_data_file, (pVars->m_block_number - 1) * BLOCK_LENGTH, SEEK_SET);
        f_status = fread(pVars->m_buf, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
        if (f_status < 1)
            istatus = NOT_OK;
        if (istatus == OK) {
            pVars->m_index1 = -1;
            istatus = next_token(pVars);
        } else
            istatus = END_OF_FILE_DETECTED;
    }
    return istatus;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int32 get_data_block(char d_type, int64 number, char **addr)
Parameter:
        d_type:     type of data (IN)
        number:     number of data items (IN)
        addr:       address where to store the data (OUT)
Return value: 0: OK, else failure

Get the specified block of data.

-------------------------------------------------------------*/

static int32 get_data_block(struct raw_infovars* pVars, char d_type, int64 number, char** addr)
{
    int32 pos;
    int32 istatus;
    int32 f_status;
    int32 i;

    istatus = OK;
    pVars->m_data_blocks = 0;
    switch (d_type) {
    case CHARTYPE:
        break;

    case INT16_LE:
    case INT16_BE:
        number *= 2;
        break;

    case INT32_LE:
    case INT32_BE:
    case IEEE32_LE:
    case IEEE32_BE:
    case VAX_FLOAT:
        number *= 4;
        break;

    case INT64_LE:
    case INT64_BE:
    case IEEE64_LE:
    case IEEE64_BE:
        number *= 8;
        break;

    default:
        number = 0;
        break;
    }
    pos = (int32)(pVars->m_index1 + number);
    if (pos >= BLOCK_LENGTH) {
        pVars->m_big_buf_poi = pVars->m_big_buf;
        for (i = 0; i < BLOCK_LENGTH; i++)
            *pVars->m_big_buf_poi++ = pVars->m_buf[i];
        pVars->m_data_blocks = pos / BLOCK_LENGTH;
        ++pVars->m_block_number;
        pVars->m_big_buf_poi = pVars->m_big_buf + BLOCK_LENGTH;
        /* f_status = */
            fseeko(pVars->m_raw_data_file, (pVars->m_block_number - 1) * BLOCK_LENGTH, SEEK_SET);
        f_status = fread(pVars->m_big_buf_poi, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
        if (f_status < 1)
            istatus = NOT_OK;
        if (istatus == OK) {
            if (pVars->m_data_blocks > 1) {
                if (pVars->m_data_blocks > 2)
                    pVars->m_saved_block_number = pVars->m_block_number + 1;
                pVars->m_block_number += pVars->m_data_blocks - 1;
                /* f_status = */ fseeko(pVars->m_raw_data_file,
                                  (pVars->m_block_number - 1) * BLOCK_LENGTH, SEEK_SET);
                if (pVars->m_data_blocks > MAX_BLOCKS)
                    pVars->m_big_buf_poi = pVars->m_big_buf + 2 * BLOCK_LENGTH;
                else
                    pVars->m_big_buf_poi = pVars->m_big_buf + pVars->m_data_blocks * BLOCK_LENGTH;
                f_status = fread(pVars->m_big_buf_poi, BLOCK_LENGTH, 1, pVars->m_raw_data_file);
                if (f_status < 1)
                    istatus = NOT_OK;
            }
            if (istatus == OK) {
                *addr = (pVars->m_big_buf + pVars->m_index1);
                pVars->m_saved_index = pVars->m_index1;
                if (pVars->m_data_blocks > MAX_BLOCKS)
                    pVars->m_big_buf_poi = pVars->m_big_buf + 2 * BLOCK_LENGTH;
                else
                    pVars->m_big_buf_poi = pVars->m_big_buf + pVars->m_data_blocks * BLOCK_LENGTH;
                memcpy(pVars->m_buf, pVars->m_big_buf_poi, BLOCK_LENGTH);
                pVars->m_index1 = (pVars->m_index1 + number) -
                                  ((pVars->m_index1 + number) / BLOCK_LENGTH) * BLOCK_LENGTH;
            }
        }
    } else {
        *addr = &pVars->m_buf[pVars->m_index1];
        pVars->m_saved_index = pVars->m_index1;
        pVars->m_index1 = pos;
    }
    if (istatus == OK)
        pVars->m_token = pVars->m_buf[pVars->m_index1];
    return (istatus == OK) ? OK : END_OF_FILE_DETECTED;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void evaluate(char d_type, int32 number, char *addr)
Parameters:
        d_type:     type of data (IN)
        number:     number of data items (IN)
        addr:

-----------------------------------------------------------*/

static void evaluate(struct raw_infovars* pVars, char d_type, int32 number, char* addr)
{
    if (d_type == CHARTYPE) {
        char str[MAX_STR_LEN > 256 ? MAX_STR_LEN : 256];
        if (number >= ARRAY_SIZE(str)) {
            if (stderr != NULL)
                fprintf(stderr, "%s(%d): truncating number %d to %d\n", __FILE__, __LINE__, number,
                        ARRAY_SIZE(str) - 1);
            number = ARRAY_SIZE(str) - 1;
        }
        memcpy(str, addr, number);
        str[number] = '\0';
        if (strncmp(str, "DEFCMD", 6) == 0) {
            next_descr(pVars, CMD_ELEMENT);
            pVars->m_data_unit_type = COMMAND_STRUCT;
            separate(pVars, str, number);
        } else if (strncmp(str, "DEFDAT", 6) == 0) {
            next_descr(pVars, DAT_ELEMENT);
            pVars->m_data_unit_type = DATA_STRUCT;
            separate(pVars, str, number);
        } else if (label_name(pVars, str, number))
            pVars->m_data_unit_type = DATA_LABEL;
        else
            pVars->m_data_unit_type = DATA_VALUES;
    } else
        pVars->m_data_unit_type = DATA_VALUES;
    if (pVars->m_data_unit_type == DATA_VALUES) {
        strcpy(pVars->m_command, pVars->m_descr_list[pVars->m_actual_descr_index].key_name[1]);
        if (pVars->m_descr_list[pVars->m_actual_descr_index].key_num > 1) {
            if (pVars->m_actual_key_index <
                pVars->m_descr_list[pVars->m_actual_descr_index].key_num) {
                if (pVars->m_actual_list_index == 1)
                    ++pVars->m_actual_key_index;
                strcpy(pVars->m_key, pVars->m_descr_list[pVars->m_actual_descr_index]
                                         .key_name[pVars->m_actual_key_index]);
            }
        }
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int32 label_name(char *str, int32 number)
Parameter:
        str:
        number:
Return value: 0: OK, else failure

--------------------------------------------------------*/

static int32 label_name(struct raw_infovars* pVars, char* str, int32 number)
{
    modname_t sub_str;
    int32 len = number < (ARRAY_SIZE(sub_str) - 1) ? number : (ARRAY_SIZE(sub_str) - 1);
    int32 i;

    strcpy(sub_str, pVars->m_blanks);
    strncpy(sub_str, str, len);
    sub_str[ARRAY_SIZE(sub_str) - 1] = '\0';
    i = pVars->m_descr_index + 1;
    do {
        --i;
    } while ((i > 1) && (strncmp(sub_str, pVars->m_descr_list[i].key_name[1], len) != 0));
    if (strncmp(sub_str, pVars->m_descr_list[i].key_name[1], len) == 0) {
        pVars->m_actual_descr_index = i;
        pVars->m_elementtype = pVars->m_descr_list[i].kind;
        ++pVars->m_element_number;
        pVars->m_actual_key_index = 1;
        return TRUE;
    } else
        return FALSE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void separate(char *str, int32 num)
Parameters:
        str:
        num:

-------------------------------------------------------*/

static void separate(struct raw_infovars* pVars, char* str, int32 num)
{
    modname_t key_name;
    int32 level;
    char* poi;
    int32 i, j;

    level = 0;
    strcpy(key_name, pVars->m_blanks);
    poi = key_name;
    i = 1;
    do {
        while (i <= num && *str != '(' && *str != ')' && *str != ' ') {
            ++i;
            ++str;
        }
        while (i <= num) {
            switch (*str) {
            case '(':
                if ((++level) >= 2)
                    delete_descr(pVars);
                ++i;
                ++str;
                continue;
            case ')':
                --level;
                ++i;
                ++str;
                continue;
            case ' ':
                ++i;
                ++str;
                break;
            }
            break;
        }
        j = 0;
        while (i <= num && *str != '(' && *str != ')' && *str != ' ') {
            ++j;
            *poi++ = *str++;
            ++i;
        }
        if (j > 0 || i <= num) {
            store_descr(pVars, key_name);
            strcpy(key_name, pVars->m_blanks);
            poi = key_name;
        }
    } while (i < num);
    insert_descr(pVars);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void store_type(char token)
Parameters:
        token:

----------------------------------------------------------*/

static void store_type(struct raw_infovars* pVars, char token)
{
    pVars->m_list[++pVars->m_list_index].d_type = token;
    pVars->m_list[pVars->m_list_index].number = 0;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void next_descr(int32 kind)
Parameters:
        kind:

------------------------------------------------------*/

static void next_descr(struct raw_infovars* pVars, int32 kind)
{
    pVars->m_descr_list[++pVars->m_descr_index].kind = kind;
    pVars->m_descr_list[pVars->m_descr_index].key_num = 0;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void store_descr(char *str)
Parameters:
        str:

-----------------------------------------------------------*/

static void store_descr(struct raw_infovars* pVars, char* str)
{
    char* p = pVars->m_descr_list[pVars->m_descr_index]
                  .key_name[++(pVars->m_descr_list[pVars->m_descr_index].key_num)];
    strncpy(p, str, MAXNAMELENGTH);
    p[MAXNAMELENGTH] = '\0';
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++

static void insert_descr(void)

----------------------------------------------------*/

static void insert_descr(struct raw_infovars* pVars)
{
    int32 i, j;

    if (pVars->m_descr_index > 1) {
        for (i = 1; i < pVars->m_descr_index; i++) {
            if (strncmp(pVars->m_descr_list[i].key_name[1],
                        pVars->m_descr_list[pVars->m_descr_index].key_name[1],
                        MAXNAMELENGTH) == 0) {
                /* match: overwrite old descriptor with new */
                pVars->m_descr_list[i].key_num = pVars->m_descr_list[pVars->m_descr_index].key_num;
                for (j = 1; j <= pVars->m_descr_list[pVars->m_descr_index].key_num; j++)
                    strcpy(pVars->m_descr_list[i].key_name[j],
                           pVars->m_descr_list[pVars->m_descr_index].key_name[j]);
                --pVars->m_descr_index;
                break;
            }
        }
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void delete_descr(void)

--------------------------------------------------------*/

static void delete_descr(struct raw_infovars* pVars)
{
    --(pVars->m_descr_list[pVars->m_descr_index].key_num);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int32 local_strncmp(register const char* szString1,
                           register const char* szString2,
                           register int32 iMaxLen)

compare two module names while ignoring blanks at end of string

--------------------------------------------------------*/

static int32 local_strncmp(register const char* szString1, register const char* szString2,
                           register int32 iMaxLen)
{
    /* pre test NULL strings */
    if (szString1 == NULL)
        return -1;
    if (szString2 == NULL)
        return 1;
    for (; iMaxLen > 0; --iMaxLen) {
        if (*szString1 == '\0') {
            /* end of string 1 */
            do {
                switch (*szString2++) {
                case '\0':
                    break; /* end of string 2 */
                case ' ':
                    continue; /* ignore spaces */
                default:
                    return -1; /* string 2 has more characters */
                }
                break;
            } while (--iMaxLen > 0);
            break;
        }
        if (*szString2 == '\0') {
            /* end of string 2 */
            do {
                switch (*szString1++) {
                case '\0':
                    break; /* end of string 1 */
                case ' ':
                    continue; /* ignore spaces */
                default:
                    return 1; /* string 1 has more characters */
                }
                break;
            } while (--iMaxLen > 0);
            break;
        }
        /* compare every character */
        if (*szString1 < *szString2)
            return -1;
        if (*szString1++ > *szString2++)
            return 1;
    }
    return 0; /* strings are equal */
}

#if defined(_MSC_VER) && defined(WINVER)
static int fseeko(FILE* pFile, int64 llPos, int iWhence)
{
    fpos_t p = llPos;
    ((void)iWhence);
    return fsetpos(pFile, &p);
}

static int64 ftello(FILE* pFile)
{
    fpos_t p = 0;
    if (fgetpos(pFile, &p) != 0)
        return -1;
    return p;
}
#endif /* _MSC_VER && WINVER */
