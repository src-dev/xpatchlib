#ifndef XIPS_H
#define XIPS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * Creates a backup copy of a file.
	 *
	 * @param src  Path to source file
	 * @param dst  Path to destination backup file
	 * @return true on success, false on error
	 */
	bool createBak(const char* src, const char* dst);

	/**
	 * Applies an IPS patch to a target file.
	 *
	 * @param ips  Path to IPS patch file
	 * @param src  Path to file to be patched (must be writable)
	 * @return true on success, false on error
	 */
	bool applyIPS(const char* ips, const char* src);

#ifdef __cplusplus
}
#endif

#endif // XIPS_H