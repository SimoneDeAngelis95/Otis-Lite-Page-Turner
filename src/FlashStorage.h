#pragma once
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

/**
 * @class FlashStorage
 * @brief Utility class to store a single bit inside the Raspberry Pi Pico flash.
 *
 * The bit is kept in the LAST flash sector, in the final byte available.
 * Each update erases the whole sector (filled with 0xFF) and programmes
 * the last page with the new value. In this project the bit is used to
 * remember whether the selected mode is horizontal or vertical.
 *
 * Important: flash wears out if you keep writing the same sector repeatedly
 * (roughly 100,000 cycles). Use these functions sparingly.
 */
class FlashStorage
{
public:
    /**
     * @brief Default constructor, nothing special required.
     */
    FlashStorage();

    /**
     * @brief Store the mode bit (0 = horizontal, 1 = vertical).
     *
     * The reserved sector is erased before programming to ensure the bit
     * can return to 1 when required.
     *
     * @param modeBit false for horizontal, true for vertical.
     * @return true if the write completed successfully.
     */
    bool write(bool modeBit);

    /**
     * @brief Read the bit stored in flash.
     *
     * If the sector is still erased (0xFF) returns false so the
     * default remains horizontal mode.
     *
     * @return true when the persisted mode is vertical, false otherwise.
     */
    bool read() const;

private:
    // ---- Costanti legate alla memoria flash ----
    
    // size_t: appropriate type to represent memory sizes.
    // constexpr: value known at compile time.
    // static: belongs to the class rather than a single object.

    /// Flash sector size (typically 4096 bytes).
    static constexpr size_t SECTOR_SIZE = FLASH_SECTOR_SIZE;

    /// Flash page size (typically 256 bytes).
    static constexpr size_t PAGE_SIZE   = FLASH_PAGE_SIZE;

    /// Pico flash size (defaults to 4 MB if not defined in CMake).
#ifndef PICO_FLASH_SIZE_BYTES
    static constexpr size_t FLASH_SIZE = (4 * 1024 * 1024);
#else
    static constexpr size_t FLASH_SIZE = PICO_FLASH_SIZE_BYTES;
#endif

    /// Offset of the last flash sector (reserved area for data).
    static constexpr uint32_t FLASH_OFFSET = FLASH_SIZE - SECTOR_SIZE;

    /// Offset of the page that contains the final flash byte.
    static constexpr uint32_t LAST_PAGE_OFFSET = FLASH_OFFSET + (SECTOR_SIZE - PAGE_SIZE);

    /// Offset of the final byte in flash (holds the mode bit).
    static constexpr uint32_t LAST_BYTE_OFFSET = FLASH_OFFSET + (SECTOR_SIZE - 1);
};
