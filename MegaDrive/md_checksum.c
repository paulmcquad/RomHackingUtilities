#include <stdio.h>
#include <stdint.h>

#define ROM_SIZE 0x400000 // 4MB ROM size
#define CHECKSUM_OFFSET 0x18E // Checksum offset in the ROM header

uint16_t calculate_checksum(uint8_t *rom) {
    uint16_t checksum = 0;
    for (int i = 0; i < ROM_SIZE; i++) {
        if (i >= CHECKSUM_OFFSET && i < CHECKSUM_OFFSET + 2) {
            continue; // Skip the checksum bytes
        }
        checksum += rom[i];
    }
    return checksum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *rom_file = fopen(argv[1], "rb+");
    if (!rom_file) {
        printf("Failed to open ROM file\n");
        return 1;
    }

    uint8_t rom[ROM_SIZE];
    fread(rom, 1, ROM_SIZE, rom_file);

    uint16_t calculated_checksum = calculate_checksum(rom);
    uint16_t existing_checksum = (rom[CHECKSUM_OFFSET] << 8) | rom[CHECKSUM_OFFSET + 1];

    if (calculated_checksum != existing_checksum) {
        printf("Checksum mismatch! Correcting...\n");
        rom[CHECKSUM_OFFSET] = (calculated_checksum >> 8) & 0xFF;
        rom[CHECKSUM_OFFSET + 1] = calculated_checksum & 0xFF;
        fseek(rom_file, 0, SEEK_SET);
        fwrite(rom, 1, ROM_SIZE, rom_file);
        printf("Checksum corrected!\n");
    } else {
        printf("Checksum is already correct!\n");
    }

    fclose(rom_file);
    return 0;
}
