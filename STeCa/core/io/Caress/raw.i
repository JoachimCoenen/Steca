// copied from raw.h

typedef char modname_t[32 + 1];

int32 open_data_file(const char*, int32*);
int32 close_data_file(void);

int32 next_data_unit(int32*, int32*, modname_t, modname_t, int32*, int32*);
int32 get_data_unit(void*);

// eof
