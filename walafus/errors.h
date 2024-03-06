#pragma once


enum class WalafusFilesystemErrorCode
{
    UNKNOWN = 0,
    FILE_OPEN_ERROR = 1,
    FILE_READ_ERROR = 2,
    FILE_WARNING = 3
};


/**
 * @brief Error callback. Called when filesystem encounters some problems
 *  Change pointer to user-defined function to accept callbacks and decide whether to throw exception or not
 *
 * @param error_code error code in enum WalafusFilesystemErrorCode
 * @param error_description string human-readable description of occurred error, useful for debugging
 * @param error_object pointer to the object that caused error
 *  the type can only be inferred by looking up at error string
 *  be careful when accessing this pointer
 *
 * @return true if exception should be raised after calling this function, false otherwise
 */
extern bool (*walafus_on_error)(WalafusFilesystemErrorCode error_code, const char* error_description, void* error_object);
