#ifndef XIPSLIB_H
#define XIPSLIB_H

typedef enum {
	E_NO_ERROR,
	E_FOPEN_SRC,
	E_FOPEN_DST,
	E_FOPEN_IPS,
	E_NOT_IPS,
	E_OUT_OF_MEMORY
} ErrorCode;

#ifdef __cplusplus
extern "C" {
#endif

	int createBak(const char* src, const char* dst);
	int applyIPS(const char* ips, const char* src);

#ifdef __cplusplus
}
#endif

#endif // XIPSLIB_H