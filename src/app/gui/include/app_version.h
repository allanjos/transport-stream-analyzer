/**
 * Application information.
 */
#ifndef APP_VERSION_H
#define APP_VERSION_H

#define APP_NAME "tsgui"
#define APP_OWNER "Allann Jones"
#define APP_URL "http://www.olivum.com.br"
#define APP_BRIEF "Transport stream analyzer"

/** Application major version. */
#define APP_VERSION_MAJOR "0"
/** Application minor version. */
#define APP_VERSION_MINOR "0"
/** Application release control. */
#define APP_RELEASE_CANDIDATE "1"

/** Application version string. */
#ifndef APP_RELEASE_CANDIDATE
#  define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR ".0"
#else
#  define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_RELEASE_CANDIDATE
#endif

#endif