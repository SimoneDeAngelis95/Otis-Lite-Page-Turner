#include "FlashStorage.h"
#include <cstring>   // for memset

FlashStorage::FlashStorage()
{
}

/**
 * @brief Write the mode bit into flash.
 */
bool FlashStorage::write(bool modeBit)
{
    // Avoid an unnecessary erase when the requested value is already stored.
    if (read() == modeBit)
    {
        return true;
    }

    // Prepare a page buffer filled with 0xFF (erased state).
    uint8_t page[PAGE_SIZE];
    memset(page, 0xFF, sizeof(page));

    // The last byte of flash keeps the mode bit (LSB).
    page[PAGE_SIZE - 1] = modeBit ? 0x01 : 0x00;

    // Erase/program operations must run with interrupts disabled.
    uint32_t ints = save_and_disable_interrupts();

    // Erase the entire reserved sector.
    flash_range_erase(FLASH_OFFSET, SECTOR_SIZE);

    // Program the last page with the bit.
    flash_range_program(LAST_PAGE_OFFSET, page, PAGE_SIZE);

    // Restore interrupts.
    restore_interrupts(ints);

    return true;
}

/**
 * @brief Read the bit stored in flash.
 */
bool FlashStorage::read() const
{
    // Compute the address of the last byte in flash mapped through XIP.
    const uint8_t* flash_addr = reinterpret_cast<const uint8_t*>(XIP_BASE + LAST_BYTE_OFFSET);
    uint8_t rawValue = *flash_addr;

    // If the sector was never programmed, the value stays 0xFF -> default to horizontal mode.
    if (rawValue == 0xFF)
    {
        return false;
    }

    // Return the least significant bit.
    return (rawValue & 0x01u) != 0;
}
