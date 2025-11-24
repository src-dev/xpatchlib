#ifndef XIPSLIB_H
#define XIPSLIB_H

typedef enum {
	E_NO_ERROR,
	E_FOPEN_SRC,
	E_FOPEN_DST,
	E_FOPEN_IPS,
	E_NOT_IPS,
	E_OUT_OF_MEMORY,
	E_CANNOT_OVR
} ErrorCode;

#ifdef __cplusplus
extern "C" {
#endif

	int createBak(const char* src, bool ovr);
	int restoreBak(const char* src, bool ovr);
	int applyIPS(const char* ips, const char* src);

#ifdef __cplusplus
}
#endif

#endif // XIPSLIB_H